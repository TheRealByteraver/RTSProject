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

void sincInterpolationDemo( Graphics& gfx )
{
    const int wdwWidth = gfx.ScreenWidth - 200;
    const int wdwHeight = gfx.ScreenHeight - 20;

    // test parameters:
    const int nrOfSamples = 64;
    const int sampleInterval = wdwWidth / nrOfSamples;
    const FPT stretcher = FPT( wdwWidth / 2 );
    const int N = 4; // nr of sinc sample ref points left & right

    // some color constants
    const Color frameColor = Colors::White;
    const Color origCurveColor = Colors::White;
    const Color sampleDotsColor = Colors::Magenta;
    const Color linearInterPolationColor = Colors::MakeRGB( 0xFF,0x20,0x20 );
    const Color cubicInterPolationColor = Colors::MakeRGB( 0xC0,0xA0,0x40 );
    const Color sincInterPolationColor = Colors::MakeRGB( 0x20,0xFF,0x20 );

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
    const int maxAmp = 4;
    const int amp = (wdwHeight / 2) / maxAmp;
    FPT lastY = 0;
    // draw original function
    for ( int t = 0; t < wdwWidth; t++ )
    {
        FPT y = (FPT)(
              sin( 10.3 * pi * (FPT)t / stretcher ) +
            + cos( 7.12 * pi * (FPT)t / stretcher )
            + cos( 1.37 * pi * (FPT)(t / 5.7) / stretcher )
            + sin( 1.92 * pi * (FPT)(t / 0.7) / stretcher )
            );
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
    
    // sinc interpolation: prepare buffer
    FPT *buf = new FPT[wdwWidth];
    memset( buf,0,sizeof( FPT ) * wdwWidth );

    for ( int s = 0; s < nrOfSamplePoints; s++ ) {
        for ( int x = (s - N) * sampleInterval; x < (s + N) * sampleInterval; x++ ) {
            FPT realX = (FPT)(x - s * sampleInterval) / (FPT)(sampleInterval);
            FPT f2;
            if ( s >= 0 && s < nrOfSamplePoints )
                f2 = sinc( realX ) * (FPT)samplePoints[s];
            else 
                f2 = 0;
            if ( (x >= 0) && (x < wdwWidth) )
                buf[x] += f2;
        }
    }
    // draw buffer contents (sinc interpolation):
    lastY = 0;
    for ( int x = 0; x < wdwWidth; x++ ) {
        int y1 = yMiddle + (int)lastY;
        if ( y1 < 0 ) 
            y1 = 0;
        if ( y1 >= gfx.ScreenHeight ) 
            y1 = gfx.ScreenHeight - 1;
        int y2 = yMiddle + (int)buf[x];
        if ( y2 < 0 ) 
            y2 = 0;
        if ( y2 >= gfx.ScreenHeight ) 
            y2 = gfx.ScreenHeight - 1;

        gfx.drawLine(
            xStart + x - 1,
            y1,
            xStart + x,
            y2,
            sincInterPolationColor
        );
        lastY = buf[x];
    }
    delete buf;
    // draw history:
    const int legendElemWidth = 30;
    const int legendX = xStart + legendElemWidth + 16;
    const int fontHeight = gfx.getFont()->height();
    int legendY = yEnd - fontHeight * 8;

    gfx.drawDisc( xStart + 8 + legendElemWidth / 2,legendY + fontHeight / 2,3,sampleDotsColor );
    gfx.printXY( legendX,legendY,"Sample points" );
    legendY += fontHeight;

    gfx.drawHorLine( xStart + 8,legendY + fontHeight / 2,legendX - 8,origCurveColor );
    gfx.printXY( legendX,legendY,"Original waveform" );
    legendY += fontHeight;

    gfx.drawHorLine( xStart + 8,legendY + fontHeight / 2,legendX - 8,cubicInterPolationColor );
    gfx.printXY( legendX,legendY,"Cubic interpolation" );

    legendY += fontHeight;
    gfx.drawHorLine( xStart + 8,legendY + fontHeight / 2,legendX - 8,sincInterPolationColor );
    gfx.printXY( legendX,legendY,"Sinc interpolation" );

    legendY += fontHeight;

    std::string tmpStr( "Nr of sample points = " );
    tmpStr += std::to_string( nrOfSamples );
    legendY += fontHeight;
    gfx.printXY( xStart + 8,legendY,tmpStr.c_str() );

    tmpStr = "Nr of reference points (left + right) = ";
    tmpStr += std::to_string( N * 2 );
    legendY += fontHeight;
    gfx.printXY( xStart + 8,legendY,tmpStr.c_str() );
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
    const int border = 40;
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

    const int nrPoints = 50;
    float m = 0;
    float mprev = 0;
    float xprev = (float)p0.x;
    float yprev = (float)p0.y;

    float x1prev = (float)p0.x;
    float x2prev = (float)p0.x;
    float y1prev = (float)p0.y;
    float y2prev = (float)p0.y;

    for ( int t = 0; t <= nrPoints; t++ ) {
        // calculate t == t2:
        float t2 = (float)t / (float)nrPoints;
        // calculate (1 - t) == t1:
        float t1 = 1.0f - t2;
        // calculate coordinates of t (quadratic):
        //int x = (int)(t1 * t1 * p0.x + t1 * 2 * t2 * p1.x + t2 * t2 * p2.x);
        //int y = (int)(t1 * t1 * p0.y + t1 * 2 * t2 * p1.y + t2 * t2 * p2.y);
        // calculate coordinates of t (cubic):
        float x = (t1 * t1 * t1 * p0.x + t1 * t1 * 3 * t2 * p1.x + t1 * 3 * t2 * t2 * p2.x + t2 * t2 * t2 * p3.x);
        float y = (t1 * t1 * t1 * p0.y + t1 * t1 * 3 * t2 * p1.y + t1 * 3 * t2 * t2 * p2.y + t2 * t2 * t2 * p3.y);
        //gfx.drawLine( (int)xprev,(int)yprev,(int)x,(int)y,Colors::Green );

        // draw perpendicular line:  
        float thickness = 20; // in pixels, should be less than border

        // get crossing point of two lines:
        float xc = xprev + (x - xprev) / 2;
        float yc = yprev + (y - yprev) / 2;

        float yD = y - yprev;
        float xD = x - xprev;

        float alfa;

        mprev = m;
        m = (-xD / yD) ;

        alfa = atan( m );

        /*
        if ( alfa > (3.14159265359 / 2.0) )
            alfa = alfa - (3.14159265359 / 2.0);
        */

        float xd = (thickness / 2) * cos( alfa );
        float yd = (thickness / 2) * sin( alfa );

        if ( m * mprev < 0 ) {
            float tmp = y1prev;
            y1prev = y2prev;
            y2prev = tmp;
            yd = -yd;
            /*
            xd = -xd;
            tmp = x1prev;
            x1prev = x2prev;
            x2prev = tmp;
            */
        }

        float x1 = xc - xd;
        float y1 = yc - yd;
        float x2 = xc + xd;
        float y2 = yc + yd;


        gfx.drawLine( (int)x1prev,(int)y1prev,(int)x1,(int)y1,Colors::Magenta );
        gfx.drawLine( (int)x2prev,(int)y2prev,(int)x2,(int)y2,Colors::Magenta );
        x1prev = x1;
        x2prev = x2;
        y1prev = y1;
        y2prev = y2;

        /*
        // perpendicular line:
        float x1 = xc - (thickness / 2) * cos( alfa );
        float y1 = yc - (thickness / 2) * sin( alfa );
        float x2 = xc + (thickness / 2) * cos( alfa );
        float y2 = yc + (thickness / 2) * sin( alfa );
        gfx.drawLine( (int)x1,(int)y1,(int)x2,(int)y2,Colors::Magenta );
        */

        gfx.drawLine( (int)xprev,(int)yprev,(int)x,(int)y,Colors::Green );

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
            intro.updateFrame( gfx );
            //doFloodFillTest( gfx );
            //doCubicBezier( gfx );
            
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
