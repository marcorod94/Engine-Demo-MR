#ifndef Component_h
#define Component_h
#include "main/Globals.h"
#include "main/GameObject.h"
#include "crossguid/guid.hpp"
#include "rapidjson/document.h"

enum class ComponentType { Transform, Mesh, Material, Camera };
class Component {
public:
	std::string uuid = xg::newGuid().str();
	ComponentType type;
	bool active = true;
	GameObject* owner = nullptr;
	Component() {}
	Component(GameObject* owner, const ComponentType type): owner(owner), type(type) {}
	virtual ~Component() {}
	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	virtual void DrawView() {}
	virtual void OnLoad(rapidjson::Document::Object*) {}
	virtual void OnSave(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*) {}
protected:
	virtual void GetFloat3FromObjectJSON(rapidjson::Document::Object* object, float3* vector) {
		vector->x = (object->FindMember("x"))->value.GetFloat();
		vector->y = (object->FindMember("y"))->value.GetFloat();
		vector->z = (object->FindMember("z"))->value.GetFloat();
	}

	virtual void AddFloat3ToObjectJSON(rapidjson::Document::Object* object, rapidjson::Document::AllocatorType* allocator, const char* label, float3* vector) {
		rapidjson::Value ob(rapidjson::kObjectType);
		ob.AddMember("x", vector->x, * allocator);
		ob.AddMember("y", vector->y, *allocator);
		ob.AddMember("z", vector->z, *allocator);
		object->AddMember(rapidjson::StringRef(label), ob, *allocator);
	}
};
#endif
