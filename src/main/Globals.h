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

// Configuration -----------
#define VSYNC true
#define SCREEN_SIZE 2
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 720
#define FULLSCREEN true
#define RESIZABLE true
#define TITLE "Infinity Engine"
#endif
