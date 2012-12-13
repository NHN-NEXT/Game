#include <Windows.h>

#include "SampleViewer.h"
#include "Sample.h"

#include "AnimateSample.h"
#include "DragDropSample.h"

SampleViewer * SampleViewer::__sampleViewer = nullptr;

SampleViewer::SampleViewer() : 
	_factory(nullptr), _target(nullptr),
	_write(nullptr),
	_input(nullptr), _mouse(nullptr), _keyboard(nullptr),
	_wic(nullptr),
	_index(-1), _indexPrev(0)
{
}

SampleViewer::~SampleViewer()
{
}

SampleViewer * SampleViewer::Instance()
{
	if (!__sampleViewer)
		__sampleViewer = new SampleViewer();

	return __sampleViewer;
}

void SampleViewer::InitD2D1(HWND hWnd)
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_factory);

	_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, _winSize), &_target);
}

void SampleViewer::InitDWrite()
{
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&_write)
    );
}

void SampleViewer::initDInput(HINSTANCE hInstance)
{
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&_input, NULL);
	
	_input->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	_mouse->SetCooperativeLevel(_target->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	_mouse->SetDataFormat(&c_dfDIMouse);
	_mouse->Acquire();
	_mouse->GetDeviceState(sizeof(_mouseState), &_mouseState);
	
	_input->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	_keyboard->SetCooperativeLevel(_target->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	_keyboard->SetDataFormat(&c_dfDIKeyboard);
	_keyboard->Acquire();
	_keyboard->GetDeviceState(sizeof(_keyState), (void *)_keyState);
}

void SampleViewer::InitWIC()
{
	CoInitialize(0);
	CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_wic));
}

void SampleViewer::InitSamples()
{	
	_samples.push_back(new DragDropSample);
	_samples.push_back(new AnimateSample);

	for each (auto i in _samples)
		i->Initialize();
	SetSampleByIndex(0);
}

void SampleViewer::SetSampleByIndex(int index)
{
	_index = index;
	
	if (_index >= 0)
		_samples[_index]->Reset();
}

void SampleViewer::SetSampleByDelta(int dir)
{
	if (_indexPrev || dir != -1 && dir != 1)
		return;

	SetSampleByIndex(_index + dir);

	_indexPrev = -dir;
	_indexChange = 0;
}

void SampleViewer::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	Release();

	RECT rc;
	GetClientRect(hWnd, &rc);
	_winSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	InitD2D1(hWnd);
	InitDWrite();
	initDInput(hInstance);
	InitWIC();
	InitSamples();

	//Loading
	LoadBitmap("Panel", L"img/Panel.png");

	LoadBrush("White", D2D1::ColorF(D2D1::ColorF::White));

	LoadFont("Title", L"Ariel", 30);
	Font("Title")->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	
	D2D1_SIZE_U panelSize; 
	panelSize = Bitmap("Panel")->GetPixelSize();
	_panelRect = D2D1::RectF(
		_winSize.width / 2 - panelSize.width / 2, _winSize.height - panelSize.height, 
		_winSize.width / 2 + panelSize.width / 2, _winSize.height);
}

void SampleViewer::Release()
{
	for each (auto i in _samples)
	{
		i->Release();
		delete i;
	}

	for each (auto i in _bitmap)
		i.second->Release();

	for each (auto i in _brush)
		i.second->Release();
	
	for each (auto i in _font)
		i.second->Release();

	if (_wic)
	{
		SAFE_RELEASE(_wic);
		CoUninitialize();
	}

	if (_mouse)
	{
		_mouse->Unacquire();
		SAFE_RELEASE(_mouse);
	}

	if (_keyboard)
	{
		_keyboard->Unacquire();
		SAFE_RELEASE(_keyboard);
	}

	SAFE_RELEASE(_input);

	SAFE_RELEASE(_write);

	SAFE_RELEASE(_target);
	SAFE_RELEASE(_factory);
}

void SampleViewer::Update(float dt)
{
	UpdateInputState();
	
	if (_indexPrev)
	{
		_indexChange += dt;
		if (_indexChange >= 1)
			_indexPrev = 0;

		return;
	}

	if (IsLButtonPress())
	{
		int x = GetAbsoluteX(), y = GetAbsoluteY();

		if (_panelRect.left <= x && x <= _panelRect.right &&
			_panelRect.top <= y && y <= _panelRect.bottom)
		{
			if (_panelRect.left <= x && x <= _panelRect.left + 45)
			{
				printf("PREVIOUS\n");

				if (_index > 0)
					SetSampleByDelta(-1);
			}
			else if (_panelRect.right - 45 <= x && x <= _panelRect.right)
			{
				printf("NEXT\n");

				if (_index < _samples.size() - 1)
					SetSampleByDelta(1);
			}
			else
			{
				printf("REFRESH\n");

				if (_index >= 0)
					_samples[_index]->Reset();
			}
		}
	}

	if (_index >= 0)
		_samples[_index]->Update(dt);
}

