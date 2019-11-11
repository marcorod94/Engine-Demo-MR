#ifndef ModuleCamera_h
#define ModuleCamera_h

#include "Module.h"
#include "MathGeoLib.h"

class ModuleCamera : public Module {
public:
	ModuleCamera();
	~ModuleCamera();
	bool Init();
	update_status PreUpdate();
	float4x4 LookAt(float3, float3, float3);
public:
	Frustum frustum;
	float4x4 proj, view, model;
	float3 helper1 = float3(0.0F, 0.0F, -4.0F);
	float3 helper2 = float3(1.0F, 1.0F, 1.0F);
	float3 helper3 = float3(0.0F, 1.0F, 0.0F);
};
#endif
