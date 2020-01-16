#ifndef ModuleFileSystem_h
#define ModuleFileSystem_h
#include "Module.h"
class ModuleFileSystem : public Module {
public:
	ModuleFileSystem() {}
	~ModuleFileSystem() {}

	bool Init();
	update_status Update();
	bool CleanUp();

	unsigned Load(const char*, const char*, char**) const;
	unsigned Save(const char*, const void*, unsigned, bool append = false) const;
	bool Remove(const char*);
	bool Exists(const char*);
	bool MakeDirectory(const char*);
	bool IsDirectory(const char*) const;
	bool Copy(const char*, const char*);
};
#endif
