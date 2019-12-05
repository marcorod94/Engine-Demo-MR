#ifndef Transform_h
#define Transform_h
#include "Component.h"
#include "MathGeoLib.h"
class Transform : public Component {
public:
	float3 scaling = float3::zero;
	float3 position = float3::zero;
	float3 rotation = float3::zero;
	float4x4 transformation = float4x4::identity;
	void DrawView() const;
	void UpdateTransform();
};
#endif
