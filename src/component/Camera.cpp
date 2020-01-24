#include "Camera.h"
#include "main/GameObject.h"
#include "component/Component.h"
#include "MathGeoLib.h"
#include "Geometry/Plane.h"
#include "main/Application.h"
#include "module/ModuleCamera.h"
#include "module/ModuleImGui.h"
#include "module/ModuleProgram.h"
#include "module/ModuleRender.h"
#include "imgui.h"
#include "GL/glew.h"
#include "util/DebugDraw.h"

Camera::Camera(GameObject* theOwner) : Component(owner, ComponentType::Camera)
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0,5,0);
	frustum.front = float3::unitX;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 5.f;
	frustum.farPlaneDistance = 500.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f));
	
	GenerateMatrices();
	glGenFramebuffers(1, &fbo);
}

Camera::~Camera()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &fb_depth);
}


void Camera::DrawFrustumPlanes()
{
	float4x4 clipMatrix = proj * view;
	dd::frustum(clipMatrix.Inverted(), float3(0, 0, 1));
}

int Camera::isCollidingFrustum(const AABB& aabb) const
{
	float3 edges[8];
	int totalPointsIn = 6;
	aabb.GetCornerPoints(edges);
	Plane viewportPlanes[6];
	frustum.GetPlanes(viewportPlanes);

	for (int pl = 0; pl < 6; pl++)
	{
		int isInPlane = 1;
		for (int p = 0; p < 8; p++)
		{
			if (viewportPlanes[pl].IsOnPositiveSide(edges[p]))
			{
				isInPlane = 0;
				--totalPointsIn;
			}
		}
	}
	if (totalPointsIn == 6)
	{
		return IS_IN;
	}

	if (totalPointsIn == 0)
	{
		return IS_OUT;
	}
	return INTERSECT;
}

void Camera::Draw(const char* name)
{
	isHovered = false;
	ImGui::SetNextWindowPos(ImVec2(256.0f, 0.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_FirstUseEver);
	
	ImGui::Begin(name);
	if (ImGui::IsWindowHovered())
	{
		//App->imgui->AddLog("TRYIN TO UPDATE CAM");
		hoveredWindowPos = ImGui::GetWindowPos();
		ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
		hoveredWindowSize = ImGui::GetWindowSize();
		hoveredWindowPos.x += contentMin.x;
		hoveredWindowPos.y += contentMin.y;
		hoveredWindowSize.x = hoveredWindowSize.x - (2 * contentMin.x);
		hoveredWindowSize.y = hoveredWindowSize.y - (contentMin.y + contentMin.x);
		isHovered = true;
	}
	width = ImGui::GetWindowContentRegionWidth();
	height = ImGui::GetContentRegionAvail().y;

	ImGui::GetWindowDrawList()->AddImage(
		(void*)fb_tex,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + fb_width,
			ImGui::GetCursorScreenPos().y + fb_height),
		ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	ImGui::End();
	
}
void Camera::GenerateMatrices()
{
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}

void Camera::GenerateFBOTexture(unsigned w, unsigned h)
{
	if (w != fb_width || h != fb_height)
	{
		if (fb_tex != 0)
		{
			glDeleteTextures(1, &fb_tex);
		}
		glGenTextures(1, &fb_tex);
		if (rbo != 0)
		{
			glDeleteRenderbuffers(1, &rbo);
		}
		glGenRenderbuffers(1, &rbo);

		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, fb_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		fb_width = w;
		fb_height = h;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::CreateRay(const float2& normalizedPos, LineSegment &value) const
{
	value = frustum.UnProjectLineSegment(normalizedPos.x, normalizedPos.y);
}

void Camera::DrawView() {
	if (ImGui::TreeNode("Camera")) {
		ImGui::Text("UUID: %s", uuid.c_str());
		ImGui::SliderFloat("Far Plane Distance", &frustum.farPlaneDistance, frustum.nearPlaneDistance + 4.0F, 10000.0F);
		ImGui::TreePop();
	}
}

void Camera::OnLoad(rapidjson::Document::Object* object) {
	uuid = (object->FindMember("uuid"))->value.GetString();
	//fbo = (object->FindMember("fbo"))->value.GetInt();
	auto frustumJSON = (object->FindMember("frustum"))->value.GetObjectA();
	frustum.type = FrustumType((frustumJSON.FindMember("type"))->value.GetInt());
	frustum.nearPlaneDistance = (frustumJSON.FindMember("nearPlane"))->value.GetFloat();
	frustum.farPlaneDistance = (frustumJSON.FindMember("farPlane"))->value.GetFloat();
	frustum.verticalFov = (frustumJSON.FindMember("verticalFov"))->value.GetFloat();
	frustum.horizontalFov = (frustumJSON.FindMember("horizontalFov"))->value.GetFloat();
	Component::GetFloat3FromObjectJSON(&(frustumJSON.FindMember("pos"))->value.GetObjectA(), &frustum.pos);
	Component::GetFloat3FromObjectJSON(&(frustumJSON.FindMember("front"))->value.GetObjectA(), &frustum.front);
	Component::GetFloat3FromObjectJSON(&(frustumJSON.FindMember("up"))->value.GetObjectA(), &frustum.up);
	GenerateMatrices();
}

void Camera::OnSave(rapidjson::Document::Array* list, rapidjson::Document::AllocatorType* allocator) {
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("uuid", rapidjson::StringRef(uuid.c_str()), *allocator);
	object.AddMember("type", int(type), *allocator);
	std::string owneruuid;
	if (owner) {
		owneruuid = owner->uuid;
	}
	object.AddMember("owneruuid", rapidjson::StringRef(owneruuid.c_str()), *allocator);
	object.AddMember("fbo", fbo, *allocator);
	rapidjson::Value frustumJSON(rapidjson::kObjectType);
	frustumJSON.AddMember("type", int(frustum.type), *allocator);
	frustumJSON.AddMember("nearPlane", frustum.nearPlaneDistance, *allocator);
	frustumJSON.AddMember("farPlane", frustum.farPlaneDistance, *allocator);
	frustumJSON.AddMember("verticalFov", frustum.verticalFov, *allocator);
	frustumJSON.AddMember("horizontalFov", frustum.horizontalFov, *allocator);
	Component::AddFloat3ToObjectJSON(&frustumJSON.GetObjectA(), allocator, "pos", &frustum.pos);
	Component::AddFloat3ToObjectJSON(&frustumJSON.GetObjectA(), allocator, "up", &frustum.up);
	Component::AddFloat3ToObjectJSON(&frustumJSON.GetObjectA(), allocator, "front", &frustum.front);
	object.AddMember("frustum", frustumJSON, *allocator);
	list->PushBack(object, *allocator);
}

