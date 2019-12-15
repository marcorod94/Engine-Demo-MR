#ifndef GameObject_h
#define GameObject_h
#include <string>
#include <vector>
#include "main/Globals.h"

class Component;
class GameObject {
public:
	int uuid;
	std::string name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
	GameObject() {}
	GameObject(const std::string& name): name(name) {
		CreateComponent(ComponentType::Transform);
	}
	~GameObject() {}
	update_status Update();
	Component* CreateComponent(const ComponentType);
	Component* FindComponent(const ComponentType);
	void DeleteChild(const GameObject*);
	void ShowProperties();
};
#endif
