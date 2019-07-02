/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "Game.h"
#include "Math.h"
#include "Graphics.h"



#include <stdlib.h>  
#include <stdio.h>  
#include <time.h>


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
    gfx( wnd )
{
    // set the current status of the game:
    gameState = terraineditorstate; // debug, should be "introstate", set in game.h
    gameState = introstate; // temp
    // load the bare minimum:
    std::string path( GAME_FOLDER );
    path += defaults.mediaFolder() + "\\" + defaults.smallFontFile();
    int error = font.loadFont( path.c_str() );
    if ( error != 0 )
    {
        //showErrorAndQuit( path );
        std::wstring errMsg( L"Unable to open the default small font file " );
        for ( char c : path ) errMsg += c;
        errMsg += L", exiting Program.";
        wnd.ShowMessageBox( L"Fatal Error",errMsg,MB_OK );
        PostQuitMessage( 0 ); 
        return;        
    }
    gfx.setFont( &font );
    // show a loading screen:
    gfx.BeginFrame();
    UpdateModel();
    gfx.printXY( 0,0,"Loading..." );
    gfx.EndFrame();
    // load other (bigger) font:
    path.assign( GAME_FOLDER );
    path += defaults.mediaFolder() + "\\" + "neuropolX.tft";
    error = neuropolXBMP.loadFont( path.c_str() );

    // Init the campaign editor
    campaignEditor.init( wnd,gfx,font,mouse,wnd.kbd );
    //int i; masterIniFile.getKeyValue( "unknownSection","unknownkey",i );  // debug test

    srand( (unsigned)time( NULL ) );
    frameNr = 0;
}

/*
void Game::showErrorAndQuit( const std::string& missingFile )
{
    std::wstring errMsg( L"Unable to open file " );
    for ( char c : missingFile ) errMsg += c;
    errMsg += L", exiting Program.";
    wnd.ShowMessageBox( L"Fatal Error",errMsg,MB_OK );
    PostQuitMessage( 0 );
}
*/

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
}

typedef float FPT;

FPT sinc( FPT x ) {
    const FPT pi = 3.141592653;
    if ( abs(x) < 0.0001 ) return 1;
    return sin( pi * x ) / (x * pi);
}
/*
double sinc( double x ) {
    
    if ( x == 0 ) return 1;
    return sin( x ) / x;
}
*/

