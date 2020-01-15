#include "Transform.h"
#include "imgui.h"

void Transform::DrawView() {
	if (ImGui::TreeNode("Transform")) {
		ImGui::Text("UUID: %s", uuid.c_str());
		ImGui::DragFloat3("Position", position.ptr(), 1.0F, -100.00F, 100.00F);
		ImGui::DragFloat3("Rotation", rotationEU.ptr(), 1.0F, -360.00F, 360.00F);
		ImGui::DragFloat3("Scalling", scaling.ptr(), 0.001F, -50.00F, 50.00F);
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

void Transform::OnLoad(rapidjson::Document::Object* object) {
	uuid = (object->FindMember("uuid"))->value.GetString();
	Component::GetFloat3FromObjectJSON(&(object->FindMember("position"))->value.GetObjectA(), &position);
	Component::GetFloat3FromObjectJSON(&(object->FindMember("rotation"))->value.GetObjectA(), &rotationEU);
	Component::GetFloat3FromObjectJSON(&(object->FindMember("scalling"))->value.GetObjectA(), &scaling);
	CalculateTransform();
}

void Transform::OnSave(rapidjson::Document::Array* list, rapidjson::Document::AllocatorType* allocator) {
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("uuid", rapidjson::StringRef(uuid.c_str()), *allocator);
	object.AddMember("type", int(type), *allocator);
	std::string owneruuid;
	if (owner) {
		owneruuid = owner->uuid;
	}
	object.AddMember("owneruuid", rapidjson::StringRef(owneruuid.c_str()), *allocator);
	Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "position", &position);
	Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "rotation", &rotationEU);
	Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "scalling", &scaling);
	list->PushBack(object, *allocator);
}
