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
    gameState = terraineditorstate; // debug, should be "introstate", set in game.h
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

    // load terrain (debug):
    // does not care about minimum size & width! should it?
    path.assign( GAME_FOLDER );
    path.append( defaults.terrainsFolder() );
    path.append( "\\" );    
    path.append( "testterrain.ini" ); // debug 
    error = terrain.loadTerrain( path );
    if ( error != 0 )
    {
        terrain.init(
            defaults.defaultTerrainWidth(),
            defaults.defaultTerrainHeight() );
    }
    // load world:
    error = world.loadTiles( terrain.getWorld() );
    if ( error != 0 )
    {
        defaults.debugLogFile << "Unable to load world " << terrain.getWorld() << ", "
            << " loading default world instead." << std::endl;
        error = world.loadTiles( defaults.defaultWorld() );
        if ( error != 0 )
        {
            //showErrorAndQuit( defaults.defaultWorld() );
            std::wstring errMsg( L"Unable to open default world gfx data file " );
            for ( char c : defaults.defaultWorld() ) errMsg += c;
            errMsg += L".bmp, exiting Program.";
            wnd.ShowMessageBox( L"Fatal Error",errMsg,MB_OK );
            PostQuitMessage( 0 );
            return;
        }
    } 
    world.loadDoodads();// temp!!! DEBUG!!!

    // draw the minimap to buffers:
    miniMap.createEmptySprite( terrain.getColumns(),terrain.getRows() );
    for ( int y = 0; y < terrain.getRows(); y++ )
        for ( int x = 0; x < terrain.getColumns(); x++ )
        {
            miniMap.putPixel( x,y,world.getAvgColor( terrain.getElement( x,y ) ) );
        }
    // initialize minimap drawing top left coordinates:
    initMiniMapCoords();
 
    // give the screen drawing class a font to write with and draw the screens
    gameScreens.setFont( &font );
    gameScreens.drawScenarioEditor(); // this is done only once :)
    
    //int i; masterIniFile.getKeyValue( "unknownSection","unknownkey",i );  // debug test
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
/*
    This function needs to be called each time a new world or terrain is loaded
*/
void Game::initMiniMapCoords()
{
    GameScreens& gS = gameScreens; // save some screen real estate here ;)
    miniMapXOrig = gS.sidebar_coords.x1 + gS.minimapclient_coords.x1 +
        (gS.minimapclient_coords.width() - miniMap.getWidth()) / 2;
    miniMapYOrig = gS.sidebar_coords.y1 + gS.minimapclient_coords.y1 +
        (gS.minimapclient_coords.height() - miniMap.getHeight()) / 2;
    // Initialize nr of Grid lines in both directions:
    visibleTilesX = gameScreens.map_coords.width() / world.tileWidth();
    visibleTilesY = gameScreens.map_coords.height() / world.tileHeight();
    /*
    visibleTilesX = (gS.map_coords.width()   * miniMap.getWidth())
        / (terrain.getColumns() * world.tileWidth());
    visibleTilesY = (gS.map_coords.height() * miniMap.getHeight())
        / (terrain.getRows()    * world.tileHeight());
    */
}

