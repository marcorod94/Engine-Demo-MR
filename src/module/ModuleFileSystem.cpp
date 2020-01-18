#include "main/Application.h"
#include "ModuleFileSystem.h"
#include "ModuleImGui.h"
#include <experimental/filesystem>

namespace filesys = std::experimental::filesystem;

bool ModuleFileSystem::Init() {
	return true;
}

update_status ModuleFileSystem::Update() {
	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp() {
	return true;
}

void ModuleFileSystem::Load(const char* path, const char* file, char** dest) const {
	std::ifstream ifs(file, std::ios::in | std::ios::binary | std::ios::ate);
	if (ifs.is_open()) {
		int size = ifs.tellg();
		*dest = new char[size];
		ifs.seekg(0, std::ios::beg);
		ifs.read(*dest, size);
		ifs.close();
	}
}

unsigned ModuleFileSystem::Save(const char* file, const void* buffer, unsigned size, bool append) const {
	int mode = std::ios::out | std::ios::binary;
	if (append) {
		mode |= std::ios::app;
	}	
	std::ofstream ofs(file, mode);
	if (ofs.is_open()) {
		ofs.write((char*)buffer, size);
		ofs.close();
	}
	return 0;
}

bool ModuleFileSystem::Remove(const char* file) {
	return filesys::remove(filesys::path(file));
}

bool ModuleFileSystem::Exists(const char* file) {
	return filesys::exists(filesys::path(file));
}

bool ModuleFileSystem::MakeDirectory(const char* directory) {
	return filesys::create_directory(filesys::path(directory));
}

bool ModuleFileSystem::IsDirectory(const char* file) const {
	return filesys::is_directory(filesys::path (file));
}

bool ModuleFileSystem::Copy(const char* source, const char* destination) {
	return filesys::copy_file(filesys::path(source), filesys::path(destination));
}
