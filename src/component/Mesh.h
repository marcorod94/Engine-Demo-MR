#ifndef Meshc_h
#define Meshc_h
#include "Component.h"
class Meshc : public Component {
public:
	unsigned int vbo, ebo, vao;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	AABB box;
	int totalPrimitives = 0;
	int totalVertex = 0;
	int totalMaterials = 0;
	Meshc() { type = ComponentType::Mesh; }
	void Setup();
	void DrawView();
};
#endif
