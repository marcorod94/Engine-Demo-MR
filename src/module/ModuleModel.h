#ifndef ModuleModel_h
#define ModuleModel_h

#include "main/Globals.h"
#include "Module.h"
#include "ModuleImGui.h"
#include "util/MeshImporter.h"
#include <vector>
#include <string>

struct par_shapes_mesh_s;
enum class ProgramType;
class GameObject;
class Mesh;
class Material;

enum class ShapeType {
	Sphere = 0,
	Torus,
	Cube,
	Cylinder,
	Count
};

struct MeshShape {
	ShapeType type;
	float size;
	float radius;
	unsigned slices;
	unsigned stacks;
};

struct Light {
	float4 color = float4::one;
	float3 pos = float3::one;
};
class ModuleModel: public Module {
public:
	MeshImporter* importer = new MeshImporter();
	Sphere bsphere;
	Light light;
	float ambient = 0.0f;

	ModuleModel() {}
	~ModuleModel() {}
	bool Init();
	bool CleanUp();
	void LoadShapes(GameObject*, const char*, const float3*, const Quat*, MeshShape*, ProgramType, const float4*);
	par_shapes_mesh_s* LoadSphere(float, unsigned, unsigned);
	par_shapes_mesh_s* LoadCylinder(float, float, unsigned, unsigned);
	par_shapes_mesh_s* LoadTorus(float, float, unsigned, unsigned);
	par_shapes_mesh_s* LoadCube(float);
	const void LoadModel(const char*);
	void UpdateTexture(Texture&);

private:
	std::string directory;
	void GenerateMesh(GameObject*, par_shapes_mesh_s*);

};
#endif