void    sincInterpolationDemo( Graphics& gfx )
{
    const int wdwWidth = gfx.ScreenWidth - 200;
    const int wdwHeight = gfx.ScreenHeight - 20;

    // test parameters:
    const int sampleInterval = wdwWidth / 16;
    const FPT stretcher = FPT( wdwWidth / 2 );
    const int N = 1; // nr of sinc sample ref points left & right

                     // colors:
    const Color frameColor = Colors::White;
    const Color origCurveColor = Colors::Green;
    const Color sampleDotsColor = Colors::Magenta;
    const Color linearInterPolationColor = Colors::MakeRGB( 0xFF,0x30,0x30 );
    const Color cubicInterPolationColor = Colors::MakeRGB( 0xD0,0xA0,0x40 );

    // some windows coordinates
    const int xStart = (gfx.ScreenWidth - wdwWidth) / 2;
    const int yStart = (gfx.ScreenHeight - wdwHeight) / 2;
    const int xEnd = xStart + wdwWidth - 1;
    const int yEnd = yStart + wdwHeight - 1;
    const int yMiddle = yStart + wdwHeight / 2 - 1;
    const FPT pi = 3.141592653;
    gfx.drawBox(
        xStart - 1,
        yStart - 1,
        xEnd + 1,
        yEnd + 1,
        frameColor
    );
    gfx.drawHorLine( xStart,yMiddle,xEnd,frameColor );
    const int nrOfSamplePoints = wdwWidth / sampleInterval + 1;
    std::vector<int> samplePoints;
    samplePoints.reserve( nrOfSamplePoints );
    const int maxAmp = 2;
    const int amp = (wdwHeight / 1) / maxAmp;
    FPT lastY = 0;
    // draw original function
    for ( int t = 0; t < wdwWidth; t++ )
    {
        FPT y = (FPT)(
            /*
            sin( 50 * pi * (FPT)t / stretcher )
            + cos( 3.0 * pi * (FPT)t / stretcher )
            + cos( 1.37 * pi * (FPT)(t / 5.7) / stretcher )
            + sin( 17.9 * pi * (FPT)(t / 0.7) / stretcher )
            */
            sin( (FPT)2.0 * pi * (FPT)t / (FPT)stretcher )
            );
        //double y = sin( 2.0 * pi * (double)t / stretcher );
        y = -y * (FPT)amp;
        gfx.drawLine(
            xStart + t - 1,
            yMiddle + (int)lastY,
            xStart + t,
            yMiddle + (int)y,
            origCurveColor
        );
        if ( (t % sampleInterval) == 0 ) samplePoints.push_back( (int)y );
        lastY = y;
    }
    samplePoints.push_back( (int)lastY );
    samplePoints.push_back( (int)lastY ); // debug
    samplePoints.push_back( (int)lastY ); // debug
                                          // draw dots on sample points
    for ( int t = 0; t < nrOfSamplePoints; t++ )
    {
        gfx.drawDisc(
            xStart + t * sampleInterval,
            yMiddle + samplePoints[t],
            3,
            sampleDotsColor
        );
    }
    /*
    // draw linear interpolation:
    lastY = samplePoints[0];
    for ( int t = 1; t < nrOfSamplePoints; t++ )
    {
    gfx.drawLine(
    xStart + (t - 1) * sampleInterval,
    yMiddle + (int)lastY,
    xStart + t * sampleInterval,
    yMiddle + samplePoints[t],
    linearInterPolationColor
    );
    lastY = samplePoints[t];
    }
    */
    // draw cubic interpolation:
    for ( int t = 0; t < nrOfSamplePoints - 1; t++ )
    {
        FPT p0 = (FPT)((t == 0) ? 0.0 : samplePoints[t - 1]);
        FPT p1 = (FPT)samplePoints[t];
        FPT p2 = (FPT)samplePoints[t + 1];
        FPT p3 = (FPT)samplePoints[t + 2];

        int lastY = (int)p1;
        for ( int t2 = 1; t2 < sampleInterval; t2++ )
        {
            FPT fract = (FPT)t2 / (FPT)sampleInterval;
            FPT k = p1 - p2;
            FPT a = ((k * 2) + k - p0 + p3) / 2;
            FPT b = (p2 * 2) + p0 - (((p1 * 4) + p1 + p3) / 2);
            FPT c = (p2 - p0) / 2;
            FPT f2 = ((((a * fract) + b) * fract) + c) * fract + p1;

            gfx.drawLine(
                xStart + t * sampleInterval + t2 - 1,
                yMiddle + lastY,
                xStart + t * sampleInterval + t2,
                yMiddle + (int)f2,
                cubicInterPolationColor
            );
            lastY = (int)f2;
        }
    }
    // sinc interpolation              
    FPT *buf = new FPT[wdwWidth];
    memset( buf,0,sizeof( FPT ) * wdwWidth );

    /*
    for ( int s = 0; s < nrOfSamplePoints; s++ )
    {
    for ( int x = 0; x < wdwWidth; x++ )
    {
    FPT realX = (FPT)(x - s * sampleInterval) / (FPT)(sampleInterval);
    FPT f2 = sinc( realX ) * (FPT)samplePoints[s];
    buf[x] += f2;
    }
    }
    */
    for ( int s = N; s < nrOfSamplePoints - N; s++ )
    {

        //for ( int x = 0; x < wdwWidth; x++ )
        for ( int x = (s - N) * sampleInterval;
            x < (s + N) * sampleInterval; x++ )
        {
            FPT realX = (FPT)(x - s * sampleInterval) / (FPT)(sampleInterval);
            FPT f2 = sinc( realX ) * (FPT)samplePoints[s];
            buf[x] += f2;
        }

    }

    // draw buffer contents:
    lastY = 0;
    for ( int x = 0; x < wdwWidth; x++ )
    {
        int y1 = yMiddle + (int)lastY;
        if ( y1 < 0 ) y1 = 0;
        if ( y1 >= gfx.ScreenHeight ) y1 = gfx.ScreenHeight - 1;
        int y2 = yMiddle + (int)buf[x];
        if ( y2 < 0 ) y2 = 0;
        if ( y2 >= gfx.ScreenHeight ) y2 = gfx.ScreenHeight - 1;

        gfx.drawLine(
            xStart + x - 1,
            y1,
            xStart + x,
            y2,
            Colors::White
        );
        lastY = buf[x];
    }
    delete buf;
}

