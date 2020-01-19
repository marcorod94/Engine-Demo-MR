#include "main/Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleImGui.h"

bool ModuleTexture::Init() {
	return true;
}

bool ModuleTexture::CleanUp() {
	return true;
}

unsigned ModuleTexture::LoadTexture(const char* file) {
	std::string name = file;
	name = name.substr(name.find_last_of('\\') + 1, name.size());
	name = name.substr(name.find_last_of('/') + 1, name.size());
	name = name.substr(0, name.find_last_of('.'));
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		if (name.compare(loadedTextures[i].name) == 0) {
			App->imgui->AddLog("Texture already loaded: %s", loadedTextures[i].name.c_str());
			return loadedTextures[i].id;
		}
	}
	Texture texture;
	importer->Load(file, &texture);
	glGenTextures(1, &texture.id);
	loadedTextures.push_back(texture);
	App->imgui->AddLog("Texture succssesfully loaded: %s", file);
	return texture.id;
}

void ModuleTexture::DrawTexture(unsigned id) {
	if (drawSelector) {
		DrawTextureSelector(id);
	}
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		if (loadedTextures[i].id == id) {
			DrawTexture(&loadedTextures[i]);
		}
	}
	if (ImGui::ImageButton((void*)(intptr_t)id, ImVec2(128, 128))) {
		drawSelector = true;
	}

}

void ModuleTexture::DrawTextureSelector(unsigned id) {
	ImGui::Begin("Textures");
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		DrawTexture(&loadedTextures[i]);
		if (ImGui::ImageButton((void*)(intptr_t)loadedTextures[i].id, ImVec2(128, 128))) {
			id = loadedTextures[i].id;
			drawSelector = false;
		}
	}
	ImGui::End();
}

void ModuleTexture::DrawTexture(Texture* texture) {
	ImGui::Text("Name: %s", texture->name.c_str());
	ImGui::Text("Width: %d", texture->width);
	ImGui::Text("Heigth: %d", texture->height);
}


Material* ModuleTexture::CreateMaterial(GameObject* owner) {
	return new Material(owner);
}

void ModuleTexture::LoadSkybox(std::vector<std::string> faces)const
{
	
}
