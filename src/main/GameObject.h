#ifndef GameObject_h
#define GameObject_h
#include <string>
#include <vector>
#include "main/Globals.h"
#include "crossguid/guid.hpp"
#include "rapidjson/document.h"

class Component;
enum class ComponentType;
class Transform;
class GameObject {
public:
	std::string uuid = xg::newGuid().str();
	std::string name;
	std::vector<Component*> components;
	GameObject* parent = nullptr;
	AABB originalBox;
	AABB box;
	std::vector<GameObject*> children;
	GameObject() {}
	GameObject(const char*);

	GameObject(const char* name, const float3* pos, const Quat* rot) : name(name) {
		CreateTransform(pos, rot);
	}

	~GameObject() {}
	Component* CreateComponent(const ComponentType);
	Component* FindComponent(const ComponentType);
	GameObject* FindGameObject(std::string * );
	Transform* myTransform = nullptr;
	void DeleteChild(const GameObject*);
	void ShowProperties(bool* show);
	void OnLoad(rapidjson::Document::Object*, GameObject*);
	void OnSave(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
	void TransformAABB(float4x4* transform);
private:
	void CreateTransform(const float3*, const Quat*);
	void LoadChildren(rapidjson::Document::Array*, GameObject*);
	void LodComponents(rapidjson::Document::Array*);
	void SaveChildren(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
	void SaveComponents(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
};
#endif
