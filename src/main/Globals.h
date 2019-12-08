#ifndef Globals_h
#define Globals_h

#include <windows.h>
#include <stdio.h>
#include <vector>
#include "MathGeoLib.h"
#include <string>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum class ComponentType { Transform, Mesh, Material };

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
	std::string  type;
	int width;
	int height;
	std::string path;
};


// Configuration -----------
#define VSYNC true
#define SCREEN_SIZE 2
#define TITLE "EM ENGINE"
#endif
