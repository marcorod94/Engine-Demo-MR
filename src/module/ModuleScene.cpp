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
#include "component/Camera.h"
#include <string>

bool ModuleScene::Init() {

	root = CreateGameObject("Root Scene");
	App->model->LoadModel(std::string("Models/BakerHouse.fbx"));
	MeshShape shape;
	shape.type = ShapeType::Torus;
	shape.size = 0.5F;
	shape.radius = 1.0F;
	shape.slices = 20;
	shape.stacks = 20;
	App->model->LoadShapes(root, "sphere0", float3(1.0f, 1.0f, 1.0f), Quat::identity, shape, ProgramType::Default, float4(0.5f, 0.0f, 0.5f, 1.0f));
	Camera* cam = App->camera->CreateComponentCamera();
	cam->owner = root;
	root->components.push_back(cam);
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
