#ifndef Material_h
#define Material_h
#include "Component.h"

struct Texture {
	unsigned id;
	unsigned width;
	unsigned height;
	std::string name;
	std::string path;
};

class Material : public Component {
public:
	unsigned program = 0;
	Texture* diffuseTex = nullptr;
	math::float4 diffuseColor = math::float4::zero;
	float kDiffuse = 0.0f;
	Texture* specularTex = nullptr;
	math::float3 specularColor = math::float3::zero;
	float shininess = 0.0f;
	float kSpecular = 0.0f;
	Texture* occlusionTex = nullptr;
	float kAmbient = 0.0f;
	Texture* emissiveTex = nullptr;
	math::float3 emissiveColor = math::float3::zero;
	Material(GameObject* owner): Component(owner, ComponentType::Material) {}
	void DrawView();
	void OnLoad(rapidjson::Document::Object*);
	void OnSave(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
};
#endif
