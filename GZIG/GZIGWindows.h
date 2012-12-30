#pragma once
#include "Resource.h"

#ifdef USEWINDOW//������� 



#include <windows.h>
#include "os.h"
class GZIGWindows :
	public OS
{
public:
	
	HINSTANCE mInstance;
	HWND hWnd;
	HWND fake;
	WNDCLASSEX wcex;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	void SetHWND(HWND hwnd);
	void Init();
	void Release();
	void PopUpWindow();
	int Loop();
	long GetHWNDinLongType();
	long GetFAKEinLongType();
	
	
	GZIGWindows(void);
	~GZIGWindows(void);
};

#endif // USEWINDOW