/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

int Graphics::ScreenWidth = 640;
int Graphics::ScreenHeight = 400;

Graphics::Graphics( HWNDKey& key )
{
	assert( key.hWnd != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		(LPVOID*)&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView( 
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1,&vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc,&pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
	}
	

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
	}
	

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
	}

	
	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
	}


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
	}

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	pSysBuffer = reinterpret_cast<Color*>( 
		_aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight,16u ) );
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if( pSysBuffer )
	{
		_aligned_free( pSysBuffer );
		pSysBuffer = nullptr;
	}
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData );
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	const size_t srcPitch = Graphics::ScreenWidth;
	const size_t rowBytes = srcPitch * sizeof( Color );
	// perform the copy line-by-line
	for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
	{
		memcpy( &pDst[ y * dstPitch ],&pSysBuffer[y * srcPitch],rowBytes );
	}
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
		}
	}
}

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}


//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}


/**************************************/
/*                                    */
/*  Non-default part of the class     */
/*  starts here                       */
/*                                    */
/**************************************/

void Graphics::drawDisc( int cx,int cy,int r,Color color )
{
    int rSquared = r * r;
    int xPivot = (int)(r * 0.707107f + 0.5f);
    for ( int x = 0; x <= xPivot; x++ )
    {
        int y = (int)(sqrt( (float)(rSquared - x*x) ) + 0.5f);
        int yHi = cy - y;
        int yLo = cy + y;
        for ( int ix = cx - x; ix <= cx + x; ix++ ) PutPixel( ix,yHi,color );
        for ( int ix = cx - x; ix <= cx + x; ix++ ) PutPixel( ix,yLo,color );
        yHi = cy - x;
        yLo = cy + x;
        for ( int ix = cx - y; ix <= cx + y; ix++ ) PutPixel( ix,yHi,color );
        for ( int ix = cx - y; ix <= cx + y; ix++ ) PutPixel( ix,yLo,color );
    }
}

void Graphics::drawLine( int x1,int y1,int x2,int y2,Color color )
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    if ( dy == 0 && dx == 0 )
    {
        PutPixel( x1,y1,color );
    } else if ( abs( dy ) > abs( dx ) )
    {
        if ( dy < 0 )
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        float m = (float)dx / (float)dy;
        float b = x1 - m*y1;
        for ( int y = y1; y <= y2; y = y + 1 )
        {
            int x = (int)(m*y + b + 0.5f);
            PutPixel( x,y,color );
        }
    } else
    {
        if ( dx < 0 )
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        float m = (float)dy / (float)dx;
        float b = y1 - m*x1;
        for ( int x = x1; x <= x2; x = x + 1 )
        {
            int y = (int)(m*x + b + 0.5f);
            PutPixel( x,y,color );
        }
    }
}

void Graphics::drawHorLine( int x1,int y,int x2,Color color )
{
    for ( int x = x1; x <= x2; x++ ) PutPixel( x,y,color );
}

void Graphics::drawVerLine( int x,int y1,int y2,Color color )
{
    for ( int y = y1; y <= y2; y++ ) PutPixel( x,y,color );
}

// can be slow since only meant for debugging
void Graphics::drawMathLine( int x,int y,float m,int length,Color color )
{
    int l = length / 2;
    int b = (int)(y - m * x);
    int x1;
    int x2;
    int y1;
    int y2;
    if ( abs( m ) < 1.0f )
    {
        x1 = x - l;
        x2 = x + l;
        y1 = (int)(m * x1 + b);
        y2 = (int)(m * x2 + b);
    } else
    {
        y1 = y - l;
        y2 = y + l;
        x1 = (int)((y1 - b) / m);
        x2 = (int)((y2 - b) / m);
    }
    //draw the Line ( x1,y1,x2,y2 ) :
    int dx = x2 - x1;
    int dy = y2 - y1;
    if ( dy == 0 && dx == 0 )
    {
        if ( x >= 0 && x < ScreenWidth && y >= 0 && y < ScreenHeight )
            PutPixel( x1,y1,color );
    } else if ( abs( dy ) > abs( dx ) )
    {
        if ( dy < 0 )
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        float m = (float)dx / (float)dy;
        float b = x1 - m*y1;
        for ( int y = y1; y <= y2; y = y + 1 )
        {
            int x = (int)(m*y + b + 0.5f);
            if ( x >= 0 && x < ScreenWidth && y >= 0 && y < ScreenHeight )
                PutPixel( x,y,color );
        }
    } else
    {
        if ( dx < 0 )
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        float m = (float)dy / (float)dx;
        float b = y1 - m*x1;
        for ( int x = x1; x <= x2; x = x + 1 )
        {
            int y = (int)(m*x + b + 0.5f);
            if ( x >= 0 && x < ScreenWidth && y >= 0 && y < ScreenHeight )
                PutPixel( x,y,color );
        }
    }
}

