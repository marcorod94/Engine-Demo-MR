#ifndef Transform_h
#define Transform_h
#include "Component.h"
#include "MathGeoLib.h"
class Transform : public Component {
public:
	float3 scaling = float3::zero;
	float3 position = float3::zero;
	float3 rotation = float3::zero;
	float4x4 localTransform = float4x4::identity;
	float4x4 worldTransform = float4x4::identity;
	Transform() { type = ComponentType::Transform; }
	void DrawView();
	void CalculateWorldTransform(const float4x4&);
};
#endif
