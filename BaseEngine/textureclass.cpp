#include "textureclass.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureClass::TextureClass()
{

	m_texture = 0;

}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,char* filename)
{

	//HRESULT result;

	

	//bool result;
	unsigned char* test;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	int nrchannel;

	/*result = LoadTarga(filename, height, width);
	if (!result)
	{
		return false;
	}*/
	

	test = stbi_load(filename, &width, &height, &nrchannel, 0);
	if (!test)
	{
		return false;
	}
	image = new unsigned char[width*height*nrchannel];
	memcpy(image, test, width*height*nrchannel);

	stbi_image_free(test);

	rowPitch = (width * nrchannel) * sizeof(unsigned char);
	//unsigned int depthPitch = (height*nrchannel) * sizeof(unsigned char);

	D3D11_SUBRESOURCE_DATA obj;
	obj.pSysMem = image;
	obj.SysMemPitch = rowPitch;
	obj.SysMemSlicePitch = 0;

	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	
	hResult = device->CreateTexture2D(&textureDesc, NULL , &m_texture);
	if (FAILED(hResult))
	{
		return false;
	}

	

	deviceContext->UpdateSubresource(m_texture, 0, NULL, image, rowPitch, 0);

	srvDesc.Format =  textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult))
	{
		return false;
	}

	deviceContext->GenerateMips(m_textureView);



	return true;
}



void TextureClass::Shutdown()
{

	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;

	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
	

	if (image)
	{
		delete[] image;
		image = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

//bool TextureClass::LoadTarga(char* filename, int& height, int& width)
//{
//	int error, bpp, imageSize, index, i, j, k;
//	FILE* filePtr;
//	unsigned int count;
//	TargetHeader targetFileHeader;
//	unsigned char* targaImage;
//
//
//	// Open the targa file for reading in binary.
//	error = fopen_s(&filePtr, filename, "rb");
//	if (error != 0)
//	{
//		return false;
//	}
//
//	// Read in the file header.
//	count = (unsigned int)fread(&targetFileHeader, sizeof(targetFileHeader), 1, filePtr);
//	if (count != 1)
//	{
//		return false;
//	}
//
//	height = (int)targetFileHeader.height;
//	width = (int)targetFileHeader.width;
//	bpp = (int)targetFileHeader.bpp;
//
//	if (bpp != 32)
//	{
//		return false;
//	}
//
//	imageSize = width * height * 4;
//
//	targaImage = new unsigned char[imageSize];
//	if (!targaImage)
//	{
//		return false;
//	}
//
//	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
//	if (count != imageSize)
//	{
//		return false;
//	}
//
//	error = fclose(filePtr);
//	if (error != 0)
//	{
//		return false;
//	}
//
//	m_targaData = new unsigned char[imageSize];
//	if (!m_targaData)
//	{
//		return false;
//	}
//
//	index = 0;
//
//	k = (width * height * 4) - (width * 4);
//
//	for (j = 0; j < height; j++)
//	{
//		for (i = 0; i < width; i++)
//		{
//			m_targaData[index + 0] = targaImage[k + 2];  // Red.
//			m_targaData[index + 1] = targaImage[k + 1];  // Green.
//			m_targaData[index + 2] = targaImage[k + 0];  // Blue
//			m_targaData[index + 3] = targaImage[k + 3];  // Alpha
//
//			k += 4;
//			index += 4;
//		}
//
//		k -= (width * 8);
//	}
//
//	delete[] targaImage;
//	targaImage = 0;
//
//	return true;
//}