#include "Transform.h"
#include "imgui.h"

void Transform::DrawView() const {
	ImGui::TreeNode("Transformation");
	ImGui::Text("Position:\tX:%.f\tY:%.f\tZ:%.f ", position.x, position.y, position.z);
	ImGui::Text("Rotation:\tX:%.f\tY:%.f\tZ:%.f", rotation.x, rotation.y, rotation.z);
	ImGui::Text("Scale:\tX:%.f\tY:%.f\tZ:%.f", scaling.x, scaling.y, scaling.z);
	ImGui::TreePop();
}

void Transform::UpdateTransform() {
	if (owner->parent != nullptr) {
		
	}
}
