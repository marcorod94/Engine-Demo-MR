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
	Camera();
	Camera(GameObject* theOwner);
	~Camera();

	void SetFrustum();
	Frustum frustum;
	/*float4x4 projectionMatrix = float4x4::zero;
	float4x4 viewMatrix = float4x4::zero;
	ImVec4 clearColor;
	int selectedClearMode;*/
	/*void SetFOV(float);
	void SetAspectRatio(float);
	void SetPlaneDistances(float, float);
	void ReloadMatrices();*/
	void Update() override;

	void Draw(const char*);
	void GenerateFBOTexture(unsigned w, unsigned h);
	void GenerateMatrices();
	int isCollidingFrustum(const AABB& aabb) const;
	
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
private:
	
	float aspect = 1.f;
	unsigned frustumVAO = 0;
	unsigned frustumVBO = 0;
	unsigned frustumEBO = 0;
	
	unsigned fb_depth = 0;
	unsigned fb_tex = 0;
	unsigned fb_width = 0;
	unsigned fb_height = 0;
};
#endif
