#pragma once
#include <string>
#include <vector>

class Texture
{
public:
	Texture();
	Texture(uint32_t numoftex);
	void AddCubeMap(std::vector<std::string> links);
	void Add(const char* Rsrc);
	void Bind();

    uint32_t TextureId[15];
	uint32_t Index = 0;
};