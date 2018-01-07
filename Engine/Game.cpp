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
    std::string path( GAME_FOLDER );
    path += defaults.mediaFolder() + "\\" + defaults.smallFontFile();
    int error = font.loadFont( path.c_str() );
    if ( error != 0 )
    {
        showErrorAndQuit( path );
        /*
        std::wstring errMsg( L"Unable to open the default small font file " );
        for ( char c : path ) errMsg += c;
        errMsg += L", exiting Program.";
        wnd.ShowMessageBox( L"Fatal Error",errMsg,MB_OK );
        PostQuitMessage( 0 ); 
        */
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

    // load terrain (debug):
    path.assign( GAME_FOLDER );
    path.append( defaults.terrainsFolder() );
    path.append( "\\" );    
    path.append( "default.ini" ); // debug 
    error = terrain.loadTerrain( path );
    if ( error != 0 )
        terrain.init( 
            defaults.defaultTerrainWidth(),
            defaults.defaultTerrainHeight() );

    // load world:
    error = world.loadTiles( terrain.getWorld() );
    if ( error != 0 )
    {
        error = world.loadTiles( defaults.defaultWorld() );
        if ( error != 0 )
        {
            showErrorAndQuit( defaults.defaultWorld() );
            /*
            std::wstring errMsg( L"Unable to open default world gfx data file " );
            for ( char c : defaults.defaultWorld() ) errMsg += c;
            errMsg += L".bmp, exiting Program.";
            wnd.ShowMessageBox( L"Fatal Error",errMsg,MB_OK );
            PostQuitMessage( 0 );
            */
            return;
        }
    }
    // draw the full terrain and the minimap to buffers:
    int tileWidth = world.tileWidth();
    int tileHeight = world.tileHeight();
    gfxTerrain.createEmptySprite(
        terrain.getColumns() * tileWidth,
        terrain.getRows()    * tileHeight );
    miniMap.createEmptySprite( terrain.getColumns(),terrain.getRows() );
    for ( int y = 0; y < terrain.getRows(); y++ )
        for ( int x = 0; x < terrain.getColumns(); x++ )
        {
            gfxTerrain.insertFromSprite(
                x * tileWidth,
                y * tileHeight,
                world.getTile( terrain.getElement( x,y ) ) );
            miniMap.putPixel( x,y,world.getAvgColor( terrain.getElement( x,y ) ) );
        }
    // initialize minimap drawing top left coordinates:
    miniMapX = gameScreens.sidebar_coords.x1 +
        (gameScreens.minimapclient_coords.width() - miniMap.getWidth()) / 2;
    miniMapY = gameScreens.sidebar_coords.y1 + 
        (gameScreens.minimapclient_coords.height() - miniMap.getHeight()) / 2;

    // give the screen drawing class a font to write with and draw the screens:
    gameScreens.setFont( &font );
    gameScreens.drawScenarioEditor();
    
    //int i; masterIniFile.getKeyValue( "unknownSection","unknownkey",i );  // debug test
}

void Game::showErrorAndQuit( const std::string& missingFile )
{
    std::wstring errMsg( L"Unable to open file " );
    for ( char c : missingFile ) errMsg += c;
    errMsg += L", exiting Program.";
    wnd.ShowMessageBox( L"Fatal Error",errMsg,MB_OK );
    PostQuitMessage( 0 );
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
    
    // Draw the terrain editor menu's:
    gfx.paintSprite(
        gameScreens.menubar_coords.x1,
        gameScreens.menubar_coords.y1,
        gameScreens.menuBar() );
    gfx.paintSprite(
        gameScreens.sidebar_coords.x1,
        gameScreens.sidebar_coords.y1,
        gameScreens.sideBar() );
    // Draw the minimap:
    gfx.paintSprite( miniMapX,miniMapY,miniMap );
    // Draw the terrain:
    int x1 = TerrainDrawXOrig * world.tileWidth();
    int y1 = TerrainDrawYOrig * world.tileHeight();
    gfx.paintSpriteSection( 
        MAP_AREA_X1,MAP_AREA_Y1,
        Rect( x1,y1,
            x1 + gameScreens.map_coords.width() - 1,
            y1 + gameScreens.map_coords.height() - 1),
        gfxTerrain );
    // Draw a Grid:
    /*
    for ( int x = gameScreens.map_coords.x1 + world.tileWidth() - 1; 
        x < gameScreens.map_coords.x2; x += world.tileWidth() )
        gfx.drawVerLine( 
            x,
            gameScreens.map_coords.y1,
            gameScreens.map_coords.y2,Colors::Gray );
    for ( int y = gameScreens.map_coords.y1 + world.tileHeight() - 1; 
        y < gameScreens.map_coords.y2; y += world.tileHeight() )
        gfx.drawHorLine(
            gameScreens.map_coords.x1,
            y,
            gameScreens.map_coords.x2,Colors::Gray );
    */






    /*
    gfx.drawBlock(0,0,33 * 16,33 * 4,Colors::Red);
    for ( int i = 0; i < 64; i++ )
        gfx.paintSprite( 1 + (i % 16) * 33, 1 + (i / 16) * 33,world.getTile( i ) );    
    */

    //gfx.paintSprite(0,50,createDefaultSprites.getSpriteLibrary() );

    //gfx.paintSprite( 100,100,Sprite() );
    //PostMessage( HWND( wnd ),WM_CLOSE,0,0 ); // proper way to exit program?
}
