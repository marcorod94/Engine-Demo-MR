#include "main/Globals.h"
#include "main/Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleImGui.h"
#include "ModuleProgram.h"
#include "ModuleScene.h"
#include "ModuleCamera.h"
#include "ModuleTimer.h"
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
	App->timer->Start();
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
			cam->GenerateFBOTexture((unsigned)cam->width, (unsigned)cam->height);
			
			glBindFramebuffer(GL_FRAMEBUFFER, cam->fbo);
			glViewport(0, 0, (GLsizei)cam->width, (GLsizei)cam->height);
			glClearColor(0.2f, 0.2f, 0.2f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			DrawGameObject(App->scene->root, cam);
			DrawAABB(App->scene->root);
			App->scene->DrawAABBTree();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			if (showAxis) {
				glBindFramebuffer(GL_FRAMEBUFFER, cam->fbo);
				float axis_size = max(1.0f, 1.0f);
				
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
	++App->timer->frameCount;
	App->timer->End();
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
		if (material->diffuseTex) {
			glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->diffuseTex->id);
			glUniform1i(glGetUniformLocation(program, "diffuse_map"), 0);
		}
		else {
			glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 0);
			glUniform4fv(glGetUniformLocation(program, "object_color"), 1, (const float*)&material->diffuseColor);
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
		if (cam->isCollidingFrustum(mesh->box) == IS_IN ||
			cam->isCollidingFrustum(mesh->box) == INTERSECT) {
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
	std::map<float, GameObject*> intersected;
	std::map<float, GameObject*>::iterator it;
	std::vector<Mesh*> intersectedMshes;
	GameObject* selected = nullptr;

	selected = SearchGO(App->scene->root, &origin, &ray, &intersected);
	Mesh* minDistMesh = nullptr;
	Transform* myTrans = nullptr;
	if (selected != nullptr)
	{
		minDistMesh = (Mesh*)selected->FindComponent(ComponentType::Mesh);
		myTrans = (Transform*)selected->FindComponent(ComponentType::Transform);
		showAxis = true;
		dd::axisTriad(myTrans->worldTransform, 0.2f, 2.25f, 0, true);
		//DrawGizmo(selected);
	}
	if (intersected.size() == 0)
	{
		return nullptr;
	}
	return selected;
}

GameObject* ModuleRender::SearchGO(GameObject* go, float3* origin, LineSegment* ray, std::map<float, GameObject*>* intersected)
{
	GameObject* selected = nullptr;
	LineSegment localRay = *ray;
	bool hit_point;
	float distance = 1000.0f;
	Mesh* mesh = (Mesh*)go->FindComponent(ComponentType::Mesh);
	Transform* trans = (Transform*)go->FindComponent(ComponentType::Transform);
	if (mesh)
	{
		localRay.Transform(trans->worldTransform.Inverted());

		hit_point = mesh->box.Intersects(localRay);
		if (hit_point)
		{
			float dist = origin->Distance(mesh->box);
			intersected->insert_or_assign(dist, mesh->owner);
			//*intersected[dist] = mesh->owner;
		}		
		Mesh* minDistMesh = nullptr;
		if (mesh)
		{
			for (unsigned int i = 0; i < mesh->indices.size() - 2; i++)
			{
				Triangle tri;
				tri.a = mesh->vertices[mesh->indices[i]].Position;
				tri.b = mesh->vertices[mesh->indices[i + 1]].Position;
				tri.c = mesh->vertices[mesh->indices[i + 2]].Position;
				//tri = Triangle(mesh->vertices[mesh->indices[i]].Position, mesh->vertices[mesh->indices[i + 1]].Position, mesh->vertices[mesh->indices[i + 2]].Position);
				
				//tri.Transform(trans->worldTransform.Inverted());
				tri.Transform(trans->localTransform.Inverted());
				bool hit = tri.Intersects(localRay);
				if (hit)
				{
					distance = tri.Distance(*origin);
					intersected->insert_or_assign(distance, mesh->owner);
				}//TODOfix changing the object
			}
		}
		for (std::pair<float, GameObject*> element : *intersected) {
			if (element.first < distance)
			{
				distance = element.first;
				selected = element.second;
				return selected;
			}
		}
	}
	for (auto child : go->children)
	{
		selected = SearchGO(child, origin, ray, intersected);
		if (selected) {
			return selected;
		}
	}
	return selected;
}

void ModuleRender::DrawGizmo(GameObject* selected) {

	ImVec2 posW = App->camera->loadedCameras[0]->hoveredWindowPos;
	float sceneHeight = App->camera->loadedCameras[0]->hoveredWindowSize.x;
	float sceneWidth = App->camera->loadedCameras[0]->hoveredWindowSize.y;
	ImGuizmo::SetRect((float)posW.x, (float)posW.y, sceneWidth, sceneHeight);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::Enable(true);

	Transform* gizTrans = nullptr;
	if (selected != nullptr) 
	{
		gizTrans = (Transform*)selected->FindComponent(ComponentType::Transform);
		float4x4 transModel = gizTrans->worldTransform.Transposed();
		float4x4 transView = App->camera->loadedCameras[0]->view.Transposed();
		float4x4 transProj = App->camera->loadedCameras[0]->proj.Transposed();
		ImGuizmo::Manipulate(transView.ptr(), transProj.ptr(), gizmoOperation, gizmoMode, transModel.ptr());

		if (ImGuizmo::IsUsing()) {
			App->imgui->AddLog("GIZMO ENABLED");
			gizTrans->SetTransform(&(transModel.Transposed()));
			gizTrans->UpdateDirtyFlag();
		}
	}

}

void ModuleRender::DrawAABB(GameObject* go) 
{
	Mesh* mesh = (Mesh*)go->FindComponent(ComponentType::Mesh);
	if (mesh)
	{
		dd::aabb(mesh->box.minPoint, mesh->box.maxPoint, math::float3(0.0f, 0.0f, 0.0f));
		
	}
	for (auto child : go->children)
	{
		DrawAABB(child);
	}
	
}

