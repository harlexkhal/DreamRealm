#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::LoadMesh(const std::string& Filename)
{
	Clear();
	
	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if(!pScene)
	{
		std::cerr << "Error parsing::No Mesh was loaded from file" << Filename << "::" << Importer.GetErrorString() << std::endl;
		std::cerr << "Attempting to load default mesh" << std::endl;
	}

	InitFromScene(pScene, Filename);
}

void Mesh::Render()
{
	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		glBindVertexArray(m_Entries[i].VAO);	
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
		{
			m_Textures[MaterialIndex]->Bind();
		}

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}
}

void Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	uint32_t numberofmeshes = 1;
	uint32_t numberofmaterials = 1;

	if (pScene)
	{
		numberofmeshes = pScene->mNumMeshes;
		numberofmaterials = pScene->mNumMaterials;
	}

	m_Entries.resize(numberofmeshes);
	m_Textures.resize(numberofmaterials);

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = nullptr;
		if (pScene)
			paiMesh = pScene->mMeshes[i];

		InitMesh(i, paiMesh);
	}

	InitMaterials(pScene, Filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	//Temporary LoadDefault mesh...
	std::vector<Vertex> Vertices =
	{
		Vertex(CrunchMath::Vec3(-1.0f, 1.0f, -1.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, 1.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(1.0f, 1.0f, -1.0f),  CrunchMath::Vec3(0.0f, 0.0f, 0.0f),  CrunchMath::Vec3(0.0f, 1.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(1.0f, 1.0f, 1.0f),   CrunchMath::Vec3(0.0f, 1.0f , 0.0f),  CrunchMath::Vec3(0.0f, 1.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(-1.0f, 1.0f, 1.0f),  CrunchMath::Vec3(1.0f, 1.0f, 0.0f),  CrunchMath::Vec3(0.0f, 1.0f, 0.0f)),
																			  
		Vertex(CrunchMath::Vec3(-1.0f, -1.0f, -1.0f), CrunchMath::Vec3(0.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, -1.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(1.0f, -1.0f, -1.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, -1.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(1.0f, -1.0f, 1.0f), CrunchMath::Vec3(1.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, -1.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(-1.0f, -1.0f, 1.0f), CrunchMath::Vec3(0.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, -1.0f, 0.0f)),
																			  
		Vertex(CrunchMath::Vec3(-1.0f, -1.0f, 1.0f), CrunchMath::Vec3(0.0f, 1.0, 0.0f),   CrunchMath::Vec3(-1.0f, 0.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(-1.0f, -1.0f, -1.0f), CrunchMath::Vec3(1.0f, 1.0f, 0.0f), CrunchMath::Vec3(-1.0f, 0.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(-1.0f, 1.0f, -1.0f), CrunchMath::Vec3(1.0f, 0.0, 0.0f),   CrunchMath::Vec3(-1.0f, 0.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(-1.0f, 1.0f, 1.0f), CrunchMath::Vec3(0.0f, 0.0f, 0.0f),   CrunchMath::Vec3(-1.0f, 0.0f, 0.0f)),
																			   
		Vertex(CrunchMath::Vec3(1.0f, -1.0f, 1.0f), CrunchMath::Vec3(1.0f, 1.0f, 0.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(1.0f, -1.0f, -1.0f), CrunchMath::Vec3(0.0f, 1.0, 0.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(1.0f, 1.0f, -1.0f), CrunchMath::Vec3(0.0f, 0.0f, 0.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f)),
		Vertex(CrunchMath::Vec3(1.0f, 1.0f, 1.0f),  CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f)),
																			   
		Vertex(CrunchMath::Vec3(-1.0f, -1.0f, -1.0f), CrunchMath::Vec3(0.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, -1.0f)),
		Vertex(CrunchMath::Vec3(1.0f, -1.0f, -1.0f), CrunchMath::Vec3(1.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, -1.0f)),
		Vertex(CrunchMath::Vec3(1.0f, 1.0f, -1.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, -1.0f)),
		Vertex(CrunchMath::Vec3(-1.0f, 1.0f, -1.0f), CrunchMath::Vec3(0.0f, 0.0, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, -1.0f)),
																			   							 
		Vertex(CrunchMath::Vec3(-1.0f, -1.0f, 1.0f), CrunchMath::Vec3(1.0f, 1.0, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, 1.0f)),
		Vertex(CrunchMath::Vec3(1.0f, -1.0f, 1.0f), CrunchMath::Vec3(0.0f, 1.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, 1.0f)),
		Vertex(CrunchMath::Vec3(1.0f, 1.0f, 1.0f),  CrunchMath::Vec3(0.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, 1.0f)),
		Vertex(CrunchMath::Vec3(-1.0f, 1.0f, 1.0f), CrunchMath::Vec3(1.0f, 0.0f, 0.0f), CrunchMath::Vec3(0.0f, 0.0f, 1.0f))
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

	if (paiMesh)
	{
		//If a loaded mesh was found clear all attempted effort to create a default mesh
		Vertices.clear();
		Indices.clear();

		m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
		{
			const aiVector3D* pPos = &(paiMesh->mVertices[i]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

			Vertex v(CrunchMath::Vec3(pPos->x, pPos->y, pPos->z),
				CrunchMath::Vec3(pTexCoord->x, pTexCoord->y, 0.0f),
				CrunchMath::Vec3(pNormal->x, pNormal->y, pNormal->z));

			Vertices.push_back(v);
		}

		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
		{
			const aiFace& Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}
	}

	m_Entries[Index].Init(Vertices, Indices);
}

void Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	//Loading default Texture...
	m_Textures[0] = new Texture(2);
	m_Textures[0]->Add("src/Resource/Defaults/Textures/wood.png");
	m_Textures[0]->Add("src/Resource/Defaults/Textures/metal.png");

	if (pScene)
	{
		//If there was a scene that means. its most likely it would have textures in it so we delete the default textures loaded
		delete m_Textures[0];

		std::string::size_type SlashIndex = Filename.find_last_of("/");
		std::string Dir;

		if (SlashIndex == std::string::npos)
		{
			Dir = ".";
		}

		else if (SlashIndex == 0)
		{
			Dir = "/";
		}

		else
		{
			Dir = Filename.substr(0, SlashIndex);
		}

		bool Ret = true;

		// Initialize the materials
		for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
			const aiMaterial* pMaterial = pScene->mMaterials[i];

			m_Textures[i] = nullptr;

			aiString Path;
			if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string File = Path.data;
					std::size_t Found = File.find_last_of("/\\");
					std::string FullPath = Dir + "/" + File.substr(Found + 1);
					m_Textures[i] = new Texture(1);
					m_Textures[i]->Add(FullPath.c_str());
				}
			}

			/*Load a white texture in case the Mesh does not include its own texture
			if (!m_Textures[i])
			{
				m_Textures[i] = new Texture();
				m_Textures[i]->Load("../Content/white.png");
			}*/
		}
	}
}

void Mesh::Clear()
{
}

void Mesh::SubMesh::Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_tex));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}