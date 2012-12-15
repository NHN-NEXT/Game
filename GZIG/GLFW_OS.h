#pragma once
#include "os.h"
class GLFW_OS :
	public OS
{
public:
	void Init();
	void PopUpWindow();
	void Safe_Exit(int return_code);
	int Loop();
	long GetHWNDinLongType();
	GLFW_OS(void);
	~GLFW_OS(void);
};