void Graphics::drawCircle( int centerX,int centerY,int radius,Color color )
{
    int rSquared = radius*radius;
    int xPivot = (int)(radius * 0.707107f + 0.5f);
    for ( int x = 0; x <= xPivot; x++ )
    {
        int y = (int)(sqrt( (float)(rSquared - x*x) ) + 0.5f);
        PutPixel( centerX + x,centerY + y,color );
        PutPixel( centerX - x,centerY + y,color );
        PutPixel( centerX + x,centerY - y,color );
        PutPixel( centerX - x,centerY - y,color );
        PutPixel( centerX + y,centerY + x,color );
        PutPixel( centerX - y,centerY + x,color );
        PutPixel( centerX + y,centerY - x,color );
        PutPixel( centerX - y,centerY - x,color );
    }
}

template <class V> inline void swap( V &a,V &b )
{
    V temp = a;
    a = b;
    b = temp;
}

void Graphics::drawBox( const Rect& coords,Color color )
{
    drawBox( coords.x1,coords.y1,coords.x2,coords.y2,color );
}

void Graphics::drawBox( int x1,int y1,int x2,int y2,Color color )
{
    for ( int x = x1; x <= x2; x++ )
    {
        PutPixel( x,y1,color );
        PutPixel( x,y2,color );
    }
    for ( int y = y1; y <= y2; y++ )
    {
        PutPixel( x1,y,color );
        PutPixel( x2,y,color );
    }
}

void Graphics::drawBoxClipped( Rect coords,Rect clippedArea,Color color )
{
    int x1;
    int y1;
    int x2;
    int y2;
    if ( coords.x1 >= clippedArea.x1 ) x1 = coords.x1;
    else x1 = clippedArea.x1;
    if ( x1 > clippedArea.x2 ) x1 = clippedArea.x2;
    if ( coords.x2 <= clippedArea.x2 ) x2 = coords.x2;
    else x2 = clippedArea.x2;
    if ( x2 < clippedArea.x1 ) x2 = clippedArea.x1;
    if ( coords.y1 >= clippedArea.y1 ) y1 = coords.y1;
    else y1 = clippedArea.y1;
    if ( y1 > clippedArea.y2 ) y1 = clippedArea.y2;
    if ( coords.y2 <= clippedArea.y2 ) y2 = coords.y2;
    else y2 = clippedArea.y2;
    if ( y2 < clippedArea.y1 ) y2 = clippedArea.y1;
    if ( coords.y1 == y1 ) drawHorLine( x1,y1,x2,color );
    if ( coords.y2 == y2 ) drawHorLine( x1,y2,x2,color );
    if ( coords.x1 == x1 ) drawVerLine( x1,y1,y2,color );
    if ( coords.x2 == x2 ) drawVerLine( x2,y1,y2,color );
}

void Graphics::drawBlock( Rect coords,Color color )
{
    drawBlock( coords.x1,coords.y1,coords.x2,coords.y2,color );
}

void Graphics::drawBlock( int x1,int y1,int x2,int y2,Color color )
{
    for ( int y = y1; y <= y2; y++ )
        for ( int x = x1; x <= x2; x++ )
            PutPixel( x,y,color );
}

