#pragma once
#include <vector>
#include <Glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.h"
#include "VertexData.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadMesh(const std::string& Filename = "");
	void LoadTexture(const char* File);
	void LoadTextureOverrite(const char* File);
	void Render();

private:
	void InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void InitMaterials(const aiScene* pScene, const std::string& Filename);

	void Clear();

	struct SubMesh
	{
		SubMesh() {};
		~SubMesh() {};

		void Init(const std::vector<StaticVertexData>& Vertices, const std::vector<unsigned int>& Indices);

		unsigned int VBO;
		unsigned int EBO;
		unsigned int VAO;

		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<SubMesh> m_Entries;
	std::vector<Texture*> m_Textures{ 0 };
};