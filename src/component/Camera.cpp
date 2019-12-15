#include "Camera.h"
#include "main/GameObject.h"
#include "component/Component.h"
#include <math.h>
#include "Geometry/Plane.h"
#include "imgui.h"
#include "GL/glew.h"

Camera::Camera(GameObject* owner): Component(owner, ComponentType::Camera)
{
	glGenFramebuffers(1, &fbo);
	aspect = 1.f;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3{ 0.0f,0.0f, -20.0f };
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 1.f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
	camPos = float3{ 0.0f,0.0f, 100.0f };
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
	glGenFramebuffers(1, &fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &fb_tex);
	glBindTexture(GL_TEXTURE_2D, fb_tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenRenderbuffers(1, &fb_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
Camera::~Camera()
{
	//glDeleteFramebuffers(1, &fbo);
	//glDeleteRenderbuffers(1, &rbo);
}

int Camera::isCollidingFrustum(const AABB& aabb) const
{
	float3 edges[8];
	int totalPointsIn = 0;
	aabb.GetCornerPoints(edges);
	Plane viewportPlanes[6];
	frustum.GetPlanes(viewportPlanes);

	for (int pl = 0; pl < 6; pl++)
	{
		int isInPlane = 0;
		for (int p = 0; p < 8; p++)
		{
			if (viewportPlanes[pl].IsOnPositiveSide(edges[p]))
			{
				isInPlane = 1;
				++totalPointsIn;
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

void Camera::SetFrustum()
{
	camPos = frustum.pos;

	Hnear = 2 * tan(frustum.verticalFov / 2) * frustum.nearPlaneDistance;
	Wnear = Hnear * aspect;

	Hfar = 2 * tan(frustum.verticalFov / 2) * frustum.farPlaneDistance;
	Wfar = Hfar * aspect;


	nCenter = frustum.pos + frustum.front * frustum.nearPlaneDistance;
	fCenter = frustum.pos + frustum.front * frustum.farPlaneDistance;

	ftl = fCenter + (frustum.up * (Hfar / 2)) - (frustum.WorldRight()* (Wfar / 2));
	ftr = fCenter + (frustum.up * (Hfar / 2)) + (frustum.WorldRight()* (Wfar / 2));

	fbl = fCenter - (frustum.up * (Hfar / 2)) - (frustum.WorldRight()* (Wfar / 2));
	fbr = fCenter - (frustum.up * (Hfar / 2)) + (frustum.WorldRight()* (Wfar / 2));

	ntl = nCenter + (frustum.up * (Hnear / 2)) - (frustum.WorldRight()* (Wnear / 2));
	ntr = nCenter + (frustum.up * (Hnear / 2)) + (frustum.WorldRight()* (Wnear / 2));

	nbl = nCenter - (frustum.up * (Hnear / 2)) - (frustum.WorldRight()* (Wnear / 2));
	nbr = nCenter - (frustum.up * (Hnear / 2)) + (frustum.WorldRight()* (Wnear / 2));
}

void Camera::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(256.0f, 0.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_FirstUseEver);

	ImGui::Begin("Viewport");
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

void Camera::GenerateFBOTexture(unsigned w, unsigned h)
{
	if (w != fb_width || h != fb_height)
	{
		if (fb_tex != 0)
		{
			glDeleteTextures(1, &fb_tex);
		}

		if (w != 0 && h != 0)
		{
			if (fbo == 0)
			{
				glGenFramebuffers(1, &fbo);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glGenTextures(1, &fb_tex);
			glBindTexture(GL_TEXTURE_2D, fb_tex);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glGenRenderbuffers(1, &fb_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		fb_width = w;
		fb_height = h;
	}
}
