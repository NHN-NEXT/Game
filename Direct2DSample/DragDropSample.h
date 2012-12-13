#pragma once

#include "Sample.h"

class DragDropSample : public Sample
{
public:
	DragDropSample();
	virtual ~DragDropSample();
	
	virtual void Initialize();
	virtual void Reset();
	virtual void Release();

	virtual void Update(float dt);
	virtual void Render(ID2D1RenderTarget * target);

private:
	bool _capture;
	float _x, _y;
	float _v;
	int _dir;
	D2D1_POINT_2F _vec;
};

