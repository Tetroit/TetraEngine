#include "tetrapc.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "Texture2D.h"

using namespace TetraEngine;

uint Framebuffer::current = 0;

Framebuffer::Framebuffer(uint width, uint height, bool generateNewTexture) : width(width), height(height)
{
	glGenFramebuffers(1, &fbo);
	if (generateNewTexture)
	{
		BindTexture(new Texture2D(width, height));
		Unbind();
	}
	else
		texture = nullptr;
	BindRenderbuffer(new Renderbuffer(width, height));
}
Framebuffer::~Framebuffer() {
	delete renderbuffer;
	glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	current = fbo;
}

void Framebuffer::BindTexture(Texture2D* texture, uint attachment)
{
	this->texture = texture;
	if (!IsAttached())
		Bind();
	if (texture != nullptr)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->ID(), 0);
}

void Framebuffer::BindRenderbuffer(Renderbuffer* rb)
{
	this->renderbuffer = rb;
	if (!IsAttached())
		Bind();
	if (rb != nullptr)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb->ID());
	Unbind();
}

Texture2D* Framebuffer::GetTexture()
{
	return texture;
}

uint Framebuffer::GetWidth() const
{
	return width;
}
uint Framebuffer::GetHeight() const
{
	return height;
}


bool Framebuffer::IsAttached()
{
	return (current == fbo);
}

void Framebuffer::Resize(int w, int h)
{
	width = w;
	height = h;
	if (renderbuffer != nullptr)
		renderbuffer->Resize(w, h);
	if (texture != nullptr)
		texture->Resize(w, h);
}

bool Framebuffer::IsComplete()
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	current = 0;	
}
