#ifndef ModuleCamera_h
#define ModuleCamera_h

#include "Module.h"
#include "MathGeoLib.h"
#include "GL/glew.h"

class GameObject;
class Camera;
class ModuleCamera : public Module {
public:
	ModuleCamera() {}
	~ModuleCamera() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	float4x4 LookAt(float3&, float3&, float3&) const;
	void SetFOV(Camera* cam, const float fov);
	void UpdateAspectRatio(Camera* cam, float w, float h);
	void SetNearDistance(Camera* cam, const float distance);
	void SetFarDistance(Camera* cam, const float distance);

	void MouseMove(Camera* cam);
	void MouseScrolling(Camera* cam);
	void ZoomIn(Camera* cam);
	void ZoomOut(Camera* cam);
	void Focus(Camera* cam);

	Camera* CreateComponentCamera(GameObject*);
public:
	Frustum frustum;
	Camera* sceneCameras =  nullptr;
	Camera* currentCamera = nullptr;
	std::vector<Camera*> loadedCameras;
	float3 helper1 = float3(0.0f, -1.0f, -4.0f);
	float3 helper2 = float3(1.0f, 1.0f, 1.0f);
	float cameraSpeed = 0.05f;
	float sensitivity = 0.2F;

private:
	GameObject *sceneCameraGO = nullptr;
	float movementSpeed = cameraSpeed;
	float3 cameraTarget = float3::zero;
	bool orbit = false;
	
	void CalculateRotationAngles(float3&);
};
#endif
