#include "main/Application.h"
#include "ModuleProgram.h"
#include "GL/glew.h"
#include <fstream>
#include <sstream>

bool ModuleProgram::Init() {
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	return true;
}

update_status ModuleProgram::Update() {
	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp() {
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return true;
}


void ModuleProgram::LoadShaders(const char* vertex_path, const char* fragment_path) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertex_path);
		fShaderFile.open(fragment_path);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		LOG("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	// Check Vertex Shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		LOG("Error Loading vertex shader");
	}

	// Compile Fragment Shader
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	// Check Fragment Shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		LOG("Error Loading fragment shader");
	}

	//// Link the program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		LOG("Error Loading program");
	}
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
