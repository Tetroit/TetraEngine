#include "tetrapc.h"
#include "Texture2D.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "stb_image.h"


using namespace TetraEngine;

uint Texture2D::attached = 0;


Texture2D::Texture2D() {
	glGenTextures(1, &texture);
}

Texture2D::Texture2D(int width, int height, int channels) : width(width), height(height), channels(channels)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int channelMode = GL_RGB;
	if (channels == 1) channelMode = GL_RED;
	if (channels == 2) channelMode = GL_RG;
	if (channels == 3) channelMode = GL_RGB;
	if (channels == 4) channelMode = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channelMode, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &texture);
}


void Texture2D::Load(const std::string& name, bool flip_vertically) {

	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(flip_vertically);
	data = stbi_load(name.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		int channelMode = 0;
		if (channels == 1) channelMode = GL_RED;
		if (channels == 2) channelMode = GL_RG;
		if (channels == 3) channelMode = GL_RGB;
		if (channels == 4) channelMode = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channelMode, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture " << name << std::endl;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	std::cout << "texture has been loaded at path " << name << "\n";
	path = name;
}
void Texture2D::Bind(int id)
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture);
}
bool Texture2D::IsAttached() const
{
	return (attached == texture);
}
void Texture2D::Unbind(int id)
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Texture2D::Resize(uint width, uint height)
{
	if (attached != texture) glBindTexture(GL_TEXTURE_2D, texture);

	this->width = width;
	this->height = height;
	int format;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);

	if (attached != texture) glBindTexture(GL_TEXTURE_2D, 0);
}

std::string Texture2D::GetPath() const {
	return path;
}

uint Texture2D::ID() const
{
	return texture;
}
