#include "MaterialImporter.h"
#include "main/Application.h"
#include "module/ModuleFileSystem.h"
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
	Texture texture;
	texture.id = 0;
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.name = name;
	std::string newFile = name;
	newFile.append(".bin");
	char* buffer = new char[sizeof(Texture)];
	memcpy(buffer, &texture, sizeof(Texture));
	//Save to new file
	App->filesys->Save(newFile.c_str(), texture.data, sizeof(Texture));
	return true;
}


bool MaterialImporter::Load(const char* exportedFile, Texture* texture) {
	char* buffer;
	App->filesys->Load("", exportedFile, &buffer);
	memcpy(texture, buffer, sizeof(Texture));
	return true;
}
