#include "Resource.h"
#include <windows.h>
#include <mmsystem.h>
#ifdef USEOPENGL
//#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"opengl32.lib")
#include <GL/GL.h>
#include <GL/GLU.h>
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
/*

	//데스크탑모드 보니 쓸모없는기능같다
	DEVMODE dm;

	//get desktop display mode
	dm.dmSize = sizeof(DEVMODE);
	(void) EnumDisplaySettings(NULL,ENUM_REGISTRY_SETTINGS,&dm);
*/


	return true;
}

void OpenGL::Release()
{
	if(GetCurrentThreadId() != thrd.WinID)
		return;
	
}

void OpenGL::Render()
{
	glBegin(GL_QUADS);
	{
		glColor3f(0.82, 0.41, 0.12);//this the color with which complete cube is drawn. 
		glVertex3f(0,0 ,0 );
		glVertex3f(5, 0, 0);
		glVertex3f(5, 5, 0);
		glVertex3f(0, 5, 0);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//face in yz plane
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 5);
		glVertex3f(0, 5, 5);
		glVertex3f(0, 5, 0);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//face in zx plance
		glColor3f(1, 1, 0);
		glVertex3f(0, 0, 0  );
		glVertex3f(0, 0, 5);
		glVertex3f(5, 0, 5);
		glVertex3f(5, 0, 0);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//|| to xy plane.
		glColor3f(1, 0, 1);
		glVertex3f(0, 0, 5);
		glVertex3f(5, 0, 5);
		glVertex3f(5, 5, 5);
		glVertex3f(0, 5, 5);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//|| to yz plane
		glColor3f(1, .5, .5);
		glVertex3f(5,0 ,0 );
		glVertex3f(5, 0, 5);
		glVertex3f(5, 5, 5);
		glVertex3f(5, 5, 0);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//|| to zx plane
		glColor3f(0.58, 0, 0.82);
		glVertex3f(0, 5, 0  );
		glVertex3f(0, 5, 5);
		glVertex3f(5, 5, 5);
		glVertex3f(5, 5, 0);
	}
	glEnd();
}


#endif // USEOPENGL