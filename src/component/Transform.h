#ifndef Transform_h
#define Transform_h
#include "Component.h"
#include "MathGeoLib.h"
#include "assimp/matrix4x4.h"

class Transform : public Component {
public:
	float3 scaling = float3::zero;
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 rotationEU = float3::zero;
	float4x4 localTransform = float4x4::identity;
	float4x4 worldTransform = float4x4::identity;

	Transform(GameObject* owner): Component(owner, ComponentType::Transform) {}
	Transform(GameObject* owner, const float3& position, const Quat& rotation) : Component(owner, ComponentType::Transform), position(position), rotation(rotation) {
		localTransform = float4x4(rotation, position);
	}
	void DrawView();
	void SetTransform(const aiMatrix4x4&);
	void CalculateTransform();
	void OnLoad(rapidjson::Document*);
	void OnSave(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);

private:
	void DrawFloat3View(const char*, float3*, float, float, float speed = 1.0F, const char* format = "%.2F");
	void CalculateWorldTransform();
};
#endif
