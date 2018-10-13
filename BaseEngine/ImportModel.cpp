#include "ImportModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "ImportMesh.h"
#include "ImportModelMaterial.h"



ImportModel::ImportModel()
{
}


bool ImportModel::LoadModel(const std::string& a_filename, bool flipUVs)
{
	Assimp::Importer importer;

	unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;// | aiProcess_FlipWindingOrder;
	if (flipUVs)
	{
		flags |= aiProcess_FlipUVs;
	}

	

	const aiScene* scene = importer.ReadFile(a_filename.c_str(), flags);
	if (scene == nullptr)
	{
		return false;
	}

	if (scene->HasMaterials())
	{
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			m_materials.push_back(new ImportModelMaterial(*this, scene->mMaterials[i]));
		}
	}

	if (scene->HasMeshes())
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			ImportMesh* mesh = new ImportMesh(*this, *(scene->mMeshes[i]));
			m_meshes.push_back(mesh);
		}
	}
}

ImportModel::~ImportModel()
{
	for (ImportMesh* mesh : m_meshes)
	{
		delete mesh;
	}

	for (ImportModelMaterial* material :m_materials )
	{
		delete material;
	}
}

const std::vector<ImportMesh*>& ImportModel::GetMeshes() const
{
	return m_meshes;
}

const std::vector<ImportModelMaterial*>& ImportModel::GetMaterials() const
{
	return m_materials;
}

		
