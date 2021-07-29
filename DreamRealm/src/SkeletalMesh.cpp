#include "SkeletalMesh.h"

SkinnedMesh::SkinnedMesh()
{
	//TO DO
}

SkinnedMesh::~SkinnedMesh()
{
	//TO DO
}

void SkinnedMesh::LoadMesh(const std::string& Filename)
{
	pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	//pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
	//	aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (pScene)
	{
		m_GlobalInverseTransform.Matrix[0][0] =pScene->mRootNode->mTransformation.a1; m_GlobalInverseTransform.Matrix[1][0] =pScene->mRootNode->mTransformation.a2; m_GlobalInverseTransform.Matrix[2][0] =pScene->mRootNode->mTransformation.a3; m_GlobalInverseTransform.Matrix[3][0] =pScene->mRootNode->mTransformation.a4;
		m_GlobalInverseTransform.Matrix[0][1] =pScene->mRootNode->mTransformation.b1; m_GlobalInverseTransform.Matrix[1][1] =pScene->mRootNode->mTransformation.b2; m_GlobalInverseTransform.Matrix[2][1] =pScene->mRootNode->mTransformation.b3; m_GlobalInverseTransform.Matrix[3][1] =pScene->mRootNode->mTransformation.b4;
		m_GlobalInverseTransform.Matrix[0][2] =pScene->mRootNode->mTransformation.c1; m_GlobalInverseTransform.Matrix[1][2] =pScene->mRootNode->mTransformation.c2; m_GlobalInverseTransform.Matrix[2][2] =pScene->mRootNode->mTransformation.c3; m_GlobalInverseTransform.Matrix[3][2] =pScene->mRootNode->mTransformation.c4;
		m_GlobalInverseTransform.Matrix[0][3] =pScene->mRootNode->mTransformation.d1; m_GlobalInverseTransform.Matrix[1][3] =pScene->mRootNode->mTransformation.d2; m_GlobalInverseTransform.Matrix[2][3] =pScene->mRootNode->mTransformation.d3; m_GlobalInverseTransform.Matrix[3][3] =pScene->mRootNode->mTransformation.d4;

		CrunchMath::Invert(m_GlobalInverseTransform);
		InitFromScene(pScene, Filename);
	}
	else
		std::cerr << "Error parsing::" << Filename << "::" << Importer.GetErrorString() << std::endl;
}

void SkinnedMesh::Render()
{

	for (unsigned int i = 0; i < m_Entries.size(); i++) 
	{
		glBindVertexArray(m_Entries[i].VAO);
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind();
		}

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}
}

void SkinnedMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	unsigned int NumVertices = 0;

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	InitMaterials(pScene, Filename);
}

void SkinnedMesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<VertexData> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);

		VertexData v(CrunchMath::Vec3(pPos->x, pPos->y, pPos->z),
			         CrunchMath::Vec3(pTexCoord->x, pTexCoord->y, 0.0f),
			         CrunchMath::Vec3(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
	}

	LoadBones(Index, paiMesh, Vertices);

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[Index].Init(Vertices, Indices);
}

void SkinnedMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
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

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) 
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
			{
				std::string File = Path.data;
				std::size_t Found = File.find_last_of("/\\");
				std::string FullPath = Dir + "/" + File.substr(Found + 1);
				m_Textures[i] = new Texture();
				m_Textures[i]->Load(FullPath.c_str());
			}
		}

		/* Load a white texture in case the Mesh does not include its own texture
		if (!m_Textures[i])
		{
			m_Textures[i] = new Texture();
			m_Textures[i]->Load("../Content/white.png");
		}*/
	}
}

