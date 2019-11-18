#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"


ModuleTexture::ModuleTexture() {}


ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init() {
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilGenImages(1, &imageName);
	ilBindImage(imageName);
	return true;
}

bool ModuleTexture::CleanUp() {
	ilDeleteImages(1, &imageName);
	return true;
}

Texture ModuleTexture::LoadTexture(const char* path) {
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		if (std::strcmp(loadedTextures[i].path, path) == 0) {
			LOG("Cargada: %s", loadedTextures[i].path);
			return loadedTextures[i];
		}
	}
	Texture texture;
	ilLoadImage(path);
	iluGetImageInfo(&imageInfo);
	texture.id = ilutGLBindTexImage();
	texture.type = "texture_diffuse";
	texture.path = path;
	loadedTextures.push_back(texture);
	return texture;
}
