#include "main/Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleModel.h"
#include "SDL_scancode.h"
#include "SDL_mouse.h"

bool ModuleCamera::Init() {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(-1.0F, 1.0F, 1.0F);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.3F;
	frustum.farPlaneDistance = 250.0F;
	frustum.verticalFov = math::pi / 4.0F;
	UpdateAspectRatio();
	CalculateRotationAngles(frustum.front);
	return true;
}

update_status  ModuleCamera::PreUpdate() {
	UpdateAspectRatio();
	proj = frustum.ProjectionMatrix();
	model =
		float4x4::FromTRS(helper1,
			float3x3::RotateY(frustum.verticalFov), helper2);
	view = LookAt(frustum.pos, frustum.pos + frustum.front - cameraTarget, frustum.up);
	return UPDATE_CONTINUE;
}

update_status  ModuleCamera::Update() {
	movementSpeed = cameraSpeed;
	orbit = false;
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
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
		MouseMove();
		if (App->input->GetKey(SDL_SCANCODE_W)) {
			frustum.pos += movementSpeed * frustum.front;
		}
		if (App->input->GetKey(SDL_SCANCODE_S)) {
			frustum.pos -= movementSpeed * frustum.front;
		}
		if (App->input->GetKey(SDL_SCANCODE_A)) {
			frustum.pos -= movementSpeed * (frustum.front.Cross(frustum.up)).Normalized();
		}
		if (App->input->GetKey(SDL_SCANCODE_D)) {
			frustum.pos += movementSpeed * (frustum.front.Cross(frustum.up)).Normalized();
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
		frustum.pos = App->model->box.CenterPoint() - App->model->box.Size().Normalize() * direction.Normalized();
		cameraTarget = frustum.pos + frustum.front;
	} else {
		cameraTarget = float3::zero;
		frustum.front = direction.Normalized();
	}
}


void ModuleCamera::MouseScrolling()
{
	float2 offset = App->input->GetMouseScroll();
	if (offset.y > 0) {
		frustum.pos += cameraSpeed * frustum.front;
	}
	if (offset.y < 0) {
		frustum.pos -= cameraSpeed * frustum.front;
	}
}

void ModuleCamera::UpdateAspectRatio() {
	frustum.horizontalFov = 2.0F * atanf(tanf(frustum.verticalFov * 0.5F) * App->window->screenWidth / App->window->screenHeight);
}

void ModuleCamera::CalculateRotationAngles(float3& vector) {
	pitch = -RadToDeg(asin(vector.y));
	yaw = -RadToDeg(acos(vector.x / cos(DegToRad(pitch))));
}

void ModuleCamera::Focus() {
	cameraTarget = float3::zero;
	frustum.front = -float3::unitZ;
	CalculateRotationAngles(frustum.front);
	frustum.pos = App->model->box.CenterPoint() - App->model->box.Size().Normalize() * frustum.front;
}
