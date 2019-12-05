#include "GameObject.h"
#include "component/Component.h"
#include "component/Transform.h"

update_status GameObject::Update() {
	return UPDATE_CONTINUE;
}

const Component* GameObject::CreateComponent(ComponentType type) {
	if (type == ComponentType::Transform) {
		
	}
	return nullptr;
}

const Component* GameObject::FindComponent(ComponentType type) {
	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i]->type == type) {
			return components[i];
		}
	}
	return nullptr;
}