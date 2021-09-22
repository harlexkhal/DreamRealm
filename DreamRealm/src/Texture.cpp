#define STB_IMAGE_IMPLEMENTATION
#include "../../Dependencies/STB/stb_image.h"
#include <glad/glad.h>
#include <iostream>
#include <string>

#include "Texture.h"

Texture::Texture(uint32_t numoftex)
{
	memset(TextureId, -1, sizeof(TextureId));
	glGenTextures(numoftex, TextureId);
}

Texture::Texture()
{
	memset(TextureId, -1, sizeof(TextureId));
	glGenTextures(1, TextureId);
}

Texture::~Texture()
{
	glDeleteTextures(Index + 1, TextureId);
}

void Texture::AddCubeMap(std::vector<std::string> links)
{
	glActiveTexture(GL_TEXTURE0 + Index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId[Index]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
	for (unsigned int i = 0; i < links.size(); i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(links[i].c_str(), &width, &height, &nrChannels, 0);

		unsigned int Internal_Format;
		unsigned int Image_Format;

		if (nrChannels == 1)
		{
			Internal_Format = GL_RED;
			Image_Format = GL_RED;
		}

		else if (nrChannels == 3)
		{
			Internal_Format = GL_RGB;
			Image_Format = GL_RGB;
		}

		else if (nrChannels == 4)
		{
			Internal_Format = GL_RGBA;
			Image_Format = GL_RGBA;
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Internal_Format, width, height, 0, Image_Format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			Index++;
		}

		else
		{
			std::cout << "Cubemap texture failed to load at path: " << links[i] << std::endl;
			stbi_image_free(data);
		}
	}
}

void Texture::Add(const char* Rsrc)
{
	glActiveTexture(GL_TEXTURE0 + Index);
	glBindTexture(GL_TEXTURE_2D, TextureId[Index]);

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
		Index++;
	}

	else
	{
		std::cerr << "Texture Failed to Load...\n";
		TextureId[Index] = -1;
	}
	stbi_image_free(Data);
}

void Texture::Bind()
{
	//integer "Index" counter is one step ahead of the last TextureIdArray index
	for (int i = 0; i < Index; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, TextureId[i]);
	}
}