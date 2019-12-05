#include "GameObject.h"

update_status GameObject::Update() {
	return UPDATE_CONTINUE;
}

Component* GameObject::createComponent(ComponentType type) {
	return nullptr;
}