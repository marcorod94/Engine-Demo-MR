#include "ModuleFileSystem.h"
bool ModuleFileSystem::Init() {
	return true;
}

update_status ModuleFileSystem::Update() {
	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp() {
	return true;
}

unsigned ModuleFileSystem::Load(const char* path, const char* file, char** buffer) const {
	return 0;
}

unsigned ModuleFileSystem::Save(const char* file, const void* buffer, unsigned size, bool append) const {
	return 0;
}

bool ModuleFileSystem::Remove(const char* file) {
	return true;
}

bool ModuleFileSystem::Exists(const char* file) {
	return true;
}

bool ModuleFileSystem::MakeDirectory(const char* directory) {
	return true;
}

bool ModuleFileSystem::IsDirectory(const char* file) const {
	return true;
}

bool ModuleFileSystem::Copy(const char* source, const char* destination) {
	return true;
}
