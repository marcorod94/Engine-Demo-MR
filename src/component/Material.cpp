#include"main/Application.h"
#include "module/ModuleTexture.h"
#include "Material.h"
#include "imgui.h"

void Material::DrawView() {
	if (ImGui::TreeNode("Material")) {
		ImGui::Text("UUID: %s", uuid.c_str());
		if (ImGui::TreeNode("Ambient")) {
			App->texture->DrawTextureButton(&occlusionTex);
			ImGui::SliderFloat("K ambient", &kAmbient, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Diffue")) {
			App->texture->DrawTextureButton(&diffuseTex);
			ImGui::ColorEdit4("object color", (float*)&diffuseColor);
			ImGui::SliderFloat("K diffuse", &kDiffuse, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Emissive")) {
			App->texture->DrawTextureButton(&emissiveTex);
			ImGui::ColorEdit4("object color", (float*)&emissiveColor);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Specular")) {
			App->texture->DrawTextureButton(&specularTex);
			ImGui::ColorEdit4("object color", (float*)&specularColor);
			ImGui::SliderFloat("K specular", &kSpecular, 0.0f, 1.0f);
			ImGui::SliderFloat("shininess", &shininess, 0, 128.0f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Material::OnLoad(rapidjson::Document::Object* object) {
	uuid = (object->FindMember("uuid"))->value.GetString();

	std::string path = (object->FindMember("diFfusePath"))->value.GetString();
	if (path.compare("") != 0) {
		diffuseTex = App->texture->LoadTexture(path.c_str());
	}
	kDiffuse = (object->FindMember("kDiffuse"))->value.GetFloat();
	Component::GetFloat4FromObjectJSON(&(object->FindMember("diffuseColor"))->value.GetObjectA(), &diffuseColor);

	path = (object->FindMember("specularPath"))->value.GetString();
	if (path.compare("") != 0) {
		specularTex = App->texture->LoadTexture(path.c_str());
	}
	shininess = (object->FindMember("shininess"))->value.GetFloat();
	kSpecular = (object->FindMember("kSpecular"))->value.GetFloat();
	Component::GetFloat3FromObjectJSON(&(object->FindMember("specularColor"))->value.GetObjectA(), &specularColor);

	path = (object->FindMember("occlusionPath"))->value.GetString();
	if (path.compare("") != 0) {
		occlusionTex = App->texture->LoadTexture(path.c_str());
	}
	kAmbient = (object->FindMember("kAmbient"))->value.GetFloat();
	
	path = (object->FindMember("emissivePath"))->value.GetString();
	if (path.compare("") != 0) {
		specularTex = App->texture->LoadTexture(path.c_str());
	}
	Component::GetFloat3FromObjectJSON(&(object->FindMember("emissiverColor"))->value.GetObjectA(), &emissiveColor);
}

void Material::OnSave(rapidjson::Document::Array* list, rapidjson::Document::AllocatorType* allocator) {
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("uuid", rapidjson::StringRef(uuid.c_str()), *allocator);
	object.AddMember("type", int(type), *allocator);
	std::string owneruuid;
	if (owner) {
		owneruuid = owner->uuid;
	}
	object.AddMember("owneruuid", rapidjson::StringRef(owneruuid.c_str()), *allocator);
	if (diffuseTex) {
		object.AddMember("diFfusePath", rapidjson::StringRef(diffuseTex->path.c_str()), *allocator);
	} else {
		object.AddMember("diFfusePath", "", *allocator);
	}
	object.AddMember("kDiffuse", kDiffuse, *allocator);
	Component::AddFloat4ToObjectJSON(&object.GetObjectA(), allocator, "diffuseColor", &diffuseColor);
	if (specularTex) {
		object.AddMember("specularPath", rapidjson::StringRef(specularTex->path.c_str()), *allocator);
	} else {
		object.AddMember("specularPath", "", *allocator);
	}
	object.AddMember("shininess", shininess, *allocator);
	object.AddMember("kSpecular", kSpecular, *allocator);
	Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "specularColor", &specularColor);
	if (occlusionTex) {
		object.AddMember("occlusionPath", rapidjson::StringRef(occlusionTex->path.c_str()), *allocator);
	} else {
		object.AddMember("occlusionPath", "", *allocator);
	}
	object.AddMember("kAmbient", kAmbient, *allocator);
	if (emissiveTex) {
		object.AddMember("emissivePath", rapidjson::StringRef(emissiveTex->path.c_str()), *allocator);
	} else {
		object.AddMember("emissivePath", "", *allocator);
	}
	Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "emissiverColor", &emissiveColor);
	list->PushBack(object, *allocator);
}
