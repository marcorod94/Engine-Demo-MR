#include "Material.h"
#include "imgui.h"

void Material::DrawView() {
	if (ImGui::TreeNode("Textures")) {
		for (unsigned int i = 0; i < textures.size(); i++) {
			ImGui::Text("Path: %s", textures[i].path.c_str());
			ImGui::Text("Type: %s", textures[i].type.c_str());
			ImGui::Text("Width: %d", textures[i].width);
			ImGui::Text("Heigth: %d", textures[i].height);
			if (ImGui::ImageButton((void*)(intptr_t)textures[i].id, ImVec2(128, 128))) {
				//App->model->UpdateTexture(textures[i]);
			}
		}
		ImGui::TreePop();
	}
}