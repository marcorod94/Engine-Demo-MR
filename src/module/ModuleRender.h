#ifndef ModuleRender_h
#define ModuleRender_h
#include "Module.h"
#include "component/Camera.h"
#include "main/Globals.h"
#include "SDL.h"
#include "ImGuizmo.h"
#include<map>

class Camera;
class GameObject;
class Mesh;
class Material;
class Transform;

class ModuleRender : public Module {
public:
	SDL_GLContext glContext;
	bool showGrid = true;
	bool showAxis = false;

	ModuleRender() {}
	~ModuleRender() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	GameObject* RayIntersectsObject(float3 origin, LineSegment &ray);
	void DrawGizmo(GameObject* selected);
	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE gizmoMode = ImGuizmo::WORLD;
	bool CleanUp();
	Mesh* CreateMesh(GameObject*);
	void DrawAABB(GameObject* go);
	
private:
	void DrawGameObject(GameObject*, Camera*);
	void DrawMesh(Camera*, Transform*, Mesh*, Material*);
	GameObject* SearchGO(GameObject* go, float3* origin, LineSegment* ray, std::map<float, GameObject*>* intersected);
	
};
#endif
