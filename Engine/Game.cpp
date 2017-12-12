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
    if ( !masterIniFile.isLoaded() ) PostQuitMessage( -1 );
    font.loadFont( "C:\\RTSMedia\\Courier.TFT" );
    neuropolXBMP.loadFont( "C:\\RTSMedia\\neuropolX.tft" );
    gfx.setFont( &font );
    // draw sprites on screen and save the screen capture to a .bmp file.
    // we need to do at least one frame to clear the screen / video memory ;)
    gfx.BeginFrame();
    UpdateModel();
    createDefaultSprites.init( &gfx );
    gfx.EndFrame();
    // finished creating sprite library
}

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

void Game::ComposeFrame()
{
    frameNr++;
    //gfx.ScreenWidth = 1600;
    createDefaultSprites.drawTerrainSprites();
    gfx.paintSprite( 100,100,Sprite() );
    //PostMessage( HWND( wnd ),WM_CLOSE,0,0 ); // proper way to exit program?

}
