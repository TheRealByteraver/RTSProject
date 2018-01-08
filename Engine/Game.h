/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Globals.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Font.h"
#include "CreateDefaultSprites.h"
#include "IniFile.h"
#include "GameScreens.h"
#include "Terrain.h"
#include "World.h"


extern class Defaults defaults;

enum gamestate { introstate,menustate,terraineditorstate,pausestate,playingstate };

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
    //void showErrorAndQuit( const std::string& missingFile );
    void    drawTerrainEditor();
private:
	MainWindow& wnd;
    Graphics    gfx;
    Mouse&      mouse = wnd.mouse;
	/********************************/
	/*  User Variables              */
	/********************************/
    int                     frameNr = 0;
    gamestate               gameState = introstate;
    Font                    font;
    Font                    neuropolXBMP;
    //IniFile                 masterIniFile = std::string( GAME_FOLDER MASTER_INIFILE_FILENAME );
    
    GameScreens             gameScreens;
    Terrain                 terrain;
    World                   world;
    Sprite                  gfxTerrain;
    Sprite                  miniMap;
    // these coordinates keep track of our location on the map in-game:
    int                     TerrainDrawXOrig = 0;
    int                     TerrainDrawYOrig = 0;
    // these coordinates are used to draw the minimap (the radar screen):
    /*
    int                     miniMapX = 0;
    int                     miniMapY = 0;
    int                     miniMapCursorW
    
    Rect                    terrainVisibleArea;
    */
    int                     miniMapXOrig;
    int                     miniMapYOrig;
    int                     visibleTilesX;
    int                     visibleTilesY;
    Rect                    miniMapCursor;

    int                     terrainNrVisibleColumns;
    int                     terrainNrVisibleRows;
private:
    void                    initMiniMapCoords();
};