void SkinnedMesh::LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<VertexData>& Vertices)
{
	for (unsigned int i = 0; i < paiMesh->mNumBones; i++)
	{
		unsigned int BoneIndex = 0;
		std::string BoneName = paiMesh->mBones[i]->mName.data;

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
		{
			//Allocate an Index for a new bone
			BoneIndex = m_NumBones;
			m_NumBones++;

			BoneInfo bi;
			m_BoneInfo.push_back(bi);

			m_BoneInfo[BoneIndex].BoneOffset.Matrix[0][0] =paiMesh->mBones[i]->mOffsetMatrix.a1; m_BoneInfo[BoneIndex].BoneOffset.Matrix[1][0] =paiMesh->mBones[i]->mOffsetMatrix.a2;   m_BoneInfo[BoneIndex].BoneOffset.Matrix[2][0] =paiMesh->mBones[i]->mOffsetMatrix.a3; m_BoneInfo[BoneIndex].BoneOffset.Matrix[3][0] =paiMesh->mBones[i]->mOffsetMatrix.a4;
			m_BoneInfo[BoneIndex].BoneOffset.Matrix[0][1] =paiMesh->mBones[i]->mOffsetMatrix.b1; m_BoneInfo[BoneIndex].BoneOffset.Matrix[1][1] =paiMesh->mBones[i]->mOffsetMatrix.b2;   m_BoneInfo[BoneIndex].BoneOffset.Matrix[2][1] =paiMesh->mBones[i]->mOffsetMatrix.b3; m_BoneInfo[BoneIndex].BoneOffset.Matrix[3][1] =paiMesh->mBones[i]->mOffsetMatrix.b4;
			m_BoneInfo[BoneIndex].BoneOffset.Matrix[0][2] = paiMesh->mBones[i]->mOffsetMatrix.c1; m_BoneInfo[BoneIndex].BoneOffset.Matrix[1][2] = paiMesh->mBones[i]->mOffsetMatrix.c2; m_BoneInfo[BoneIndex].BoneOffset.Matrix[2][2] = paiMesh->mBones[i]->mOffsetMatrix.c3; m_BoneInfo[BoneIndex].BoneOffset.Matrix[3][2] = paiMesh->mBones[i]->mOffsetMatrix.c4;
			m_BoneInfo[BoneIndex].BoneOffset.Matrix[0][3] =paiMesh->mBones[i]->mOffsetMatrix.d1; m_BoneInfo[BoneIndex].BoneOffset.Matrix[1][3] =paiMesh->mBones[i]->mOffsetMatrix.d2;   m_BoneInfo[BoneIndex].BoneOffset.Matrix[2][3] =paiMesh->mBones[i]->mOffsetMatrix.d3; m_BoneInfo[BoneIndex].BoneOffset.Matrix[3][3] =paiMesh->mBones[i]->mOffsetMatrix.d4;

			m_BoneMapping[BoneName] = BoneIndex;
		}

		else
		{
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (unsigned int j = 0; j < paiMesh->mBones[i]->mNumWeights; j++)
		{
			float Weigth = paiMesh->mBones[i]->mWeights[j].mWeight;
			unsigned int VertexID = paiMesh->mBones[i]->mWeights[j].mVertexId;

			Vertices[VertexID].AddBoneData(BoneIndex, Weigth);
		}
	}
}

void SkinnedMesh::BoneTransform(float Ts, std::vector<CrunchMath::Mat4x4>& Transforms)
{
	CrunchMath::Mat4x4 Identity(1.0f);
	
	float TicksPerSecond = (float)(pScene->mAnimations[0]->mTicksPerSecond != 0 ? pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = Ts * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)pScene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(AnimationTime, pScene->mRootNode, Identity);

	Transforms.resize(m_NumBones);

	for (unsigned int i = 0; i < m_NumBones; i++)
	{
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}

const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) 
	{
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName)
		{
			return pNodeAnim;
		}
	}

	return nullptr;
}

unsigned int SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	//assert(0);

	return 0;
}

unsigned int SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	//assert(0);

	return 0;
}

unsigned int SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) 
	{
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) 
		{
			return i;
		}
	}

	//assert(0);

	return 0;
}

