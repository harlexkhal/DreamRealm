#pragma once
#include "CrunchMath.h"

struct StaticVertexData
{
	CrunchMath::Vec3 m_Position;
	CrunchMath::Vec3 m_Texture;
	CrunchMath::Vec3 m_Normal;

	StaticVertexData(const CrunchMath::Vec3 pos, const CrunchMath::Vec3 tex, const CrunchMath::Vec3 normal)
	{
		m_Position = pos;
		m_Texture = tex;
		m_Normal = normal;
	}
};

struct SkeletalVertexData : public StaticVertexData
{
	unsigned int  IDs[4];
	float Weights[4];

	SkeletalVertexData(const CrunchMath::Vec3 Pos, const CrunchMath::Vec3 Tex, const CrunchMath::Vec3 Normal)
		:StaticVertexData(Pos, Tex, Normal)
	{
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