void Game::drawTerrainEditor()
{
    // Draw the terrain:
    int tileWidth = world.tileWidth();
    int tileHeight = world.tileHeight();
    Rect tR = gameScreens.map_coords;
    int j = TerrainDrawYOrig;    
    tR.x2 = tR.x1 + (tR.width() / tileWidth) * tileWidth - 1;
    tR.y2 = tR.y1 + (tR.height() / tileHeight) * tileHeight - 1;
    int xLeftOver = gameScreens.map_coords.width() - tR.width();
    int yLeftOver = gameScreens.map_coords.height() - tR.height();
    int y;
    for ( y = tR.y1; y < tR.y2; y += tileHeight )
    {   
        if ( j >= terrain.getRows() ) break;
        int i = TerrainDrawXOrig;
        int x;
        for ( x = tR.x1; x < tR.x2; x += tileWidth )
        {
            if ( i >= terrain.getColumns() ) break;
            Tile tile = terrain.getElement( i,j );
            if  (tile != DOODAD_NOT_USED ) 
                gfx.paintSprite( x,y,world.getTile( tile ) );
            i++;
        }
        // draw the cut-off tiles on the right edge of the map editor:
        if ( (xLeftOver > 0) && (i < terrain.getColumns()) )
        {
            Tile tile = terrain.getElement( i,j );
            if ( tile != DOODAD_NOT_USED ) 
                gfx.paintSpriteSection( 
                    x,y,
                    Rect( 0,0,xLeftOver - 1,tileHeight - 1 ),
                    world.getTile( tile ) );
        }
        j++;
    }
    // draw the cut-off tiles on the bottom edge of the map editor:
    if ( (yLeftOver > 0) && (j < terrain.getRows()) )
    {
        int i = TerrainDrawXOrig;
        int x;
        for ( x = tR.x1; x < tR.x2; x += tileWidth )
        {
            if ( i >= terrain.getColumns() ) break;
            Tile tile = terrain.getElement( i,j );
            if ( tile != DOODAD_NOT_USED ) 
                gfx.paintSpriteSection(
                    x,y,
                    Rect( 0,0,tileWidth - 1,yLeftOver - 1 ),
                    world.getTile( tile ) );
            i++;
        }
        // draw the cut-off tile on the bottom right edge of the map editor:
        if ( (xLeftOver > 0) && (i < terrain.getColumns()) )
        {            
            Tile tile = terrain.getElement( i,j );
            if ( tile != DOODAD_NOT_USED ) 
                gfx.paintSpriteSection(
                    x,y,
                    Rect( 0,0,xLeftOver - 1,yLeftOver - 1 ),
                    world.getTile( tile ) );
        }
    }     
    // Now Draw the doodadd's.
    // These are the visible tiles on the screen, including the ones 
    // that are only part visible on the right & bottom edges:
    int minX = TerrainDrawXOrig;
    int minY = TerrainDrawYOrig;
    int maxX = minX + visibleTilesX; 
    int maxY = minY + visibleTilesY; 
    const std::vector<DoodadLocation>& doodadList = terrain.getDoodadList();
    for ( int doodadNr = 0; doodadNr < doodadList.size(); doodadNr++ )
    {
        const DoodadLocation doodadLocation = doodadList[doodadNr];
        const Doodad& doodad = world.getDoodad( doodadLocation.doodadNr );
        if ( doodadLocation.isUsed )
        {
            // The next coords / values are expressed in tiles, not pixels:
            int doodadWidth = doodad.width();
            int doodadHeight = doodad.height();
            int x1 = doodadLocation.x;
            int x2 = x1 + doodadWidth - 1;
            int y1 = doodadLocation.y;
            int y2 = y1 + doodadHeight - 1;
            if ( (x2 < minX) || (x1 > maxX) || (y2 < minY) || (y1 > maxY) ) continue; // tile is not visible
            int xOfs = (x1 < minX) ? minX - x1 : 0;
            int yOfs = (y1 < minY) ? minY - y1 : 0;
            // The next coords are expressed in pixels, not tiles:
            int pixX1 = gameScreens.map_coords.x1 + (doodadLocation.x - TerrainDrawXOrig + xOfs) * tileWidth;
            int pixY1 = gameScreens.map_coords.y1 + (doodadLocation.y - TerrainDrawYOrig + yOfs) * tileHeight;
            int pixWidthMinus1 = (doodadWidth - xOfs) * tileWidth - 1;
            int pixHeightMinus1 = (doodadHeight - yOfs) * tileHeight - 1;
            // make sure we clip to the visible part of the map:
            if ( pixX1 + pixWidthMinus1 > gameScreens.map_coords.x2 )
                pixWidthMinus1 = gameScreens.map_coords.x2 - pixX1;
            if ( pixY1 + pixHeightMinus1 > gameScreens.map_coords.y2 )
                pixHeightMinus1 = gameScreens.map_coords.y2 - pixY1;
            // convert tile offsets to pixel coords:
            xOfs *= tileWidth;
            yOfs *= tileHeight;
            gfx.paintSpriteSection( 
                pixX1,pixY1,
                Rect (xOfs,yOfs,xOfs + pixWidthMinus1,yOfs + pixHeightMinus1 ),
                doodad.image() );
        }
    }
    // Draw the Grid:    
    if ( isGridVisible )
    {
        //const Rect& map = gameScreens.map_coords;
        for ( int x = gameScreens.map_coords.x1 + tileWidth - 1;
            x < gameScreens.map_coords.x2; x += tileWidth )
            gfx.drawVerLine(
                x,
                gameScreens.map_coords.y1,
                gameScreens.map_coords.y2,GRID_COLOR );
        for ( int y = gameScreens.map_coords.y1 + tileHeight - 1;
            y < gameScreens.map_coords.y2; y += tileHeight )
            gfx.drawHorLine(
                gameScreens.map_coords.x1,
                y,
                gameScreens.map_coords.x2,GRID_COLOR );
    }
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
    gfx.paintSprite( 
        miniMapXOrig,
        miniMapYOrig,
        miniMap 
    );
    // draw the minimap highlighted area delimiter:
    miniMapCursor.x1 = miniMapXOrig + TerrainDrawXOrig;
    miniMapCursor.y1 = miniMapYOrig + TerrainDrawYOrig;
    miniMapCursor.x2 = miniMapCursor.x1 + visibleTilesX - 1;
    miniMapCursor.y2 = miniMapCursor.y1 + visibleTilesY - 1;
    gfx.drawBox( miniMapCursor,Colors::White );
    // debug:
    gfx.paintSprite( 
        gameScreens.terrainTypeIcon1AbsCoords.x1,
        gameScreens.terrainTypeIcon1AbsCoords.y1,
        world.getTile( T_LOW_WATER )
    );
    gfx.paintSprite(
        gameScreens.terrainTypeIcon2AbsCoords.x1,
        gameScreens.terrainTypeIcon2AbsCoords.y1,
        world.getTile( T_LOW       )
    );
    gfx.paintSprite(
        gameScreens.terrainTypeIcon3AbsCoords.x1,
        gameScreens.terrainTypeIcon3AbsCoords.y1,
        world.getTile( T_HIGH      )
    );
    gfx.paintSprite(
        gameScreens.terrainTypeIcon4AbsCoords.x1,
        gameScreens.terrainTypeIcon4AbsCoords.y1,
        world.getTile( T_HIGH_WATER )
    );
    /*
    gfx.drawBox( gameScreens.terrainTypeIcon1AbsCoords,Colors::Blue );
    gfx.drawBox( gameScreens.terrainTypeIcon2AbsCoords,Colors::Green );
    gfx.drawBox( gameScreens.terrainTypeIcon3AbsCoords,Colors::LightGreen );
    gfx.drawBox( gameScreens.terrainTypeIcon4AbsCoords,Colors::LightBlue );
    */
    /* debug:
    std::string s( "# of doodadd's: " );
    s += std::to_string( world.nrOfDoodads() );
    gfx.printXY( 300,3,s.c_str() );
    */
}

