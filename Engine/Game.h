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
#include "CampaignEditor.h"


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
    //void    drawTerrainEditor();
    // must be called each time a new terrain or world is loaded:
    //void    initMiniMapCoords();
    //bool    canPlaceDoodadAtLocation( int x,int y,const Doodad& doodad ) const;
    //void    checkDoodads();
    //void    checkTerrainIntegrity();
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
    GameScreens             gameScreens;
    CampaignEditor          campaignEditor;
    //Terrain                 terrain;
    //World                   world;


    //for the intro:
    int                     xSineBlock;
    int                     ySineBlock;
    int                     ampSineBlock;
};