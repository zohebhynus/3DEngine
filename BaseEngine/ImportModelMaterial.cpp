#include "ImportModelMaterial.h"

#include <assimp/scene.h>
#include <utility>

#include "ImportModel.h"



std::map<ImportTextureType, unsigned int> ImportModelMaterial::s_textureTypeMapping;

/*ImportModelMaterial::ImportModelMaterial(ImportModel& a_model) : m_importModel(a_model), m_textures(),m_name("")
{
	InitializeTextureTypeMapping();
}*/

ImportModelMaterial::ImportModelMaterial(ImportModel& a_model, aiMaterial* a_material) : m_importModel(a_model), m_textures()
{
	InitializeTextureTypeMapping();

	aiString name;
	a_material->Get(AI_MATKEY_NAME, name);
	m_name = name.C_Str();

	for (ImportTextureType textureType = (ImportTextureType)0; textureType < TextureTypeEnd; textureType = (ImportTextureType)(textureType + 1))
	{
		aiTextureType mappedTextureType = (aiTextureType)s_textureTypeMapping[textureType];

		unsigned int textureCount = a_material->GetTextureCount(mappedTextureType);
		if (textureCount > 0)
		{
			std::vector<std::string>* textures = new std::vector<std::string>();
			m_textures.insert(std::pair<ImportTextureType, std::vector<std::string>*>(textureType, textures));

			textures->reserve(textureCount);
			for (unsigned int textureIndex = 0; textureIndex < textureCount; textureIndex++)
			{
				aiString path;
				if (a_material->GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
				{
					std::string stringPath;
					ToString(path.C_Str(), stringPath);

					textures->push_back(stringPath);
				}
			}
		}
	}
}

/*ImportModelMaterial::ImportModelMaterial(const ImportModelMaterial & a_rhs) : m_importModel(nullptr), m_textures(), m_name("")
{
}*/

void ImportModelMaterial::ToString(const std::string& a_source, std::string& a_dest)
{
	a_dest.assign(a_source.begin(), a_source.end());
}

ImportModelMaterial::~ImportModelMaterial()
{
	for (std::pair<ImportTextureType, std::vector<std::string>*> textures : m_textures)
	{
		delete textures.second;
		textures.second = 0;
	}
}

ImportModel& ImportModelMaterial::GetModel()
{
	return m_importModel;
}

const std::string& ImportModelMaterial::GetName() const
{
	return m_name;
}

const std::map<ImportTextureType, std::vector<std::string>*>& ImportModelMaterial::GetTexture() const
{
	return m_textures;
}

void ImportModelMaterial::InitializeTextureTypeMapping()
{
	if (s_textureTypeMapping.size() != TextureTypeEnd)
	{
		s_textureTypeMapping[TextureTypeDiffuse] = aiTextureType_DIFFUSE;
		s_textureTypeMapping[TextureTypeSpecularMap] = aiTextureType_SPECULAR;
		s_textureTypeMapping[TextureTypeAmbient] = aiTextureType_AMBIENT;
		s_textureTypeMapping[TextureTypeHeightmap] = aiTextureType_HEIGHT;
		s_textureTypeMapping[TextureTypeNormalMap] = aiTextureType_NORMALS;
		s_textureTypeMapping[TextureTypeSpecularPowerMap] = aiTextureType_SHININESS;
		s_textureTypeMapping[TextureTypeDisplacementMap] = aiTextureType_DISPLACEMENT;
		s_textureTypeMapping[TextureTypeLightMap] = aiTextureType_LIGHTMAP;
	}
}

