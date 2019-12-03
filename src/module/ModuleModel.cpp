#include "Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include  <io.h>

bool ModuleModel::Init() {
	return true;
}

bool ModuleModel::CleanUp() {
	return true;
}

void ModuleModel::LoadModel(std::string& path) {
	meshes.clear();
	totalPrimitives = 0;
	totalVertex = 0;
	totalMaterials = 0;
	box.SetNegativeInfinity();
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	Assimp::Importer importer;
	Assimp::DefaultLogger::get()->attachStream(new AssimpLog, severity);
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		App->imgui->AddLog("ERROR::ASSIMP:: %s", importer.GetErrorString());
		return;
	}
	directory = path.substr(0, path.find_last_of('\\') + 1);
	processNode(scene->mRootNode, scene);
	rotation.x = RadToDeg(rotation.x);
	rotation.y = RadToDeg(rotation.y);
	rotation.z = RadToDeg(rotation.z);
	App->camera->Focus();
	Assimp::DefaultLogger::kill();

}

void ModuleModel::processNode(aiNode *node, const aiScene *scene) {
	node->mTransformation.Decompose(scaling, rotation, position);
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh ModuleModel::processMesh(aiMesh *mesh, const aiScene *scene) {
	totalPrimitives += mesh->mNumFaces;
	totalVertex += mesh->mNumVertices;
	totalMaterials = scene->mNumMaterials;
	Mesh meshAux;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		float3 vector;
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		if (vector.x > box.maxPoint.x) {
			box.maxPoint.x = vector.x;
		}
		if (vector.x < box.minPoint.x) {
			box.minPoint.x = vector.x;
		}
		if (vector.y > box.maxPoint.y) {
			box.maxPoint.y = vector.y;
		}
		if (vector.y < box.minPoint.y) {
			box.minPoint.y = vector.y;
		}
		if (vector.z > box.maxPoint.z) {
			box.maxPoint.z = vector.z;
		}
		if (vector.z < box.minPoint.z) {
			box.minPoint.z = vector.z;
		}
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
		} else {
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
		meshAux.vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			meshAux.indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	meshAux.textures.insert(meshAux.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	meshAux.textures.insert(meshAux.textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	meshAux.textures.insert(meshAux.textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	meshAux.textures.insert(meshAux.textures.end(), heightMaps.begin(), heightMaps.end());

	return meshAux;
}

std::vector<Texture> ModuleModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const char* typeName) {
	std::vector<Texture> textures;
	App->imgui->AddLog("\nLoading textures of type : %s", typeName);
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = str.C_Str();
		App->imgui->AddLog("Trying to load texture: %s", path.c_str());
		if (existsFile(path.c_str()) == 1) {
			path = directory;
			path = path.append(str.C_Str());
			App->imgui->AddLog("Trying to load texture: %s", path.c_str());
			if (existsFile(path.c_str()) == 1) {
				path = TEXTURE_PATH;
				path = path.append(str.C_Str());
				App->imgui->AddLog("Trying to load texture: %s", path.c_str());
				if (existsFile(path.c_str()) == 1) {
					path = TEXTURE_PATH;
					path = path.append(DEFAULT_TEXTURE);
					App->imgui->AddLog("Trying to load texture: %s", path.c_str());
				}
			}
		}
		Texture texture = App->texture->LoadTexture(path);
		texture.type = typeName;
		textures.push_back(texture);
	}
	return textures;
}


int  ModuleModel::existsFile(const char* path) const{
	if ((_access(path, 0)) == -1 ) {
		App->imgui->AddLog("Couldn't find: %s", path);
		return 1;
	} 
	return 0;
}


void ModuleModel::UpdateTexture(Texture& texture) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].textures.clear();
		meshes[i].textures.push_back(texture);
	}
}
