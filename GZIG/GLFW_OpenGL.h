#pragma once
#include "graphic.h"
class GLFW_OpenGL :
	public Graphic
{
public:

	bool Init(long hwnd);//�ƹ��ų� �޾Ƶ� �� �Ⱦ�
	void Render();
	void Release();
	GLFW_OpenGL(void);
	~GLFW_OpenGL(void);
};

