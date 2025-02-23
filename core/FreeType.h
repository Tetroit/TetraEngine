#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <map>

#include "rendering/Shader.h"

namespace TetraEngine {
	class FreeType
	{

	public:
		static FT_Library ft;
		static FT_Face defaultFace;
		static FT_Face currentFace;
		static unsigned int VAO, VBO;
		struct Character {
			unsigned int TextureID;  // ID handle of the glyph texture
			glm::ivec2   Size;       // Size of glyph
			glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
			unsigned int Advance;    // Offset to advance to next glyph
		};
		static std::map<GLchar, Character> characters;

		static bool Initialise() {

			if (FT_Init_FreeType(&ft))
			{
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
				return false;
			}
			if (!LoadFont())
				return false;
			currentFace = defaultFace;

			FT_Set_Pixel_Sizes(currentFace, 0, 48);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			LoadChars();

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			return true;
		}
		static void LoadChars() {
			for (unsigned char c = 0; c < 128; c++)
			{
				// Load character glyph 
				if (FT_Load_Char(currentFace, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				// generate texture
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					currentFace->glyph->bitmap.width,
					currentFace->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					currentFace->glyph->bitmap.buffer
				);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// now store character for later use
				Character character = {
					texture,
					glm::ivec2(currentFace->glyph->bitmap.width, currentFace->glyph->bitmap.rows),
					glm::ivec2(currentFace->glyph->bitmap_left, currentFace->glyph->bitmap_top),
					static_cast<unsigned int>(currentFace->glyph->advance.x)
				};
				characters.insert(std::pair<char, Character>(c, character));
			}
		}
		static bool LoadFont(const std::string& path = fontPath + "/arial.ttf") {

			if (FT_New_Face(ft, path.c_str(), 0, &defaultFace))
			{
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
				return false;
			}
			return true;
		}

		static void RenderText(std::string text, float x, float y, float scale, glm::vec3 color, Shader* shader = Shader::textShader)
		{
			// activate corresponding render state
			shader->Use();
			shader->SetVec3("color", color);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);

			// iterate through all characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = characters[*c];

				float xpos = x + ch.Bearing.x * scale;
				float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;
				// update VBO for each character
				float vertices[6][4] = {
					{ xpos,     ypos + h,  0.0f, 0.0f },
					{ xpos,     ypos,      0.0f, 1.0f },
					{ xpos + w, ypos,      1.0f, 1.0f },

					{ xpos,     ypos + h,  0.0f, 0.0f },
					{ xpos + w, ypos,      1.0f, 1.0f },
					{ xpos + w, ypos + h,  1.0f, 0.0f }
				};
				// render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};
}
