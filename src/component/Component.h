#ifndef Component_h
#define Component_h
#include "main/Globals.h"
#include "main/GameObject.h"

enum class ComponentType { Transform, Mesh, Material };
class Component {
public :
	ComponentType type;
	bool active;
	GameObject* owner;
	Component() {}
	~Component() {}
	virtual void Enable() { active = true; }
	virtual update_status Update() { return UPDATE_CONTINUE; }
	virtual void Disable() { active = false; }
	virtual void DrawView() {}
};
#endif
