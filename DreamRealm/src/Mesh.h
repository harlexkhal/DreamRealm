#pragma once
#include <vector>
#include <Glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "CrunchMath/CrunchMath.h"

#include "Texture.h"

struct Vertex
{
	CrunchMath::Vec3 m_pos;
	CrunchMath::Vec3 m_tex;
	CrunchMath::Vec3 m_normal;

	Vertex() {}

	Vertex(const CrunchMath::Vec3 pos, const CrunchMath::Vec3 tex, const CrunchMath::Vec3 normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadMesh(const std::string& Filename);
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

		void Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);

		unsigned int VBO;
		unsigned int EBO;
		unsigned int VAO;

		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<SubMesh> m_Entries;
	std::vector<Texture*> m_Textures;
};