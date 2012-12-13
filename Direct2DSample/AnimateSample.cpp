#include "AnimateSample.h"

Particle::Particle(std::string name, D2D1_POINT_2F vec) : _name(name), _v(vec), _a(1.f)
{
	_p = D2D1::Point2F(SampleViewer::Instance()->GetAbsoluteX(), SampleViewer::Instance()->GetAbsoluteY());
}

bool Particle::Update(float dt)
{
	_a -= dt / 2;

	_p.x += _v.x * dt * 100;
	_p.y += _v.y * dt * 100;

	if (_a < 0)
		return true;

	return false;
}

void Particle::Render(ID2D1RenderTarget * target)
{
	D2D1_SIZE_U bmpSize = SampleViewer::Instance()->Bitmap(_name)->GetPixelSize();
	bmpSize = D2D1::SizeU(bmpSize.width / 1.5f, bmpSize.height / 1.5f);

	target->DrawBitmap(SampleViewer::Instance()->Bitmap(_name), 
		D2D1::RectF(_p.x - bmpSize.width / 2, _p.y - bmpSize.height / 2, _p.x + bmpSize.width / 2, _p.y + bmpSize.height / 2), _a);
}

AnimateSample::AnimateSample()
{
}

AnimateSample::~AnimateSample()
{
}

void AnimateSample::Initialize() 
{
	printf("[AnimateSample] Initialized\n");
	
	_title = L"AnimateSample";
	
	SampleViewer::Instance()->LoadBitmap("Anim", L"img/Anim.png");

	_name[0] = "GreenFlower";
	_name[1] = "OrangeFlower";
	_name[2] = "PurpleFlower";
	_name[3] = "YellowFlower";
	
	SampleViewer::Instance()->LoadBitmap(_name[0], L"img/flower_green.png");
	SampleViewer::Instance()->LoadBitmap(_name[1], L"img/flower_orange.png");
	SampleViewer::Instance()->LoadBitmap(_name[2], L"img/flower_purple.png");
	SampleViewer::Instance()->LoadBitmap(_name[3], L"img/flower_yellow.png");
}

void AnimateSample::Reset()
{
	_frame = 0;
	_titleFrame = _title.size();
	
	_rate = 0;
	for each (auto p in _particle)
		delete p;
	_particle.clear();
}

void AnimateSample::Release() 
{
	for each (auto p in _particle)
		delete p;
}

void AnimateSample::Update(float dt)
{
	_frame += dt * 40;
	if (_frame >= 25 * 3 - 4)
		_frame = 0;

	_titleFrame += dt * 10;
	if (_titleFrame >= _title.size() + 1)
		_titleFrame = 0;

	for each (auto p in _particle)
		p->Update(dt);

	std::list<Particle *>::iterator next;
	for (std::list<Particle *>::iterator i = _particle.begin(); i != _particle.end(); i = next)
	{
		bool result = (*i)->Update(dt);
		next = i;
		++next;

		if (result)
		{
			delete *i;
			_particle.remove(*i);
		}
	}
	
	_rate += dt;

	if (_rate > 0.025f)
	{
		if (SampleViewer::Instance()->IsLButtonPressing())
		{
			float angle = (float)rand() / RAND_MAX * M_PI * 2;
			D2D1_POINT_2F vec = D2D1::Point2F(cos(angle), sin(angle));

			int name = rand() % 4;
			_particle.push_back(new Particle(_name[name], vec));
		}

		_rate = 0;
	}
}

void AnimateSample::Render(ID2D1RenderTarget * target)
{
	D2D1_SIZE_U winSize = SampleViewer::Instance()->GetWinSize();
	
	D2D1_SIZE_U bmpSize = SampleViewer::Instance()->Bitmap("Anim")->GetPixelSize();
	D2D1_SIZE_U sprSize = D2D1::SizeU(80, 80);
	int col = bmpSize.width / sprSize.width;
	
	for each (auto p in _particle)
		p->Render(target);

	target->DrawBitmap(SampleViewer::Instance()->Bitmap("Anim"), D2D1::RectF(
		winSize.width / 2 - sprSize.width / 2, winSize.height / 2 - sprSize.height / 2, 
		winSize.width / 2 + sprSize.width / 2, winSize.height / 2 + sprSize.height / 2), 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF((int)_frame % col * sprSize.width, (int)_frame / col * sprSize.height, ((int)_frame % col + 1) * sprSize.width, ((int)_frame / col + 1) * sprSize.height));
	
	target->DrawText(_title.c_str(), (int)_titleFrame, SampleViewer::Instance()->Font("Title"), D2D1::RectF(0, 0, winSize.width, 0), SampleViewer::Instance()->Brush("White"));
}