void Game::ComposeFrame()
{
    frameNr++;
    switch ( gameState )
    {
        case    introstate:
        {
            break;
        }
        case    menustate:
        {
            break;
        }
        case    terraineditorstate:
        {
            drawTerrainEditor();
            int mX = mouse.GetPosX();
            int mY = mouse.GetPosY();

            // draw mouse cursor (clipped):
            if ( mouse.isInArea( gameScreens.map_coords ) )
            {
                int curX = (mX - gameScreens.map_coords.x1) / world.tileWidth();
                int curY = (mY - gameScreens.map_coords.y1) / world.tileHeight();
                if ( (TerrainDrawXOrig + curX) & 0x1 ) curX--;
                if ( (TerrainDrawYOrig + curY) & 0x1 ) curY--;
                bool drawTop = true;
                bool drawBottom = true;
                bool drawLeft = true;
                bool drawRight = true;
                int x1 = gameScreens.map_coords.x1 + curX * world.tileWidth();
                int x2 = x1 + world.tileWidth() * 2 - 1;
                int y1 = gameScreens.map_coords.y1 + curY * world.tileHeight();
                int y2 = y1 + world.tileHeight() * 2 - 1;

                if ( x1 < gameScreens.map_coords.x1 )
                {
                    drawLeft = false;
                    x1 = gameScreens.map_coords.x1;
                }
                if ( y1 < gameScreens.map_coords.y1 )
                {
                    drawTop = false;
                    y1 = gameScreens.map_coords.y1;
                }
                int maxX = gameScreens.map_coords.x1 + (terrain.getColumns() - TerrainDrawXOrig) * world.tileWidth() - 1;
                if ( maxX > gameScreens.map_coords.x2 ) maxX = gameScreens.map_coords.x2;
                
                if ( x2 > maxX ) {
                    x2 = maxX;
                    drawRight = false;
                }
                int maxY = gameScreens.map_coords.y1 + (terrain.getRows() - TerrainDrawYOrig) * world.tileHeight() - 1;
                if ( maxY > gameScreens.map_coords.y2 ) maxY = gameScreens.map_coords.y2;

                if ( y2 > maxY ) {
                    y2 = maxY;
                    drawBottom = false;
                }
                if ( (x1 < maxX) && (y1 < maxY) )
                {
                    if ( drawTop ) gfx.drawHorLine( x1,y1,x2,CURSOR_COLOR );
                    if ( drawBottom ) gfx.drawHorLine( x1,y2,x2,CURSOR_COLOR );
                    if ( drawLeft ) gfx.drawVerLine( x1,y1,y2,CURSOR_COLOR );
                    if ( drawRight ) gfx.drawVerLine( x2,y1,y2,CURSOR_COLOR );
                    if ( drawTop ) y1++;
                    if ( drawBottom ) y2--;
                    if ( drawLeft ) x1++;
                    if ( drawRight ) x2--;
                    if ( drawTop ) gfx.drawHorLine( x1,y1,x2,CURSOR_COLOR );
                    if ( drawBottom ) gfx.drawHorLine( x1,y2,x2,CURSOR_COLOR );
                    if ( drawLeft ) gfx.drawVerLine( x1,y1,y2,CURSOR_COLOR );
                    if ( drawRight ) gfx.drawVerLine( x2,y1,y2,CURSOR_COLOR );
                }
            } // end draw cursor

            // debug:
            std::stringstream s;
            s << "Mouse is at pos " << mX << "," << mY;
            gfx.printXY( 100,3,s.str().c_str() );
            // end debug
            // Scrolling function:
            if ( mouse.isInArea( gameScreens.scrollMapLeft ) && (TerrainDrawXOrig > 0) ) 
                TerrainDrawXOrig--;
            if ( mouse.isInArea( gameScreens.scrollMapRight ) && 
                (TerrainDrawXOrig < terrain.getColumns() - visibleTilesX) )
                TerrainDrawXOrig++;
            if ( mouse.isInArea( gameScreens.scrollMapUp ) && (TerrainDrawYOrig > 0) )
                TerrainDrawYOrig--;
            if ( mouse.isInArea( gameScreens.scrollMapDown ) &&
                (TerrainDrawYOrig < terrain.getRows() - visibleTilesY) )
                TerrainDrawYOrig++;
            // drawing function:
            if ( mouse.LeftIsPressed() )
            {
                if ( mouse.isInArea( gameScreens.map_coords ) )
                {
                    int tileX = (mX - gameScreens.map_coords.x1) / world.tileWidth()  + TerrainDrawXOrig;
                    int tileY = (mY - gameScreens.map_coords.y1) / world.tileHeight() + TerrainDrawYOrig;
                    if ( tileX >= terrain.getColumns() ) tileX--; // safety 
                    if ( tileY >= terrain.getRows() ) tileY--;    // safety 
                    if ( tileX & 0x1 ) tileX--;
                    if ( tileY & 0x1 ) tileY--;
                    Rect redraw = terrain.drawTerrain( tileX,tileY,terrainType );
                    
                    // redraw modified terrain (on minimap):
                    for ( int y = redraw.y1; y < redraw.y2; y++ )
                    {
                        for ( int x = redraw.x1; x < redraw.x2; x++ )
                        {
                            /*
                            gfxTerrain.insertFromSprite(
                                x * world.tileWidth(),
                                y * world.tileHeight(),
                                world.getTile( terrain.getElement( x,y ) ) );
                            */
                            miniMap.putPixel( x,y,world.getAvgColor( terrain.getElement( x,y ) ) );
                        }
                    }
                } 
                else if ( mouse.isInArea( gameScreens.terrainTypeIcon1AbsCoords ) )
                {
                    terrainType = T_LOW_WATER;
                } 
                else if ( mouse.isInArea( gameScreens.terrainTypeIcon2AbsCoords ) )
                {
                    terrainType = T_LOW;
                } 
                else if ( mouse.isInArea( gameScreens.terrainTypeIcon3AbsCoords ) )
                {
                    terrainType = T_HIGH;
                } 
                else if ( mouse.isInArea( gameScreens.terrainTypeIcon4AbsCoords ) )
                {
                    terrainType = T_HIGH_WATER;
                }
            }
            // debug:
            /*
            CreateDefaultSprites test;
            test.createDesertWorldDoodAdds();
            //gfx.paintSprite( -1,21,test.getSpriteLibrary() );
            gfx.paintSprite( 10,31,test.getSpriteLibrary() );
            */
            /*
            for ( ;;)
            {
                Mouse::Event event = mouse.Read();
                if ( !event.IsValid() ) break;
                int mX = event.GetPosX();
                int mY = event.GetPosY();
            }
            */
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
