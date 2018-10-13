
#include "ImportMesh.h"

#include "ImportModel.h"
#include "ImportModelMaterial.h" 


#include <assimp/scene.h>






	
ImportMesh::ImportMesh(ImportModel& a_importModel, aiMesh& a_mesh) : m_importModel(a_importModel), m_importModelMaterial(nullptr),m_name(a_mesh.mName.C_Str()), m_vertices(), m_faceCount(0), m_indices()
{
	m_importModelMaterial = m_importModel.GetMaterials().at(a_mesh.mMaterialIndex);

	//Vertices
	m_vertices.reserve(a_mesh.mNumVertices);
	for (unsigned int i = 0; i < a_mesh.mNumVertices; i++)
	{
		XMFLOAT3 vertices;
		vertices.x = a_mesh.mVertices[i].x;
		vertices.y = a_mesh.mVertices[i].y;
		vertices.z = a_mesh.mVertices[i].z;

		m_vertices.push_back(vertices);

		//m_vertices.push_back(Vector3(reinterpret_cast<const float*>(&a_mesh.mVertices[i])));
	}

	//Normals
	if (a_mesh.HasNormals())
	{
		m_normals.reserve(a_mesh.mNumVertices);
		for (unsigned int i = 0; i < a_mesh.mNumVertices; i++)
		{
			XMFLOAT3 normals;
			normals.x = a_mesh.mNormals[i].x;
			normals.y = a_mesh.mNormals[i].y;
			normals.z = a_mesh.mNormals[i].z;

			m_normals.push_back(normals);
		}
	}
	
	//Tangents and BiTangents
	if (a_mesh.HasTangentsAndBitangents())
	{
		m_tangents.reserve(a_mesh.mNumVertices);
		for (unsigned int i = 0; i < a_mesh.mNumVertices; i++)
		{
			XMFLOAT3 tangents;
			XMFLOAT3 biTangents;
			tangents.x = a_mesh.mTangents[i].x;
			tangents.y = a_mesh.mTangents[i].y;
			tangents.z = a_mesh.mTangents[i].z;

			biTangents.x = a_mesh.mBitangents[i].x;
			biTangents.y = a_mesh.mBitangents[i].y;
			biTangents.z = a_mesh.mBitangents[i].z;

			m_tangents.push_back(tangents);
			m_biTangents.push_back(biTangents);

		}
	}

	//TexCoords
	if (a_mesh.HasTextureCoords(0))
	{
		for (unsigned int i = 0; i < a_mesh.mNumVertices; i++)
		{
			XMFLOAT2 texCoords;
			texCoords.x = a_mesh.mTextureCoords[0][i].x;
			texCoords.y = a_mesh.mTextureCoords[0][i].y;

			m_texCoords.push_back(texCoords);
		}
	}

	if (a_mesh.HasFaces())
	{
		for (unsigned int i = 0; i < a_mesh.mNumFaces; i++)
		{
			aiFace* face = &a_mesh.mFaces[i];

			for (unsigned int j = 0; j < face->mNumIndices; j++)
			{
				m_indices.push_back(face->mIndices[j]);
			}
		}
	}

}

ImportMesh::~ImportMesh()
{
}

ImportModel& ImportMesh::GetModel()
{
	return m_importModel;
}

ImportModelMaterial* ImportMesh::GetMaterial()
{
	return m_importModelMaterial;
}

const std::string& ImportMesh::GetName() const
{
	return m_name;
}

const std::vector<XMFLOAT3>& ImportMesh::GetVertices() const
{
	return m_vertices;
}

const std::vector<XMFLOAT3>& ImportMesh::GetNormals() const
{
	return m_normals;
}

const std::vector<XMFLOAT3>& ImportMesh::GetTangents() const
{
	return m_tangents;
}

const std::vector<XMFLOAT3>& ImportMesh::GetBiTangents() const
{
	return m_biTangents;
}

const std::vector<XMFLOAT2>& ImportMesh::GetTexCoords() const
{
	return m_texCoords;
}

unsigned int ImportMesh::GetFaceCount() const
{
	return m_faceCount;
}

const std::vector<unsigned int>& ImportMesh::GetIndices() const
{
	return m_indices;
}
