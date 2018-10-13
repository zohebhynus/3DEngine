#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{

	m_D3D = 0;
	m_Camera = 0;
	m_Light = 0;
	m_model = 0;


	m_LightShader = 0;

}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	x = 0.0f;
	y = -5.0f;
	z = -10.0f;
	xr = 0.0f;
	yr = 0.0f;
	zr = 0.0f;



	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DIRECT3D", L"Error", MB_OK);
		return false;
	}
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	m_Camera->SetPosition(x, y, z);

	m_model = new ModelClass;
	if (!m_model)
	{
		return false;
	}

	result = m_model->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), (char*)"../BaseEngine/data/1911_d.png", (char*)"../BaseEngine/data/Freefall's Colt 1911.obj");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}
	

	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 2.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}

void GraphicsClass::Update()
{
	m_Camera->SetPosition(x, y, z);
	m_Camera->SetRotation(xr, yr, zr);
}

void GraphicsClass::Shutdown()
{
	
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	

	if (m_model)
	{
		delete m_model;
		m_model = 0;
	}





	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = /*XMMatrixTranslation(0,-10,-30)**/ XMMatrixScaling(0.5, 0.5, 0.5) * XMMatrixRotationY(rotation);

	m_model->Render(m_D3D->GetDeviceContext());





	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

