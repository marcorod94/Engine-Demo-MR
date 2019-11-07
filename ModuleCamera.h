#ifndef ModuleCamera_h
#define ModuleCamera_h

#include "Module.h"
#include "MathGeoLib.h"

class ModuleCamera : public Module {
public:
	ModuleCamera();
	~ModuleCamera();
	bool Init();
	float4x4 LookAt(float3, float3, float3);
public:
	Frustum frustum;
	float4x4 proj, view, model;
};
#endif
