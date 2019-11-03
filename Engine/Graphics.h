/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include <stack>
#include "ChiliException.h"
#include "Colors.h"
#include "EvoConsole.h"
#include "Font.h"


// added to the original file:
struct Rect
{
    int x1;
    int y1;
    int x2;
    int y2;
    Rect() {}
    Rect( int x1_,int y1_,int x2_,int y2_ )
    {
        x1 = x1_;
        y1 = y1_;
        x2 = x2_;
        y2 = y2_;
    }
    bool contains( int x,int y ) const
    {
        if ( x < x1 ) return false;
        if ( x > x2 ) return false;
        if ( y < y1 ) return false;
        if ( y > y2 ) return false;
        return true;
    }
    int width() const
    {
        return x2 - x1 + 1;
    }
    int height() const
    {
        return y2 - y1 + 1;
    }
    bool isValid()
    {
        return ((x2 > x1) && 
                (y2 > y1) &&
                (x1 >= 0) &&
                (y1 >= 0) &&
                (x2 > 0)  &&
                (y2 > 0));
    }
};

// forward declarations for stupid linker:
class Font;
class EvoConsole;
class Sprite;
// end of addition

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
    Color getPixel( int x,int y );
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	//static /*constexpr*/ int ScreenWidth = 1280;
	//static /*constexpr*/ int ScreenHeight = 720;
    static int ScreenWidth;
    static int ScreenHeight;

    /**************************************/
    /*                                    */
    /*  Non-default part of the class     */
    /*  starts here                       */
    /*                                    */
    /**************************************/
public:
    Color   *getpSysBuffer() { return pSysBuffer; }
    Color   darken( Color src,int darken )
    {
        int r = (src.dword >> 16) & 0xFF;
        int g = (src.dword >> 8) & 0xFF;
        int b = (src.dword) & 0xFF;
        return Color(
            (unsigned char)((r > darken) ? r - darken : 0),
            (unsigned char)((g > darken) ? g - darken : 0),
            (unsigned char)((b > darken) ? b - darken : 0)
        );
    }
    void    drawLine( int x1,int y1,int x2,int y2,Color color );
    void    drawHorLine( int x1,int y,int x2,Color color );
    void    drawVerLine( int x,int y1,int y2,Color color );
    void    drawMathLine( int x,int y,float m,int length,Color color );
    void    drawCircle( int cx,int cy,int radius,Color color );
    void    drawDisc( int cx,int cy,int r,Color color );
    void    floodFill( int x,int y,Color borderColor,Color fillColor );
    void floodFill2( int x,int y,Color borderColor,Color fillColor );
    void    drawBox( const Rect& coords,Color color );
    void    drawBox( int x1,int y1,int x2,int y2,Color color );
    void    drawBoxClipped( Rect coords,Rect clippedArea,Color color );
    void    drawBlock( Rect coords,Color color );
    void    drawBlock( int x1,int y1,int x2,int y2,Color color );
    void    setFrameWidth( int frameWidth ) 
        { assert( frameWidth >= 0 ); frameWidth_ = frameWidth; }
    void    setFrameColor( Color color )
    {
        frameColor_ = color;
    }
    void    setTextColor( Color color ) { textColor = color; }
    void    setFont( Font *f ) { font = f; }
    Font    *getFont() { return font; }
    int     getStrLen( const char *s,Font *font );
    void    printXY( int x,int y,const char *s );
    void    printXY( int x,int y,const char *s,int opacity );
    void    printXY( int x,int y,const char *s,int opacity,Font& font );
    void    printXYSolid( int x,int y,int xSpacer,const char *s,Color color /*, HRESULT *hres*/ );
    void    paintConsole( int x,int y,EvoConsole& console );
    void    paintSprite( int x,int y,const Sprite &sprite );
    void    paintSpriteSection( int x,int y,Rect area,const Sprite &sprite );
    void    paintSpriteKeyed( int x,int y,const Sprite &sprite,Color keyColor );
    void    paintBMPClearType( int x,int y,const Sprite &sprite,Color keyColor );
    void    paintBMPClearType( int x,int y,const Sprite &sprite,Color keyColor,int opacity );
    void    paintBMPClearTypeColor( int x,int y,int startY,int endY,const Sprite & sprite,Color keyColor,int opacity );
    void    drawNiceBlock( Rect r );
    void    drawNiceBlockInv( Rect r );
    void    drawButton( Rect r );
    void    drawButtonPlusMinus( Rect r,int width );
    void    printText( int x,int y,const char *text[] );
private:
    Color   textColor = Colors::White;
    int     frameWidth_ = 3;
    Color   frameColor_ = Colors::LightGray;
    Font    *font;
};