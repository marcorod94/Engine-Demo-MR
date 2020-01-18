#include "MeshImporter.h"
#include "main/Application.h"
#include "module/ModuleImGui.h"
#include "module/ModuleFileSystem.h"
#include "module/ModuleTexture.h"
#include "component/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MeshImporter::MeshImporter() {

}

MeshImporter::~MeshImporter() {

}
bool MeshImporter::Import(const char* file, const char* path, std::string* output) {
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	Assimp::Importer importer;
	Assimp::DefaultLogger::get()->attachStream(new AssimpLog, severity);
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		App->imgui->AddLog("ERROR::ASSIMP:: %s", importer.GetErrorString());
		return false;
	}
	processNode(scene->mRootNode, scene);

	Assimp::DefaultLogger::kill();
	return true;
}

bool MeshImporter::Load(const char* file, Mesh* mesh) {
	char * data;
	App->filesys->Load(file, "", &data);
	char* cursor = data;

	unsigned int ranges[3];
	unsigned int bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	std::vector<unsigned int> indices;
	std::vector<Vertex> vertexs;

	indices.resize(ranges[0]);

	cursor += bytes; // Get indices
	bytes = sizeof(uint32_t) * ranges[0];
	memcpy(&indices.front(), cursor, bytes);

	vertexs.resize(ranges[1]);

	cursor += bytes; // Get vertices
	bytes = sizeof(Vertex) * ranges[1];
	memcpy(&vertexs.front(), cursor, bytes);

	return true;
}

void MeshImporter::processNode(const aiNode *node, const aiScene *scene) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh);
		processMaterials(scene->mMaterials[mesh->mMaterialIndex]);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}


void MeshImporter::processMesh(const aiMesh* mesh) {
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

	App->filesys->Save("test.bin", data, size);
}

void MeshImporter::processMaterials(const aiMaterial* mat) {
	for (int i = 0; i <= aiTextureType_UNKNOWN; i++) {
		loadMaterialTextures(mat, aiTextureType(i));
	}
}

void MeshImporter::loadMaterialTextures(const aiMaterial* mat, aiTextureType type) {
	for (unsigned i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		aiTextureMapping mapping;
		unsigned uvindex = 0;
		mat->GetTexture(type, i, &str, &mapping, &uvindex);
		std::string path = str.C_Str();
		std::string textureName = path.substr(path.find_last_of('\\') + 1, path.size());
		App->imgui->AddLog("Trying to load texture: %s", path.c_str());
		if (App->filesys->Exists(path.c_str())) {
			path = directory;
			path = path.append(textureName);
			App->imgui->AddLog("Trying to load texture: %s", path.c_str());
			if (App->filesys->Exists(path.c_str())) {
				path = TEXTURE_PATH;
				path = path.append(textureName);
				App->imgui->AddLog("Trying to load texture: %s", path.c_str());
				if (App->filesys->Exists(path.c_str())) {
					path = TEXTURE_PATH;
					path = path.append(DEFAULT_TEXTURE);
					App->imgui->AddLog("Trying to load texture: %s", path.c_str());
				}
			}
		}
		App->texture->importer->Import(path.c_str(), "", nullptr);
	}
}

