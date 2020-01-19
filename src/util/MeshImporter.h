#ifndef MeshImporter_h
#define MeshImporter_h
#include "Importer.h"
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/DefaultLogger.hpp>
#define TEXTURE_PATH "Textures/"
#define DEFAULT_TEXTURE "checkers.jpg"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;
class GameObject;
class MeshImporter : public Importer {
public:
	MeshImporter();
	~MeshImporter();
	bool Import(const char*, const char*, std::string*);
	bool Load(const char*, GameObject*);
private:
	std::string directory;
	void processNode(const aiNode*, const aiScene*, const char*);
	void processMesh(const aiMesh*, const char*);
};

class AssimpLog : public Assimp::LogStream {
public:
	// Constructor
	AssimpLog()
	{
		// empty
	}

	// Destructor
	~AssimpLog()
	{
		// empty
	}
	// Write womethink using your own functionality
	void write(const char* message)
	{
		//App->imgui->AddLog("Assimp INFO %s", message);
	}
};
#endif
