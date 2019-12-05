#ifndef GameObject_h
#define GameObject_h
#include <string>
#include <vector>
#include "main/Globals.h"
enum class ComponentType;
class Component;
class GameObject {
public:
	std::string name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
	GameObject() {}
	~GameObject() {}
	update_status Update();
	const Component* CreateComponent(const ComponentType);
	const Component* FindComponent(const ComponentType);
};
#endif
