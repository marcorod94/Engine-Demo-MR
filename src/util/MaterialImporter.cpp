#include "MaterialImporter.h"
#include "main/Application.h"
#include "module/ModuleFileSystem.h"
#include "component/Material.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

MaterialImporter::MaterialImporter() {
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilGenImages(1, &imageName);
	ilBindImage(imageName);
}

MaterialImporter::~MaterialImporter() {
	ilDeleteImages(1, &imageName);
}

bool MaterialImporter::Import(const char* file, const char* path, std::string* output) {
	std::string name = file;
	name = name.substr(name.find_last_of('\\') + 1, name.size());
	name = name.substr(name.find_last_of('/') + 1, name.size());
	name = name.substr(0 , name.find_last_of('.'));
	ilLoadImage(file);
	std::string newFile = path;
	newFile.append("/").append(name.c_str()).append(".tbin");
	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, NULL, 0);
	if (size > 0) {
		data = new ILubyte[size];
		int fsize = ilSaveL(IL_DDS, data, size);
		if (fsize > 0) {
			App->filesys->Save(newFile.c_str(), data, size);
		}
		
	}
	return true;
}


bool MaterialImporter::Load(const char* file, Texture* texture) {
	std::string name = file;
	name = name.substr(name.find_last_of('/') + 1, name.size());
	name = name.substr(0, name.find_last_of('.'));
	char *buffer;
	int size;
	App->filesys->Load(file, &buffer, &size);
	ilLoadL(IL_DDS, buffer, size);
	texture->id = ilutGLBindTexImage();
	texture->name = name;
	texture->path = file;
	texture->width = ilGetInteger(IL_IMAGE_WIDTH);
	texture->height = ilGetInteger(IL_IMAGE_HEIGHT);
	return true;
}
