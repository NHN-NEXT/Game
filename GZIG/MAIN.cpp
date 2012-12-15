#include "Resource.h"
#include "MAIN.h"
#include "GZIGWindows.h"
#include "Graphic.h"
#include "DX.h"
#include "GLFW_OpenGL.h"
#include "GLFW_OS.h"
MAIN::MAIN(void)
{
#ifdef USEWINDOW
	os = new GZIGWindows;
	graphic = new DX;
#endif // USEWINDOW
#ifdef USEGLFW
	os = new GLFW_OS;
	graphic = new GLFW_OpenGL;
#endif // USEGLFW



}


MAIN::~MAIN(void)
{
}

void MAIN::OpenWindow()
{
	os->PopUpWindow();
}

void MAIN::Init()
{
	os->Init();
	graphic->Init((long)(os->GetHWNDinLongType()));
}

void MAIN::Loop()
{
	int ret = 1;
	while( ret != MESSAGEQUIT)
	{
		ret = os->Loop();
		if(ret == MESSAGENOTHING)
		{
			//render
			graphic->Render();
		}
		else//(ret == MESSAGEPROCESSING)
		{
			continue;
		}
	}
}

void MAIN::Release()
{
	if(os) delete os;
	if(graphic)
	{
		graphic->Release();
		delete graphic;
	}
}
