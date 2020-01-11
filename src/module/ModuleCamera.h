#ifndef ModuleCamera_h
#define ModuleCamera_h

#include "Module.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
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
	//void SetTransform(const math::float3& f, const math::float3& r, const math::float3& u, const math::float3& pos);
	void SetPosition(Camera* cam, const math::float3& pos);
	void SetRotation(Camera* cam, const math::Quat& quat);
	void SetPerspective(Camera* cam, float fovY, float aspect, float near, float far);
	void SetOrtho(Camera* cam, float left, float right, float bottom, float top, float near, float far);
	void CenterCamera(Camera* cam);

	void MouseMove(Camera* cam);
	void MouseScrolling(Camera* cam);
	void ZoomIn(Camera* cam);
	void ZoomOut(Camera* cam);
	void Focus(Camera* cam);

	Camera* CreateComponentCamera();
public:
	Frustum frustum;
	Camera* sceneCameras =  nullptr;
	Camera* currentCamera = nullptr;
	std::vector<Camera*> loadedCameras;
	float3 helper1 = float3(0.0f, -1.0f, -4.0f);
	float3 helper2 = float3(1.0f, 1.0f, 1.0f);
	float cameraSpeed = 0.05f;

private:
	GameObject *sceneCameraGO = nullptr;
	float movementSpeed = cameraSpeed;
	float3 cameraTarget = float3::zero;
	bool orbit = false;
	float yaw = 0.0f;
	float pitch = -90.0f;
	void CalculateRotationAngles(float3&);
};
#endif
