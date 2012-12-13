#pragma once

#include "SampleViewer.h"

class Sample
{
public:
	Sample();
	virtual ~Sample();

	virtual void Initialize();
	virtual void Reset();
	virtual void Release();

	virtual void Update(float dt);
	virtual void Render(ID2D1RenderTarget * target);
};

