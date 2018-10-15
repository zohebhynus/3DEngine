#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

struct aiMesh;

#define NUM_BONES_PER_VEREX 4


class ImportModelMaterial;
class ImportModel;
class ImportMesh
{
	//friend class ImportModel;

public:
	~ImportMesh();

/*	ImportMesh(const ImportMesh& rhs) = delete;
	ImportMesh& operator=(const ImportMesh& rhs) = delete;*/

	

	ImportModel& GetModel();
	ImportModelMaterial* GetMaterial();
	const std::string& GetName() const;

	const std::vector<XMFLOAT3>& GetVertices() const;
	const std::vector<XMFLOAT3>& GetNormals() const;
	const std::vector<XMFLOAT3>& GetTangents() const;
	const std::vector<XMFLOAT3>& GetBiTangents() const;
	const std::vector<XMFLOAT2>& GetTexCoords() const;

	unsigned int GetFaceCount() const;
	const std::vector<unsigned int>& GetIndices() const;

	ImportMesh(ImportModel& a_importModel, aiMesh& a_mesh);
	
private:

	struct BoneInfo
	{
		XMMATRIX BoneOffset;
		XMMATRIX FinalTransformation;

		BoneInfo()
		{
			BoneOffset *= 0;
			FinalTransformation *= 0;
		}
	};

	struct VertexBoneData
	{
		unsigned int IDs[NUM_BONES_PER_VEREX];
		float Weights[NUM_BONES_PER_VEREX];

		VertexBoneData()
		{
			Reset();
		};

		void Reset()
		{
			memset(IDs, 0, sizeof(IDs));
			memset(Weights, 0, sizeof(Weights));
		}

		void AddBoneData(unsigned int BoneID, float Weight);
	};

	std::string m_name;
	std::vector<XMFLOAT3> m_vertices;
	std::vector<XMFLOAT3> m_normals;
	std::vector<XMFLOAT3> m_tangents;
	std::vector<XMFLOAT3> m_biTangents;

	std::vector<XMFLOAT2> m_texCoords;

	unsigned int m_faceCount;
	std::vector<unsigned int> m_indices;
	
	

	ImportModel& m_importModel;
	ImportModelMaterial* m_importModelMaterial;
};


