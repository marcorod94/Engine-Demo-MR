#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleImGui.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

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

Texture ModuleTexture::LoadTexture(std::string& path) {
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		if (path.compare(loadedTextures[i].path) == 0) {
			App->imgui->AddLog("Texture already loaded: %s", loadedTextures[i].path.c_str());
			return loadedTextures[i];
		}
	}
	Texture texture;
	ilLoadImage(path.c_str());
	iluGetImageInfo(&imageInfo);
	texture.id = ilutGLBindTexImage();
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.type = "texture_diffuse";
	texture.path = path;
	loadedTextures.push_back(texture);
	App->imgui->AddLog("Texture succssesfully loaded: %s", path.c_str());
	return texture;
}
