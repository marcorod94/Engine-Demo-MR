#ifndef ModuleDebugDraw_h
#define ModuleDebugDraw_h
#include "Module.h"
class Camera;
class DDRenderInterface;
class ModuleDebugDraw : public Module {
public:
	ModuleDebugDraw() {}
	~ModuleDebugDraw() {}

	bool Init();
	bool CleanUp();
	void Draw(Camera*);
private:
	static DDRenderInterface* ddRenderer;
};
#endif
