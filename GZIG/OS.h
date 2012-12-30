#pragma once
class OS
{
public:
	virtual void PopUpWindow() = 0;
	virtual void Init() = 0;
	virtual void Release() = 0;
	
	
	virtual int Loop() =0;
	virtual long GetHWNDinLongType() = 0;
	virtual long GetFAKEinLongType() = 0;
	OS(void);
	virtual ~OS(void);
};

