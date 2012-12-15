//#include <iostream>
#include <windows.h>
#include "MAIN.h"
using namespace std;

void main()
{
	MAIN main;
	main.Init();
	main.OpenWindow();
	main.Loop();
	main.Release();
}

