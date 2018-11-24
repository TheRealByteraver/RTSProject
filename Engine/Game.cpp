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



Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
    gfx( wnd )
{
    // set the current status of the game:
    //gameState = terraineditorstate; // debug, should be "introstate", set in game.h
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

double sinc( double x ) {
    const double pi = 3.141592653;
    if ( abs(x) < 0.0000001 ) return 1;
    return sin( pi * x ) / (x * pi);
}
/*
double sinc( double x ) {
    
    if ( x == 0 ) return 1;
    return sin( x ) / x;
}
*/

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
            const int wdwWidth = gfx.ScreenWidth - 200;
            const int wdwHeight = gfx.ScreenHeight - 20;

            // test parameters:
            const int sampleInterval = wdwWidth / 6;
            const double stretcher = wdwWidth / 1;

            // colors:
            const Color frameColor = Colors::White;
            const Color origCurveColor = Colors::Green;
            const Color sampleDotsColor = Colors::Magenta;
            const Color linearInterPolationColor = Colors::MakeRGB( 0xFF,0x30,0x30 );
            const Color cubicInterPolationColor = Colors::MakeRGB( 0xD0,0xA0,0x40 );
            const int xStart = (gfx.ScreenWidth - wdwWidth) / 2;
            const int yStart = (gfx.ScreenHeight - wdwHeight) / 2;
            const int xEnd = xStart + wdwWidth - 1;
            const int yEnd = yStart + wdwHeight - 1;
            const int yMiddle = yStart + wdwHeight / 2 - 1;
            const double pi = 3.141592653;
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
            const int amp = (wdwHeight / 2) / maxAmp;
            double lastY = 0;
            // draw original function
            for ( int t = 0; t < wdwWidth; t++ )
            {
                double y = sin( 2.0 * pi * (double)t / stretcher ) + cos( 3.0 * pi * (double)t / stretcher );
                //double y = sin( 2.0 * pi * (double)t / stretcher );
                y = -y * (double)amp;
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
                double p0 = (t == 0) ? 0.0 : samplePoints[t - 1];
                double p1 = samplePoints[t];
                double p2 = samplePoints[t + 1];
                double p3 = samplePoints[t + 2];

                int lastY = (int)p1;
                for ( int t2 = 1; t2 < sampleInterval; t2++ )
                {
                    double fract = (double)t2 / (double)sampleInterval;
                    double k = p1 - p2;
                    double a = ((k * 2) + k - p0 + p3)  / 2;
                    double b = (p2 * 2) + p0 - (((p1 * 4) + p1 + p3) / 2);
                    double c = (p2 - p0) / 2;
                    double f2 = ((((a * fract) + b) * fract) + c) * fract + p1;

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
            
            int buf[1600];
            memset( buf,0,sizeof( int ) * 1600 );

            for ( int s = 0; s < nrOfSamplePoints; s++ ) 
            { 
                const int colorStep = 0xFF / nrOfSamplePoints;
                const Color c = Colors::MakeRGB(
                    s * colorStep,
                    0xFF,
                    0xFF - s * colorStep
                );

                lastY = 0;
                for ( int x = 0; x < wdwWidth; x++ )
                {

                    double realX = (double)(x - s * sampleInterval) / (double)(sampleInterval);// / pi);
                    double f2 = sinc( realX ) * (double)samplePoints[s];
                    /*
                    if ( f2 < -wdwHeight / 2 ) f2 = -wdwHeight / 2;
                    if ( f2 >= wdwHeight / 2 ) f2 = wdwHeight / 2 - 1;
                    */
                    /*
                    gfx.PutPixel(
                        xStart + x,
                        yMiddle + (int)f2,
                        c
                    );
                    */
                    buf[x] += (int)f2;
                    /*
                    gfx.drawLine(
                        xStart + x - 1,
                        yMiddle + (int)lastY,
                        xStart + x,
                        yMiddle + (int)f2,
                        c
                    );
                    */

                    lastY = (int)f2;

                }
            }

            lastY = 0;
            for ( int x = 0; x < wdwWidth; x++ )
            {
                int y1 = yMiddle + (int)lastY;
                if ( y1 < 0 ) y1 = 0;
                if ( y1 >= gfx.ScreenHeight ) y1 = gfx.ScreenHeight - 1;
                int y2 = yMiddle + buf[x];
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





            /*
            const int N = 6;
            lastY = 0;
            for ( int x = 1; x < wdwWidth - N * sampleInterval; x++ )
            {
                int t = x / sampleInterval;
                double f2 = 0.0;
                for ( int n = 0; n < N; n++ )
                {
                    //f2 += sinc( (double)(x - (t + n) * sampleInterval)) * samplePoints[t + n];
                    
                    double a = (double)(x - (t + n) * sampleInterval);
                    if ( a > 0.000001 )
                        f2 += (sin( pi * a ) / (pi * a)) * (double)samplePoints[t + n];
                    else
                        f2 += (double)samplePoints[t + n];

                }

                int y1 = yMiddle + (int)lastY;
                if ( y1 < 0 ) y1 = 0;
                if ( y1 >= gfx.ScreenHeight ) y1 = gfx.ScreenHeight - 1;
                int y2 = yMiddle + (int)f2;
                if ( y2 < 0 ) y2 = 0;
                if ( y2 >= gfx.ScreenHeight ) y2 = gfx.ScreenHeight - 1;

                gfx.drawLine(
                    xStart + x - 1,
                    y1,
                    xStart + x,
                    y2,
                    Colors::White
                );
                lastY = (int)f2;
            }
            */



            /*
            const int w = sampleInterval;
            //const int N = 2;
            for ( int t = 0; t < nrOfSamplePoints - N; t++ )
            {
                int lastY = 0;


                for ( int t2 = -w; t2 < w; t2++ )
                {
                    double p1 = samplePoints[t];
                    double f2 = 0;

                    for ( int n = 0; n < N; n++ )
                    {
                        f2 += sinc( (double)t2 ) * (double)samplePoints[t + n];
                    }
                    f2 = sinc( (double)t2 ) * (double)samplePoints[t];
                    gfx.drawLine(
                        xStart + t * sampleInterval + t2 - 1,
                        yMiddle + (int)lastY,
                        xStart + t * sampleInterval + t2,
                        yMiddle + (int)f2,
                        Colors::Magenta
                    );
                    lastY = (int)f2;
                }

            }
            





            /*

            // intro code:
            int blockWidth = 2;
            int blockHeight = 80;
            ampSineBlock = 200;
            float s = sin( frameNr * 3.1415 * 2.0 / 360.0 );
            int xOffset = (int)(( s * (float)ampSineBlock ) * 1);
            xSineBlock = (Graphics::ScreenWidth - blockWidth) / 2 + xOffset;



            ySineBlock = (Graphics::ScreenHeight - blockHeight) / 2;
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
                (Graphics::ScreenWidth  - blockWidth ) / 2 - 1,
                (Graphics::ScreenHeight - blockHeight) / 2 - 1,
                (Graphics::ScreenWidth  - blockWidth ) / 2 + blockWidth  - 1 + 1,
                (Graphics::ScreenHeight - blockHeight) / 2 + blockHeight - 1 + 1 ),
                0xFFFFFF
            );
            */

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