void Graphics::printXY( int x,int y,const char *s )
{
    if ( s == nullptr ) return;
    int slen = (int)strlen( s );
    int height = font->height();
    if ( !font->isBitmap() )
    {
        int width = font->width();
        int stringWidth = width * slen;

        assert( (x + stringWidth) < ScreenWidth );
        assert( (y + height) < ScreenHeight );

        Color *s1 = (Color *)pSysBuffer;
        s1 += x + ScreenWidth * y;
        for ( int iChar = 0; iChar < slen; iChar++ )
        {
            Color    *s2 = s1;
            char *iData = font->getCharData( s[iChar] );
            for ( int j = 0; j < height; j++ )
            {
                int iByte;
                for ( int i = 0; i < width; i++ )
                {
                    iByte = i % 8;
                    if ( (iByte == 0) && (i > 0) ) iData++;
                    if ( ((*iData) & (1 << (7 - iByte))) != 0 )
                        *s2 = textColor;
                    s2++;
                }
                if ( iByte > 0 ) iData++;
                s2 += ScreenWidth - width;
            }
            s1 += width;
        }
    } else {
        int startX = x;
        for ( int i = 0; i < slen; i++ )
        {
            paintBMPClearType( startX,y,*((Sprite *)(font->getBmpData( s[i] ))),0 );
            startX += ((Sprite *)(font->getBmpData( s[i] )))->getWidth();
        }
    }
}

void Graphics::printXY( int x,int y,const char *s,int opacity )
{
    if ( s == nullptr ) return;
    int slen = (int)strlen( s );
    int height = font->height();
    if ( font->isBitmap() == false )
    {
        int width = font->width();
        int stringWidth = width * slen;
        int destNextLine = ScreenWidth - width;

        assert( (x + stringWidth) < ScreenWidth );
        assert( (y + height) < ScreenHeight );

        Color *s1 = (Color *)pSysBuffer;
        s1 += x + ScreenWidth * y;
        for ( int iChar = 0; iChar < slen; iChar++ )
        {
            Color    *s2 = s1;
            char *iData = font->getCharData( s[iChar] );
            for ( int j = 0; j < height; j++ )
            {
                int iByte;
                for ( int i = 0; i < width; i++ )
                {
                    iByte = i % 8;
                    if ( (iByte == 0) && (i > 0) ) iData++;
                    if ( ((*iData) & (1 << (7 - iByte))) != 0 )
                        *s2 = textColor;
                    s2++;
                }
                if ( iByte > 0 ) iData++;
                s2 += destNextLine;
            }
            s1 += width;
        }
    } else {
        int startX = x;
        for ( int i = 0; i < slen; i++ )
        {
            paintBMPClearType( startX,y,*((Sprite *)(font->getBmpData( s[i] ))),0,opacity );
            startX += ((Sprite *)(font->getBmpData( s[i] )))->getWidth();
        }
    }
}

int Graphics::getStrLen( const char *s,Font *font )
{
    if ( s == nullptr ) return 0;
    int width = font->width();
    int slen = (int)strlen( s );
    if ( font->isBitmap() == false ) return slen * width;
    else {
        width = 0;
        for ( int i = 0; i < slen; i++ )
            width += ((Sprite *)font->getBmpData( s[i] ))->getWidth();
        return width;
    }
}

void Graphics::printXY( int x,int y,const char *s,int opacity,Font& font )
{
    Font *oldFont = getFont();
    setFont( &font );
    printXY( x,y,s,opacity );
    setFont( oldFont );
}

void Graphics::printXYSolid( int x,int y,int xSpacer,char *s,Color color /*, HRESULT *hres*/ )
{
    if ( s == nullptr ) return;
    int width = font->width();
    int height = font->height();
    int slen = (int)strlen( s );
    int stringWidth = (width + xSpacer) * slen;

    assert( (x + stringWidth) < ScreenWidth );
    assert( (y + height)     < ScreenHeight );

    int i = x;
    Color *s1 = (Color *)pSysBuffer;
    s1 += x + ScreenWidth * y;
    for ( int iChar = 0; iChar < slen; iChar++ )
    {
        unsigned char c = s[iChar];
        char *charMask = font->getCharData( c );
        Color    *s2 = s1;
        for ( int py = 0; py < height; py++ )
        {
            unsigned char d = charMask[py];
            for ( int px = 0; px < width; px++ )
            {
                if ( d & (1 << (7 - px)) ) *s2 = Color( 255,255,255 );
                else                     *s2 = color;
                s2++;
            }
            s2 += ScreenWidth - width;
        }
        s1 += width + xSpacer;
    }
}

