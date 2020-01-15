#ifndef Mesh_h
#define Mesh_h
#include "Component.h"
class Mesh : public Component {
public:
	unsigned int vbo, ebo, vao;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	AABB box;
	int totalPrimitives = 0;
	int totalVertex = 0;
	int totalMaterials = 0;
	Mesh(GameObject* owner): Component(owner, ComponentType::Mesh) {
		box.SetNegativeInfinity();
	}
	void Setup();
	void DrawView();
	void OnLoad(rapidjson::Document::Object*);
	void OnSave(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
};
#endif
