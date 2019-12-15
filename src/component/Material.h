#ifndef Material_h
#define Material_h
#include "Component.h"
class Material : public Component {
public:
	unsigned program;
	unsigned diffuseMap = 0;
	math::float4 diffuseColor = math::float4::zero;
	float kDiffuse = 0.0f;
	unsigned specularMap = 0;
	math::float3 specularColor = math::float3::zero;
	float shininess = 0.0f;
	float kSpecular = 0.0f;
	unsigned occlusionMap = 0;
	float kAmbient = 0.0f;
	unsigned emissiveMap = 0;
	math::float3 emissiveColor = math::float3::zero;
	Material(GameObject* owner): Component(owner, ComponentType::Material) {}
	void DrawView();
};
#endif
