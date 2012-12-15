#pragma once
#include "graphic.h"
class GLFW_OpenGL :
	public Graphic
{
public:

	bool Init(long hwnd);//¾Æ¹«°Å³ª ¹Þ¾Æµµ µÊ ¾È¾¸
	void Render();
	void Release();
	GLFW_OpenGL(void);
	~GLFW_OpenGL(void);
};

