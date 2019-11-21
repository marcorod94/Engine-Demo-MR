#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "SDL_scancode.h"
#include "SDL_mouse.h"


ModuleCamera::ModuleCamera() {}

ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::Init() {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(-1.0F, 1.0F, 1.0F);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.3F;
	frustum.farPlaneDistance = 250.0F;
	frustum.verticalFov = math::pi / 4.0F;
	frustum.horizontalFov = 2.0F * atanf(tanf(frustum.verticalFov * 0.5F) * 2.F);
	pitch = -RadToDeg(asin(frustum.front.y));
	yaw = -RadToDeg(acos(frustum.front.x / cos(DegToRad(pitch))));
	return true;
}

update_status  ModuleCamera::PreUpdate() {
	proj = frustum.ProjectionMatrix();
	model =
		float4x4::FromTRS(helper1,
			float3x3::RotateY(frustum.verticalFov), helper2);
	view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	return UPDATE_CONTINUE;
}

update_status  ModuleCamera::Update() {
	movementSpeed = cameraSpeed;
	orbit = false;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT)) {
		movementSpeed = cameraSpeed * 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_LALT) || App->input->GetKey(SDL_SCANCODE_RALT)) {
		orbit = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_F)) {
		frustum.pos = float3(0.0F, 0.0F, 0.0F);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)) {
		MouseMove();
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE)) {
		MouseScrolling();
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
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

float4x4 ModuleCamera::LookAt(float3 eye, float3 target, float3 up) {
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

	float sensitivity = 0.1F; // change this value to your liking
	offset.x *= sensitivity;
	offset.y *= sensitivity;

	yaw += offset.x;
	pitch += offset.y;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0F)
		pitch = 89.0F;
	if (pitch < -89.0F)
		pitch = -89.0F;

	float3 front;
	front.x = cos(DegToRad(yaw)) * cos(DegToRad(pitch));
	front.y = sin(DegToRad(pitch));
	front.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));
	frustum.front = front.Normalized();
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