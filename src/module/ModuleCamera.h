#ifndef ModuleCamera_h
#define ModuleCamera_h

#include "Module.h"
#include "MathGeoLib.h"

class ModuleCamera : public Module {
public:
	ModuleCamera() {}
	~ModuleCamera() {}
	bool Init();
	update_status PreUpdate();
	update_status Update();
	float4x4 LookAt(float3&, float3&, float3&) const;
	void MouseMove();
	void MouseScrolling();
	void UpdateAspectRatio();
	void Focus();
public:
	Frustum frustum;
	float4x4 proj, view, model;
	float3 helper1 = float3(0.0F, -1.0F, -4.0F);
	float3 helper2 = float3(1.0F, 1.0F, 1.0F);
	float cameraSpeed = 2.0f;
private:
	float movementSpeed = cameraSpeed;
	float3 cameraTarget = float3::zero;
	bool orbit = false;
	float yaw = 0.0f;
	float pitch = -90.0f;
	void CalculateRotationAngles(float3&);
};
#endif
