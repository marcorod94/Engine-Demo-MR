#include "Application.h"
#include "ModuleTriangle.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"

ModuleTriangle::ModuleTriangle() {

}

ModuleTriangle::~ModuleTriangle() {

}

bool ModuleTriangle::Init() {
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * 2.f);
	proj = frustum.ProjectionMatrix();
	model =
			float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f),
			float3x3::RotateY(math::pi / 4.0f), float3(1.0f,
				1.0f, 1.0f));
	view =
		LookAt(float3(0.0f, 1.f, 4.0f),
			float3(0.0f, 0.0f, 0.0f), float3(0.0f,
				1.0f, 0.0f));
	float4x4 transform = proj * view * float4x4(model);
	float4 v1 = transform * float4(-1.0f, -1.0f, 0.0f, 1.0f);
	float4 v2 = transform * float4(1.0f, -1.0f, 0.0f, 1.0f);
	float4 v3 = transform * float4(0.0f, 1.0f, 0.0f, 1.0f);

	v1 = v1 / v1.w;
	v2 = v2 / v2.w;
	v3 = v3 / v3.w;
	
	float buffer_data[] = { v1.x, v1.y, v1.z,
		 v2.x, v2.y, v2.z,
		 v3.x, v3.y, v3.z };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	return true;
}

update_status ModuleTriangle::PreUpdate()
{
	App->program->LoadShaders("default.vs", "default.fs");
	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::Update() {
	glUseProgram(App->program->program);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "proj"), 1, GL_TRUE, &proj[0][0]);
	glEnableVertexAttribArray(0); // attribute 0
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	return UPDATE_CONTINUE;
}

bool ModuleTriangle::CleanUp() {
	return true;
}

float4x4 ModuleTriangle::LookAt(float3 eye, float3 target, float3 up) {
	float3 f(target - eye); f.Normalize();
	float3 s(f.Cross(up)); s.Normalize();
	float3 u(s.Cross(f));
	float4x4 matrix;
	matrix.At(0,0) = s.x; matrix.At(0,1) = s.y; matrix.At(0,2) = s.z;
	matrix.At(1,0) = u.x; matrix.At(1,1) = u.y; matrix.At(1,2) = u.z;
	matrix.At(2,0) = -f.x; matrix.At(2,1) = -f.y; matrix.At(2,2) = -f.z;
	matrix.At(0,3) = -s.Dot(eye); matrix.At(1,3) = -u.Dot(eye); matrix.At(2,3) = f.Dot(eye);
	matrix.At(3,0) = 0.0f; matrix.At(3,1) = 0.0f; matrix.At(3,2) = 0.0f; matrix.At(3,3) = 1.0f;
	return matrix;
}
