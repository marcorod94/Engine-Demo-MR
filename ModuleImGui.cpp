#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleModelLoader.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"


ModuleImGui::ModuleImGui() {
}


ModuleImGui::~ModuleImGui() {
}


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
		showInfo = ImGui::MenuItem("Information");
		showConsole = ImGui::MenuItem("Console Window");
		showModule = ImGui::MenuItem("Module Configuration");
		showAbout = ImGui::MenuItem("About");
		if (ImGui::MenuItem("Visit Page")) {
			ShellExecute(0, 0, "https://github.com/marcorod94/Engine-Demo-MR", 0,0, SW_SHOW);
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
		ImGui::End();
	}
	if (showModule) {
		ImGui::Begin("Module Configuration", &showModule);
		if (ImGui::TreeNode("Window")) {
			if (ImGui::SliderInt("Width", &(App->window->screenWidth), 640, 1024) || ImGui::SliderInt("Height", &(App->window->screenHeight), 480, 720)) {
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
			for (unsigned int i = 0; i < App->texture->loadedTextures.size(); i++) {
				if (ImGui::ImageButton((void*)(intptr_t)App->texture->loadedTextures[i].id, ImVec2(128, 128))) {
					App->modelLoader->UpdateTexture(App->texture->loadedTextures[i]);
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Model")) {
			ImGui::Text("Mesh Total: %d", App->modelLoader->meshes.size());
			ImGui::Text("Vertex Total: %d", App->modelLoader->totalVertex);
			ImGui::Text("Primitive Total: %d", App->modelLoader->totalPrimitives);
			ImGui::Text("Material Total: %d", App->modelLoader->totalMaterials);
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
	//SDL_GL_SwapWindow(App->window->window);
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
	scrollToBottom = true;
}