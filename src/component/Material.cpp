#include"main/Application.h"
#include "Material.h"
#include "module/ModuleTexture.h"
#include "imgui.h"

void Material::DrawView() {

	if (ImGui::TreeNode("Material")) {
		if (ImGui::TreeNode("Ambient")) {
			App->texture->DrawTexture(occlusionMap);
			ImGui::SliderFloat("K ambient", &kAmbient, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Diffue")) {
			App->texture->DrawTexture(diffuseMap);
			ImGui::ColorEdit4("object color", (float*)&diffuseColor);
			ImGui::SliderFloat("K diffuse", &kDiffuse, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Emissive")) {
			App->texture->DrawTexture(emissiveMap);
			ImGui::ColorEdit4("object color", (float*)&emissiveColor);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Specular")) {
			App->texture->DrawTexture(specularMap);
			ImGui::ColorEdit4("object color", (float*)&specularColor);
			ImGui::SliderFloat("K specular", &kSpecular, 0.0f, 1.0f);
			ImGui::SliderFloat("shininess", &shininess, 0, 128.0f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}