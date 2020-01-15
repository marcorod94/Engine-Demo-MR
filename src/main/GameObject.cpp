#include "GameObject.h"
#include "component/Component.h"
#include "component/Transform.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <algorithm>
//#include "rapidjson/rapidjson.h"
//#include "rapidjson/document.h"

Component* GameObject::CreateComponent(ComponentType type) {
	if (type == ComponentType::Transform) {
		components.push_back(new Transform(this));
	}

	Component *newComponent;
	switch (type)
	{
	case ComponentType::Camera:
		//components.push_back(new Camera());
		break;

	case ComponentType::Material:
		//components.push_back(new Material());
		break;

	case ComponentType::Mesh:
		//components.push_back(new Mesh());
		break;
	default:
		return nullptr;
	}

	newComponent->owner = this;
	components.push_back(newComponent);

	return newComponent;
	
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
	ImGui::SetNextWindowPos(ImVec2(916.0f, 16.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(256.0f, 600.0f), ImGuiCond_FirstUseEver);
	ImGui::Begin("Properties");
	ImGui::Text("UUID: %s", uuid.c_str());
	ImGui::InputText("Name", &name);
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->DrawView();
	}
	ImGui::End();
}

void GameObject::CreateTransform(const float3& pos, const Quat& rot) {
	components.push_back(new Transform(this, pos, rot));
}

void GameObject::OnLoad(rapidjson::Document* config) {

}

void GameObject::OnSave(rapidjson::Document::Array* list, rapidjson::Document::AllocatorType* allocator) {
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("uuid", rapidjson::StringRef(uuid.c_str()), *allocator);
	object.AddMember("name", rapidjson::StringRef(name.c_str()), *allocator);
	std::string parentuuid;
	if (parent) {
		parentuuid = parent->uuid;
	}
	object.AddMember("parentuuid", rapidjson::StringRef(parentuuid.c_str()), *allocator);

	rapidjson::Value jComponent(rapidjson::kArrayType);
	SaveComponents(&jComponent.GetArray(), allocator);
	object.AddMember("component", jComponent, *allocator);

	rapidjson::Value jChildren(rapidjson::kArrayType);
	SaveChildren(&jChildren.GetArray(), allocator);
	object.AddMember("children", jChildren, *allocator);

	list->PushBack(object, *allocator);

}

void GameObject::SaveChildren(rapidjson::Document::Array* list, rapidjson::Document::AllocatorType* allocator) {
	for (GameObject* child : children) {
		child->OnSave(list, allocator);
	}
}

void GameObject::SaveComponents(rapidjson::Document::Array* list, rapidjson::Document::AllocatorType* allocator) {
	for (Component* component : components) {
		component->OnSave(list, allocator);
	}
}
