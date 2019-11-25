#include "Application.h"
#include "ModuleScene.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleModel.h"
#include <string>

bool ModuleScene::Init() {
	
	App->model->LoadModel(std::string("Models/BakerHouse.fbx"));
	for (unsigned int i = 0; i < App->model->meshes.size(); i++) {
		SetupMesh(App->model->meshes[i]);
	}
	
	App->program->LoadShaders("Shaders/default.vs", "Shaders/default.fs");
	return true;
}

update_status ModuleScene::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update() {
	glUseProgram(App->program->program);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "model"), 1, GL_TRUE, &(App->camera->model[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "view"), 1, GL_TRUE, &(App->camera->view[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "proj"), 1, GL_TRUE, &(App->camera->proj[0][0]));
	
	for (unsigned int i = 0; i < App->model->meshes.size(); i++) {
		DrawMesh(App->model->meshes[i]);
	}
	if (showAxis) {
		DrawAxis();
	}
	if (showGrid) {
		DrawGrid();
	}
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	return true;
}

void ModuleScene::DrawGrid() const {
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

void ModuleScene::DrawAxis() const {
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

void ModuleScene::SetupMesh(Mesh& mesh) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void ModuleScene::DrawMesh(Mesh& mesh) {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < mesh.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = mesh.textures[i].type;
        if(name == "texture_diffuse")
			number = std::to_string(++diffuseNr);
		else if(name == "texture_specular")
			number = std::to_string(++specularNr);
        else if(name == "texture_normal")
			number = std::to_string(++normalNr);
        else if(name == "texture_height")
			number = std::to_string(++heightNr);
													
		glUniform1i(glGetUniformLocation(App->program->program, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
    }
        
    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
