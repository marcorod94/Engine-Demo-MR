#ifndef ModuleFileSystem_h
#define ModuleFileSystem_h
#include "Module.h"
#include <fstream>
#define ASSETS_FOLDER "Assets"
#define LIBRARY_FOLDER "Library"
#define TEXTURES_FOLDER "Textures"
#define MODELS_FOLDER "Models"

class ModuleFileSystem : public Module {
public:
	ModuleFileSystem() {}
	~ModuleFileSystem() {}

	bool Init();
	update_status Update();
	bool CleanUp();

	void Load(const char*, char**) const;
	void Load(const char*, char**, int*) const;
	unsigned Save(const char*, const void*, unsigned, bool append = false);
	bool Remove(const char*);
	bool Exists(const char*);
	bool MakeDirectory(const char*);
	bool IsDirectory(const char*) const;
	bool Copy(const char*, const char*);
};
#endif
