#include "main/Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "MathGeoLib.h"
#include "ModuleModel.h"
#include "main/Application.h"
#include "module/ModuleScene.h"
#include "module/ModuleProgram.h"
#include "module/ModuleImGui.h"
#include "component/Camera.h"
#include "component/Mesh.h"
#include "SDL_scancode.h"
#include "SDL_mouse.h"

bool ModuleCamera::Init() {
	int windowWidth;
	int windowHeight;
	SDL_GetWindowSize(App->window->window, &windowWidth, &windowHeight);
	return true;
}

Camera* ModuleCamera::CreateComponentCamera(GameObject* owner)
{
	Camera* cam = new Camera(owner);
	loadedCameras.push_back(cam);
	return cam;
}//dont forget to create a remove component also

update_status  ModuleCamera::PreUpdate() {
	for (auto cam : loadedCameras)
	{
		cam->proj = cam->frustum.ProjectionMatrix();

		cam->model = float4x4::FromTRS(helper1, float3x3::RotateY(cam->frustum.verticalFov), helper2);
		cam->view = LookAt(cam->frustum.pos, cam->frustum.front + cam->frustum.pos, cam->frustum.up);
		UpdateAspectRatio(cam, cam->width, cam->height);
	}
	return UPDATE_CONTINUE;
}

update_status  ModuleCamera::Update() {
	for (auto cam : loadedCameras)
	{
		if (cam->isHovered)
		{
			orbit = false;
			if (App->input->GetKey(SDL_SCANCODE_W)) {

				cam->frustum.pos += movementSpeed * cam->frustum.front;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
				MouseMove(cam);
				if (App->input->GetKey(SDL_SCANCODE_W)) {
					cam->frustum.pos += movementSpeed * cam->frustum.front;
				}
				if (App->input->GetKey(SDL_SCANCODE_S)) {
					cam->frustum.pos -= movementSpeed * cam->frustum.front;
				}
				if (App->input->GetKey(SDL_SCANCODE_A)) {
					cam->frustum.pos -= movementSpeed * (cam->frustum.front.Cross(cam->frustum.up)).Normalized();
				}
				if (App->input->GetKey(SDL_SCANCODE_D)) {
					cam->frustum.pos += movementSpeed * (cam->frustum.front.Cross(cam->frustum.up)).Normalized();
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT)) {
				movementSpeed = cameraSpeed * 2;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && (App->input->GetKey(SDL_SCANCODE_LALT) || App->input->GetKey(SDL_SCANCODE_RALT))) {
				orbit = true;
				MouseMove(cam);
			}
			if (App->input->GetKey(SDL_SCANCODE_F)) {
				Focus(cam);
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE)) {
				MouseScrolling(cam);
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)) {
			
				App->scene->PickObject(loadedCameras[0]->hoveredWindowSize, loadedCameras[0]->hoveredWindowPos);
			
			}
			movementSpeed = cameraSpeed;
			
			UpdateAspectRatio(cam, cam->width, cam->height);
		}
	}
	return UPDATE_CONTINUE;
}