void SkinnedMesh::CalculateInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) 
	{
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void SkinnedMesh::CalculateInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) 
	{
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int  RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void SkinnedMesh::CalculateInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) 
	{
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void SkinnedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const CrunchMath::Mat4x4& ParentTransform)
{
	std::string NodeName = pNode->mName.data;

	const aiAnimation* pAnimation = pScene->mAnimations[0];

	CrunchMath::Mat4x4 NodeTransformation;

	NodeTransformation.Matrix[0][0] = pNode->mTransformation.a1; NodeTransformation.Matrix[1][0] = pNode->mTransformation.a2; NodeTransformation.Matrix[2][0] = pNode->mTransformation.a3; NodeTransformation.Matrix[3][0] = pNode->mTransformation.a4;
	NodeTransformation.Matrix[0][1] = pNode->mTransformation.b1; NodeTransformation.Matrix[1][1] = pNode->mTransformation.b2; NodeTransformation.Matrix[2][1] = pNode->mTransformation.b3; NodeTransformation.Matrix[3][1] = pNode->mTransformation.b4;
	NodeTransformation.Matrix[0][2] = pNode->mTransformation.c1; NodeTransformation.Matrix[1][2] = pNode->mTransformation.c2; NodeTransformation.Matrix[2][2] = pNode->mTransformation.c3; NodeTransformation.Matrix[3][2] = pNode->mTransformation.c4;
	NodeTransformation.Matrix[0][3] = pNode->mTransformation.d1; NodeTransformation.Matrix[1][3] = pNode->mTransformation.d2; NodeTransformation.Matrix[2][3] = pNode->mTransformation.d3; NodeTransformation.Matrix[3][3] = pNode->mTransformation.d4;

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim)
	{
		//Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalculateInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		CrunchMath::Mat4x4 ScalingM(1.0f);
		ScalingM.InsertDiagonal(CrunchMath::Vec3(Scaling.x, Scaling.y, Scaling.z));

		//Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalculateInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		CrunchMath::Mat4x4 RotationM(1.0f);

		//To do;;; OPTIMIZE LATER CALLING THE FUCNTION EVERYTIME DIRECTLY IS NOT THE BEST THING TO DO
		RotationM.Matrix[0][0] = RotationQ.GetMatrix().a1; RotationM.Matrix[1][0] = RotationQ.GetMatrix().a2; RotationM.Matrix[2][0] = RotationQ.GetMatrix().a3;
		RotationM.Matrix[0][1] = RotationQ.GetMatrix().b1; RotationM.Matrix[1][1] = RotationQ.GetMatrix().b2; RotationM.Matrix[2][1] = RotationQ.GetMatrix().b3;
		RotationM.Matrix[0][2] = RotationQ.GetMatrix().c1; RotationM.Matrix[1][2] = RotationQ.GetMatrix().c2; RotationM.Matrix[2][2] = RotationQ.GetMatrix().c3;

		//Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalculateInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		CrunchMath::Mat4x4 TranslationM(1.0F);
		TranslationM.Translate(CrunchMath::Vec3(Translation.x, Translation.y, Translation.z));

		//Combine the above Transformations
		NodeTransformation = TranslationM * RotationM;
		NodeTransformation *= ScalingM;
	}

	CrunchMath::Mat4x4 GlobalTransformation;
	//since Parent Transform has to be const and me not wanting to risk the CrunchMath API from disfiguring my data i would just need
	// to create a copy of it so the copy would be passed into the API for security reason's. and yes i know i wrote the CrunchMath API myself.
	// I chose chose to assume i don't know what's really going on in it....;-). but then there would be no need to pass it by reference.
	//if it would still be copied. it nullifies the point of passing it by reference to prevent copy which makes things actually slow..... but then.
	//this code is not for production use.... just for my learning purposes...

	CrunchMath::Mat4x4 CopiedParentTransform = ParentTransform;
	GlobalTransformation = CopiedParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
	{
		unsigned int BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation;
		m_BoneInfo[BoneIndex].FinalTransformation *= m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void SkinnedMesh::SubMesh::Init(const std::vector<VertexData>& Vertices, const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();

	unsigned int VBO;
	unsigned int EBO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, m_Texture));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, m_Normal));
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexData), (void*)offsetof(VertexData, IDs));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Weights));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
}

void SkinnedMesh::Clear()
{
	//TO DO
}