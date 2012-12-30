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
	wcex.lpszClassName  = REALCLASSNAME;
	wcex.hIconSm        = LoadIcon(mInstance, (LPCTSTR)IDI_APPLICATION);


	RegisterClassEx(&wcex);

	hWnd = CreateWindowEx(0,REALCLASSNAME,"windowname",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,mInstance,reinterpret_cast<LPVOID>(this));
/*

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC) msgHandlerSimpleOpenGLClass;
	wc.cbClsExtra = 0; wc.cbWndExtra = 0;
	wc.hInstance = mInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = SIMPLE_OPENGL_CLASS_NAME;

	RegisterClassEx(&wc);

	fake = CreateWindow(FAKECLASSNAME, "FAKE", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, mInstance, NULL);//glew 초기화할라고 만든 가짜임 링크 참고 fake HWND for initilizing glew http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=2*/
/*same size걍 체크해본거임
	int a, b;
	a = sizeof(hWnd);
	b = sizeof(long);*/
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

long GZIGWindows::GetFAKEinLongType()
{
	return (long)fake;
}

void GZIGWindows::Release()
{
	DestroyWindow(hWnd);
	UnregisterClass(REALCLASSNAME,mInstance);
}

#endif // USEWINDOW