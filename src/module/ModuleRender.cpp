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
#include "component/Camera.h"
#include "SDL.h"
#include "GL/glew.h"

class Camera;

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
	if (App->scene->root) {
		Camera* cam = (Camera*)App->scene->root->FindComponent(ComponentType::Camera);
		cam->GenerateFBOTexture(cam->width, cam->height);
		glBindFramebuffer(GL_FRAMEBUFFER, cam->fbo);
		glViewport(0, 0, cam->width, cam->height);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		unsigned program = App->program->programs[int(ProgramType::Default)];
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &(cam->model[0][0]));
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &(cam->view[0][0]));
		glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &(cam->proj[0][0]));
		DrawGameObject(App->scene->root, cam);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (showAxis) {
			glBindFramebuffer(GL_FRAMEBUFFER, cam->fbo);
			DrawAxis();
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		if (showGrid) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//DrawGrid(cam);
			glBindFramebuffer(GL_FRAMEBUFFER, cam->fbo);
			DrawGrid(cam);
		}
		//cam->DrawView();
	}

	if (App->scene->mainCamera) {
		Camera* cam2 = (Camera*)App->scene->mainCamera->FindComponent(ComponentType::Camera);
		cam2->GenerateFBOTexture(cam2->width, cam2->height);
		glBindFramebuffer(GL_FRAMEBUFFER, cam2->fbo);
		glViewport(0, 0, cam2->width, cam2->height);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		unsigned program2 = App->program->programs[int(ProgramType::Default)];
		glUseProgram(program2);
		glUniformMatrix4fv(glGetUniformLocation(program2, "model"), 1, GL_TRUE, &(cam2->model[0][0]));
		glUniformMatrix4fv(glGetUniformLocation(program2, "view"), 1, GL_TRUE, &(cam2->view[0][0]));
		glUniformMatrix4fv(glGetUniformLocation(program2, "proj"), 1, GL_TRUE, &(cam2->proj[0][0]));
		DrawGameObject(App->scene->mainCamera, cam2);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (showAxis) {
			glBindFramebuffer(GL_FRAMEBUFFER, cam2->fbo);
			DrawAxis();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		if (showGrid) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//DrawGrid(cam);
			glBindFramebuffer(GL_FRAMEBUFFER, cam2->fbo);
			DrawGrid(cam2);
		}
		//cam->DrawView();
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

Mesh* ModuleRender::CreateMesh() {
	return new Mesh(nullptr);
}

void ModuleRender::DrawGrid(Camera* cam) const {
	glLineWidth(1.0F);
	float d = 200.0F;
	glBegin(GL_LINES);
	//Near
	//glVertex3f(cam->ntl.x, cam->ntl.y, cam->ntl.z);
	//glVertex3f(cam->ntr.x, cam->ntr.y, cam->ntr.z);

	//glVertex3f(cam->nbl.x, cam->nbl.y, cam->nbl.z);
	//glVertex3f(cam->ntr.x, cam->ntr.y, cam->ntr.z);

	//glVertex3f(cam->nbl.x, cam->nbl.y, cam->nbl.z);
	//glVertex3f(cam->nbr.x, cam->nbr.y, cam->nbr.z);

	//glVertex3f(cam->ntl.x, cam->ntl.y, cam->ntl.z);
	//glVertex3f(cam->nbr.x, cam->nbr.y, cam->nbr.z);

	////Far
	//glVertex3f(cam->ftl.x, cam->ftl.y, cam->ftl.z);
	//glVertex3f(cam->ftr.x, cam->ftr.y, cam->ftr.z);

	//glVertex3f(cam->fbl.x, cam->fbl.y, cam->fbl.z);
	//glVertex3f(cam->ftr.x, cam->ftr.y, cam->ftr.z);

	//glVertex3f(cam->fbl.x, cam->fbl.y, cam->fbl.z);
	//glVertex3f(cam->fbr.x, cam->fbr.y, cam->fbr.z);

	//glVertex3f(cam->ftl.x, cam->ftl.y, cam->ftl.z);
	//glVertex3f(cam->fbr.x, cam->fbr.y, cam->fbr.z);

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

void  ModuleRender::DrawGameObject(GameObject* parent, Camera* cam) {
	DrawMaterial((Material*)parent->FindComponent(ComponentType::Material));
	DrawMesh((Mesh*)parent->FindComponent(ComponentType::Mesh));
	for (unsigned i = 0; i < parent->children.size(); i++) {
		DrawGameObject(parent->children[i], cam);
	}
}

void ModuleRender::DisplayFrameBuffer(Camera* camera, unsigned fbo, unsigned fb_width, unsigned fb_height)
{

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
