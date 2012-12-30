#include "Resource.h"
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#ifdef USEOPENGL
//#pragma comment(lib,"gdi32.lib")

#include "OpenGL.h"

#include <gl/glew.h>
#include <gl/wglew.h>
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

/*
HDC hDC;

PFNWGLGETEXTENSIONSSTRINGARBPROC GetExtensionsStringARB;//wglext.h 에서 쓰는것
PFNWGLGETEXTENSIONSSTRINGEXTPROC GetExtensionsStringEXT;*/
/*
typedef int  (WINAPI * CHOOSEPIXELFORMAT_T) (HDC,CONST PIXELFORMATDESCRIPTOR*);
typedef int  (WINAPI * DESCRIBEPIXELFORMAT_T) (HDC,int,UINT,LPPIXELFORMATDESCRIPTOR);
typedef int  (WINAPI * GETPIXELFORMAT_T) (HDC);
typedef BOOL (WINAPI * SETPIXELFORMAT_T) (HDC,int,const PIXELFORMATDESCRIPTOR*);
typedef BOOL (WINAPI * SWAPBUFFERS_T) (HDC);

HINSTANCE gdi32;
CHOOSEPIXELFORMAT_T   ChoosePixelFormat_t;
DESCRIBEPIXELFORMAT_T DescribePixelFormat_t;
GETPIXELFORMAT_T      GetPixelFormat_t;
SETPIXELFORMAT_T      SetPixelFormat_t;
SWAPBUFFERS_T         SwapBuffers_t;

CRITICAL_SECTION CriticalSection;
int NextID;
typedef void (WINAPI * GLFWthreadfun)(void *);

struct THREADS {

	// ========= PLATFORM INDEPENDENT MANDATORY PART =========================

	// Pointer to previous and next threads in linked list
	THREADS   *Previous, *Next;

	// GLFW user side thread information
	int    ID;
	GLFWthreadfun Function;

	// ========= PLATFORM SPECIFIC PART ======================================

	// System side thread information
	HANDLE        Handle;
	DWORD         WinID;

}thrd;
*/

OpenGL::OpenGL(void)
{
}


OpenGL::~OpenGL(void)
{
}

bool OpenGL::Init( long _hwnd, long fake )
{
	hwnd = _hwnd;
	hDC = GetDC((HWND)hwnd);
	//Initialize GLEW using Fake Windowhandle
	//더 최신방법 else if(WGLEW_ARB_create_context && WGLEW_ARB_pixel_format) 이건 나중에 알아보자
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;//이건걍 닥치고 1하라고함msdn이
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(hDC,&pfd);

	if(nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(hDC,nPixelFormat,&pfd);

	if(!bResult) return false;

	HGLRC tempContext = wglCreateContext(hDC);//이걸먼저해야 wglCreateContextAttribsARB가 된단다 순서가 좀 이상하긴해도 이래야됨 http://www.opengl.org/wiki/Tutorial:_OpenGL_3.1_The_First_Triangle_(C%2B%2B/Win)
	wglMakeCurrent(hDC,tempContext);

	GLenum err;
	
	err = glewInit();
	if(GLEW_OK != err)
	{
		printf("GLEW is not initialized!");
		return false;
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	if(wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		printf("supported!!");
		m_hrc = wglCreateContextAttribsARB(hDC, 0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, m_hrc);
	}
	else
	{
		m_hrc = tempContext;
	}

	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	if(!m_hrc) return false;

	glClearColor (.1, .2, .0, 0.0);

	/*hDC = GetDC((HWND)hwnd);

	GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
	if(GetExtensionsStringEXT == NULL)
	{
		GetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	}*/

/*
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd,sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;//msdn에서도 1로 세팅하라고한다
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;*/
/*
	//GLFW 소스 뭐가뭔지모르니깐 일단 그대로 옮기기
	//이상한거
	ChoosePixelFormat_t   = (CHOOSEPIXELFORMAT_T)
		GetProcAddress( gdi32, "ChoosePixelFormat" );
	DescribePixelFormat_t = (DESCRIBEPIXELFORMAT_T)
		GetProcAddress( gdi32, "DescribePixelFormat" );
	GetPixelFormat_t      = (GETPIXELFORMAT_T)
		GetProcAddress( gdi32, "GetPixelFormat" );
	SetPixelFormat_t      = (SETPIXELFORMAT_T)
		GetProcAddress( gdi32, "SetPixelFormat" );
	SwapBuffers_t         = (SWAPBUFFERS_T)
		GetProcAddress( gdi32, "SwapBuffers" );

	if( ChoosePixelFormat   == NULL ||
		DescribePixelFormat == NULL ||
		GetPixelFormat      == NULL ||
		SetPixelFormat      == NULL ||
		SwapBuffers         == NULL )
	{
		FreeLibrary( gdi32 );
		gdi32 = NULL;
		return false;
	}
	//쓰레드
	InitializeCriticalSection(&CriticalSection);
	NextID = 0;
	thrd.ID = NextID++;
	thrd.Function = NULL;
	thrd.Handle = GetCurrentThread();
	thrd.WinID = GetCurrentThreadId();
	thrd.Previous = NULL;
	thrd.Next = NULL;
/ *

	//데스크탑모드 보니 쓸모없는기능같다
	DEVMODE dm;

	//get desktop display mode
	dm.dmSize = sizeof(DEVMODE);
	(void) EnumDisplaySettings(NULL,ENUM_REGISTRY_SETTINGS,&dm);
* /
	glViewport(0,0,800,600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,(float)800/(float)600,1.0,100.0);*/
	return true;
}

void OpenGL::Release()
{
	wglMakeCurrent(NULL,NULL);
	if(m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
//	ReleaseDC((HWND)hwnd,hDC);
/*
	if(GetCurrentThreadId() != thrd.WinID)
		return;*/
	
}

void OpenGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SwapBuffers(hDC);
}


#endif // USEOPENGL