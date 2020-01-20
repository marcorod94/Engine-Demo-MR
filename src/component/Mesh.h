#ifndef Mesh_h
#define Mesh_h
#include "Component.h"

struct Vertex {
	// position
	math::float3 Position;
	// normal
	math::float3 Normal;
	// texCoords
	math::float2 TexCoords;
	// tangent
	math::float3 Tangent;
	// bitangent
	math::float3 Bitangent;
};

class Mesh : public Component {
public:
	unsigned int vbo, ebo, vao;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	AABB box;
	std::string src;
	Mesh(GameObject* owner): Component(owner, ComponentType::Mesh) {
		box.SetNegativeInfinity();
		originalBox.SetNegativeInfinity();
	}
	void Setup();
	void DrawView();
	void OnLoad(rapidjson::Document::Object*);
	void OnSave(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);
	void TransformAABB(float4x4* transform);
private:
	AABB originalBox;
	void CalculateBoundigBox();
};
#endif
