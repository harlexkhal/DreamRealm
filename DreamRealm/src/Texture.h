#pragma once
#include <string>

class Texture
{
public:
	Texture();
	Texture(uint32_t numoftex);
	void Add(const char* Rsrc);
	void Bind();

    uint32_t TextureId[15];
	uint32_t Index = 0;
};