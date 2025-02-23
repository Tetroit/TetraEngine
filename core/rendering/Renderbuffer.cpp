#include "tetrapc.h"
#include "Renderbuffer.h"

using namespace TetraEngine;

uint Renderbuffer::current = 0;

Renderbuffer::Renderbuffer(uint width, uint height)
{
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::Resize(uint width, uint height)
{
	if (current != rbo) glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	if (current != rbo) glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::Bind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	current = rbo;
}
uint Renderbuffer::ID() const
{
	return rbo;
}
bool Renderbuffer::IsAttached() const
{
	return (rbo == current);
}
void Renderbuffer::Unbind()
{
	current = 0;
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}