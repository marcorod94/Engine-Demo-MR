#ifndef Component_h
#define Component_h
#include "main/Globals.h"
#include "main/GameObject.h"
#include "crossguid/guid.hpp"
class Component {
public:
	std::string uuid = xg::newGuid().str();
	ComponentType type;
	bool active;
	GameObject* owner;
	Component() {}
	Component(GameObject* owner, const ComponentType type): owner(owner), type(type) {}
	virtual ~Component() {}
	virtual void Enable() { active = true; }
	virtual update_status Update() { return UPDATE_CONTINUE; }
	virtual void Disable() { active = false; }
	virtual void DrawView() {}
};
#endif
