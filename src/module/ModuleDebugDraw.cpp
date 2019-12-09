#include "util/DDRenderInterface.h"
#include "ModuleDebugDraw.h"

DDRenderInterface* ModuleDebugDraw::ddRenderer = 0;

bool ModuleDebugDraw::Init() {
	ddRenderer = new DDRenderInterface;
	/*dd::initialize(ddRenderer);*/
	return true;
}
bool ModuleDebugDraw::CleanUp() {
	/*dd::shutdown();*/

	delete ddRenderer;
	ddRenderer = 0;

	return true;
}