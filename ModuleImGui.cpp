#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "assimp/version.h"

bool ModuleImGui::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->glContext);
	ImGui_ImplOpenGL3_Init();
	return true;
}

update_status ModuleImGui::Update() {
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Menu")) {
		showModule = ImGui::MenuItem("Module Configuration");
		showProperties = ImGui::MenuItem("Properties");
		showInfo = ImGui::MenuItem("System Information");
		showConsole = ImGui::MenuItem("Console Window");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help")) {
		showAbout = ImGui::MenuItem("About");
		if (ImGui::MenuItem("Visit Page")) {
			ShellExecute(0, 0, "https://github.com/marcorod94/Engine-Demo-MR", 0, 0, SW_SHOW);
		}
		if (ImGui::MenuItem("Quit")) {
			return UPDATE_STOP;
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	
	if (showConsole) {
		ImGui::Begin("Console", &showConsole);
		ImGui::TextUnformatted(buffer.begin());
		if (scrollToBottom)
			ImGui::SetScrollHere(1.0F);
		scrollToBottom = false;
		ImGui::End();
	}
	if (showInfo) {
		ShowInformationWindow(io);
	}
	if (showModule) {
		ShowModulesWindow();
	}
	if (showProperties) {
		ImGui::Begin("Properties", &showProperties);
		if (ImGui::TreeNode("Model")) {

			if (ImGui::TreeNode("Transformation")) {
				ImGui::Text("Position:\tX:%.f\tY:%.f\tZ:%.f ", App->model->position.x, App->model->position.y, App->model->position.z);
				ImGui::Text("Rotation:\tX:%.f\tY:%.f\tZ:%.f", App->model->rotation.x, App->model->rotation.y, App->model->rotation.z);
				ImGui::Text("Scale:\tX:%.f\tY:%.f\tZ:%.f", App->model->scaling.x, App->model->scaling.y, App->model->scaling.z);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Geometry")) {
				ImGui::Text("Mesh Total: %d", App->model->meshes.size());
				ImGui::Text("Vertex Total: %d", App->model->totalVertex);
				ImGui::Text("Primitive Total: %d", App->model->totalPrimitives);
				ImGui::Text("Material Total: %d", App->model->totalMaterials);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Textures")) {
				for (unsigned int i = 0; i < App->model->meshes.size(); i++) {
					ImGui::Text("Mesh %d:", i + 1);
					ShowTextures(App->model->meshes[i].textures);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::End();
		
	}
	if (showAbout) {
		ImGui::Begin("About", &showAbout);
		ImGui::Text("Engine: EM ENGINE");
		ImGui::Text("Desciption: Super Cool Engine develop with love <3");
		ImGui::Text("Author: Marco Rodriguez");
		ImGui::End();
	}
	
	// Render
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp() {
	buffer.clear();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

void ModuleImGui::AddLog(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	buffer.appendfv(fmt, args);
	va_end(args);
	buffer.append("\n");
	scrollToBottom = true;
}


void  ModuleImGui::ShowModulesWindow() {
	ImGui::Begin("Module Configuration", &showModule);
	if (ImGui::TreeNode("Window")) {
		if (ImGui::SliderInt("Width", &(App->window->screenWidth), App->window->minScreenWidth, App->window->maxScreenWidth)) {
			App->window->UpdateScreenSize();
		}
		if (ImGui::SliderInt("Height", &(App->window->screenHeight), App->window->minScreenHeight, App->window->maxScreenHeight)) {
			App->window->UpdateScreenSize();
		}
		if (ImGui::Checkbox("Resizable", &(App->window->resizable))) {
			App->window->UpdateResizable();
		}
		if (ImGui::Checkbox("Full Screen", &(App->window->fullScreen))) {
			App->window->UpdateFullScreen();
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Camera")) {
		ImGui::SliderFloat("Camera Speed", &(App->camera->cameraSpeed), 0.01F, 2.0F);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Textures")) {
		ShowTextures(App->texture->loadedTextures);
		ImGui::TreePop();
	}
	ImGui::End();
}

void ModuleImGui::ShowInformationWindow(ImGuiIO& io) {
	ImGui::Begin("System Information", &showInfo);
	if (ImGui::TreeNode("Frame rate")) {
		fps_log.push_back(io.Framerate);
		sprintf_s(title, 25, "Framerate %.1F", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0F, 100.0F, ImVec2(310, 100));
		ms_log.push_back(1000.0F / io.Framerate);
		sprintf_s(title, 25, "Milliseconds %.1F", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0F, 40.0F, ImVec2(310, 100));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Hardware")) {
		ImGui::Text("CPU cores: %d (Cache: %d Kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: %d Mb", SDL_GetSystemRAM());
		ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("GPU Model: %s", glGetString(GL_RENDERER));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Libraries")) {
		SDL_version compiled;
		SDL_VERSION(&compiled);
		ImGui::Text("SDL: %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
		ImGui::Text("DevIL: %d", IL_VERSION);
		ImGui::Text("Glew: %s", glewGetString(GLEW_VERSION));
		ImGui::Text("ImGui: %s", IMGUI_VERSION);
		ImGui::Text("Assimp: %d.%d.%d", ASSIMP_API::aiGetVersionMajor(), ASSIMP_API::aiGetVersionMinor(), ASSIMP_API::aiGetVersionRevision());
		ImGui::TreePop();
	}
	ImGui::End();
}

void ModuleImGui::ShowTextures(std::vector<Texture>& textures) {
	for (unsigned int i = 0; i < textures.size(); i++) {
		ImGui::Text("Path: %s", textures[i].path.c_str());
		ImGui::Text("Type: %s", textures[i].type.c_str());
		ImGui::Text("Width: %d", textures[i].width);
		ImGui::Text("Heigth: %d", textures[i].height);
		if (ImGui::ImageButton((void*)(intptr_t)textures[i].id, ImVec2(128, 128))) {
			App->model->UpdateTexture(textures[i]);
		}
	}
}
