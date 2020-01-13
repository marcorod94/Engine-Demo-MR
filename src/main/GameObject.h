#ifndef GameObject_h
#define GameObject_h
#include <string>
#include <vector>
#include "main/Globals.h"
#include "crossguid/guid.hpp"
class Component;
class Transform;
class GameObject {
public:
	std::string uuid = xg::newGuid().str();
	std::string name;
	std::vector<Component*> components;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	GameObject() {}
	GameObject(const std::string& name): name(name) {
		CreateComponent(ComponentType::Transform);
	}
	GameObject(const std::string& name, const float3& pos, const Quat& rot) : name(name) {
		CreateTransform(pos, rot);
	}
	~GameObject() {}
	update_status Update();
	Component* CreateComponent(const ComponentType);
	Component* FindComponent(const ComponentType);
	Transform* myTransform = nullptr;
	void DeleteChild(const GameObject*);
	void ShowProperties();
private:
	void CreateTransform(const float3&, const Quat&);
};
#endif