void Graphics::paintSprite( int x,int y,const Sprite &sprite )
{
    if ( !sprite.isImagePresent() ) return;
    if ( (x >= ScreenWidth) || (y >= ScreenHeight) ) return;
    int xStart,yStart;
    int xEnd,yEnd;
    int xOffset,yOffset;
    if ( x < 0 ) { xStart = 0; xOffset = -x; } else { xStart = x; xOffset = 0; }
    if ( y < 0 ) { yStart = 0; yOffset = -y; } else { yStart = y; yOffset = 0; }
    xEnd = x + sprite.getWidth();
    yEnd = y + sprite.getHeight();
    if ( (xEnd <= 0) || (yEnd <= 0) ) return;
    if ( xEnd > ScreenWidth ) xEnd = ScreenWidth;
    if ( yEnd > ScreenHeight ) yEnd = ScreenHeight;
    Color *data = sprite.getPixelData() + yOffset * sprite.getWidth() + xOffset;
    Color *s = ((Color*)pSysBuffer) + ScreenWidth * yStart + xStart;
    int sNextLine = ScreenWidth - (xEnd - xStart);
    int dataNextLine = sprite.getWidth() - (xEnd - xStart);
    for ( int j = yStart; j < yEnd; j++ )
    {
        for ( int i = xStart; i < xEnd; i++ ) { *s++ = *data++; }
        data += dataNextLine;
        s += sNextLine;
    }
}

void Graphics::paintSpriteSection( int x,int y,Rect area,const Sprite &sprite )
{
    assert( area.x1 >= 0 );
    assert( area.y1 >= 0 );
    if ( !sprite.isImagePresent() ) return;
    if ( (x >= ScreenWidth) || (y >= ScreenHeight) ) return;
    int xOffset = 0;
    int yOffset = 0;
    if ( x < 0 ) { xOffset = -x; x = 0; }
    if ( y < 0 ) { yOffset = -y; y = 0; }
    int sourceWidth = area.x2 - area.x1 + 1;
    int sourceHeight = area.y2 - area.y1 + 1;
    int drawWidth = sourceWidth - xOffset;
    int drawHeight = sourceHeight - yOffset;
    if ( x + drawWidth  > ScreenWidth ) drawWidth = ScreenWidth - x;
    if ( y + drawHeight > ScreenHeight ) drawHeight = ScreenHeight - y;
    if ( area.x1 + xOffset + drawWidth  > sprite.getWidth() )
        drawWidth  = sprite.getWidth()  - xOffset - area.x1;
    if ( area.y1 + yOffset + drawHeight > sprite.getHeight() )
        drawHeight = sprite.getHeight() - yOffset - area.y1;
    Color *src = sprite.getPixelData() + area.x1 + xOffset
        + (area.y1 + yOffset) * sprite.getWidth();
    Color *dst = ((Color *)pSysBuffer) + ScreenWidth * y + x;
    int srcNextLine = sprite.getWidth() - drawWidth;
    int dstNextLine = ScreenWidth - drawWidth;
    for ( int j = 0; j < drawHeight; j++ )
    {
        for ( int i = 0; i < drawWidth; i++ ) *dst++ = *src++;
        src += srcNextLine;
        dst += dstNextLine;
    }
}

void Graphics::paintSpriteKeyed( int x,int y,const Sprite &sprite,Color keyColor )  // not tested
{
    if ( !sprite.isImagePresent() ) return;
    if ( (x >= ScreenWidth) || (y >= ScreenHeight) ) return;
    int xStart,yStart;
    int xEnd,yEnd;
    int xOffset,yOffset;
    if ( x < 0 ) { xStart = 0; xOffset = -x; } else { xStart = x; xOffset = 0; }
    if ( y < 0 ) { yStart = 0; yOffset = -y; } else { yStart = y; yOffset = 0; }
    xEnd = x + sprite.getWidth();
    yEnd = y + sprite.getHeight();
    if ( (xEnd < 0) || (yEnd < 0) ) return;
    if ( xEnd > ScreenWidth ) xEnd = ScreenWidth;
    if ( yEnd > ScreenHeight ) yEnd = ScreenHeight;
    Color *data = sprite.getPixelData() + yOffset * sprite.getWidth() + xOffset;
    Color *s = ((Color*)pSysBuffer) + ScreenWidth * yStart + xStart;
    int sNextLine = ScreenWidth - (xEnd - xStart);
    int dataNextLine = sprite.getWidth() - (xEnd - xStart);
    for ( int j = yStart; j < yEnd; j++ )
    {
        for ( int i = xStart; i < xEnd; i++ )
        {
            Color c = *s++;
            if ( c != keyColor ) *data = c;
            data++;
        }
        data += dataNextLine;
        s += sNextLine;
    }
}

