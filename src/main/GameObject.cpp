#include "GameObject.h"
#include "main/Application.h"
#include "module/ModuleCamera.h"
#include "module/ModuleRender.h"
#include "module/ModuleTexture.h"

#include "component/Component.h"
#include "component/Transform.h"
#include "component/Mesh.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <algorithm>
//#include "rapidjson/rapidjson.h"
//#include "rapidjson/document.h"

GameObject::GameObject(const char* name) : name(name) {
	CreateComponent(ComponentType::Transform);
}

Component* GameObject::CreateComponent(ComponentType type) {
	Component* newComponent;
	switch (type)
	{
	case ComponentType::Transform :
		newComponent = new Transform(this);
		break;

	case ComponentType::Mesh:
		newComponent = App->renderer->CreateMesh(this);
		break;

	case ComponentType::Material:
		newComponent = App->texture->CreateMaterial(this);
		break;

	case ComponentType::Camera:
		newComponent = App->camera->CreateComponentCamera(this);
		break;

	default:
		return nullptr;
	}
	if (newComponent) {
		newComponent->owner = this;
		components.push_back(newComponent);
	}
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

GameObject* GameObject::FindGameObject(std::string* uuid) {
	GameObject* obj = nullptr;
	if (this->uuid.compare(*uuid) == 0) {
		return this;
	}
	else {
		for (GameObject* child : children) {
			if (obj = child->FindGameObject(uuid)) {
				break;
			}
		}
	}
	return obj;
}

void GameObject::DeleteChild(const GameObject* child) {
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
	
}

void GameObject::ShowProperties(bool* show) {	
	ImGui::SetNextWindowPos(ImVec2(916.0f, 16.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(256.0f, 600.0f), ImGuiCond_FirstUseEver);
	ImGui::Begin("Properties", show);
	if (parent) {
		ImGui::Text("UUID: %s", uuid.c_str());
		ImGui::InputText("Name", &name);
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
			(*it)->DrawView();
		}
	}
	ImGui::End();
}

void GameObject::CreateTransform(const float3* pos, const Quat* rot) {
	components.push_back(new Transform(this, pos, rot));
}

void GameObject::OnLoad(rapidjson::Document::Object* object, GameObject* parent) {
	uuid = (object->FindMember("uuid"))->value.GetString();
	name = (object->FindMember("name"))->value.GetString();
	LoadChildren(&(object->FindMember("children"))->value.GetArray(), this);
	LodComponents(&(object->FindMember("component"))->value.GetArray());
	if (parent) {
		this->parent = parent;
		parent->children.push_back(this);
	}

}

void GameObject::LoadChildren(rapidjson::Document::Array* list, GameObject* parent) {
	assert(list);
	GameObject* obj = nullptr;
	for (auto& item : *list) {
		obj = new GameObject();
		obj->OnLoad(&item.GetObjectA(), parent);
	}
}
void GameObject::LodComponents(rapidjson::Document::Array* list) {
	assert(list);
	Component* comp = nullptr;
	for (auto& item : *list) {
		auto obj = item.GetObjectA();
		comp = CreateComponent(ComponentType((obj.FindMember("type"))->value.GetInt()));
		comp->OnLoad(&obj);
	}
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
