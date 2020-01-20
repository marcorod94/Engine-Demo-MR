#include "main/Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleImGui.h"

bool ModuleTexture::Init() {
	return true;
}

bool ModuleTexture::CleanUp() {
	for (auto item : loadedTextures) {
		delete item;
	}
	loadedTextures.clear();
	return true;
}

unsigned ModuleTexture::LoadTexture(const char* file) {
	std::string name = file;
	name = name.substr(name.find_last_of('/') + 1, name.size());
	name = name.substr(0, name.find_last_of('.'));
	for (auto item : loadedTextures) {
		if (name.compare(item->name) == 0) {
			App->imgui->AddLog("Texture already loaded: %s", item->name.c_str());
			return item->id;
		}
	}
	Texture* texture = new Texture;
	importer->Load(file, texture);
	loadedTextures.push_back(texture);
	App->imgui->AddLog("Texture succssesfully loaded: %s", file);
	return texture->id;
}

void ModuleTexture::DrawTextureButton(Texture** texture) {	
	int id = 0;
	if (*texture) {
		DrawTexture(*texture);
		id = (*texture)->id;
	}
	if (ImGui::ImageButton((void*)(intptr_t)id, ImVec2(128, 128))) {
		drawSelector = true;
	}
	Texture* selected = DrawTextureSelector();
	if (selected) {
		*texture = selected;
	}

}

Texture* ModuleTexture::DrawTextureSelector() {
	Texture* selected = nullptr;
	if (drawSelector) {
		ImGui::Begin("Textures", &drawSelector);
		for (auto item : loadedTextures) {
			DrawTexture(item);
			if (ImGui::ImageButton((void*)(intptr_t)item->id, ImVec2(128, 128))) {
				drawSelector = false;
				selected = item;
			}
		}
		ImGui::End();
	}
	return selected;
}

void ModuleTexture::DrawTextureSelector(unsigned* id) {
	ImGui::Begin("Textures");
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		DrawTexture(loadedTextures[i]);
		if (ImGui::ImageButton((void*)(intptr_t)loadedTextures[i]->id, ImVec2(128, 128))) {
			*id = loadedTextures[i]->id;
			drawSelector = false;
		}
	}
	ImGui::End();
}

void ModuleTexture::DrawTexture(const Texture* texture) {
	if (texture) {
		ImGui::Text("Name: %s", texture->name.c_str());
		ImGui::Text("Width: %d", texture->width);
		ImGui::Text("Heigth: %d", texture->height);
	}
}

Material* ModuleTexture::CreateMaterial(GameObject* owner) {
	return new Material(owner);
}

void ModuleTexture::LoadSkybox(std::vector<std::string> faces)const
{
	
}
