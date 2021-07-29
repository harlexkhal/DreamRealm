#pragma once
#include <string>

class Texture
{
public:
	Texture();
	void Load(const char* Rsrc);
	void Bind();

    unsigned TextureId;
};