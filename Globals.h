#pragma once
#include <windows.h>
#include <stdio.h>
#include <vector>
#include "MathGeoLib.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

struct Vertex {
	// position
	float3 Position;
	// normal
	float3 Normal;
	// texCoords
	float2 TexCoords;
	// tangent
	float3 Tangent;
	// bitangent
	float3 Bitangent;
};


struct Texture {
	unsigned int id;
	char*  type;
	const char* path;
	int width;
	int height;
	unsigned char *data;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
};


// Configuration -----------
#define VSYNC true
#define SCREEN_SIZE 2
#define TITLE "Super Awesome Engine"
