#include <Windows.h>

#include "SampleViewer.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, NULL);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Direct2dSample";
    wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);
    
	if(!RegisterClassEx(&wcex))
        return false;

    // Create window
    RECT rc = { 0, 0, 900, 500 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, FALSE);

	HWND hWnd;
	hWnd = CreateWindow("Direct2DSample", "Direct2D Sample", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL);
    ShowWindow(hWnd, nCmdShow);

    return hWnd;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;

    if(!(hWnd = InitWindow(hInstance, nCmdShow)))
        return 0;

	SampleViewer::Instance()->Initialize(hInstance, hWnd);

    // Main message loop
    MSG msg = {0};

	DWORD prevTime = timeGetTime();

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else
        {
			DWORD nowTime = timeGetTime();
			SampleViewer::Instance()->Update((nowTime - prevTime) / 1000.f);
			SampleViewer::Instance()->Render();
			prevTime = nowTime;
        }
    }

	SampleViewer::Instance()->Release();
	delete SampleViewer::Instance();

    return (int)msg.wParam;
}

int main()
{
	return WinMain(GetModuleHandle(NULL), 0, 0, 1);
}
