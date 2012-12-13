#include "AnimateSample.h"

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
}

void AnimateSample::Reset()
{
	_speed = 1;
	
	_frame = 0;
	_titleFrame = _title.size();
}

void AnimateSample::Release() 
{
}

void AnimateSample::Update(float dt)
{
	_frame += dt * 40 * _speed;
	if (_frame >= 25 * 3 - 4)
		_frame = 0;

	_titleFrame += dt * 10 * _speed;
	if (_titleFrame >= _title.size() + 1)
		_titleFrame = 0;
}

void AnimateSample::Render(ID2D1RenderTarget * target)
{
	D2D1_SIZE_U winSize = SampleViewer::Instance()->GetWinSize();
	
	D2D1_SIZE_U bmpSize = SampleViewer::Instance()->Bitmap("Anim")->GetPixelSize();
	D2D1_SIZE_U sprSize = D2D1::SizeU(80, 80);
	int col = bmpSize.width / sprSize.width;

	target->DrawBitmap(SampleViewer::Instance()->Bitmap("Anim"), D2D1::RectF(
		winSize.width / 2 - sprSize.width / 2, winSize.height / 2 - sprSize.height / 2, 
		winSize.width / 2 + sprSize.width / 2, winSize.height / 2 + sprSize.height / 2), 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF((int)_frame % col * sprSize.width, (int)_frame / col * sprSize.height, ((int)_frame % col + 1) * sprSize.width, ((int)_frame / col + 1) * sprSize.height));
	
	target->DrawText(_title.c_str(), (int)_titleFrame, SampleViewer::Instance()->Font("Title"), D2D1::RectF(0, 0, winSize.width, 0), SampleViewer::Instance()->Brush("White"));
}