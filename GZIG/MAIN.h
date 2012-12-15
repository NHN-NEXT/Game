#pragma once
class OS;
class Graphic;
class MAIN
{
	
public:
	OS* os;
	Graphic* graphic;

	void Init();
	void OpenWindow();
	void Loop();
	void Release();
	MAIN(void);
	~MAIN(void);
};

