#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
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
	ImGui::ShowDemoWindow();
	ImGui::BeginMainMenuBar();
	if(ImGui::BeginMenu("Help")) {
		ImGui::MenuItem("Gui Demo");
		ImGui::MenuItem("Documentacion");
		ImGui::MenuItem("Download lastest");
		ImGui::MenuItem("Report bug");
		ImGui::MenuItem("About");
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	ImGui::Begin("Console");
	ImGui::TextUnformatted(buffer.begin());
	if (scrollToBottom)
		ImGui::SetScrollHere(1.0F);
	scrollToBottom = false;
	ImGui::End();
	// Frame Rate
	ImGui::Begin("Frame rate");
	fps_log.push_back(io.Framerate);
	sprintf_s(title, 25, "Framerate %.1F", fps_log[fps_log.size()-1]);
	ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0F, 100.0F, ImVec2(310, 100));
	ms_log.push_back(1000.0F / io.Framerate);
	sprintf_s(title, 25, "Milliseconds %.1F", ms_log[ms_log.size() - 1]);
	ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0F, 40.0F, ImVec2(310, 100));
	ImGui::End();
	//
	ImGui::Begin("Configuration");
	ImGui::SliderInt("Width", &(App->window->screenWidth), 640, 1024);
	ImGui::SliderInt("Height", &(App->window->screenHeight), 480, 720);
	if (ImGui::Checkbox("Resizable", &(App->window->resizable))) {
		App->window->UpdateResizable();
	}
	ImGui::End();
	// Hardware
	ImGui::Begin("Hardware");
	ImGui::Text("CPU cores: %d (Cache: %d Kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
	ImGui::Text("System RAM: %d Mb", SDL_GetSystemRAM());
	ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("GPU Model: %s", glGetString(GL_RENDERER));
	ImGui::End();
	// Camera

	ImGui::Begin("Camera Position");
	ImGui::SliderFloat("X", &(App->camera->frustum.pos.x), -50.0F, 50.0F);
	ImGui::SliderFloat("Y", &(App->camera->frustum.pos.y), -50.0F, 50.0F);
	ImGui::SliderFloat("Z", &(App->camera->frustum.pos.z), -50.0F, 50.0F);
	ImGui::End();

	ImGui::Begin("Camera Front");
	ImGui::SliderFloat("X", &(App->camera->frustum.front.x), -50.0F, 50.0F);
	ImGui::SliderFloat("Y", &(App->camera->frustum.front.y), -50.0F, 50.0F);
	ImGui::SliderFloat("Z", &(App->camera->frustum.front.z), -50.0F, 50.0F);
	ImGui::End();

	ImGui::Begin("Camera Up");
	ImGui::SliderFloat("X", &(App->camera->frustum.up.x), -50.0F, 50.0F);
	ImGui::SliderFloat("Y", &(App->camera->frustum.up.y), -50.0F, 50.0F);
	ImGui::SliderFloat("Z", &(App->camera->frustum.up.z), -50.0F, 50.0F);
	ImGui::End();

	ImGui::Begin("Helper 1");
	ImGui::SliderFloat("X", &(App->camera->helper1.x), -50.0F, 50.0F);
	ImGui::SliderFloat("Y", &(App->camera->helper1.y), -50.0F, 50.0F);
	ImGui::SliderFloat("Z", &(App->camera->helper1.z), -50.0F, 50.0F);
	ImGui::End();

	ImGui::Begin("helepr2");
	ImGui::SliderFloat("X", &(App->camera->helper2.x), -50.0F, 50.0F);
	ImGui::SliderFloat("Y", &(App->camera->helper2.y), -50.0F, 50.0F);
	ImGui::SliderFloat("Z", &(App->camera->helper2.z), -50.0F, 50.0F);
	ImGui::End();
	// Render
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	/*glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);*/
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