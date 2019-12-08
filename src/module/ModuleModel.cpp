#include "main/Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include  <io.h>
#include "main/GameObject.h"
#include "component/Mesh.h"
#include "component/Material.h"
bool ModuleModel::Init() {
	return true;
}

bool ModuleModel::CleanUp() {
	return true;
}

const void ModuleModel::LoadModel(std::string& path) {
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
	processNode(scene->mRootNode, scene, App->scene->root);
	App->camera->Focus();
	Assimp::DefaultLogger::kill();
}

void ModuleModel::processNode(const aiNode *node, const aiScene *scene, GameObject* parent) {
	GameObject* model = App->scene->CreateGameObject(node->mName.C_Str());
	model->parent = parent;
	//node->mTransformation.Decompose(scaling, rotation, position);
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, model);
		processMaterials(scene->mMaterials[mesh->mMaterialIndex], model);
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, model);
	}
	parent->children.push_back(model);
}


void ModuleModel::processMesh(const aiMesh* mesh, GameObject* parent) {
	Meshc* meshAux = App->renderer->CreateMesh();
	meshAux->totalPrimitives = mesh->mNumFaces;
	meshAux->totalVertex = mesh->mNumVertices;
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
		meshAux->vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			meshAux->indices.push_back(face.mIndices[j]);
	}
	meshAux->Setup();
	parent->components.push_back(meshAux);
}

void ModuleModel::processMaterials(const aiMaterial* mat, GameObject* parent) {
	Material* material = App->texture->CreateMaterial();
	// 1. diffuse maps
	loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse", material);
	// 2. specular maps
	loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular", material);
	// 3. normal maps
	loadMaterialTextures(mat, aiTextureType_HEIGHT, "texture_normal", material);
	// 4. height maps
	loadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_height", material);

	parent->components.push_back(material);
}

void ModuleModel::loadMaterialTextures(const aiMaterial *mat, aiTextureType type, const char* typeName,  Material* material) {
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
		material->textures.push_back(texture);
	}
}

int  ModuleModel::existsFile(const char* path) const{
	if ((_access(path, 0)) == -1 ) {
		App->imgui->AddLog("Couldn't find: %s", path);
		return 1;
	} 
	return 0;
}


void ModuleModel::UpdateTexture(Texture& texture) {
	/*for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].textures.clear();
		meshes[i].textures.push_back(texture);
	}*/
}
