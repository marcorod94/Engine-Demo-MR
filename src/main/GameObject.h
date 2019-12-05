#ifndef GameObject_h
#define GameObject_h
#include <string>
#include <vector>
#include "component/Component.h"
class GameObject {
public:
	std::string name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
	GameObject() {}
	~GameObject() {}
	update_status Update();
	Component* createComponent(ComponentType);
};

#endif
