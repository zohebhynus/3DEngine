#pragma once
#include <vector>
#include <map>

struct aiMaterial;


class ImportModel;

enum ImportTextureType
{
	TextureTypeDiffuse = 0,
	TextureTypeSpecularMap,
	TextureTypeAmbient,
	TextureTypeEmissive,
	TextureTypeHeightmap,
	TextureTypeNormalMap,
	TextureTypeSpecularPowerMap,
	TextureTypeDisplacementMap,
	TextureTypeLightMap,
	TextureTypeEnd
};

class ImportModelMaterial
{
	//friend class ImportModel;

public:
	
	//ImportModelMaterial& operator=(const ImportModelMaterial& rhs) = delete;

	
	~ImportModelMaterial();

	ImportModel& GetModel();
	const std::string& GetName() const;
	const std::map<ImportTextureType, std::vector<std::string>*>& GetTexture() const;
	std::string m_name;

	//(ImportModel& a_model);
	ImportModelMaterial(ImportModel& a_model, aiMaterial* a_material);
	//ImportModelMaterial(const ImportModelMaterial& a_rhs);

private:
	static void InitializeTextureTypeMapping();

	

	static std::map<ImportTextureType, unsigned int> s_textureTypeMapping;
	void ToString(const std::string& a_source, std::string& a_dest);
	

	ImportModel& m_importModel;
	std::map<ImportTextureType, std::vector<std::string>*> m_textures;
	//std::string m_name;
};

