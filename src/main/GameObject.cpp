#include "GameObject.h"
#include "component/Component.h"
#include "component/Transform.h"
#include "imgui.h"
#include <algorithm>

update_status GameObject::Update() {
	return UPDATE_CONTINUE;
}

void GameObject::CreateComponent(ComponentType type) {
	if (type == ComponentType::Transform) {
		components.push_back(new Transform(this));
	}
	
}

Component* GameObject::FindComponent(ComponentType type) {
	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i]->type == type) {
			return components[i];
		}
	}
	return nullptr;
}

void GameObject::DeleteChild(const GameObject* child) {
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
	
}

void GameObject::ShowProperties() {
	ImGui::Begin("Properties Test");
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->uuid = uuid;
		(*it)->DrawView();
	}
	ImGui::End();
}