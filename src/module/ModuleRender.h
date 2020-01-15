#ifndef ModuleRender_h
#define ModuleRender_h
#include "Module.h"
#include "component/Camera.h"
#include "main/Globals.h"
#include "SDL.h"

class Camera;
class GameObject;
class Mesh;
class Material;
class Transform;

class ModuleRender : public Module {
public:
	SDL_GLContext glContext;
	bool showGrid = true;
	bool showAxis = true;

	ModuleRender() {}
	~ModuleRender() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	GameObject* RayIntersectsObject(float3 origin, LineSegment &ray);
	bool CleanUp();
	Mesh* CreateMesh(GameObject*);
private:
	void DrawGameObject(GameObject*, Camera*);
	void DrawMesh(Camera*, Transform*, Mesh*, Material*);
};
#endif
