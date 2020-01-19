#include "main/Application.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleTimer.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "assimp/version.h"
#include "util/DebugDraw.h"
#include "main/GameObject.h"
#include "component/Camera.h"
#include "component/Transform.h"
#include "util/DebugDraw.h"

bool ModuleImGui::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->glContext);
	ImGui_ImplOpenGL3_Init();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	static const ImWchar iconsRanges[] = { 0xf000, 0xf8d9, 0 };
	static const ImWchar iconsBrandRange[] = { 0xf081, 0xf8e8, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", 16.0F, &icons_config, iconsRanges);
	io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", 16.0F, &icons_config, iconsRanges);
	io.Fonts->AddFontFromFileTTF("Fonts/fa-brands-400.ttf", 16.0F, &icons_config, iconsBrandRange);
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	return true;
}

update_status ModuleImGui::Update() {
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	if (App->camera->loadedCameras[0]->isHovered) 
	{
		//ImGui::lo
	}
	//ImGui::ShowDemoWindow();
	Camera* cam = nullptr;
	if (App->scene->root) {
		for (unsigned i = 0; i < App->camera->loadedCameras.size(); i++) {
			cam = App->camera->loadedCameras[i];
			cam->Draw(cam->owner->name.c_str());
			cam->DrawFrustumPlanes();
		}
	}
	if (showProperties) {
		App->scene->root->ShowProperties(&showProperties);
	}
	
	if (showHierarchy) {
		ImGui::Begin(u8"\uf542 GameObjects Hierarchy", &showHierarchy);
		DrawHierarchy(App->scene->root);
		ImGui::End();
	}
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu(u8"\uf0c9 Menu")) {
		if (ImGui::MenuItem("Module Configuration")) {
			showModule = true;
		}
		if (ImGui::MenuItem("Properties")) {
			showProperties = true;
		}
		if (ImGui::MenuItem("System Information")) {
			showInfo = true;
		}
		if (ImGui::MenuItem(u8"\uf120 Console Window")) {
			showConsole = true;
		}
		if (ImGui::MenuItem(u8"\uf542 Hierarchy")) {
			showHierarchy = true;
		}
		if (ImGui::MenuItem(u8"\uf093 Load")) {
			App->scene->LoadScene();
		}
		if (ImGui::MenuItem(u8"\uf019 Save")) {
			App->scene->SaveScene();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(u8"\uf059 Help")) {
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
		DrawConsoleWindow();
	}

	if (showInfo) {
		ShowInformationWindow(io);
	}
	if (showModule) {
		ShowModulesWindow();
	}
	if (showAbout) {
		ImGui::Begin("About", &showAbout);
		ImGui::Text(u8"Engine: \uf534 Engine");
		ImGui::Text("Desciption: Super Cool Engine develop with love <3");
		ImGui::Text("Author: Artemis Georgakopoulou && Marco Rodriguez");
		ImGui::End();
	}
	ShowGizmosButtons();
	//ImGuizmo::ViewManipulate(App->camera->loadedCameras[0]->view.Transposed().ptr(), 1.f, ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);
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

const void  ModuleImGui::ShowModulesWindow() {
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
	if (ImGui::TreeNode("Render")) {
		ImGui::Checkbox("Show Grid", &(App->renderer->showGrid));
		ImGui::Checkbox("Show Axis", &(App->renderer->showAxis));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Input")) {
		ImGui::Text("Last key Pressed: %s", App->input->currentKey.c_str());
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

const void ModuleImGui::ShowInformationWindow(ImGuiIO& io) {
	ImGui::Begin("System Information", &showInfo);
	if (ImGui::TreeNode("Frame rate")) {
		fps_log.push_back(io.Framerate);
		sprintf_s(title, 25, "Framerate %.1F", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0F, 100.0F, ImVec2(310, 100));
		ms_log.push_back(1000.0F / io.Framerate);
		sprintf_s(title, 25, "Milliseconds %.1F", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0F, 40.0F, ImVec2(310, 100));
		ImGui::Text("Frames:  %d", App->timer->frameCount);
		sprintf(frameInfo, "Limiting to %d fps means each frame needs to take %f ms", App->timer->limitFPS, 1000.f / App->timer->limitFPS);
		ImGui::Text(frameInfo);
		ImGui::SliderInt("FPS", &App->timer->limitFPS, 30, 60);
		ImGui::Separator();
		if (ImGui::SliderFloat("Game Clock Scale", &App->timer->timeScale, 0.5, 2))
		{
			App->timer->SetTimeScale(App->timer->timeScale);
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode(u8"\uf2db Hardware")) {
		ImGui::Text("CPU cores: %d (Cache: %d Kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: %d Mb", SDL_GetSystemRAM());
		ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("GPU Model: %s", glGetString(GL_RENDERER));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode(u8"\uf02d Libraries")) {
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

const void ModuleImGui::ShowTextures(std::vector<Texture>& textures) {
	for (unsigned int i = 0; i < textures.size(); i++) {
		ImGui::Text("Name: %s", textures[i].name.c_str());
		ImGui::Text("Width: %d", textures[i].width);
		ImGui::Text("Heigth: %d", textures[i].height);
		if (ImGui::ImageButton((void*)(intptr_t)textures[i].id, ImVec2(128, 128))) {
			App->model->UpdateTexture(textures[i]);
		}
	}
}

const void ModuleImGui::DrawHierarchy(GameObject* object) {
	
	unsigned flags = ImGuiTreeNodeFlags_None;
	if (object->children.size() == 0) {
		flags = ImGuiTreeNodeFlags_Leaf;
	}
	flags |= selected.compare(object->uuid) == 0 ? ImGuiTreeNodeFlags_Selected : 0;
	if (ImGui::TreeNodeEx(object->name.c_str(), flags)) {
		if (ImGui::IsItemClicked()) {
			selected = object->uuid;
		}
		if (selected.compare(object->uuid) == 0 && showProperties) {
				object->ShowProperties(&showProperties);
				dd::axisTriad(((Transform*)object->FindComponent(ComponentType::Transform))->worldTransform, 0.125f, 1.25f, 0, false);
				selectedGO = object;
				//TODO---App->render->ShowGizmo(object);
		}
		if (ImGui::BeginDragDropTarget()) {
			if (ImGui::AcceptDragDropPayload("ITEM")) {
				sourceGO->parent->DeleteChild(sourceGO);
				sourceGO->parent = object;
				object->children.push_back(sourceGO);
				Transform* trans = (Transform*)object->FindComponent(ComponentType::Transform);
				trans->UpdateDirtyFlag();
				sourceGO = nullptr;
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource()) {
			sourceGO = object;
			ImGui::SetDragDropPayload("ITEM", nullptr, 0);
			ImGui::EndDragDropSource();
		}
		for (auto item : object->children) {
			DrawHierarchy(item);
		}
		ImGui::TreePop();
	}
}


const void ModuleImGui::DrawShaderProperties() {
	ImGui::Begin("Parameters");
	char* shape_names[int(ShapeType::Count)] = { "Sphere", "Torus", "Cube", "Cylinder"};
	if (ImGui::CollapsingHeader("Shape and material"))
	{
		for (unsigned i = 0; i < int(ShapeType::Count); ++i)
		{
			if (ImGui::RadioButton(shape_names[i], shape == int(ShapeType(i)))) {
				LoadShapes(ShapeType(i));
				shape = i;
			}
		}

		/*ImGui::ColorEdit4("object color", (float*)&material.object_color);
		ImGui::SliderFloat("shininess", &material.shininess, 0, 128.0f);
		ImGui::SliderFloat("K ambient", &material.k_ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("K diffuse", &material.k_diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("K specular", &material.k_specular, 0.0f, 1.0f);*/
	}

	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::SliderFloat3("light position", (float*)&App->model->light.pos, -15.0f, 15.0f);
		ImGui::SliderFloat("ambient", (float*)&App->model->ambient, 0.0f, 1.0f);
	}

	if (ImGui::CollapsingHeader("Options"))
	{
		/*ImGui::Checkbox("show axis", &show_axis);
		ImGui::Checkbox("show grid", &show_grid);
		ImGui::Checkbox("auto rotate", &auto_rotate);*/
	}
	ImGui::End();
}

void ModuleImGui::LoadShapes(ShapeType s) {
	
	GameObject* root = App->scene->root;
	root->children.clear();
	MeshShape shape;
	shape.type = s;
	shape.size = 0.5F;
	shape.radius = 1.0F;
	shape.slices = 30;
	shape.stacks = 30;
	App->model->LoadShapes(root, "shape0", float3(2.0F, 2.0F, 0.0F), Quat::identity, shape, ProgramType::Default, float4(1.0F, 1.0F, 1.0F, 1.0F));
	App->model->LoadShapes(root, "shape1", float3(5.0F, 2.0F, 0.0F), Quat::identity, shape, ProgramType::Default, float4(1.0F, 1.0F, 1.0F, 1.0F));
	App->model->LoadShapes(root, "shape2", float3(8.0F, 2.0F, 0.0F), Quat::identity, shape, ProgramType::Default, float4(1.0F, 1.0F, 1.0F, 1.0F));
	App->model->LoadShapes(root, "shape3", float3(11.0F, 2.0F, 0.0F), Quat::identity, shape, ProgramType::Default, float4(1.0F, 1.0F, 1.0F, 1.0F));
	App->model->LoadShapes(root, "shape4", float3(14.0F, 2.0F, 0.0F), Quat::identity, shape, ProgramType::Default, float4(1.0F, 1.0F, 1.0F, 1.0F));
}

const void ModuleImGui::DrawConsoleWindow()
{
	int consoleW = 1120;
	int consoleH = 270;
	ImGui::Begin("Console", &showConsole);
	ImGui::SetWindowSize(ImVec2(consoleW, consoleH));
	ImGui::SetWindowPos(ImVec2(0, App->window->screenHeight - consoleH));
	//ImVec2 size = ImGui::GetWindowSize();//to be able to resize it
	//if (size.x != consoleW && size.y != consoleH)
	//{
	//	consoleW = size.x;
	//	consoleH = size.y;
	//	ImGui::SetWindowSize(ImVec2(consoleW, consoleH));
	//}

	ImGui::TextUnformatted(buffer.begin());
	if (scrollToBottom)
		ImGui::SetScrollHere(1.0F);
	scrollToBottom = false;
	ImGui::End();
}


void ModuleImGui::ShowGizmosButtons()
{
	if (ImGui::Button(u8"\uf0b2"))
	{
		App->renderer->gizmoOperation = ImGuizmo::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"\uf021"))
	{
		App->renderer->gizmoOperation = ImGuizmo::ROTATE;
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"\uf31e"))
	{
		App->renderer->gizmoOperation = ImGuizmo::SCALE;
	}
	ImGui::SameLine(0.0f, 700.0f);
	if (ImGui::Button(u8"\uf04b"))
	{
		App->timer->Play();
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"\uf04c"))
	{
		App->timer->Pause();
		AddLog("Game Time: %f", App->timer->gameTime/1000);
		AddLog("Real Time: %f", App->timer->realTime/1000);
	}
}

