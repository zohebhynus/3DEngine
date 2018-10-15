#pragma once
#ifndef GRAPHICSCLASS_H_
#define GRAPHICSCLASS_H_
#define NOMINMAX
#include <Windows.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#//include "colorshaderclass.h"
//#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "deferredshaderclass.h"
#include "deferredbuffersclass.h"
#include "inputclass.h"
#include "orthorwindowclass.h"



const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Update();
	void Shutdown();
	bool Frame();

	float x, y, z;
	float posx, posy, posz;
	float xr, yr, zr;

private:
	bool Render(float);



	D3DClass* m_D3D;
	CameraClass* m_Camera;
	
	LightClass* m_Light;

	std::vector<ModelClass*> m_model;
	ImportModel* m_modelImporter;


	LightShaderClass* m_LightShader;


	
};

#endif // !GRAPHICSCLASS_H_