void    doIntro( Graphics& gfx,int frameNr )
{
    // intro code:
    int blockWidth = 2;
    int blockHeight = 80;
    int ampSineBlock = 200;
    float s = (float)sin( frameNr * 3.1415 * 2.0 / 360.0 );
    int xOffset = (int)((s * (float)ampSineBlock) * 1);
    int xSineBlock = (Graphics::ScreenWidth - blockWidth) / 2 + xOffset;



    int ySineBlock = (Graphics::ScreenHeight - blockHeight) / 2;
    gfx.setFrameColor( 0xA06090 );
    gfx.drawBlock( Rect(
        xSineBlock,
        ySineBlock,
        xSineBlock + blockWidth - 1,
        ySineBlock + blockHeight - 1 ),
        0xA06090
    );
    gfx.drawButton( Rect(
        xSineBlock,
        ySineBlock,
        xSineBlock + blockWidth - 1,
        ySineBlock + blockHeight - 1 )
    );

    gfx.drawBox( Rect(
        (Graphics::ScreenWidth - blockWidth) / 2 - 1,
        (Graphics::ScreenHeight - blockHeight) / 2 - 1,
        (Graphics::ScreenWidth - blockWidth) / 2 + blockWidth - 1 + 1,
        (Graphics::ScreenHeight - blockHeight) / 2 + blockHeight - 1 + 1 ),
        0xFFFFFF
    );
}

void    doFloodFillTest( Graphics& gfx )
{
    // draw polygon and do a floodfill test:
    gfx.drawLine( 10,10,200,70,Colors::Red );
    gfx.drawLine( 200,70,180,300,Colors::Red );
    gfx.drawLine( 180,300,25,270,Colors::Red );
    gfx.drawLine( 25,270,12,100,Colors::Red );
    gfx.drawLine( 12,100,150,121,Colors::Red );
    gfx.drawLine( 150,121,140,146,Colors::Red );
    gfx.drawLine( 140,146,40,130,Colors::Red );
    gfx.drawLine( 40,130,46,180,Colors::Red );
    gfx.drawLine( 46,180,66,160,Colors::Red );
    gfx.drawLine( 66,160,160,270,Colors::Red );
    gfx.drawLine( 160,270,158,76,Colors::Red );
    gfx.drawLine( 158,76,10,10,Colors::Red );
    gfx.drawDisc( 170,280,5,Colors::Yellow );
    gfx.floodFill( 170,280,Colors::Red,Colors::Blue );
}

