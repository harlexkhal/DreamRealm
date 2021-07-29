#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <string>

#include "Texture.h"

Texture::Texture()
{
}

void Texture::Load(const char* Rsrc)
{
	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_2D, TextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int h, w, c;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* Data = stbi_load(Rsrc, &w, &h, &c, 0);

	unsigned int Internal_Format;
	unsigned int Image_Format;

	if (c == 1)
	{
		Internal_Format = GL_RED;
		Image_Format = GL_RED;
	}

	else if (c == 3)
	{
		Internal_Format = GL_RGB;
		Image_Format = GL_RGB;
	}

	else if (c == 4)
	{
		Internal_Format = GL_RGBA;
		Image_Format = GL_RGBA;
	}

	if (Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, w, h, 0, Image_Format, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture Successfully loaded" << std::endl;
	}

	else
	{
		std::cerr << "Texture Failed to Load...\n";
	}
	stbi_image_free(Data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, TextureId);
}