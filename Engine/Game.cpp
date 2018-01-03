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
    // load the bare minimum:
    std::string fontPath( GAME_FOLDER );
    fontPath.append( defaults.mediaFolder() );
    fontPath.append( "\\" );
    fontPath.append( defaults.smallFontFile() );
    font.loadFont( fontPath.c_str() );
    gfx.setFont( &font );
    // show a loading screen:
    gfx.BeginFrame();
    UpdateModel();
    gfx.printXY(0,0,"Loading...");
    gfx.EndFrame();
    // load other necessities:
    fontPath.assign( GAME_FOLDER );
    fontPath.append( defaults.mediaFolder() );
    fontPath.append( "\\" );
    fontPath.append( "neuropolX.tft" );
    neuropolXBMP.loadFont( fontPath.c_str() );

    gameScreens.init( gfx );
    
    //int i; masterIniFile.getKeyValue( "unknownSection","unknownkey",i );  // debug test
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
    gameScreens.drawScenarioEditor();

    
    // gfx.paintSprite(0,0,createDefaultSprites.getSpriteLibrary() );


    //gfx.paintSprite( 100,100,Sprite() );
    //PostMessage( HWND( wnd ),WM_CLOSE,0,0 ); // proper way to exit program?
}
