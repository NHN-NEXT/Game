#pragma once
#include "Resource.h"

#ifdef USEWINDOW//윈도우면 



#include <windows.h>
#include "os.h"
class GZIGWindows :
	public OS
{
public:
	
	HINSTANCE mInstance;
	HWND hWnd;
	WNDCLASSEX wcex;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	void SetHWND(HWND hwnd);
	void Init();
	void PopUpWindow();
	int Loop();
	long GetHWNDinLongType();
	
	GZIGWindows(void);
	~GZIGWindows(void);
};

#endif // USEWINDOW