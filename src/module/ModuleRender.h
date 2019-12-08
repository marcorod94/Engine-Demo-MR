#ifndef ModuleRender_h
#define ModuleRender_h
#include "Module.h"
#include "main/Globals.h"
#include "SDL.h"

class Meshc;
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
	bool CleanUp();
	Meshc* CreateMesh();
private:
	void DrawGrid() const;
	void DrawAxis() const;
	void DrawGameObject(GameObject*);
	void DrawMesh(Meshc*);
	void DrawMaterial(Material*);
};
#endif
