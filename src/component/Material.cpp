#include"main/Application.h"
#include "Material.h"
#include "module/ModuleTexture.h"
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
	list->PushBack(object, *allocator);
}
