#include "main/Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include  <io.h>
#include "main/GameObject.h"
#include "component/Mesh.h"
#include "component/Material.h"
#include "component/Transform.h"
#include "ModuleProgram.h"
#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
bool ModuleModel::Init() {
	light.pos = math::float3(-2.0f, 0.0f, 6.0f);
	ambient = 0.3F;
	return true;
}

bool ModuleModel::CleanUp() {
	return true;
}

const void ModuleModel::LoadModel(const char* file) {
	std::string name = file;
	name = name.substr(name.find_last_of('/') + 1, name.size());
	name = name.substr(0, name.find_last_of('.'));
	GameObject* object = App->scene->CreateGameObject(name.c_str());
	importer->Load(file, object);
	object->parent = App->scene->root;
	App->scene->root->children.push_back(object);
}

void ModuleModel::LoadShapes(GameObject* parent, const char* name, const float3* pos, const Quat* rot, MeshShape* shape, ProgramType programType, const float4* color) {
	par_shapes_mesh* mesh = nullptr;
	switch (shape->type) {
		case ShapeType::Sphere:
			mesh = LoadSphere(shape->size, shape->slices, shape->stacks);
			break;
		case ShapeType::Cylinder:
			mesh = LoadCylinder(shape->size, shape->radius, shape->slices, shape->stacks);
			break;
		case ShapeType::Cube:
			mesh = LoadCube(shape->size);
			break;
		case ShapeType::Torus:
			mesh = LoadTorus(shape->radius, shape->size, shape->slices, shape->stacks);
			break;
	}
	if (mesh) {
		GameObject* model = App->scene->CreateGameObject(name, pos, rot);
		GenerateMesh(model, mesh);
		par_shapes_free_mesh(mesh);

		Material* material = (Material*) model->CreateComponent(ComponentType::Material);
		material->program = int(programType);
		material->diffuseColor = *color;
		material->shininess = 64.0f;
		material->kSpecular = 0.6f;
		material->kDiffuse = 0.5f;
		material->kAmbient = 1.0f;
		model->parent = parent;
		parent->children.push_back(model);
	}

}

par_shapes_mesh* ModuleModel::LoadSphere(float size, unsigned slices, unsigned stacks) {
	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));
	par_shapes_scale(mesh, size, size, size);
	return mesh;
}

par_shapes_mesh* ModuleModel::LoadCylinder(float height, float radius, unsigned slices, unsigned stacks) {
	par_shapes_mesh* mesh = par_shapes_create_cylinder(int(slices), int(stacks));
	par_shapes_rotate(mesh, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(mesh, 0.0f, -0.5f, 0.0f);

	par_shapes_mesh* top = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, 0.0f, height*0.5f, 0.0f);

	par_shapes_mesh* bottom = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, 0.0f, height*-0.5f, 0.0f);

	par_shapes_scale(mesh, radius, height, radius);
	par_shapes_merge_and_free(mesh, top);
	par_shapes_merge_and_free(mesh, bottom);

	return mesh;
}

par_shapes_mesh* ModuleModel::LoadTorus(float inner_r, float outer_r, unsigned slices, unsigned stacks)
{
	par_shapes_mesh* mesh = par_shapes_create_torus(int(slices), int(stacks), inner_r);
	par_shapes_scale(mesh, outer_r, outer_r, outer_r);
	return mesh;
}

par_shapes_mesh* ModuleModel::LoadCube(float size)
{
	par_shapes_mesh* mesh = par_shapes_create_plane(1, 1);
	par_shapes_mesh* top = par_shapes_create_plane(1, 1);
	par_shapes_mesh* bottom = par_shapes_create_plane(1, 1);
	par_shapes_mesh* back = par_shapes_create_plane(1, 1);
	par_shapes_mesh* left = par_shapes_create_plane(1, 1);
	par_shapes_mesh* right = par_shapes_create_plane(1, 1);

	par_shapes_translate(mesh, -0.5f, -0.5f, 0.5f);

	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, -0.5f, 0.5f, 0.5f);

	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, -0.5f, -0.5f, -0.5f);

	par_shapes_rotate(back, float(PAR_PI), (float*)&math::float3::unitX);
	par_shapes_translate(back, -0.5f, 0.5f, -0.5f);

	par_shapes_rotate(left, float(-PAR_PI * 0.5), (float*)&math::float3::unitY);
	par_shapes_translate(left, -0.5f, -0.5f, -0.5f);

	par_shapes_rotate(right, float(PAR_PI*0.5), (float*)&math::float3::unitY);
	par_shapes_translate(right, 0.5f, -0.5f, 0.5f);

	par_shapes_merge_and_free(mesh, top);
	par_shapes_merge_and_free(mesh, bottom);
	par_shapes_merge_and_free(mesh, back);
	par_shapes_merge_and_free(mesh, left);
	par_shapes_merge_and_free(mesh, right);

	par_shapes_scale(mesh, size, size, size);

	return mesh;
}


void ModuleModel::GenerateMesh(GameObject* owner, par_shapes_mesh_s* shape)
{
	Mesh* meshDest = (Mesh*)owner->CreateComponent(ComponentType::Mesh);
	Transform* transform = (Transform*) (owner->FindComponent(ComponentType::Transform));

	// Positions
	for (unsigned i = 0; i< unsigned(shape->npoints); ++i) {
		Vertex vertex;
		float3 vector;
		vector = float3(shape->points[i * 3], shape->points[i * 3 + 1], shape->points[i * 3 + 2]);
		meshDest->box.Enclose(vector);
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
	meshDest->Setup();
	/*bsphere.center = (max_v + min_v)*0.5f;
	bsphere.radius = (max_v - min_v).Length()*0.5f;*/
}


void ModuleModel::UpdateTexture(Texture& texture) {
	/*for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].textures.clear();
		meshes[i].textures.push_back(texture);
	}*/
}
