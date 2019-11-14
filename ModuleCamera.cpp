#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "SDL_scancode.h"


ModuleCamera::ModuleCamera() {}

ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::Init() {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.3F;
	frustum.farPlaneDistance = 250.0F;
	frustum.verticalFov = math::pi / 4.0F;
	frustum.horizontalFov = 2.0F * atanf(tanf(frustum.verticalFov * 0.5F) * 2.F);
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
	if (App->input->GetKey(SDL_SCANCODE_W)) {
		frustum.pos += cameraSpeed * frustum.front;
	}
	if (App->input->GetKey(SDL_SCANCODE_S)) {
		frustum.pos -= cameraSpeed * frustum.front;
	}
	if (App->input->GetKey(SDL_SCANCODE_A)) {
		frustum.pos -= cameraSpeed * (frustum.front.Cross(frustum.up)).Normalized();
	}
	if (App->input->GetKey(SDL_SCANCODE_D)) {
		frustum.pos += cameraSpeed * (frustum.front.Cross(frustum.up)).Normalized();
	}
	if (App->input->IsMouseDown()) {
		MouseMove();
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
	if (frustum.verticalFov >= 1.0f && frustum.verticalFov <= 45.0f)
		frustum.verticalFov -= offset.y;
	if (frustum.verticalFov <= 1.0f)
		frustum.verticalFov = 1.0f;
	if (frustum.verticalFov >= 45.0f)
		frustum.verticalFov = 45.0f;
}