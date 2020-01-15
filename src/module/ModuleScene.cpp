#include "main/Application.h"
#include "ModuleScene.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleModel.h"
#include "component/Mesh.h"
#include "component/Material.h"
#include "module/ModuleInput.h"
#include "module/ModuleRender.h"
#include "component/Camera.h"
#include <string>
#include "imgui.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>

bool ModuleScene::Init() {
	root = CreateGameObject("Root Scene");
	App->model->LoadModel(std::string("Models\\Zombunny.fbx"));
	/*MeshShape shape;
	shape.type = ShapeType::Torus;
	shape.size = 0.5F;
	shape.radius = 1.0F;
	shape.slices = 20;
	shape.stacks = 20;
	App->model->LoadShapes(root, "sphere0", float3(1.0f, 1.0f, 1.0f), Quat::identity, shape, ProgramType::Default, float4(0.5f, 0.0f, 0.5f, 1.0f));*/
    //sceneViewCamera
	Camera* cam = (Camera*) root->CreateComponent(ComponentType::Camera);
	//activeCamera
	GameObject* mainCamera = CreateGameObject("Main Camera");
	Camera* cam2 = (Camera*)mainCamera->CreateComponent(ComponentType::Camera);
	mainCamera->parent = root;
	root->children.push_back(mainCamera);
	return true;
}

update_status ModuleScene::PreUpdate() {
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update() {	
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp() {
	return true;
}


GameObject* ModuleScene::CreateGameObject(const std::string& name) const {
	return new GameObject(name);

}

GameObject* ModuleScene::CreateGameObject(const std::string& name, const float3& pos, const Quat& rot) const {
	return new GameObject(name, pos, rot);
}

void ModuleScene::PickObject(const ImVec2& winSize, const ImVec2& winPos)
{
	/*float2 mouse = App->input->GetMouseMotion();
	float2 normalizedPos = float2(mouse.x - winSize.x / 2, (winSize.y - mouse.y) - winSize.y / 2).Normalized();

	LineSegment ray;
	App->camera->loadedCameras[0]->CreateRay(normalizedPos, ray);

	GameObject* isIntersected = App->renderer->RayIntersectsObject(App->camera->loadedCameras[0]->frustum.pos, ray);*/
}

void ModuleScene::LoadScene() {
	delete root;
	root = new GameObject();
	std::ifstream ifs("example.json");
	rapidjson::IStreamWrapper isw(ifs);
	config.ParseStream(isw);
	assert(config.IsArray());
	for (auto& item : config.GetArray()) {
		root->OnLoad(&item.GetObjectA(), nullptr);
	}
	
}

void ModuleScene::SaveScene() {
	config.SetArray();
	root->OnSave(&config.GetArray(), &config.GetAllocator());
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	config.Accept(writer);
	App->imgui->AddLog("Test: %s", buffer.GetString());

	std::string json(buffer.GetString(), buffer.GetSize());
	std::ofstream of("example.json");
	of << json;
}
