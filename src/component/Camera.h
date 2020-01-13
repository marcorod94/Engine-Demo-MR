#ifndef Camera_h
#define Camera_h

#include "MathGeoLib.h"
#include "Component.h"
#include <string>

const int IS_IN = 0;
const int IS_OUT = 1;
const int INTERSECT = 2;

class Camera : public Component {
public:
	Camera(GameObject* theOwner);
	~Camera();

	void SetFrustum();
	Frustum frustum;

	void Draw(const char*);
	void GenerateFBOTexture(unsigned w, unsigned h);
	void GenerateMatrices();
	int isCollidingFrustum(const AABB& aabb) const;
	void DrawFrustumPlanes();
	void DrawView();

	void CreateRay(const float2& normalizedPos, LineSegment &value) const;

	float4x4 proj, view, model;
	float cameraSpeed = 0.05f;
	unsigned fbo = 0;
	unsigned rbo = 0;
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
	bool isHovered = false;
	float yaw = 0.0f;
	float pitch = 0.0f;
private:
	
	float aspect = 1.f;
	unsigned fb_depth = 0;
	unsigned fb_tex = 0;
	unsigned fb_width = 0;
	unsigned fb_height = 0;
};
#endif
