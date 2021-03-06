#ifndef Camera_h
#define Camera_h

#include "MathGeoLib.h"
#include "Component.h"
#include "imgui.h"
#include <string>

const int IS_IN = 0;
const int IS_OUT = 1;
const int INTERSECT = 2;

class Camera : public Component {
public:
	Camera(GameObject* theOwner);
	~Camera();

	Frustum frustum;

	void Draw(const char*);
	void GenerateFBOTexture(unsigned w, unsigned h);
	void GenerateMatrices();
	int isCollidingFrustum(const AABB& aabb) const;
	void DrawFrustumPlanes();
	void DrawView();
	void OnLoad(rapidjson::Document::Object*);
	void OnSave(rapidjson::Document::Array*, rapidjson::Document::AllocatorType*);

	void CreateRay(const float2& normalizedPos, LineSegment &value) const;

	float4x4 proj, view, model;
	float cameraSpeed = 0.05f;
	ImVec2 hoveredWindowPos;
	ImVec2 hoveredWindowSize;
	unsigned fbo = 0;
	unsigned rbo = 0;
	

	float height = 600;
	float width = 800;
	bool isHovered = false;
	float yaw = 0.0f;
	float pitch = 0.0f;
private:
	unsigned fb_depth = 0;
	unsigned fb_tex = 0;
	unsigned fb_width = 0;
	unsigned fb_height = 0;
};
#endif
