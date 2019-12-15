#ifndef ModuleDebugDraw_h
#define ModuleDebugDraw_h
#include "Module.h"
class DDRenderInterface;
class ModuleDebugDraw : public Module {
public:
	ModuleDebugDraw() {}
	~ModuleDebugDraw() {}

	bool Init();
	bool CleanUp();
	void Draw(unsigned fbo);
private:
	static DDRenderInterface* ddRenderer;
};
#endif
