#include "Mesh.h"
#include "GL/glew.h"
#include "imgui.h"

void Mesh::Setup() {
	CalculateBoundigBox();
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void Mesh::DrawView() {
	if (ImGui::TreeNode("Geometry ")) {
		ImGui::Text("UUID: %s", uuid.c_str());
		ImGui::Text("Vertex Total: %d", vertices.size());
		ImGui::Text("Primitive Total: %d", indices.size()/3);
		ImGui::TreePop();
	}
}

void Mesh::OnLoad(rapidjson::Document::Object* object) {
	uuid = (object->FindMember("uuid"))->value.GetString();
	src = (object->FindMember("src"))->value.GetString();
	auto indicesJSON = (object->FindMember("indices"))->value.GetArray();
	for (auto& indice : indicesJSON) {
		indices.push_back(indice.GetInt());
	}
	auto vertexsJSON = (object->FindMember("vertexs"))->value.GetArray();
	for (auto& item : vertexsJSON) {
		auto vertexJSON = item.GetObjectA();
		Vertex vertex;
		Component::GetFloat3FromObjectJSON(&(vertexJSON.FindMember("position"))->value.GetObjectA(), &vertex.Position);
		Component::GetFloat3FromObjectJSON(&(vertexJSON.FindMember("normal"))->value.GetObjectA(), &vertex.Normal);
		Component::GetFloat2FromObjectJSON(&(vertexJSON.FindMember("texcoords"))->value.GetObjectA(), &vertex.TexCoords);
		Component::GetFloat3FromObjectJSON(&(vertexJSON.FindMember("tagent"))->value.GetObjectA(), &vertex.Tangent);
		Component::GetFloat3FromObjectJSON(&(vertexJSON.FindMember("bitagent"))->value.GetObjectA(), &vertex.Bitangent);
		vertices.push_back(vertex);
	}
	Setup();
}

void Mesh::OnSave(rapidjson::Document::Array* list, rapidjson::Document::AllocatorType* allocator) {
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("uuid", rapidjson::StringRef(uuid.c_str()), *allocator);
	object.AddMember("src", rapidjson::StringRef(src.c_str()), *allocator);
	object.AddMember("type", int(type), *allocator);
	std::string owneruuid;
	if (owner) {
		owneruuid = owner->uuid;
	}
	object.AddMember("owneruuid", rapidjson::StringRef(owneruuid.c_str()), *allocator);
	rapidjson::Value indicesJSON(rapidjson::kArrayType);
	for (unsigned index : indices) {
		indicesJSON.PushBack(index, *allocator);
	}
	object.AddMember("indices", indicesJSON, *allocator);
	rapidjson::Value vertexsJSON(rapidjson::kArrayType);
	for (auto vertex : vertices) {
		rapidjson::Value object(rapidjson::kObjectType);
		Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "position", &vertex.Position);
		Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "normal", &vertex.Normal);
		Component::AddFloat2ToObjectJSON(&object.GetObjectA(), allocator, "texcoords", &vertex.TexCoords);
		Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "tagent", &vertex.Tangent);
		Component::AddFloat3ToObjectJSON(&object.GetObjectA(), allocator, "bitagent", &vertex.Bitangent);
		vertexsJSON.PushBack(object, *allocator);
	}
	object.AddMember("vertexs", vertexsJSON, *allocator);
	list->PushBack(object, *allocator);
}

void Mesh::TransformAABB(float4x4* transform) {
	if (originalBox.IsDegenerate()) {
		originalBox = box;
	}
	box = originalBox;
	box.TransformAsAABB(*transform);
}

void Mesh::CalculateBoundigBox() {
	for (auto item : vertices) {
		originalBox.Enclose(item.Position);
	}
}