void Graphics::paintBMPClearType( int x,int y,const Sprite &sprite,Color keyColor )
{
    if ( !sprite.isImagePresent() ) return;
    if ( (x >= ScreenWidth) || (y >= ScreenHeight) ) return;
    int xStart,yStart;
    int xEnd,yEnd;
    int xOffset,yOffset;
    if ( x < 0 ) { xStart = 0; xOffset = -x; } else { xStart = x; xOffset = 0; }
    if ( y < 0 ) { yStart = 0; yOffset = -y; } else { yStart = y; yOffset = 0; }
    xEnd = x + sprite.getWidth();
    yEnd = y + sprite.getHeight();
    if ( (xEnd < 0) || (yEnd < 0) ) return;
    if ( xEnd > ScreenWidth ) xEnd = ScreenWidth;
    if ( yEnd > ScreenHeight ) yEnd = ScreenHeight;
    Color *bmpData = sprite.getPixelData() + yOffset * sprite.getWidth() + xOffset;
    Color *dest = (Color*)pSysBuffer + ScreenWidth * yStart + xStart;
    int destNextLine = ScreenWidth - (xEnd - xStart);
    int bmpDataNextLine = sprite.getWidth() - (xEnd - xStart);
    for ( int j = yStart; j < yEnd; j++ )
    {
        int pixelCnt = xOffset;
        for ( int i = xStart; i < xEnd; i++ )
        {
            Color c = *bmpData++;
            if ( c != keyColor )
            {
                if ( c == Color(0xFFFFFF) ) *dest = Color(0xFFFFFF);
                else {
                    // recycle MS cleartype technology a little ;)
                    Color s = *dest;
                    int sr = s.GetR();
                    int sg = s.GetG();
                    int sb = s.GetB();
                    int dr = c.GetR();
                    int dg = c.GetG();
                    int db = c.GetB();
                    sr = sr + dr; if ( sr > 255 ) sr = 255;
                    sg = sg + dg; if ( sg > 255 ) sg = 255;
                    sb = sb + db; if ( sb > 255 ) sb = 255;
                    *dest = (Color)((sr << 16) + (sg << 8) + sb);
                }
            }
            dest++;
        }
        bmpData += bmpDataNextLine;
        dest += destNextLine;
    }
}

void Graphics::paintBMPClearType( int x,int y,const Sprite &sprite,Color keyColor,int opacity )
{
    if ( !sprite.isImagePresent() ) return;
    if ( (x >= ScreenWidth) || (y >= ScreenHeight) ) return;
    int xStart,yStart;
    int xEnd,yEnd;
    int xOffset,yOffset;
    if ( x < 0 ) { xStart = 0; xOffset = -x; } else { xStart = x; xOffset = 0; }
    if ( y < 0 ) { yStart = 0; yOffset = -y; } else { yStart = y; yOffset = 0; }
    xEnd = x + sprite.getWidth();
    yEnd = y + sprite.getHeight();
    if ( (xEnd < 0) || (yEnd < 0) ) return;
    if ( xEnd > ScreenWidth ) xEnd = ScreenWidth;
    if ( yEnd > ScreenHeight ) yEnd = ScreenHeight;
    Color *bmpData = sprite.getPixelData() + yOffset * sprite.getWidth() + xOffset;
    Color *dest = (Color*)pSysBuffer + ScreenWidth * yStart + xStart;
    int destNextLine = ScreenWidth - (xEnd - xStart);
    int bmpDataNextLine = sprite.getWidth() - (xEnd - xStart);
    for ( int j = yStart; j < yEnd; j++ )
    {
        int pixelCnt = xOffset;
        for ( int i = xStart; i < xEnd; i++ )
        {
            Color c = *bmpData++;
            if ( c != keyColor )
            {
                Color s = *dest;  // video memory read == slow!
                int sr = s.GetR();
                int sg = s.GetG();
                int sb = s.GetB();
                int dr = c.GetR();
                int dg = c.GetG();
                int db = c.GetB();
                if ( c != Color(0xFFFFFF) ) {
                    // recycle MS cleartype technology a little ;)
                    dr = sr + dr; if ( dr > 255 ) dr = 255;
                    dg = sg + dg; if ( dg > 255 ) dg = 255;
                    db = sb + db; if ( db > 255 ) db = 255;
                }
                // blend it with the background based on the opacity
                *dest = (((dr << 16) + ((opacity  * (sr - dr)) << 8)) & 0xFF0000)
                    | (dg << 8) + ((opacity  * (sg - dg)) & 0xFF00)
                    | (db + ((opacity  * (sb - db)) >> 8));
            }
            dest++;
        }
        bmpData += bmpDataNextLine;
        dest += destNextLine;
    }
}

