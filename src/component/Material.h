#ifndef Material_h
#define Material_h
#include "Component.h"
class Material : public Component {
public:
	std::vector<Texture> textures;
	Material(GameObject* owner) : Component(owner, ComponentType::Material) {}
	void DrawView();
};
#endif
