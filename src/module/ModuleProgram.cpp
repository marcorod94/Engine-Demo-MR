#include "main/Application.h"
#include "ModuleProgram.h"
#include "GL/glew.h"
#include <fstream>
#include <sstream>

bool ModuleProgram::Init() {
	programs[int(Program::Default)] = CreateProgram("Shaders/default.vs", "Shaders/default.fs");
	/*programs[int(Program::Flat)] = LoadShaders("Shaders/flat.vs", "Shaders/flat.fs");
	programs[int(Program::Gouraud)] = LoadShaders("Shaders/gouraud.vs", "Shaders/gouraud.fs");
	programs[int(Program::Phong)] = LoadShaders("Shaders/phong.vs", "Shaders/phong.fs");
	programs[int(Program::Blinn)] = LoadShaders("Shaders/blinn.vs", "Shaders/blinn.fs");*/
	return true;
}

bool ModuleProgram::CleanUp()
{
	for (unsigned i = 0; i < int(Program::Count); ++i)
	{
		glDeleteProgram(programs[i]);
	}

	return true;
}

unsigned ModuleProgram::CreateProgram(const char* vshader, const char* fshader)
{
	unsigned programId = 0;
	unsigned vertexId = glCreateShader(GL_VERTEX_SHADER);
	unsigned fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	bool ok = Compile(vertexId, LoadFile(vshader)) && Compile(fragmentId, LoadFile(fshader));

	
	if (ok) {
		programId = glCreateProgram();
		GLint result = GL_FALSE;
		int infoLogLength;

		//// Link the program
		glAttachShader(programId, vertexId);
		glAttachShader(programId, fragmentId);
		glLinkProgram(programId);

		// Check the program
		glGetProgramiv(programId, GL_LINK_STATUS, &result);
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			LOG("Error Loading program");
		}
		glDetachShader(programId, vertexId);
		glDetachShader(programId, fragmentId);
	}

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	return programId;
}

bool ModuleProgram::Compile(unsigned id, std::string* data) {

	GLint result = GL_FALSE;
	int InfoLogLength;
	const char* code = data->c_str();
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);

	// Check Vertex Shader
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		LOG("Error Loading Shader");
		return false;
	}
	return true;
}

std::string* ModuleProgram::LoadFile(const char* path) {
	std::string fileCode;
	std::ifstream file;
	// ensure ifstream objects can throw exceptions:
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(path);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		return new std::string(stream.str());
	} catch (const std::ifstream::failure& e) {
		LOG("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ %s", e.what());
	}
	return nullptr;
}
