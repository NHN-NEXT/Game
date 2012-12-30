#pragma once
#ifdef USEDIRECTX10

#include "graphic.h"

class	ID3D10Device;
class	IDXGISwapChain;
class	ID3D10RenderTargetView;
class	ID3D10Effect; 
class	ID3D10EffectTechnique;
class	ID3D10InputLayout;  
class	ID3D10Buffer;    
class DX :
	public Graphic
{
public:

	int       g_driverType;
	long hwnd;
	ID3D10Device*           g_pd3dDevice;
	IDXGISwapChain*         g_pSwapChain;
	ID3D10RenderTargetView* g_pRenderTargetView;
	ID3D10Effect*           g_pEffect;
	ID3D10EffectTechnique*  g_pTechnique;
	ID3D10InputLayout*      g_pVertexLayout;
	ID3D10Buffer*           g_pVertexBuffer;

	bool Init(long hwnd, long fake);
	void Render();
	void Release();

	DX();
	~DX(void);
};

#endif // USEDIRECTX10