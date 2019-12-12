#include "main/Globals.h"
#include "main/Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleImGui.h"
#include "ModuleProgram.h"
#include "ModuleScene.h"
#include "ModuleCamera.h"
#include "ModuleModel.h"
#include "main/GameObject.h"
#include "component/Mesh.h"
#include "component/Material.h"
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
	SDL_GetWindowSize(App->window->window, &App->window->screenWidth, &App->window->screenHeight);
	glViewport(0, 0, App->window->screenWidth, App->window->screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	unsigned program = App->program->programs[int(ProgramType::Default)];
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &(App->camera->model[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &(App->camera->view[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &(App->camera->proj[0][0]));
	if (App->scene->root) {
		DrawGameObject(App->scene->root);
	}
	if (showAxis) {
		DrawAxis();
	}
	if (showGrid) {
		DrawGrid();
	}
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

Mesh* ModuleRender::CreateMesh() {
	return new Mesh(nullptr);
}

void ModuleRender::DrawGrid() const {
	glLineWidth(1.0F);
	float d = 200.0F;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0F)
	{
		glVertex3f(i, 0.0F, -d);
		glVertex3f(i, 0.0F, d);
		glVertex3f(-d, 0.0F, i);
		glVertex3f(d, 0.0F, i);
	}
	glEnd();
}

void ModuleRender::DrawAxis() const {
	glLineWidth(2.0F);
	glBegin(GL_LINES);
	// red X
	glColor4f(1.0F, 0.0F, 0.0F, 1.0F);
	glVertex3f(0.0F, 0.0F, 0.0F); glVertex3f(1.0F, 0.0F, 0.0F);
	glVertex3f(1.0F, 0.1F, 0.0F); glVertex3f(1.1F, -0.1F, 0.0F);
	glVertex3f(1.1F, 0.1F, 0.0F); glVertex3f(1.0F, -0.1F, 0.0F);
	// green Y
	glColor4f(0.0F, 1.0F, 0.0F, 1.0F);
	glVertex3f(0.0F, 0.0F, 0.0F); glVertex3f(0.0F, 1.0F, 0.0F);
	glVertex3f(-0.05F, 1.25F, 0.0F); glVertex3f(0.0F, 1.15F, 0.0F);
	glVertex3f(0.05F, 1.25F, 0.0F); glVertex3f(0.0F, 1.15F, 0.0F);
	glVertex3f(0.0F, 1.15F, 0.0F); glVertex3f(0.0F, 1.05F, 0.0F);

	glColor4f(0.0F, 0.0F, 1.0F, 1.0F);
	glVertex3f(0.0F, 0.0F, 0.0F); glVertex3f(0.0F, 0.0F, 1.0F);
	glVertex3f(-0.05F, 0.1F, 1.05F); glVertex3f(0.05F, 0.1F, 1.05F);
	glVertex3f(0.05F, 0.1F, 1.05F); glVertex3f(-0.05F, -0.1F, 1.05F);
	glVertex3f(-0.05F, -0.1F, 1.05F); glVertex3f(0.05F, -0.1F, 1.05F);
	glEnd();
	glLineWidth(1.0F);
}

void  ModuleRender::DrawGameObject(GameObject* parent) {
	DrawMaterial((Material*)parent->FindComponent(ComponentType::Material));
	DrawMesh((Mesh*)parent->FindComponent(ComponentType::Mesh));
	for (unsigned i = 0; i < parent->children.size(); i++) {
		DrawGameObject(parent->children[i]);
	}
}

void  ModuleRender::DrawMesh(Mesh* mesh) {
	if (mesh) {
		glBindVertexArray(mesh->vao);
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
}

void  ModuleRender::DrawMaterial(Material* material) {
	if (material) {
		unsigned program = App->program->programs[material->program];
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
		} else {
			glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->diffuseMap);
			glUniform1i(glGetUniformLocation(program, "diffuse_map"), 0);
		}
	}
}
