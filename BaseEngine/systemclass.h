#pragma once

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WINS32_LEAN_AND_MEAN

#define NOMINMAX
#include <Windows.h>

#include "inputclass.h"
#include "graphicsclass.h"

class SystemClass
{
public:
	SystemClass(void);
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize(void);
	void Shutdown(void);
	void Run(void);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif
