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

void ModuleTexture::LoadTexture(const char* path) {
	ilLoadImage(path);
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	textureID = ilutGLBindTexImage();
	glGenTextures(1, &textureID);
	LOG("Width: %d", ilGetInteger(IL_IMAGE_WIDTH));
	LOG("Height: %d", ilGetInteger(IL_IMAGE_HEIGHT));

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_BGR, GL_UNSIGNED_BYTE, ilGetData());
	glGenerateMipmap(GL_TEXTURE_2D);
}