void    doCubicBezier( Graphics& gfx )
{
    struct Point {
        int x;
        int y;
    } p0,p1,p2,p3;

    // make sure we don't draw off the screen
    const int border = 20;
    p0.x = border + rand() % (Graphics::ScreenWidth - (border * 2));
    p1.x = border + rand() % (Graphics::ScreenWidth - (border * 2));
    p2.x = border + rand() % (Graphics::ScreenWidth - (border * 2));
    p3.x = border + rand() % (Graphics::ScreenWidth - (border * 2));
    p0.y = border + rand() % (Graphics::ScreenHeight - (border * 2));
    p1.y = border + rand() % (Graphics::ScreenHeight - (border * 2));
    p2.y = border + rand() % (Graphics::ScreenHeight - (border * 2));
    p3.y = border + rand() % (Graphics::ScreenHeight - (border * 2));

    // draw four reference points. First the endpoints in red:
    gfx.drawLine( p0.x - 4,p0.y - 4,p0.x + 4,p0.y + 4,Colors::Red );
    gfx.drawLine( p0.x + 4,p0.y - 4,p0.x - 4,p0.y + 4,Colors::Red );
    gfx.drawLine( p3.x - 4,p3.y - 4,p3.x + 4,p3.y + 4,Colors::Red );
    gfx.drawLine( p3.x + 4,p3.y - 4,p3.x - 4,p3.y + 4,Colors::Red );
    // and now the middle points:
    gfx.drawLine( p1.x - 4,p1.y - 4,p1.x + 4,p1.y + 4,Colors::LightBlue );
    gfx.drawLine( p1.x + 4,p1.y - 4,p1.x - 4,p1.y + 4,Colors::LightBlue );
    gfx.drawLine( p2.x - 4,p2.y - 4,p2.x + 4,p2.y + 4,Colors::LightBlue );
    gfx.drawLine( p2.x + 4,p2.y - 4,p2.x - 4,p2.y + 4,Colors::LightBlue );
    // name them:
    gfx.printXY( p0.x - 16,p0.y - 16,"P0" );
    gfx.printXY( p1.x - 16,p1.y - 16,"P1" );
    gfx.printXY( p2.x - 16,p2.y - 16,"P2" );
    gfx.printXY( p3.x - 16,p3.y - 16,"P3" );

    const int nrPoints = 1000;
    int xprev = p0.x;
    int yprev = p0.y;
    for ( int t = 0; t <= nrPoints; t++ ) {
        // calculate t == t2:
        float t2 = (float)t / (float)nrPoints;
        // calculate (1 - t) == t1:
        float t1 = 1.0f - t2;
        // calculate coordinates of t (quadratic):
        //int x = (int)(t1 * t1 * p0.x + t1 * 2 * t2 * p1.x + t2 * t2 * p2.x);
        //int y = (int)(t1 * t1 * p0.y + t1 * 2 * t2 * p1.y + t2 * t2 * p2.y);
        // calculate coordinates of t (cubic):
        int x = (int)(t1 * t1 * t1 * p0.x + t1 * t1 * 3 * t2 * p1.x + t1 * 3 * t2 * t2 * p2.x + t2 * t2 * t2 * p3.x);
        int y = (int)(t1 * t1 * t1 * p0.y + t1 * t1 * 3 * t2 * p1.y + t1 * 3 * t2 * t2 * p2.y + t2 * t2 * t2 * p3.y);
        gfx.drawLine( xprev,yprev,x,y,Colors::Green );
        xprev = x;
        yprev = y;
    }
}

void Game::ComposeFrame()
{
    frameNr++;
    if ( frameNr >= 360 ) frameNr = 0;
    /*
    int mX = mouse.GetPosX();
    int mY = mouse.GetPosY();
    */

    switch ( gameState )
    {
        case    introstate:
        {          
            //sincInterpolationDemo( gfx );
            //doIntro( gfx,frameNr );
            //doFloodFillTest( gfx );
            doCubicBezier( gfx );
            
            break;
        }
        case    menustate:
        {
            PostQuitMessage( 0 ); // temp
            break;
        }
        case    terraineditorstate:
        {
            campaignEditor.bootCampaignEditor(); // only needed once whengoing to the campaignEditor actually 
            campaignEditor.draw();
            campaignEditor.handleInput();
            if ( campaignEditor.campaignEditorIsReadyForShutdown() )
                gameState = menustate;
            break;
        }
        case    pausestate:
        {
            break;
        }
        case    playingstate:
        {
            break;
        }
    }
    /*
    gfx.drawBlock(0,0,33 * 16,33 * 4,Colors::Red);
    for ( int i = 0; i < 64; i++ )
        gfx.paintSprite( 1 + (i % 16) * 33, 1 + (i / 16) * 33,world.getTile( i ) );    
    */
    //gfx.paintSprite(0,50,createDefaultSprites.getSpriteLibrary() );
    //gfx.paintSprite( 100,100,Sprite() );
    //PostMessage( HWND( wnd ),WM_CLOSE,0,0 ); // proper way to exit program?
}
