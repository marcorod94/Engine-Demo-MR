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
