#ifndef ModuleCamera_h
#define ModuleCamera_h

#include "Module.h"
#include "MathGeoLib.h"
#define CAM_SPEED 0.05f

class ModuleCamera : public Module {
public:
	ModuleCamera();
	~ModuleCamera();
	bool Init();
	update_status PreUpdate();
	update_status Update();
	float4x4 LookAt(float3, float3, float3);
	void MouseMove();
	void MouseScrolling();
public:
	Frustum frustum;
	float4x4 proj, view, model;
	float3 helper1 = float3(0.0F, -1.0F, -4.0F);
	float3 helper2 = float3(1.0F, 1.0F, 1.0F);
	float3 helper3 = float3(0.0F, 1.0F, 0.0F);
	float cameraSpeed = CAM_SPEED; 
	float yaw = -90.0f;
	float pitch = 0.0f;
};
#endif
