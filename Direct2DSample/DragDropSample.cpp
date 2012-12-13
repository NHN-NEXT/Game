#include "DragDropSample.h"

DragDropSample::DragDropSample()
{
}

DragDropSample::~DragDropSample()
{
}

void DragDropSample::Initialize() 
{ 
	printf("[DragDropSample] Initialized\n");
	
	SampleViewer::Instance()->LoadBitmap("DragDrop", L"img/DragDrop.png");
}

void DragDropSample::Reset()
{
	_capture = false;

	D2D1_SIZE_U winSize = SampleViewer::Instance()->GetWinSize();
	_x = winSize.width / 2.f;
	_y = winSize.height / 2.f;

	_v = 0;
	_dir = 0;
	_vec = D2D1::Point2F(0, 0);
}

void DragDropSample::Release() 
{
}

void DragDropSample::Update(float dt)
{
	if (_capture)
	{
		//_x = SampleViewer::Instance()->GetAbsoluteX();
		//_y = SampleViewer::Instance()->GetAbsoluteY();

		_x += SampleViewer::Instance()->GetDeltaX();
		_y += SampleViewer::Instance()->GetDeltaY();
	}
	
	if (_dir)
	{
		float x = _x, y = _y;

		_x += _vec.x * _v * dt * 10;
		_y += _vec.y * _v * dt * 10;
		
		D2D1_SIZE_U winSize = SampleViewer::Instance()->GetWinSize();

		if (x < winSize.width / 2 && winSize.width / 2 <= _x || x > winSize.width / 2 &&  winSize.width / 2 >= _x ||
			y < winSize.height / 2 && winSize.height / 2 <= _y || y > winSize.width / 2 && winSize.height / 2 >= _y)
			_dir *= -1;
		
		D2D1_POINT_2F vec = D2D1::Point2F(winSize.width / 2 - _x, winSize.height / 2 - _y);
		float length = sqrtf(vec.x * vec.x + vec.y * vec.y);
		
		if (length < 0.1f && _v < 0.1f)
			_dir = 0;
		else
		{
			_v -= 0.1f * 30 * _v * dt;
			_v += _dir * length * dt * 10;
		}
		
		printf("[DragDropSample] Speed : %f\n", _v);
	}

	if (SampleViewer::Instance()->IsLButtonPress())
	{
		int x = SampleViewer::Instance()->GetAbsoluteX();
		int y = SampleViewer::Instance()->GetAbsoluteY();
		
		D2D1_SIZE_U bmpSize = SampleViewer::Instance()->Bitmap("DragDrop")->GetPixelSize();

		if (_x - bmpSize.width / 2 <= x && x <= _x + bmpSize.width / 2 &&
			_y - bmpSize.height / 2 <= y && y <= _y + bmpSize.height / 2)
			_capture = true, _dir = 0;
	}

	if (_capture && SampleViewer::Instance()->IsLButtonPressed())
	{
		_capture = false;

		D2D1_SIZE_U winSize = SampleViewer::Instance()->GetWinSize();
		_vec = D2D1::Point2F(winSize.width / 2 - _x, winSize.height / 2 - _y);
		float length = sqrtf(_vec.x * _vec.x + _vec.y * _vec.y);
		_vec = D2D1::Point2F(_vec.x / length, _vec.y / length);

		_v = 0;
		_dir = 1;
	}
}

void DragDropSample::Render(ID2D1RenderTarget * target)
{
	D2D1_SIZE_U winSize = SampleViewer::Instance()->GetWinSize();
	D2D1_SIZE_U bmpSize = SampleViewer::Instance()->Bitmap("DragDrop")->GetPixelSize();

	target->DrawBitmap(SampleViewer::Instance()->Bitmap("DragDrop"), D2D1::RectF(_x - bmpSize.width / 2, _y - bmpSize.height / 2, _x + bmpSize.width / 2, _y + bmpSize.height / 2));

	target->DrawLine(D2D1::Point2F(0, 0), 
		D2D1::Point2F(_x - bmpSize.width / 2, _y - bmpSize.height / 2), SampleViewer::Instance()->Brush("White"));
	target->DrawLine(D2D1::Point2F(winSize.width, 0), 
		D2D1::Point2F(_x + bmpSize.width / 2, _y - bmpSize.height / 2), SampleViewer::Instance()->Brush("White"));
	target->DrawLine(D2D1::Point2F(0, winSize.height), 
		D2D1::Point2F(_x - bmpSize.width / 2, _y + bmpSize.height / 2), SampleViewer::Instance()->Brush("White"));
	target->DrawLine(D2D1::Point2F(winSize.width, winSize.height), 
		D2D1::Point2F(_x + bmpSize.width / 2, _y + bmpSize.height / 2), SampleViewer::Instance()->Brush("White"));
	
	std::wstring title = L"DragDropSample";
	target->DrawText(title.c_str(), title.size(), SampleViewer::Instance()->Font("Title"), D2D1::RectF(0, 0, winSize.width, 0), SampleViewer::Instance()->Brush("White"));
}