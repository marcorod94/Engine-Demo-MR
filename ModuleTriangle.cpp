﻿#include "Application.h"
#include "ModuleTriangle.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"

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
	frustum.nearPlaneDistance = 0.3f;
	frustum.farPlaneDistance = 250.0f;
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

	float4 v4 = transform * float4(0.5f, -0.5f, 0.0f, 1.0f);
	float4 v5 = transform * float4(-0.5f, -0.5f, 0.0f, 1.0f);
	float4 v6 = transform * float4(-0.5f, 0.5f, 0.0f, 1.0f);

	v1 = v1 / v1.w;
	v2 = v2 / v2.w;
	v3 = v3 / v3.w;

	v4 = v4 / v4.w;
	v5 = v5 / v5.w;
	v6 = v6 / v6.w;
	
	//float vertices[] = { v1.x, v1.y, v1.z,
	//	 v2.x, v2.y, v2.z,
	//	 v3.x, v3.y, v3.z,
	//	/*v4.x, v4.y, v4.z,
	//	 v5.x, v5.y, v5.z,
	//	 v6.x, v6.y, v6.z,*/
	//		0.0f, 0.0f,
	//		1.0f, 0.0f,
	//		0.5f, 1.0f
	//};
	//

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, // ← v0 pos
			1.0f, -1.0f, 0.0f, // ← v1 pos
			0.0f, 1.0f, 0.0f, // ← v2 pos
			0.0f, 0.0f, // ← v0 texcoord
			1.0f, 0.0f, // ← v1 texcoord
			0.5f, 1.0f // ← v2 texcoord
	};
	//float vertices[] = {
	//	 1.0f,  1.0f, 0.0f,  // top right
	//	 1.0f, -1.0f, 0.0f,  // bottom right
	//	-1.0f, -1.0f, 0.0f,  // bottom left
	//	-1.0f,  1.0f, 0.0f   // top left 
	//};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	//glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(3 * 3 * sizeof(float)));
	//float vertices[] = {
	//	// positions          // colors           // texture coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	//};
	//unsigned int indices[] = {
	//	0, 1, 3, // first triangle
	//	1, 2, 3  // second triangle
	//};
	//glGenVertexArrays(1, &vao);
	//glGenBuffers(1, &vbo);
	//glGenBuffers(1, &ebo);

	//glBindVertexArray(vao);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//// texture coord attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);
	App->program->LoadShaders("default.vs", "default.fs");
	App->texture->LoadTexture("Lenna.png");
	return true;
}

update_status ModuleTriangle::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::Update() {
	glUseProgram(App->program->program);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "proj"), 1, GL_TRUE, &proj[0][0]);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	return UPDATE_CONTINUE;
}

bool ModuleTriangle::CleanUp() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
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