/*
Specialized drawing functions for interface
*/
void Graphics::drawNiceBlock( Rect r )
{
    int rd = frameColor_.GetR() / frameWidth_;
    int gd = frameColor_.GetG() / frameWidth_;
    int bd = frameColor_.GetB() / frameWidth_;
    int red = rd;
    int grn = gd;
    int blu = bd;
    int i;
    for ( i = 0; i < frameWidth_; i++ )
    {
        drawBox( r.x1 + i,r.y1 + i,r.x2 - i,r.y2 - i,Color( red,grn,blu ) );
        red += rd;
        grn += gd;
        blu += bd;
    }
    drawBlock( r.x1 + i,r.y1 + i,r.x2 - i,r.y2 - i,frameColor_ );
}

void Graphics::drawNiceBlockInv( Rect r )
{
    int red = frameColor_.GetR();
    int grn = frameColor_.GetG();
    int blu = frameColor_.GetB();
    int rd = red / frameWidth_;
    int gd = grn / frameWidth_;
    int bd = blu / frameWidth_;
    int i;
    Color c;
    for ( i = 0; i < frameWidth_; i++ )
    {
        c = Color( red,grn,blu );
        drawBox( r.x1 + i,r.y1 + i,r.x2 - i,r.y2 - i,c );
        red -= rd;
        grn -= gd;
        blu -= bd;
    }
    drawBlock( r.x1 + i,r.y1 + i,r.x2 - i,r.y2 - i,c );
}

void Graphics::drawButton( Rect r )
{
    drawBox( r.x1,r.y1,r.x2,r.y2,0 );
    int colorDelta = 60;
    int red = frameColor_.GetR();
    int grn = frameColor_.GetG();
    int blu = frameColor_.GetB();
    int rd = red - colorDelta; if ( rd < 0 ) rd = 0;
    int gd = grn - colorDelta; if ( gd < 0 ) gd = 0;
    int bd = blu - colorDelta; if ( bd < 0 ) bd = 0;
    int darkColor = (rd << 16) + (gd << 8) + bd;
    int rl = red + colorDelta; if ( rl > 0xFF ) rl = 0xFF;
    int gl = grn + colorDelta; if ( gl > 0xFF ) gl = 0xFF;
    int bl = blu + colorDelta; if ( bl > 0xFF ) bl = 0xFF;
    int lightColor = (rl << 16) + (gl << 8) + bl;
    for ( int i = 1; i < frameWidth_ - 1; i++ )
    {
        drawHorLine( r.x1 + i,r.y1 + i,r.x2 - i,lightColor );
        drawVerLine( r.x2 - i,r.y1 + i,r.y2 - i,lightColor );
        drawHorLine( r.x1 + i,r.y2 - i,r.x2 - i,darkColor );
        drawVerLine( r.x1 + i,r.y1 + i,r.y2 - i,darkColor );
    }
}

void Graphics::drawButtonPlusMinus( Rect r,int width )
{
    int ySplit = r.y1 + (r.y2 - r.y1) / 2 + 1;
    Rect rUp = r;
    rUp.y2 = ySplit;
    Rect rDown = r;
    rDown.y1 = ySplit;
    drawButton( rUp );
    drawButton( rDown );
    for ( int i = 0; i < width; i++ )
    {
        int x = r.x1 + 5 + i * font->width();
        printXY( x,r.y1 + 1,"+" );
        printXY( x,ySplit,"-" );
    }
}

void Graphics::paintConsole( int x,int y,EvoConsole *console )
{
    assert( console->columns > 0 );
    assert( console->rows    > 0 );

    char buf[2];
    buf[1] = '\0';

    for ( int r = 0; r < console->rows; r++ )
        for ( int c = 0; c < console->columns; c++ )
        {
            buf[0] = console->data[r][c];
            if ( !buf[0] ) buf[0] = ' ';
            printXY( x + c * font->width(),y + r * font->height(),buf );
        }
}

void Graphics::printText( int x,int y,const char *text[] )
{
    int iLine = 0;
    for ( int j = y; text[iLine] != nullptr; j += font->height() )
        printXY( x,j,text[iLine++] );
}