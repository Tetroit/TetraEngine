#include "tetrapc.h"
#include "Cubemap.h"

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

using namespace TetraEngine;

Cubemap::Cubemap() {
	glGenTextures(1, &texture);
}
void Cubemap::Load(const std::string& name, const std::string& ext)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	std::string folder = name;
	std::string faces[]{"px","nx","py","ny","pz","nz"};
	std::string extension(ext);
	for (int i = 0; i < 6; i++)
	{
		std::string fullPath = folder + "/" + faces[i] + "." + extension;
		data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);
		if (data)
		{
			int channelMode = GL_RGB;
			if (channels == 4) channelMode = GL_RGBA;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, channelMode, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		else
		{
			std::cout << "Failed to load cubemap " + std::to_string(i) << std::endl;
		}
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
void Cubemap::Bind(int id)
{
	if (this != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	}
}
void Cubemap::Unbind(int id)
{
	if (this != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);
	}
}
