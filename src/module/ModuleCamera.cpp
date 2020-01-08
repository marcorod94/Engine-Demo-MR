#include "main/Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "MathGeoLib.h"
#include "ModuleModel.h"
#include "main/Application.h"
#include "module/ModuleScene.h"
#include "module/ModuleProgram.h"
#include "component/Camera.h"
#include "SDL_scancode.h"
#include "SDL_mouse.h"


bool ModuleCamera::Init() {
	int windowWidth, windowHeight;
	SDL_GetWindowSize(App->window->window, &windowWidth, &windowHeight);
	/*sceneCamera = new Camera(App->scene->root);

	glBindBuffer(GL_UNIFORM_BUFFER, App->program->uniformsBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float4x4), &sceneCamera->proj[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, App->program->uniformsBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float4x4), sizeof(float4x4), &sceneCamera->view[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
	/*defaultCameraGO = new GameObject();
	sceneCamera = (Camera*)defaultCameraGO->CreateComponent(ComponentType::Camera);*/
	//sceneCamera->SetFarDistance();
	
	///////////////previous code
	//frustum.type = FrustumType::PerspectiveFrustum;
	//frustum.pos = float3(-1.0F, 1.0F, 1.0F);
	//frustum.front = -float3::unitZ;
	//frustum.up = float3::unitY;
	//frustum.nearPlaneDistance = 0.3F;
	//frustum.farPlaneDistance = 250.0F;
	//frustum.verticalFov = math::pi / 4.0F;
	////UpdateAspectRatio();
	//CalculateRotationAngles(frustum.front);
	sceneCameraGO = App->scene->CreateGameObject("Root Scene");
	//scene_cameraGO->transform.SetTranslation(float3(0.5f, 2.f, -15.f));
	sceneCamera = CreateComponentCamera();
	//scene_camera->SetFarDistance(500);
	return true;
}

Camera* ModuleCamera::CreateComponentCamera()
{
	sceneCamera = new Camera();
	loadedCameras.push_back(sceneCamera);
	return sceneCamera;
}//dont forget to create a remove component also

Camera* ModuleCamera::GetComponentCamera()
{
	return sceneCamera;
}//


update_status  ModuleCamera::PreUpdate() {
	sceneCamera->proj = sceneCamera->frustum.ProjectionMatrix();
	sceneCamera->model = float4x4::FromTRS(helper1, float3x3::RotateY(sceneCamera->frustum.verticalFov), helper2);
	sceneCamera->view = LookAt(sceneCamera->frustum.pos, sceneCamera->frustum.pos + sceneCamera->frustum.front - cameraTarget, sceneCamera->frustum.up);
	return UPDATE_CONTINUE;
}

update_status  ModuleCamera::Update() {
	/*for (int i = 0; i<loadedCameras.size(); i++)
	{
		if (App->input->GetKey(SDL_SCANCODE_W)) {

			loadedCameras[i]->frustum.pos += movementSpeed * loadedCameras[i]->frustum.front;
		}
	}*/
	if(sceneCamera->isHovered == true)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
			MouseMove();
			if (App->input->GetKey(SDL_SCANCODE_W)) {
				sceneCamera->frustum.pos += movementSpeed * sceneCamera->frustum.front;
			}
			if (App->input->GetKey(SDL_SCANCODE_S)) {
				sceneCamera->frustum.pos -= movementSpeed * sceneCamera->frustum.front;
			}
			if (App->input->GetKey(SDL_SCANCODE_A)) {
				sceneCamera->frustum.pos -= movementSpeed * (sceneCamera->frustum.front.Cross(sceneCamera->frustum.up)).Normalized();
			}
			if (App->input->GetKey(SDL_SCANCODE_D)) {
				sceneCamera->frustum.pos += movementSpeed * (sceneCamera->frustum.front.Cross(sceneCamera->frustum.up)).Normalized();
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT)) {
			movementSpeed = cameraSpeed * 2;
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && (App->input->GetKey(SDL_SCANCODE_LALT) || App->input->GetKey(SDL_SCANCODE_RALT))) {
			orbit = true;
			MouseMove();
		}
		if (App->input->GetKey(SDL_SCANCODE_F)) {
			Focus();
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE)) {
			MouseScrolling();
		}
		movementSpeed = cameraSpeed;
		orbit = false;
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
void ModuleCamera::MouseMove()
{
	float2 offset = App->input->GetMouseMotion();

	float sensitivity = 0.1F;
	offset.x *= sensitivity;
	offset.y *= sensitivity;

	yaw += offset.x;
	pitch += offset.y;

	
	if (orbit == false) {
		if (pitch > 89.0F)
			pitch = 89.0F;
		if (pitch < -89.0F)
			pitch = -89.0F; 

	}

	float3 direction;
	direction.x = cos(DegToRad(yaw)) * cos(DegToRad(pitch));
	direction.y = sin(DegToRad(pitch));
	direction.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));
	if (orbit) {
		/*frustum.pos = App->model->box.CenterPoint() - App->model->box.Size().Normalize() * direction.Normalized();*/
		cameraTarget = sceneCamera->frustum.pos + sceneCamera->frustum.front;
	} else {
		cameraTarget = float3::zero;
		sceneCamera->frustum.front = direction.Normalized();
	}
}


void ModuleCamera::MouseScrolling()
{
	float2 offset = App->input->GetMouseScroll();
	if (offset.y > 0) {
		sceneCamera->frustum.pos += cameraSpeed * sceneCamera->frustum.front;
	}
	if (offset.y < 0) {
		sceneCamera->frustum.pos -= cameraSpeed * sceneCamera->frustum.front;
	}
}
void ModuleCamera::SetFOV(float fov)
{
	sceneCamera->frustum.verticalFov = fov;
	//UpdateAspectRatio();
	sceneCamera->proj = sceneCamera->frustum.ProjectionMatrix();
}

void ModuleCamera::UpdateAspectRatio(float w, float h) {
	sceneCamera->frustum.horizontalFov = 2.0F * atanf(tanf(sceneCamera->frustum.verticalFov * 0.5F) * w / h);
}

void ModuleCamera::SetNearDistance(const float nearDist)
{
	sceneCamera->frustum.nearPlaneDistance = nearDist;
	sceneCamera->proj = sceneCamera->frustum.ProjectionMatrix();
}

void ModuleCamera::SetFarDistance(const float farDist)
{
	sceneCamera->frustum.farPlaneDistance = farDist;
	sceneCamera->proj = sceneCamera->frustum.ProjectionMatrix();
}

void ModuleCamera::CalculateRotationAngles(float3& vector) {
	pitch = -RadToDeg(asin(vector.y));
	yaw = -RadToDeg(acos(vector.x / cos(DegToRad(pitch))));
}

void ModuleCamera::Focus() {
	/*cameraTarget = float3::zero;
	sceneCamera->frustum.front = -float3::unitZ;
	CalculateRotationAngles(sceneCamera->frustum.front);*/
	//sceneCamera->frustum.pos = App->model->box.CenterPoint() - App->model->box.Size().Normalize() * sceneCamera->frustum.front;
}

void ModuleCamera::ZoomIn()
{
	sceneCamera->frustum.pos += cameraSpeed * sceneCamera->frustum.front;
}

void ModuleCamera::ZoomOut()
{
	sceneCamera->frustum.pos -= cameraSpeed * sceneCamera->frustum.front;
}

bool ModuleCamera::CleanUp()
{
	delete sceneCamera;
	//sceneCamera = nullptr;
	return true;
}

void ModuleCamera::SetPosition(const math::float3& pos)
{

}
