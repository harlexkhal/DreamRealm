#include "SkyDome.h"


SkyDome::~SkyDome()
{
	//Flush all buffers
}

void SkyDome::Load()
{
	//Temporary LoadDefault SkyDome...
	std::vector<StaticVertexData> Vertices =
	{
		StaticVertexData(CrunchMath::Vec3(-1.0f, 1.0f, -1.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, 1.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, 1.0f, -1.0f),  CrunchMath::Vec3(0.0f, 0.0f, 0.0f),  CrunchMath::Vec3(0.0f, 1.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, 1.0f, 1.0f),   CrunchMath::Vec3(0.0f, 1.0f , 0.0f),  CrunchMath::Vec3(0.0f, 1.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(-1.0f, 1.0f, 1.0f),  CrunchMath::Vec3(1.0f, 1.0f, 0.0f),  CrunchMath::Vec3(0.0f, 1.0f, 0.0f)),

		StaticVertexData(CrunchMath::Vec3(-1.0f, -1.0f, -1.0f), CrunchMath::Vec3(0.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, -1.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, -1.0f, -1.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, -1.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, -1.0f, 1.0f), CrunchMath::Vec3(1.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, -1.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(-1.0f, -1.0f, 1.0f), CrunchMath::Vec3(0.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, -1.0f, 0.0f)),

		StaticVertexData(CrunchMath::Vec3(-1.0f, -1.0f, 1.0f), CrunchMath::Vec3(0.0f, 1.0, 0.0f),   CrunchMath::Vec3(-1.0f, 0.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(-1.0f, -1.0f, -1.0f), CrunchMath::Vec3(1.0f, 1.0f, 0.0f), CrunchMath::Vec3(-1.0f, 0.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(-1.0f, 1.0f, -1.0f), CrunchMath::Vec3(1.0f, 0.0, 0.0f),   CrunchMath::Vec3(-1.0f, 0.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(-1.0f, 1.0f, 1.0f), CrunchMath::Vec3(0.0f, 0.0f, 0.0f),   CrunchMath::Vec3(-1.0f, 0.0f, 0.0f)),

		StaticVertexData(CrunchMath::Vec3(1.0f, -1.0f, 1.0f), CrunchMath::Vec3(1.0f, 1.0f, 0.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, -1.0f, -1.0f), CrunchMath::Vec3(0.0f, 1.0, 0.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, 1.0f, -1.0f), CrunchMath::Vec3(0.0f, 0.0f, 0.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, 1.0f, 1.0f),  CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f)),

		StaticVertexData(CrunchMath::Vec3(-1.0f, -1.0f, -1.0f), CrunchMath::Vec3(0.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, -1.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, -1.0f, -1.0f), CrunchMath::Vec3(1.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, -1.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, 1.0f, -1.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, -1.0f)),
		StaticVertexData(CrunchMath::Vec3(-1.0f, 1.0f, -1.0f), CrunchMath::Vec3(0.0f, 0.0, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, -1.0f)),

		StaticVertexData(CrunchMath::Vec3(-1.0f, -1.0f, 1.0f), CrunchMath::Vec3(1.0f, 1.0, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, 1.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, -1.0f, 1.0f), CrunchMath::Vec3(0.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, 1.0f)),
		StaticVertexData(CrunchMath::Vec3(1.0f, 1.0f, 1.0f),  CrunchMath::Vec3(0.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, 1.0f)),
		StaticVertexData(CrunchMath::Vec3(-1.0f, 1.0f, 1.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, 1.0f))
	};

	std::vector<unsigned int> Indices =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(StaticVertexData) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(StaticVertexData), (void*)offsetof(StaticVertexData, m_Position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(StaticVertexData), (void*)offsetof(StaticVertexData, m_Texture));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(StaticVertexData), (void*)offsetof(StaticVertexData, m_Normal));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	m_Textures.Add("src/Resource/Defaults/Textures/skybox/top.bmp");
}

void SkyDome::Render()
{
		glBindVertexArray(VAO);
		m_Textures.Bind();

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
