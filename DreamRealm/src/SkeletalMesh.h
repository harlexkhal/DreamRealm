#pragma once
#include <vector>
#include <Glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "CrunchMath/CrunchMath.h"
#include <map>
#include "Texture.h"

struct VertexData
{
	CrunchMath::Vec3 m_Position;
	CrunchMath::Vec3 m_Texture;
	CrunchMath::Vec3 m_Normal;

	unsigned int  IDs[4];
	float Weights[4];

	VertexData(const CrunchMath::Vec3 Pos, const CrunchMath::Vec3 Tex, const CrunchMath::Vec3 Normal)
	{
		m_Position = Pos;
		m_Texture = Tex;
		m_Normal = Normal;

		Reset();
	}

	void Reset()
	{
		memset(IDs, 0, sizeof(IDs));
		memset(Weights, 0, sizeof(Weights));
	}

	void AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < (sizeof(IDs) / sizeof(IDs[0])); i++)
		{
			if (Weights[i] == 0.0f)
			{
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}
		//should never get here more bones than we have space for...
		//assert(0);
	}
};

class SkinnedMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

	void LoadMesh(const std::string& Filename);
	void Render();

	void BoneTransform(float Ts, std::vector<CrunchMath::Mat4x4>& Transforms);
private:
	void InitFromScene(const aiScene* pScene, const std::string& Filename);

	void InitMesh(unsigned int Index, const aiMesh* paiMesh);

	void InitMaterials(const aiScene* pScene, const std::string& Filename);
	void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<VertexData>& Vertices);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

	void CalculateInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalculateInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalculateInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const CrunchMath::Mat4x4& ParentTransform);

	void Clear();

	struct SubMesh
	{
		SubMesh() {};
		~SubMesh() {};

		void Init(const std::vector<VertexData>& Vertices, const std::vector<unsigned int>& Indices);

		unsigned int VAO;
		unsigned int NumIndices = 0;
		unsigned int MaterialIndex = 0;
	};

	struct BoneInfo
	{
		CrunchMath::Mat4x4 BoneOffset;
		CrunchMath::Mat4x4 FinalTransformation;
		BoneInfo()
		{
			//memset(&BoneOffset, 0, sizeof(BoneOffset));
			//memset(&FinalTransformation, 0, sizeof(FinalTransformation));
		}
	};

	std::vector<SubMesh> m_Entries;
	std::vector<Texture*> m_Textures;

	std::map<std::string, unsigned int> m_BoneMapping;
	unsigned int m_NumBones = 0;

	std::vector<BoneInfo> m_BoneInfo;

	CrunchMath::Mat4x4 m_GlobalInverseTransform;

	Assimp::Importer Importer;
	const aiScene* pScene;
};