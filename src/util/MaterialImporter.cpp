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
	texture.id = ilutGLBindTexImage();
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.name = name;
	std::string newFile = path;
	newFile.append("/").append(name.c_str()).append(".tbin");
	//output->append(newFile.c_str());
	ILuint fileSize;
	ILubyte *fiLeData;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	fileSize = ilSaveL(IL_DDS, NULL, 0);
	if (fileSize > 0) {
		fiLeData = new ILubyte[fileSize];
		if (ilSaveL(IL_DDS, fiLeData, fileSize) > 0) {
			unsigned int ranges[5] = { texture.id, texture.width, texture.height, texture.name.size(), fileSize };
			unsigned int size = sizeof(ranges) + name.size() + fileSize;

			char* data = new char[size];
			char* cursor = data;
			unsigned int bytes = sizeof(ranges);
			memcpy(cursor, ranges, bytes);

			cursor += bytes;
			bytes = name.size();
			memcpy(cursor, name.c_str(), bytes);

			cursor += bytes;
			bytes = fileSize;
			memcpy(cursor, fiLeData, bytes);

			App->filesys->Save(newFile.c_str(), data, size);
		}
	}
	return true;
}


bool MaterialImporter::Load(const char* exportedFile, Texture* texture) {
	char* buffer;
	App->filesys->Load(exportedFile, &buffer);
	char* cursor = buffer;
	char* name;
	unsigned int ranges[5];
	unsigned int bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	
	texture->id = ranges[0];
	texture->width = ranges[1];
	texture->height = ranges[2];

	name = new char[ranges[3]];
	cursor += bytes;
	bytes = ranges[3];
	memcpy(name, cursor, bytes);

	texture->name.append(name);
	texture->data = new unsigned char[ranges[4]];
	cursor += bytes;
	bytes = ranges[4];
	memcpy(texture->data, cursor, bytes);

	return true;
}
