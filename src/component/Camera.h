<<<<<<< HEAD
#ifndef Camera_h
#define Camera_h

#include "MathGeoLib.h"
#include "Component.h"

const int IS_IN = 0;
const int IS_OUT = 1;
const int INTERSECT = 2;

class Camera : public Component {
public:
	Camera(GameObject* owner);
	~Camera();

	void SetFrustum();

	void Draw();
	void GenerateFBOTexture(unsigned w, unsigned h);

	int isCollidingFrustum(const AABB& aabb) const;
	
	float4x4 proj, view, model;
	float cameraSpeed = 0.05f;
	unsigned fbo = 0;
	float3 camPos;

	float Hnear;
	float Wnear;

	float Hfar;
	float Wfar;

	float nearDist;
	float farDist;

	float3 nCenter;
	float3 fCenter;

	float3 ntl;//near top left
	float3 ntr;//near top right
	float3 nbl;
	float3 nbr;

	float3 ftl;
	float3 ftr;
	float3 fbl;//far bottom left
	float3 fbr;//far bottom right

	float height = 600;
	float width = 800;

private:
	Frustum frustum;
	float aspect = 1.f;
	
	unsigned fb_depth = 0;
	unsigned fb_tex = 0;
	unsigned fb_width = 0;
	unsigned fb_height = 0;
};
#endif
=======
#ifndef Camera_h
#define Camera_h

#include "MathGeoLib.h"
#include "Component.h"

const int IS_IN = 0;
const int IS_OUT = 1;
const int INTERSECT = 2;

class Camera : public Component {
public:
	Camera(GameObject* owner);
	~Camera();

	void SetFrustum();
	void SetFOV(const float fov);
	void UpdateAspectRatio();
	void SetNearDistance(const float distance);
	void SetFarDistance(const float distance);

	float4x4 LookAt(float3&, float3&, float3&) const;

	void SetTransform(const math::float3& f, const math::float3& r, const math::float3& u, const math::float3& pos);
	void SetPosition(const math::float3& pos);
	void SetRotation(const math::Quat& quat);



	void SetPerspective(float fovY, float aspect, float near, float far);
	void SetOrtho(float left, float right, float bottom, float top, float near, float far);

	void Focus();

	void Draw();
	void GenerateFBOTexture(unsigned w, unsigned h);
	void CenterCamera();

	int isCollidingFrustum(const AABB& aabb) const;

	void GenerateFrameBuffers(const float width, const float height);//check

	
	float4x4 proj, view, model;
	float cameraSpeed = 0.05f;
	unsigned fbo = 0;
	float3 camPos;

	float Hnear;
	float Wnear;

	float Hfar;
	float Wfar;

	float nearDist;
	float farDist;

	float3 nCenter;
	float3 fCenter;

	float3 ntl;//near top left
	float3 ntr;//near top right
	float3 nbl;
	float3 nbr;

	float3 ftl;
	float3 ftr;
	float3 fbl;//far bottom left
	float3 fbr;//far bottom right

	float height = 600;
	float width = 800;
private:
	float movementSpeed = cameraSpeed;
	float3 cameraTarget = float3::zero;
	bool orbit = false;
	float yaw = 0.0f;
	float pitch = -90.0f;
	void CalculateRotationAngles(float3&);

	Frustum frustum;
	float aspect = 1.f;
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	unsigned fb_depth = 0;
	unsigned fb_tex = 0;
	unsigned fb_width = 0;
	unsigned fb_height = 0;
};
#endif
>>>>>>> refs/remotes/origin/develop
