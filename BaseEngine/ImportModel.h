#pragma once
#include <vector>


class ImportMesh;
class ImportModelMaterial;

class ImportModel
{
	//friend class ImportMesh;

public:
	ImportModel();
	~ImportModel();

	bool LoadModel(const std::string& a_filename, bool a_flipUVS = false);
	const std::vector<ImportMesh*>& GetMeshes() const;
	const std::vector<ImportModelMaterial*>& GetMaterials() const;

private:

	ImportModel(const ImportModel& rhs);
	std::vector<ImportMesh*> m_meshes;
	std::vector<ImportModelMaterial*> m_materials;
};


