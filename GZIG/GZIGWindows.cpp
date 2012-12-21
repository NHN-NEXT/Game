#include "Resource.h"
#ifdef USEWINDOW



#include "GZIGWindows.h"


GZIGWindows::GZIGWindows(void)
{
	mInstance = NULL;
}


GZIGWindows::~GZIGWindows(void)
{
}

void GZIGWindows::PopUpWindow()
{
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
}

LRESULT CALLBACK GZIGWindows::WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	GZIGWindows* wnd = NULL;
	if(message == WM_NCCREATE)
	{

		wnd = reinterpret_cast<GZIGWindows*> (((LPCREATESTRUCT)lparam)->lpCreateParams);
		::SetWindowLong(hwnd, GWL_USERDATA, reinterpret_cast<long>(wnd));
//		wnd->SetHWND(hwnd);
		return 1;
	}
	else
	{
		wnd = reinterpret_cast<GZIGWindows*>(::GetWindowLong(hwnd,GWL_USERDATA));
		switch(message)
		{
		case WM_DESTROY:
			PostQuitMessage(0) ;
			return 0L ;
		case WM_LBUTTONDOWN:
			// fall thru
		default:
			return DefWindowProc( hwnd, message, wparam, lparam ) ;
		}
	}
}

void GZIGWindows::Init()
{
	if(mInstance == NULL)
		mInstance = GetModuleHandle(0);
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = mInstance;
	wcex.hIcon          = LoadIcon(mInstance, (LPCTSTR)IDI_APPLICATION);
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = "myclass";
	wcex.hIconSm        = LoadIcon(mInstance, (LPCTSTR)IDI_APPLICATION);


	RegisterClassEx(&wcex);

	hWnd = CreateWindowEx(0,"myclass","windowname",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,mInstance,reinterpret_cast<LPVOID>(this));
	int a, b;
	a = sizeof(hWnd);
	b = sizeof(long);
}

void GZIGWindows::SetHWND( HWND hwnd )
{
	hWnd = hwnd;
}

int GZIGWindows::Loop()
{
	BOOL bRet;
	MSG msg;
	bRet = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	if(WM_QUIT == msg.message)
	{
		return MESSAGEQUIT;
	}
	else if(bRet)
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		return MESSAGEPROCESSING;
	}
	else
	{
		return MESSAGENOTHING;
	}

}

long GZIGWindows::GetHWNDinLongType()
{
	return (long)hWnd;
}

#endif // USEWINDOW