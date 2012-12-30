#pragma once
#include "graphic.h"

class CGLShader;
class CGLProgram;

class OpenGL :
	public Graphic
{
	long hwnd;
	HGLRC m_hrc;
	HDC hDC;
public:
	bool Init(long hwnd, long fake);
	void Render();
	void Release();
	OpenGL(void);
	~OpenGL(void);
};