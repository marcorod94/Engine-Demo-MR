#include "MeshImporter.h"
#include "main/Application.h"
#include "module/ModuleImGui.h"
#include "module/ModuleFileSystem.h"
#include "module/ModuleTexture.h"
#include "module/ModuleScene.h"
#include "main/GameObject.h"
#include "component/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MeshImporter::MeshImporter() {

}

MeshImporter::~MeshImporter() {

}
bool MeshImporter::Import(const char* file, const char* path, std::string* output) {
	std::string name = file;
	name = name.substr(name.find_last_of('\\') + 1, name.size());
	name = name.substr(name.find_last_of('/') + 1, name.size());
	name = name.substr(0, name.find_last_of('.'));
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	Assimp::Importer importer;
	Assimp::DefaultLogger::get()->attachStream(new AssimpLog, severity);
	const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		App->imgui->AddLog("ERROR::ASSIMP:: %s", importer.GetErrorString());
		return false;
	}
	std::string newFile = path;
	newFile.append("/").append(name.c_str()).append(".mbin");
	processNode(scene->mRootNode, scene, newFile.c_str());
	Assimp::DefaultLogger::kill();
	return true;
}

bool MeshImporter::Load(const char* file, GameObject* parent) {
	std::string name = file;
	name = name.substr(name.find_last_of('\\') + 1, name.size());
	name = name.substr(name.find_last_of('/') + 1, name.size());
	name = name.substr(0, name.find_last_of('.'));
	std::string fileName = name;
	char * data;
	int size;
	App->filesys->Load(file, &data, &size);
	GameObject* child;
	char* cursor = data;
	int cursorAux = 0;
	int i = 1;
	while (cursorAux < size) {
		fileName = name;
		fileName.append(std::to_string(i));
		child = App->scene->CreateGameObject(fileName.c_str());
		Mesh* mesh = (Mesh*)child->CreateComponent(ComponentType::Mesh);
		//cursor += cursorAux;
		unsigned int ranges[2];
		unsigned int bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		mesh->indices.resize(ranges[0]);

		cursor += bytes;
		cursorAux += bytes;
		bytes = sizeof(unsigned int) * ranges[0];
		memcpy(&mesh->indices.front(), cursor, bytes);

		mesh->vertices.resize(ranges[1]);

		cursor += bytes;
		cursorAux += bytes;
		bytes = sizeof(Vertex) * ranges[1];
		memcpy(&mesh->vertices.front(), cursor, bytes);
		++i;
		mesh->Setup();
		child->parent = parent;
		parent->children.push_back(child);
		cursor += bytes;
		cursorAux += bytes;
	}
	

	return true;
}

void MeshImporter::processNode(const aiNode *node, const aiScene *scene, const char* file) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, file);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, file);
	}
}


void MeshImporter::processMesh(const aiMesh* mesh, const char* file) {
	std::vector<Vertex> vertexs;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		float3 vector;
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) {
			float2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = float2(0.0f, 0.0f);
		}
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertexs.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	unsigned int ranges[2] = { indices.size(), vertexs.size() };
	unsigned int size = sizeof(ranges) + sizeof(unsigned int) * indices.size() + sizeof(Vertex) * vertexs.size();

	char* data = new char[size];
	char* cursor = data;
	unsigned int bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	cursor += bytes;
	bytes = sizeof(unsigned int) * indices.size();
	memcpy(cursor, &indices.front(), bytes);

	cursor += bytes;
	bytes = sizeof(Vertex) *  vertexs.size();
	memcpy(cursor, &vertexs.front(), bytes);

	App->filesys->Save(file, data, size, true);
}

