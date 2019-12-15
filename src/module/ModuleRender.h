#ifndef ModuleRender_h
#define ModuleRender_h
#include "Module.h"
#include "component/Camera.h"
#include "main/Globals.h"
#include "SDL.h"


class Camera;
class Mesh;
class Material;
class GameObject;
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

	void DisplayFrameBuffer(Camera* camera, unsigned fbo, unsigned fb_width, unsigned fb_height);

	bool CleanUp();
	Mesh* CreateMesh();
private:
	void DrawGrid(Camera* cam) const;
	void DrawAxis() const;
	void DrawGameObject(GameObject*, Camera*);
	void DrawMesh(Mesh*);
	void DrawMaterial(Material*);
};
#endif
