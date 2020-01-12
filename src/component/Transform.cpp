#include "Transform.h"
#include "imgui.h"

void Transform::DrawView() {
	if (ImGui::TreeNode("Transform")) {
		DrawFloat3View("Position", &position, -100.00F, 100.00F);
		DrawFloat3View("Rotation", &rotationEU, -360.00F, 360.00F);
		DrawFloat3View("Scalling", &scaling, 0.000001, 50.00F, "%.6F");
		ImGui::TreePop();
	}
}

void Transform::CalculateWorldTransform() {
	if (owner->parent) {
		Transform* trans = ((Transform*)owner->parent->FindComponent(ComponentType::Transform));
		if (trans) {
			worldTransform = trans->localTransform * localTransform;
		}
	} else {
		worldTransform = localTransform;
	}
}


void Transform::SetTransform(const aiMatrix4x4& trans) {
	localTransform.SetRow(0, float4(trans.a1, trans.a2, trans.a3, trans.a4));
	localTransform.SetRow(1, float4(trans.b1, trans.b2, trans.b3, trans.d4));
	localTransform.SetRow(2, float4(trans.c1, trans.c2, trans.c3, trans.c4));
	localTransform.SetRow(3, float4(trans.d1, trans.d2, trans.d3, trans.d4));	
	localTransform.Decompose(position, rotation, scaling);
	rotationEU = rotation.ToEulerXYZ();
	rotationEU = RadToDeg(rotationEU);
	CalculateWorldTransform();
}

void Transform::CalculateTransform() {
	rotation = rotation.FromEulerXYZ(DegToRad(rotationEU.x), DegToRad(rotationEU.y), DegToRad(rotationEU.z));
	localTransform = localTransform.FromTRS(position, rotation, scaling);
	CalculateWorldTransform();
}


void Transform::DrawFloat3View(const char* label, float3* vector, float min, float max, const char* format) {
	if (ImGui::TreeNode(label)) {
		ImGui::SliderFloat("X", &vector->x, min, max, format);
		ImGui::SliderFloat("Y", &vector->y, min, max, format);
		ImGui::SliderFloat("Z", &vector->z, min, max, format);
		ImGui::TreePop();
	}
}