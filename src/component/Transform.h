#ifndef Transform_h
#define Transform_h
#include "Component.h"
#include "MathGeoLib.h"
#include "assimp/matrix4x4.h"

class Transform : public Component {
public:
	float3 scale = float3::zero;
	float3 position = float3::zero;
	/*float3 rotation = float3::zero;*/
	Quat rotation = Quat::identity;
	float4x4 localTransform = float4x4::identity;
	float4x4 worldTransform = float4x4::identity;
	Transform(GameObject* owner): Component(owner, ComponentType::Transform) {}
	void DrawView();
	void CalculateWorldTransform(const float4x4&);
	void SetTransform(const aiMatrix4x4&);
};
#endif
