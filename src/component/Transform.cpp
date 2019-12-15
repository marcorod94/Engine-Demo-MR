#include "Transform.h"
#include "imgui.h"

void Transform::DrawView() {
	if (ImGui::TreeNode("Transform test")) {
		ImGui::Text("Position:\tX:%.f\tY:%.f\tZ:%.f ", position.x, position.y, position.z);
		ImGui::Text("Rotation:\tX:%.f\tY:%.f\tZ:%.f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("Scale:\tX:%.f\tY:%.f\tZ:%.f", scaling.x, scaling.y, scaling.z);
		ImGui::TreePop();
	}
}

void Transform::CalculateWorldTransform(const float4x4& trans) {
	/*if (parent != nullptr) {
		trans = trans * 
		transformation = transformation * ((Transform*) owner->parent->FindComponent(ComponentType::Transform))->transformation;
	}*/
}
