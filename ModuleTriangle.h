#ifndef ModuleTriangle_h
#define ModuleTriangle_h
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"
#include "MathGeoLib.h"

class ModuleTriangle : public Module {
public:
	ModuleTriangle();
	~ModuleTriangle();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	float4x4 LookAt(float3, float3, float3);
private:
	GLuint vbo = 1;
	float4x4 proj, view, model;
};
#endif

