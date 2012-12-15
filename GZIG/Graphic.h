#pragma once
class Graphic
{
public:
	virtual bool Init(long hwnd) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	Graphic(void);
	virtual ~Graphic(void);
};