#pragma once

#include "Sample.h"

class AnimateSample : public Sample
{
public:
	AnimateSample();
	virtual ~AnimateSample();
	
	virtual void Initialize();
	virtual void Reset();
	virtual void Release();

	virtual void Update(float dt);
	virtual void Render(ID2D1RenderTarget * target);

private:
	float _speed;

	float _frame;
	float _titleFrame;

	std::wstring _title;
};