void SampleViewer::Render()
{
	_target->BeginDraw();

	_target->Clear(D2D1::ColorF(0.1f, 0.1f, 0.1f));

	if (_indexPrev)
	{
		D2D1_SIZE_U winSize = GetWinSize();
		D2D1_MATRIX_3X2_F indexTransform, prevTransform;
	
		float change = sin(_indexChange * M_PI_2);

		if (_indexPrev < 0)
		{
			indexTransform = D2D1::Matrix3x2F::Translation(winSize.width - change * winSize.width, 0);
			prevTransform = D2D1::Matrix3x2F::Translation(-change * winSize.width, 0);
		}
		else
		{
			indexTransform = D2D1::Matrix3x2F::Translation(change * winSize.width - winSize.width, 0);
			prevTransform = D2D1::Matrix3x2F::Translation(change * winSize.width, 0);
		}
		
		_target->SetTransform(indexTransform);
		_samples[_index]->Render(_target);
		
		_target->SetTransform(prevTransform);
		_samples[_index + _indexPrev]->Render(_target);

		_target->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	else if (_index >= 0)
		_samples[_index]->Render(_target);
	
	_target->DrawBitmap(Bitmap("Panel"), _panelRect);
	
	_target->EndDraw();
}

ID2D1Bitmap * SampleViewer::Bitmap(std::string name)
{
	return _bitmap[name];
}

ID2D1Brush * SampleViewer::Brush(std::string name)
{
	return _brush[name];
}

IDWriteTextFormat * SampleViewer::Font(std::string name)
{
	return _font[name];
}

void SampleViewer::LoadBitmap(std::string name, std::wstring path)
{
	IWICBitmapDecoder *decoder = 0;
	_wic->CreateDecoderFromFilename(path.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	
	IWICBitmapFrameDecode *frameDecode = 0;
	decoder->GetFrame(0, &frameDecode);
	
	IWICFormatConverter *converter = 0;
	_wic->CreateFormatConverter(&converter);
	converter->Initialize(frameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 0, 0, WICBitmapPaletteTypeCustom);
	
	ID2D1Bitmap * bitmap;
	_target->CreateBitmapFromWicBitmap(converter, 0, &bitmap);

	_bitmap[name] = bitmap;
	
	decoder->Release();
	frameDecode->Release();
	converter->Release();

	printf("[%s] Loaded at ", name.c_str());
	wprintf(L"%s\n", path.c_str());
}

void SampleViewer::LoadBrush(std::string name, D2D1_COLOR_F color)
{
	ID2D1SolidColorBrush * brush;
	_target->CreateSolidColorBrush(color, &brush);
	_brush[name] = brush;
}

void SampleViewer::LoadFont(std::string name, std::wstring font, float size)
{
	IDWriteTextFormat * format;

	_write->CreateTextFormat(
		font.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        size,
        L"en-us",
        &format
	);

	_font[name] = format;
}

void SampleViewer::UpdateInputState()
{
	HRESULT hr;

	_mouseStatePrev = _mouseState;
	hr = _mouse->GetDeviceState(sizeof(_mouseState), &_mouseState);

	if (FAILED(hr))
		do hr = _mouse->Acquire(); while (hr == DIERR_INPUTLOST);

	for (int i = 0; i < 256; ++i)
		_keyStatePrev[i] = _keyState[i];
	hr = _keyboard->GetDeviceState(sizeof(_keyState), (void *)_keyState);
	
	if (FAILED(hr))
		do hr = _keyboard->Acquire(); while (hr == DIERR_INPUTLOST);
}

D2D1_SIZE_U SampleViewer::GetWinSize()
{
	return _winSize;
}

bool SampleViewer::IsKeyPress(DWORD dik)
{
	if (!(_keyStatePrev[dik] & 0x80) && (_keyState[dik] & 0x80))
		return true;
	return false;
}

bool SampleViewer::IsKeyPressing(DWORD dik)
{
	if ((_keyStatePrev[dik] & 0x80) && (_keyState[dik] & 0x80))
		return true;
	return false;
}

bool SampleViewer::IsKeyPressed(DWORD dik)
{
	if ((_keyStatePrev[dik] & 0x80) && !(_keyState[dik] & 0x80))
		return true;
	return false;
}

bool SampleViewer::IsLButtonPress()
{
	if (!_mouseStatePrev.rgbButtons[0] && _mouseState.rgbButtons[0])
		return true;
	return false;
}

bool SampleViewer::IsLButtonPressing()
{
	if (_mouseStatePrev.rgbButtons[0] && _mouseState.rgbButtons[0])
		return true;
	return false;
}

bool SampleViewer::IsLButtonPressed()
{
	if (_mouseStatePrev.rgbButtons[0] && !_mouseState.rgbButtons[0])
		return true;
	return false;
}

bool SampleViewer::IsRButtonPress()
{
	if (!_mouseStatePrev.rgbButtons[1] && _mouseState.rgbButtons[1])
		return true;
	return false;
}

bool SampleViewer::IsRButtonPressing()
{
	if (_mouseStatePrev.rgbButtons[1] && _mouseState.rgbButtons[1])
		return true;
	return false;
}

bool SampleViewer::IsRButtonPressed()
{
	if (_mouseStatePrev.rgbButtons[1] && !_mouseState.rgbButtons[1])
		return true;
	return false;
}

int SampleViewer::GetAbsoluteX()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(_target->GetHwnd(), &p);
	return p.x;
}

int SampleViewer::GetAbsoluteY()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(_target->GetHwnd(), &p);
	return p.y;
}

int SampleViewer::GetDeltaX()
{
	return _mouseState.lX;
}

int SampleViewer::GetDeltaY()
{
	return _mouseState.lY;
}

int SampleViewer::GetDeltaZ()
{
	return _mouseState.lZ;
}