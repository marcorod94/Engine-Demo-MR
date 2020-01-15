#ifndef GameObject_h
#define GameObject_h
#include <string>
#include <vector>
#include "main/Globals.h"
#include "crossguid/guid.hpp"
#include "rapidjson/document.h"

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
	Component* CreateComponent(const ComponentType);
	Component* FindComponent(const ComponentType);
	Transform* myTransform = nullptr;
	void DeleteChild(const GameObject*);
	void ShowProperties();
	void OnLoad(rapidjson::Document*);
	void OnSave(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
private:
	void CreateTransform(const float3&, const Quat&);
	void SaveChildren(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
	void SaveComponents(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
};
#endif
