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
#include "util/DebugDraw.h"
#include "imgui.h"

bool ModuleScene::Init() {

	root = CreateGameObject("Root Scene");
	App->model->LoadModel(std::string("Models\\Zombunny.fbx"));
	MeshShape shape;
	shape.type = ShapeType::Torus;
	shape.size = 0.5F;
	shape.radius = 1.0F;
	shape.slices = 20;
	shape.stacks = 20;
	App->model->LoadShapes(root, "sphere0", float3(1.0f, 1.0f, 1.0f), Quat::identity, shape, ProgramType::Default, float4(0.5f, 0.0f, 0.5f, 1.0f));
    //sceneViewCamera
	Camera* cam = App->camera->CreateComponentCamera();
	cam->owner = root;
	root->components.push_back(cam);
	//activeCamera
	GameObject* mainCamera = CreateGameObject("Main Camera");
	Camera* cam2 = App->camera->CreateComponentCamera();
	cam2->owner = mainCamera;
	mainCamera->parent = root;
	mainCamera->components.push_back(cam2);
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
	int mouseX, mouseY;
	SDL_GetGlobalMouseState(&mouseX, &mouseY);
	//float2 normalizedPos = float2(mouseX - winSize.x / 2, (winSize.y - mouseY) - winSize.y / 2).Normalized();
	//float2 normalizedPos = float2((mouseX - (winPos.x - winSize.x) / 2) - winPos.x, ((winSize.y - winPos.y) / 2 - mouseY) - winPos.y).Normalized();
	//float2 normalizedPos = float2(mouseX - winSize.x / 2, - mouseY / 2).Normalized();
	//(mouseX - (winPos.x + (winSize.x / 2)) / (winSize.x / 2), ((winPos.y + (winSize.y / 2)) - mouseY) / (winSize.x / 2))
	App->imgui->AddLog("Mouse X: %d, Y: %d", mouseX, mouseY);
	float2 normalizedPos = float2((mouseX - (winPos.x + (winSize.x / 2))) / (winSize.x / 2), ((winPos.y + (winSize.y / 2)) - mouseY) / (winSize.x / 2));
	App->imgui->AddLog("X: %.2F, Y: %.2F", normalizedPos.x, normalizedPos.y);
	LineSegment ray;
	App->camera->loadedCameras[1]->CreateRay(normalizedPos, ray);

	GameObject* isIntersected = App->renderer->RayIntersectsObject(App->camera->loadedCameras[1]->frustum.pos, ray);
	//dd::arrow(ray.a, ray.b, float3(0,0,1), 10.0f);
	dd::line(ray.a, ray.b, float3(0, 0, 1));
	if (isIntersected != nullptr)
	{
		//App->imgui->AddLog("Selecting object: %s", isIntersected->name);
		App->imgui->selected = isIntersected->uuid;
	}
	else
	{
		return;
	}
	
}
