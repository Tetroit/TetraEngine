#include "tetrapc.h"
#include "Viewport.h"
#include "Framebuffer.h"
#include "Texture2D.h"
#include "ViewportCamera.h"

using namespace TetraEngine;

Viewport::Viewport(uint width, uint height, ViewProvider* cam) : cam(cam), width(width), height(height)
{
	framebuffer = new Framebuffer(width, height, true);
}

void Viewport::Unbind(uint width, uint height)
{
	Framebuffer::Unbind();
	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH_TEST);
}

void Viewport::Bind()
{
	if (!framebuffer->IsComplete())
		std::cout << "FRAMEBUFFER NOT COMPLETE\n";
	framebuffer->Bind();
	glViewport(0, 0, GetWidth(), GetHeight());
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

uint Viewport::GetWidth()
{
	return framebuffer->GetWidth();
}
uint Viewport::GetHeight()
{
	return framebuffer->GetHeight();
}

void Viewport::SetSize(uint width, uint height)
{
    //framebuffer->Unbind();
	if (height > 0 && cam != nullptr) {
		float ratio = (float)width / (float)height;
		cam->SetRatio(ratio);
	}
	framebuffer->Resize(width, height);
	glViewport(0, 0, width, height);
    //framebuffer->Bind();
}

void Viewport::SetCamera(ViewProvider *pCam) {
	cam = pCam;
}

Texture2D* Viewport::GetTexture()
{
	return framebuffer->GetTexture();
}
