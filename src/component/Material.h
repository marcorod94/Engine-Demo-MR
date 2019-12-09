#ifndef Material_h
#define Material_h
#include "Component.h"
class Material : public Component {
public:
	std::vector<Texture> textures;
	unsigned program;
	math::float4 color = math::float4::zero;
	float shininess = 0.0f;
	float kSpecular = 0.0f;
	float kDiffuse = 0.0f;
	float kAmbient = 0.0f;
	Material(GameObject* owner): Component(owner, ComponentType::Material) {}
	void DrawView();
};
#endif
