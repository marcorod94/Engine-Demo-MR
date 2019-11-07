#ifndef ModuleModelLoader_h
#define ModuleModelLoader_h
#include "Module.h"
#include <assimp/scene.h>
class ModuleModelLoader :
	public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();
	bool Init();
	bool CleanUp();
	void LoadModel(const char*);
public:
	aiScene* scene;
};
#endif
