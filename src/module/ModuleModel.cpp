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
#include "component/Transform.h"
#include "ModuleProgram.h"
#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
bool ModuleModel::Init() {
	return true;
}

bool ModuleModel::CleanUp() {
	return true;
}

const void ModuleModel::LoadModel(std::string& path) {
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
	((Transform*)model->FindComponent(ComponentType::Transform))->SetTransform(node->mTransformation);
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


void ModuleModel::processMesh(const aiMesh* mesh, GameObject* owner) {
	Mesh* meshAux = App->renderer->CreateMesh();
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
		meshAux->box.maxPoint.x = max(meshAux->box.maxPoint.x, vector.x);
		meshAux->box.minPoint.x = min(meshAux->box.minPoint.x, vector.x);
		meshAux->box.maxPoint.y = max(meshAux->box.maxPoint.y, vector.y);
		meshAux->box.minPoint.y = min(meshAux->box.minPoint.y, vector.y);
		meshAux->box.maxPoint.z = max(meshAux->box.maxPoint.z, vector.z);
		meshAux->box.minPoint.z = min(meshAux->box.minPoint.z, vector.z);
		
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
	meshAux->owner = owner;
	owner->components.push_back(meshAux);
}

void ModuleModel::processMaterials(const aiMaterial* mat, GameObject* owner) {
	Material* material = App->texture->CreateMaterial();
	// 1. diffuse maps
	loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse", material);
	// 2. specular maps
	loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular", material);
	// 3. normal maps
	loadMaterialTextures(mat, aiTextureType_HEIGHT, "texture_normal", material);
	// 4. height maps
	loadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_height", material);
	material->owner = owner;
	material->program = int(ProgramType::Default);
	owner->components.push_back(material);
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

bool ModuleModel::LoadSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size,
	unsigned slices, unsigned stacks, const math::float4& color, GameObject* parent)
{
	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));

	if (mesh) {
		GameObject* model = App->scene->CreateGameObject(name);
		par_shapes_scale(mesh, size, size, size);

		GenerateMesh(name, pos, rot, mesh, model);
		par_shapes_free_mesh(mesh);

		Material* material = App->texture->CreateMaterial();
		material->program = int(ProgramType::Default);
		material->color = color;
		material->kSpecular = 0.9f;
		material->shininess = 64.0f;
		material->kSpecular = 0.6f;
		material->kDiffuse = 0.5f;
		material->kAmbient = 1.0f;
		material->owner = model;
		model->components.push_back(material);
		model->parent = parent;
		parent->children.push_back(model);
		return true;
	}

	return false;
}

void ModuleModel::GenerateMesh(const char* name, const math::float3& pos, const math::Quat& rot, par_shapes_mesh_s* shape, GameObject* owner)
{
	Mesh* meshDest = App->renderer->CreateMesh();
	Transform* transform = (Transform*) (owner->FindComponent(ComponentType::Transform));
	/*meshDest->name = name;
	meshDest->transform = math::float4x4(rot, pos);*/

	// Positions
	for (unsigned i = 0; i< unsigned(shape->npoints); ++i) {
		Vertex vertex;
		float3 vector;
		vector = float3(shape->points[i * 3], shape->points[i * 3 + 1], shape->points[i * 3 + 2]);
		meshDest->box.maxPoint.x = max(meshDest->box.maxPoint.x, vector.x);
		meshDest->box.minPoint.x = min(meshDest->box.minPoint.x, vector.x);
		meshDest->box.maxPoint.y = max(meshDest->box.maxPoint.y, vector.y);
		meshDest->box.minPoint.y = min(meshDest->box.minPoint.y, vector.y);
		meshDest->box.maxPoint.z = max(meshDest->box.maxPoint.z, vector.z);
		meshDest->box.minPoint.z = min(meshDest->box.minPoint.z, vector.z);
		vertex.Position = vector;
		if (shape->normals) {
			vector = float3(shape->normals[i * 3], shape->normals[i * 3 + 1], shape->normals[i * 3 + 2]);
			vertex.Normal = vector;
		}
		meshDest->vertices.push_back(vertex);
	}
	
	for (unsigned i = 0; i< unsigned(shape->ntriangles * 3); ++i) {
		meshDest->indices.push_back(shape->triangles[i]);
	}


	meshDest->totalVertex = shape->npoints;
	meshDest->totalPrimitives = shape->ntriangles;
	meshDest->Setup();
	meshDest->owner = owner;
	owner->components.push_back(meshDest);
	/*bsphere.center = (max_v + min_v)*0.5f;
	bsphere.radius = (max_v - min_v).Length()*0.5f;*/
}


void ModuleModel::UpdateTexture(Texture& texture) {
	/*for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].textures.clear();
		meshes[i].textures.push_back(texture);
	}*/
}