float4x4 ModuleCamera::LookAt(float3& eye, float3& target, float3& up) const {
	float3 f(target - eye); f.Normalize();
	float3 s(f.Cross(up)); s.Normalize();
	float3 u(s.Cross(f));
	float4x4 matrix;
	matrix.At(0, 0) = s.x; matrix.At(0, 1) = s.y; matrix.At(0, 2) = s.z;
	matrix.At(1, 0) = u.x; matrix.At(1, 1) = u.y; matrix.At(1, 2) = u.z;
	matrix.At(2, 0) = -f.x; matrix.At(2, 1) = -f.y; matrix.At(2, 2) = -f.z;
	matrix.At(0, 3) = -s.Dot(eye); matrix.At(1, 3) = -u.Dot(eye); matrix.At(2, 3) = f.Dot(eye);
	matrix.At(3, 0) = 0.0F; matrix.At(3, 1) = 0.0F; matrix.At(3, 2) = 0.0F; matrix.At(3, 3) = 1.0F;
	return matrix;
}
void ModuleCamera::MouseMove(Camera* cam)
{
	float2 offset = App->input->GetMouseMotion();

	offset.x *= sensitivity;
	offset.y *= sensitivity;

	cam->yaw += offset.x;
	cam->pitch += offset.y;
	
	if (orbit == false) {
		if (cam->pitch > 80.0F)
			cam->pitch = 80.0F;
		if (cam->pitch < -80.0F)
			cam->pitch = -80.0F;
		UpdateAspectRatio(cam, cam->width, cam->height);
	}

	float3 direction;
	direction.x = cos(DegToRad(cam->yaw)) * cos(DegToRad(cam->pitch));
	direction.y = sin(DegToRad(cam->pitch));
	direction.z = sin(DegToRad(cam->yaw)) * cos(DegToRad(cam->pitch));
	if (orbit) {
		cameraTarget = cam->frustum.pos + cam->frustum.front;
	} else {
		cam->frustum.front = direction.Normalized();
	}
	UpdateAspectRatio(cam, cam->width, cam->height);
}


void ModuleCamera::MouseScrolling(Camera* cam)
{
	float2 offset = App->input->GetMouseScroll();
	if (offset.y > 0) {
		cam->frustum.pos += cameraSpeed * cam->frustum.front;
	}
	if (offset.y < 0) {
		cam->frustum.pos -= cameraSpeed * cam->frustum.front;
	}
}
void ModuleCamera::SetFOV(Camera* cam, float fov)
{
	cam->frustum.verticalFov = fov;
	cam->proj = cam->frustum.ProjectionMatrix();
	
}

void ModuleCamera::UpdateAspectRatio(Camera* cam, float w, float h) 
{
	cam->frustum.horizontalFov = 2.0F * atanf(tanf(cam->frustum.verticalFov * 0.5F) * w / h);
}

void ModuleCamera::SetNearDistance(Camera* cam, const float nearDist)
{
	cam->frustum.nearPlaneDistance = nearDist;
	cam->proj = cam->frustum.ProjectionMatrix();
}

void ModuleCamera::SetFarDistance(Camera* cam, const float farDist)
{
	cam->frustum.farPlaneDistance = farDist;
	cam->proj = cam->frustum.ProjectionMatrix();
}

void ModuleCamera::Focus(Camera* cam) {
	Mesh* mesh = (Mesh*)App->imgui->selectedGO->FindComponent(ComponentType::Mesh);
	if (mesh == nullptr && App->imgui->selectedGO->children.size()> 0)
	{
		 mesh = (Mesh*)App->imgui->selectedGO->children[0]->FindComponent(ComponentType::Mesh);
	}
	if (mesh != nullptr)
	{
		float3 size = mesh->box.Size();
		float3 center = (mesh->box.maxPoint - mesh->box.minPoint) / 2;

		float3 direction = (center - cam->frustum.pos).Normalized();
		float3x3 rotationMatrix = float3x3::LookAt(cam->frustum.front, direction, cam->frustum.up, float3::unitY);

		cam->frustum.pos = mesh->box.CenterPoint() - mesh->box.Size().Normalize() * (cam->frustum.front);
		cam->frustum.pos.y = (size.Length() / 4);
		cam->frustum.pos.z -= 10;
		cam->frustum.front = rotationMatrix * cam->frustum.front;
		cam->frustum.up = float3::unitY;
		cam->view = cam->frustum.ViewMatrix();
	}
}

void ModuleCamera::ZoomIn(Camera* cam)
{
	cam->frustum.pos += cameraSpeed * cam->frustum.front;
}

void ModuleCamera::ZoomOut(Camera* cam)
{
	cam->frustum.pos -= cameraSpeed * cam->frustum.front;
}

bool ModuleCamera::CleanUp()
{
	for (auto item : loadedCameras) {
		delete item;
	}
	loadedCameras.clear();
	return true;
}


