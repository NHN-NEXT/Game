#pragma once

#include "Sample.h"

class Particle
{
public:
	Particle(std::string name, D2D1_POINT_2F vec);

	bool Update(float dt);
	void Render(ID2D1RenderTarget * target);

private:
	std::string _name;
	D2D1_POINT_2F _p, _v;
	float _a;
};

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
	std::string _name[4];

	float _frame;

	float _titleFrame;
	std::wstring _title;

	float _rate;
	std::list<Particle *> _particle;
};