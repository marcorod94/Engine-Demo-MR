#include "main/Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleImGui.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

bool ModuleTexture::Init() {
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilGenImages(1, &imageName);
	ilBindImage(imageName);
	return true;
}

bool ModuleTexture::CleanUp() {
	ilDeleteImages(1, &imageName);
	return true;
}

unsigned ModuleTexture::LoadTexture(std::string& path) {
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		if (path.compare(loadedTextures[i].path) == 0) {
			App->imgui->AddLog("Texture already loaded: %s", loadedTextures[i].path.c_str());
			return loadedTextures[i].id;
		}
	}
	Texture texture;
	ilLoadImage(path.c_str());
	iluGetImageInfo(&imageInfo);
	texture.id = ilutGLBindTexImage();
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.path = path;
	loadedTextures.push_back(texture);
	App->imgui->AddLog("Texture succssesfully loaded: %s", path.c_str());
	return texture.id;
}

void ModuleTexture::DrawTexture(unsigned& id) {
	if (drawSelector) {
		DrawTextureSelector(id);
	}
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		if (loadedTextures[i].id == id) {
			DrawTexture(loadedTextures[i]);
		}
	}
	if (ImGui::ImageButton((void*)(intptr_t)id, ImVec2(128, 128))) {
		drawSelector = true;
	}

}

void ModuleTexture::DrawTextureSelector(unsigned& id) {
	ImGui::Begin("Textures");
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		DrawTexture(loadedTextures[i]);
		if (ImGui::ImageButton((void*)(intptr_t)loadedTextures[i].id, ImVec2(128, 128))) {
			id = loadedTextures[i].id;
			drawSelector = false;
		}
	}
	ImGui::End();
}

void ModuleTexture::DrawTexture(Texture& texture) {
	ImGui::Text("Path: %s", texture.path.c_str());
	ImGui::Text("Type: %s", texture.type.c_str());
	ImGui::Text("Width: %d", texture.width);
	ImGui::Text("Heigth: %d", texture.height);
}


Material* ModuleTexture::CreateMaterial(GameObject* owner) {
	return new Material(owner);
}

void ModuleTexture::LoadSkybox(std::vector<std::string> faces)const
{
	
}
