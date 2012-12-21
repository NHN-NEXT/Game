#include "Resource.h"
#include <windows.h>
#include <mmsystem.h>
#ifdef USEOPENGL
//#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"opengl32.lib")

#include "OpenGL.h"

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

OpenGL::OpenGL(void)
{
}


OpenGL::~OpenGL(void)
{
}

bool OpenGL::Init( long hwnd )
{
	//GLFW ㅅ스 크대로 옮기기
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

	return false;
}

void OpenGL::Release()
{

}

void OpenGL::Render()
{

}


#endif // USEOPENGL