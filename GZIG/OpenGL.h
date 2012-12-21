#pragma once
#include "graphic.h"
class OpenGL :
	public Graphic
{
public:
	bool Init(long hwnd);
	void Render();
	void Release();
	OpenGL(void);
	~OpenGL(void);
};

