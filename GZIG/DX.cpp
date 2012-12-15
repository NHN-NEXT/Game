#include "Resource.h"
#ifdef USEDIRECTX10

#include "DX.h"
#include <d3d10.h>
#include <D3DX10.h>

#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "d3d10.lib")

DX::DX(void)
{
	
	
	g_driverType = D3D10_DRIVER_TYPE_NULL;
	g_pd3dDevice = NULL;
	g_pSwapChain = NULL;
	g_pRenderTargetView = NULL;
	g_pEffect = NULL;
	g_pTechnique = NULL;
	g_pVertexLayout = NULL;
	g_pVertexBuffer = NULL;
}


DX::~DX(void)
{
}

bool DX::Init(long _hwnd)
{
	hwnd = _hwnd;
	   HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( (HWND)hwnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;

    D3D10_DRIVER_TYPE driverTypes[] =
    {
        D3D10_DRIVER_TYPE_HARDWARE,
        D3D10_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )//�˸´� ����̹�Ÿ���� ã�� ��� ����̹�Ÿ���� ���� ������ �ٲ��ְ� �ٽýõ�
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D10CreateDeviceAndSwapChain( NULL, (D3D10_DRIVER_TYPE)g_driverType, NULL, createDeviceFlags,
                                            D3D10_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D10Texture2D* pBuffer;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBuffer, NULL, &g_pRenderTargetView );
    pBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    g_pd3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

    // Setup the viewport
    D3D10_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pd3dDevice->RSSetViewports( 1, &vp );

    // Create the effect
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

    hr = D3DX10CreateEffectFromFileW( L"Tutorial02.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0,
                                         g_pd3dDevice, NULL, NULL, &g_pEffect, NULL, NULL );
    if( FAILED( hr ) )
    {
        MessageBoxW( NULL,
                    L"The FX file cannot be located.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Obtain the technique
    g_pTechnique = g_pEffect->GetTechniqueByName( "Render" );

    // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof( layout ) / sizeof( layout[0] );

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    g_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	/*�׷��ϱ�.. D3D10_INPUT_ELEMENT_DESC�� ���̴��� �ƴ� �̰������� ���ؽ�������... 
	D3D10_PASS_DESC�� ���̴����� ���� Ư�� PASS��������...  �Ʒ� Create Input Layout 
	�Լ��� ���ؽ��� input stage�� ���� ���� ���ؽ��� ������ ����̽��� �˾ƾ��ϱ� 
	������ ȣ���ϴ°Ŵ�?*/
	/*�ƴ��� D3D10_INPUT_ELEMENT_DESC�� ù��° ����� SemanticNAme�� ���̴����� �����ϱ����� ����
	�ɷ� �̷����� D3D10_INPUT_ELEMENT_DESC�� ���̴��� ù��° �ܰ��� ���ؽ� ���̴��� ������
	���̾ƿ������� �� D3D10_INPUT_ELEMENT_DESC�� ���ؽ� ���̴��� INPUT���� ���� ���ؽ������ϱ�
	�װſ� D3D10_PASS_DESC�� ���ؽ��ʹ� ������ Ư��PASS�� �����̰�.. ����̽��� �ᱹ Ư�� �н���
	�� �н��� INPUT�� ���ؽ��� ���̾ƿ������� �˾ƾ� InputLayout�� �ᱹ ���� �� ������ �Ѵ� �ʿ������ǵ�*/
    hr = g_pd3dDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature,
                                          PassDesc.IAInputSignatureSize, &g_pVertexLayout );
    if( FAILED( hr ) )
        return hr;

    // Set the input layout
    g_pd3dDevice->IASetInputLayout( g_pVertexLayout );

    // Create vertex buffer
    D3DXVECTOR3 vertices[] =
    {
        D3DXVECTOR3( 0.0f, 0.5f, 0.5f ),
        D3DXVECTOR3( 0.0f, -0.0f, 0.5f ),
        D3DXVECTOR3( -0.5f, -0.5f, 0.5f ),
    };
    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( D3DXVECTOR3 ) * 3;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( D3DXVECTOR3 );
    UINT offset = 0;
    g_pd3dDevice->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Set primitive topology
    g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    return S_OK;
}

void DX::Release()
{
	if( g_pd3dDevice ) g_pd3dDevice->ClearState();

	if( g_pVertexBuffer ) g_pVertexBuffer->Release();
	if( g_pVertexLayout ) g_pVertexLayout->Release();
	if( g_pEffect ) g_pEffect->Release();
	if( g_pRenderTargetView ) g_pRenderTargetView->Release();
	if( g_pSwapChain ) g_pSwapChain->Release();
	if( g_pd3dDevice ) g_pd3dDevice->Release();
}

void DX::Render()
{
	// Clear the back buffer 
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	g_pd3dDevice->ClearRenderTargetView( g_pRenderTargetView, ClearColor );

	// Render a triangle
	D3D10_TECHNIQUE_DESC techDesc;
	g_pTechnique->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		g_pTechnique->GetPassByIndex( p )->Apply( 0 );
		g_pd3dDevice->Draw( 3, 0 );
	}

	// Present the information rendered to the back buffer to the front buffer (the screen)
	g_pSwapChain->Present( 0, 0 );
}

#endif // USEDIRECTX10