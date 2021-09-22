#pragma once
#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include "CrunchMath.h"
#include "Texture.h"
#include "VertexData.h"

class SkyDome
{
public:
	~SkyDome();

	void Load();
	void Render();

	unsigned int VBO;
	unsigned int EBO;
	unsigned int VAO;

	Texture m_Textures;
};