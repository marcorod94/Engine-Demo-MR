#include "main/Globals.h"
#include "main/Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleImGui.h"
#include "ModuleProgram.h"
#include "ModuleScene.h"
#include "ModuleCamera.h"
#include "ModuleModel.h"
#include "util/DebugDraw.h"
#include "ModuleDebugDraw.h"
#include "main/GameObject.h"
#include "component/Mesh.h"
#include "component/Material.h"
#include "component/Camera.h"
#include "component/Transform.h"
#include "SDL.h"
#include "GL/glew.h"
#include <map>

// Called before render is available
bool ModuleRender::Init()
{
	App->imgui->AddLog("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	glContext = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0F);

	glClearColor(0.F, 0.F, 0.F, 1.F);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	return true;
}

update_status ModuleRender::PreUpdate()
{
	SDL_GetWindowSize(App->window->window, &App->window->screenWidth, &App->window->screenHeight);
	glViewport(0, 0, App->window->screenWidth, App->window->screenHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{	
	Camera* cam = nullptr;
	if (App->scene->root) {
		for (unsigned i = 0; i < App->camera->loadedCameras.size(); i++) {
			cam = App->camera->loadedCameras[i];
			cam->GenerateFBOTexture(cam->width, cam->height);
			glBindFramebuffer(GL_FRAMEBUFFER, cam->fbo);
			glViewport(0, 0, cam->width, cam->height);
			glClearColor(0.2f, 0.2f, 0.2f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			DrawGameObject(App->scene->root, cam);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			if (showAxis) {
				glBindFramebuffer(GL_FRAMEBUFFER, cam->fbo);
				float axis_size = max(1.0f, 1.0f);
				/*dd::axisTriad(math::float4x4::identity, axis_size*0.125f, axis_size*1.25f, 0, false);*/
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			if (showGrid) {
				glBindFramebuffer(GL_FRAMEBUFFER, cam->fbo);
				dd::xzSquareGrid(-40.0f, 40.0f, 0.0f, 1.0f, math::float3(0.65f, 0.65f, 0.65f));
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			cam->DrawFrustumPlanes();
			App->debugDraw->Draw(cam);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	App->imgui->AddLog("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(glContext);

	return true;
}

Mesh* ModuleRender::CreateMesh(GameObject* owner) {
	return new Mesh(owner);
}


void  ModuleRender::DrawGameObject(GameObject* parent, Camera* cam) {
	DrawMesh(cam, (Transform*)parent->FindComponent(ComponentType::Transform), (Mesh*)parent->FindComponent(ComponentType::Mesh), (Material*)parent->FindComponent(ComponentType::Material));
	for (unsigned i = 0; i < parent->children.size(); i++) {
		DrawGameObject(parent->children[i], cam);
	}
}

void  ModuleRender::DrawMesh(Camera* cam, Transform* trans, Mesh* mesh, Material* material) {
	unsigned program = App->program->programs[int(ProgramType::Default)];
	glUseProgram(program);
	if (material) {
		program = App->program->programs[material->program];
		glUseProgram(program);
		glUniform3fv(glGetUniformLocation(program, "light_pos"), 1, (const float*)&App->model->light.pos);
		glUniform1f(glGetUniformLocation(program, "ambient"), App->model->ambient);
		glUniform1f(glGetUniformLocation(program, "shininess"), material->shininess);
		glUniform1f(glGetUniformLocation(program, "k_ambient"), material->kAmbient);
		glUniform1f(glGetUniformLocation(program, "k_diffuse"), material->kDiffuse);
		glUniform1f(glGetUniformLocation(program, "k_specular"), material->kSpecular);
		if (material->diffuseMap == 0) {
			glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 0);
			glUniform4fv(glGetUniformLocation(program, "object_color"), 1, (const float*)&material->diffuseColor);
		}
		else {
			glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->diffuseMap);
			glUniform1i(glGetUniformLocation(program, "diffuse_map"), 0);
		}
	}
	if (trans) {
		trans->CalculateTransform();
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &(trans->worldTransform[0][0]));
	}

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &(cam->view[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &(cam->proj[0][0]));
	if (mesh) {
		if (trans->isDirty) {
			mesh->TransformAABB(&trans->worldTransform);
			trans->isDirty = false;
		}
		if(cam->isCollidingFrustum(mesh->box) == IS_IN) {
			dd::aabb(mesh->box.minPoint, mesh->box.maxPoint, float3(0, 0, 1));
			glBindVertexArray(mesh->vao);
			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);
		}
	}
}

GameObject* ModuleRender::RayIntersectsObject(float3 origin, LineSegment &ray)
{
	LineSegment localRay = ray;
	Triangle tri;
	bool hit_point;
	std::map<float, GameObject*> intersected;
	std::map<float, GameObject*>::iterator it;
	std::vector<Mesh*> intersectedMshes;
	GameObject* selected = nullptr;
	float distance = 1000.0f;

	for (unsigned i = 0; i < App->scene->root->children.size(); i++) {
		Mesh* mesh = (Mesh*)App->scene->root->children[i]->FindComponent(ComponentType::Mesh);
		Transform* trans = (Transform*)App->scene->root->children[i]->FindComponent(ComponentType::Transform);
		if (mesh != nullptr)
		{
			localRay.Transform(trans->worldTransform.Inverted());
			
			hit_point = localRay.Intersects(mesh->box); // ray vs. AABB
			if (hit_point)
			{
				float dist = origin.Distance(mesh->box);
				intersected[dist] = mesh->owner;
				intersectedMshes.push_back(mesh);
			}

			for (std::pair<float, GameObject*> element : intersected) {
				if (element.first < distance)
				{
					distance = element.first;
					selected = element.second;
				}
			}
			Mesh* minDistMesh = nullptr;
			if (selected != nullptr)
			{
				minDistMesh = (Mesh*)selected->FindComponent(ComponentType::Mesh);
				showAxis = true;
				dd::axisTriad(((Transform*)selected->FindComponent(ComponentType::Transform))->worldTransform, 0.125f, 1.25f, 0, false);
			}
			
			if (minDistMesh != nullptr)
			{
				for (unsigned int i = 0; i < minDistMesh->indices.size() - 2; i++)
				{
					tri = Triangle(mesh->vertices[mesh->indices[i]].Position, mesh->vertices[mesh->indices[i + 1]].Position, mesh->vertices[mesh->indices[i + 2]].Position);
					bool hit = tri.Intersects(localRay);
					if (hit)
					{
						distance = tri.Distance(origin);
					}//TODOfix changing the object
				}
			}
		}
	}
	if (intersected.size() == 0)
	{
		return nullptr;
	}
	
	return selected;
}
