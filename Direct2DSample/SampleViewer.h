#pragma once

#define _USE_MATH_DEFINES
#define DIRECTINPUT_VERSION 0x0800

#include <vector>
#include <map>
#include <string>

#include <d2d1.h>
#include <d2d1helper.h>

#include <dwrite.h>

#include <dinput.h>

#include <wincodec.h>

#undef LoadBitmap

#define SAFE_RELEASE(t) if (t) t->Release(); t = nullptr

class Sample;

class SampleViewer
{
public:
	~SampleViewer();

	void Initialize(HINSTANCE hInstance, HWND hWnd);
	void Release();

	void Update(float dt);
	void Render();

	ID2D1Bitmap * Bitmap(std::string name);
	ID2D1Brush * Brush(std::string name);
	IDWriteTextFormat * Font(std::string name);

	void LoadBitmap(std::string name, std::wstring path);
	void LoadBrush(std::string name, D2D1_COLOR_F color);
	void LoadFont(std::string name, std::wstring font, float size);

	D2D1_SIZE_U GetWinSize();
	
	bool IsKeyPress(DWORD dik);
	bool IsKeyPressing(DWORD dik);
	bool IsKeyPressed(DWORD dik);

	bool IsLButtonPress();
	bool IsLButtonPressing();
	bool IsLButtonPressed();

	bool IsRButtonPress();
	bool IsRButtonPressing();
	bool IsRButtonPressed();

	int GetAbsoluteX();
	int GetAbsoluteY();

	int GetDeltaX();
	int GetDeltaY();
	int GetDeltaZ();

	static SampleViewer * Instance();

private:
	SampleViewer();

	void InitD2D1(HWND hWnd);
	void InitDWrite();
	void initDInput(HINSTANCE hInstance);
	void InitWIC();

	void InitSamples();
	void SetSampleByIndex(int index);
	void SetSampleByDelta(int dir);

	void UpdateInputState();

	ID2D1Factory * _factory;
	ID2D1HwndRenderTarget * _target;

	IDWriteFactory * _write;
	
	IDirectInput * _input;
	IDirectInputDevice * _mouse, * _keyboard;
	DIMOUSESTATE _mouseState, _mouseStatePrev;
	byte _keyState[256], _keyStatePrev[256];

	IWICImagingFactory * _wic;

	std::vector<Sample *> _samples;
	int _index, _indexPrev;
	float _indexChange;

	std::map<std::string, IDWriteTextFormat *> _font;
	std::map<std::string, ID2D1Bitmap *> _bitmap;
	std::map<std::string, ID2D1Brush *> _brush;

	D2D1_SIZE_U _winSize;
	D2D1_RECT_F _panelRect;

	static SampleViewer * __sampleViewer;
};
