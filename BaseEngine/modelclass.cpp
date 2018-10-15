#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;

	m_model = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<ImportMesh*> mesh, int num)
{
	bool result;
	//imp = new ImportModel();
	//result = LoadModel(modelFilename);
	//if (!result)
	//{
	//	return false;
	//}
	/*result = imp->LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}*/


	result = InitializeBuffer(device,  mesh, num);
	if (!result)
	{
		return false;
	}

	ImportModelMaterial* material_data;
	material_data = mesh[num]->GetMaterial();

	

	

	result = LoadTexture(device,deviceContext, (char*)"../BaseEngine/data/rock.jpg");
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffer();

	ReleaseModel();
	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffer(ID3D11Device* device, std::vector<ImportMesh*> mesh, int num)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT  result;
	int i, j;






	std::vector<XMFLOAT3> verts;
	std::vector<XMFLOAT3> norms;
	std::vector<XMFLOAT2> texx;
	std::vector<unsigned int> indi;


	verts = mesh[num]->GetVertices();
	norms = mesh[num]->GetNormals();
	texx = mesh[num]->GetTexCoords();
	indi = mesh[num]->GetIndices();

	//for (i = 0; i < mesh.size(); i++)
	//{
	////i = 2;
	//	for (j = 0; j < mesh[i]->m_vertices.size(); j++)
	//	{
	//		verts.push_back(mesh[i]->m_vertices[j]);
	//		norms.push_back(mesh[i]->m_normals[j]);
	//		if(mesh[i]->m_texCoords.size() ==! 0)
	//		texx.push_back(mesh[i]->m_texCoords[j]);
	//	}

	//	for (j = 0; j < mesh[i]->m_indices.size(); j++)
	//	{
	//		indi.push_back(mesh[i]->m_indices[j]);
	//	}
	//}
	m_vertexCount = verts.size();
	m_indexCount = indi.size();

	vertices = new VertexType[m_vertexCount];

	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	if (texx.size()==0)
	{
		for (i = 0; i < m_vertexCount; i++)
		{
			vertices[i].position = XMFLOAT3(verts[i].x, verts[i].y, verts[i].z);
			//vertices[i].texture = XMFLOAT2(texx[i].x, texx[i].y);
			vertices[i].texture = XMFLOAT2(0, 0);
			vertices[i].normal = XMFLOAT3(norms[i].x, norms[i].y, norms[i].z);
		}
	}
	else
	{
		for (i = 0; i < m_vertexCount; i++)
		{
			vertices[i].position = XMFLOAT3(verts[i].x, verts[i].y, verts[i].z);
			vertices[i].texture = XMFLOAT2(texx[i].x, texx[i].y);
			//vertices[i].texture = XMFLOAT2(0, 0);
			//vertices[i].normal = XMFLOAT3(0, 0,0);
			vertices[i].normal = XMFLOAT3(norms[i].x, norms[i].y, norms[i].z);
		}
	}
	for (i = 0; i < m_indexCount; i++)
	{
	
		indices[i] = indi[i];
		
	}

	//// Load the vertex array with data.
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].texture = XMFLOAT2(0.5f, 0.0f);
	//vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//// Load the index array with data.
	//indices[0] = 0;  // Bottom left.
	//indices[1] = 1;  // Top middle.
	//indices[2] = 2;  // Bottom right.

	//VertexType vVertices[36] =
	//{	//Position						 // UV co-ordinates	   //Normals	

	//	{ XMFLOAT3(-1.0f,  1.0f, -1.0f),     XMFLOAT2(0.0, 0.0),	       XMFLOAT3(0.0, 0.0,-1.0) },
	//	{ XMFLOAT3(1.0f,  1.0f , -1.0f),     XMFLOAT2(1.0, 0.0),		   XMFLOAT3(0.0, 0.0,-1.0) },
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f),     XMFLOAT2(0.0, 1.0),		   XMFLOAT3(0.0, 0.0,-1.0) },
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f),     XMFLOAT2(0.0, 1.0),		   XMFLOAT3(0.0, 0.0,-1.0) },
	//	{ XMFLOAT3(1.0f,  1.0f , -1.0f),     XMFLOAT2(1.0, 0.0),		   XMFLOAT3(0.0, 0.0,-1.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f),      XMFLOAT2(1.0, 1.0),		   XMFLOAT3(0.0, 0.0,-1.0) },
	//	{ XMFLOAT3(1.0f,  1.0f , -1.0f),     XMFLOAT2(0.0, 0.0),		   XMFLOAT3(1.0, 0.0, 0.0) },
	//	{ XMFLOAT3(1.0f,  1.0f ,  1.0f),     XMFLOAT2(1.0, 0.0),		   XMFLOAT3(1.0, 0.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f),      XMFLOAT2(0.0, 1.0),		   XMFLOAT3(1.0, 0.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f),      XMFLOAT2(0.0, 1.0),		   XMFLOAT3(1.0, 0.0, 0.0) },
	//	{ XMFLOAT3(1.0f,  1.0f ,  1.0f),     XMFLOAT2(1.0, 0.0),		   XMFLOAT3(1.0, 0.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f,  1.0f),      XMFLOAT2(1.0, 1.0),		   XMFLOAT3(1.0, 0.0, 0.0) },
	//	{ XMFLOAT3(1.0f,  1.0f ,  1.0f),     XMFLOAT2(0.0, 0.0),		   XMFLOAT3(0.0, 0.0, 1.0) },
	//	{ XMFLOAT3(-1.0f,  1.0f ,  1.0f),    XMFLOAT2(1.0, 0.0),		   XMFLOAT3(0.0, 0.0, 1.0) },
	//	{ XMFLOAT3(1.0f, -1.0f,  1.0f),      XMFLOAT2(0.0, 1.0),		   XMFLOAT3(0.0, 0.0, 1.0) },
	//	{ XMFLOAT3(1.0f, -1.0f,  1.0f),      XMFLOAT2(0.0, 1.0),		   XMFLOAT3(0.0, 0.0, 1.0) },
	//	{ XMFLOAT3(-1.0f,  1.0f ,  1.0f),    XMFLOAT2(1.0, 0.0),		   XMFLOAT3(0.0, 0.0, 1.0) },
	//	{ XMFLOAT3(-1.0f, -1.0f,  1.0f),     XMFLOAT2(1.0, 1.0),		   XMFLOAT3(0.0, 0.0, 1.0) },
	//	{ XMFLOAT3(-1.0f,  1.0f ,  1.0f),    XMFLOAT2(0.0, 0.0),		   XMFLOAT3(-1.0, 0.0, 0.0)  },
	//	{ XMFLOAT3(-1.0f,  1.0f , -1.0f),    XMFLOAT2(1.0, 0.0),		   XMFLOAT3(-1.0, 0.0, 0.0)  },
	//	{ XMFLOAT3(-1.0f, -1.0f,  1.0f),     XMFLOAT2(0.0, 1.0),		   XMFLOAT3(-1.0, 0.0, 0.0)  },
	//	{ XMFLOAT3(-1.0f, -1.0f,  1.0f),     XMFLOAT2(0.0, 1.0),		   XMFLOAT3(-1.0, 0.0, 0.0)  },
	//	{ XMFLOAT3(-1.0f,  1.0f , -1.0f),    XMFLOAT2(1.0, 0.0),		   XMFLOAT3(-1.0, 0.0, 0.0)  },
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f),     XMFLOAT2(1.0, 1.0),		   XMFLOAT3(-1.0, 0.0, 0.0)  },
	//	{ XMFLOAT3(-1.0f,  1.0f ,  1.0f),    XMFLOAT2(0.0, 0.0),		   XMFLOAT3(0.0, 1.0, 0.0) },
	//	{ XMFLOAT3(1.0f,  1.0f ,  1.0f),     XMFLOAT2(1.0, 0.0),		   XMFLOAT3(0.0, 1.0, 0.0) },
	//	{ XMFLOAT3(-1.0f,  1.0f , -1.0f),    XMFLOAT2(0.0, 1.0),		   XMFLOAT3(0.0, 1.0, 0.0) },
	//	{ XMFLOAT3(-1.0f,  1.0f , -1.0f),    XMFLOAT2(0.0, 1.0),		   XMFLOAT3(0.0, 1.0, 0.0) },
	//	{ XMFLOAT3(1.0f,  1.0f ,  1.0f),     XMFLOAT2(1.0, 0.0),		   XMFLOAT3(0.0, 1.0, 0.0) },
	//	{ XMFLOAT3(1.0f,  1.0f , -1.0f),     XMFLOAT2(1.0, 1.0),		   XMFLOAT3(0.0, 1.0, 0.0) },
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f),     XMFLOAT2(0.0, 0.0),		   XMFLOAT3(0.0,-1.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f),      XMFLOAT2(1.0, 0.0),		   XMFLOAT3(0.0,-1.0, 0.0) },
	//	{ XMFLOAT3(-1.0f, -1.0f,  1.0f),     XMFLOAT2(0.0, 1.0),	       XMFLOAT3(0.0,-1.0, 0.0) },
	//	{ XMFLOAT3(-1.0f, -1.0f,  1.0f),     XMFLOAT2(0.0, 1.0),	       XMFLOAT3(0.0,-1.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f),      XMFLOAT2(1.0, 0.0),		   XMFLOAT3(0.0,-1.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f,  1.0f),      XMFLOAT2(1.0, 1.0)	,	       XMFLOAT3(0.0,-1.0, 0.0) }
	//};

	//for (int i = 0; i < 36; ++i)
	//{
	//	indices[i] = i;
	//}


	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffer()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;

}
bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext,char* filename)
{
	bool result;


	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	result = m_Texture->Initialize(device, deviceContext,filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void ModelClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void ModelClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}



