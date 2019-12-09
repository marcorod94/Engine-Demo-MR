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
#include <string>

bool ModuleScene::Init() {

	root = CreateGameObject("Root Scene");
	App->model->LoadModel(std::string("Models/BakerHouse.fbx"));
	App->model->LoadSphere("sphere0", float3(1.0f, 1.0f, 1.0f), Quat::identity, 1.0f, 20, 20, float4(0.5f, 0.0f, 0.5f, 1.0f), root);
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
