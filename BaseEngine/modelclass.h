#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

using namespace std;
using namespace DirectX;

#include "textureclass.h"
#include "ImportMesh.h"
#include "ImportModel.h"

class ModelClass
{
private:
	struct  VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};


public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, std::vector<ImportMesh*>, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(float, float, float);
	void GetPosition(float&, float&, float&);

private:
	bool InitializeBuffer(ID3D11Device*, std::vector<ImportMesh*>, int);
	void ShutdownBuffer();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	ModelType* m_model;
	float m_positionX, m_positionY, m_positionZ;
	ImportModel* imp;
	std::vector<ImportMesh*> mesh;

};


#endif // !_MODELCLASS_H_

