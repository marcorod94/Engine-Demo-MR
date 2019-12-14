#ifndef ModuleCamera_h
#define ModuleCamera_h

#include "Module.h"
#include "MathGeoLib.h"
#include "component/Camera.h"
#include "GL/glew.h"

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
	void ZoomIn();
	void ZoomOut();
	void UpdateAspectRatio();
	void Focus();
	bool CleanUp();

	void Draw();
	void CenterCamera();

	Camera* CreateComponentCamera();
	Camera *sceneCamera = nullptr;
public:
	Frustum frustum;
	float4x4 proj, view, model;
	float3 helper1 = float3(0.0F, -1.0F, -4.0F);
	float3 helper2 = float3(1.0F, 1.0F, 1.0F);
	float cameraSpeed = 0.05f;
private:
	std::vector<Camera*> cameras;
	GameObject *defaultCameraGO = nullptr;

	float movementSpeed = cameraSpeed;
	float3 cameraTarget = float3::zero;
	bool orbit = false;
	float yaw = 0.0f;
	float pitch = -90.0f;
	void CalculateRotationAngles(float3&);

	Camera*  camera = nullptr;
	
};
#endif
