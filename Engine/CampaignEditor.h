#pragma once

#include "MainWindow.h"
#include "graphics.h"
#include "Globals.h"
#include "gameScreens.h"
#include "World.h"
#include "Terrain.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sprite.h"
#include "Font.h"

#define BASIC_TERRAIN_PALETTE           0
#define DOODAD_PALETTE                  1
#define NR_OF_PALETTES                  2

/*
For the minimap.
A zoom level of 0 means 1 pixel per 4 tiles (2x2), because the minimap is
128 pixels wide and the largest map is 256 tiles wide / high.
A zoom level of 1 means 1 pixel per 1 tile (1x1), ideal for when the map
is 128x128.
A zoom level of 2 means 2x2 pixels per tile (1x1), ideal for when the map
is 64x64 (or smaller)
*/

#define ZOOM_QUARTER_PIXEL_PER_TILE     0
#define ZOOM_ONE_PIXEL_PER_TILE         1
#define ZOOM_FOUR_PIXELS_PER_TILE       2


class CampaignEditor
{
public:
    CampaignEditor( ) {}
    void    init( 
        MainWindow& wndRef,
        Graphics& gfxRef,
        Font& fontRef,
        Mouse& mouseRef,
        Keyboard& keyboardRef
    );
    void            draw();
    void            handleInput();
    bool            isInitialized() const { return isInitialized_; }
private:
    int             loadTerrain( const std::string& terrainName );
    bool            canPlaceDoodadAtLocation( int x,int y,const Doodad& doodad ) const;
    void            checkDoodads();
    void            initMapCoords();
    void            createBasicTerrainPalette();
    void            createDoodadPalette();
    void            initPalettePointers();
    void            redrawPalette();
    void            drawTerrain();
    void            drawTerrainGrid();
    void            drawTerrainCursor();
    void            drawMiniMap();
    void            drawMiniMapCursor();
private:
    bool            isInitialized_ = false;
    // main system components we got from the constructor in the Game class:
    MainWindow*     wnd_;
    Graphics*       gfx_;
    Font*           font_;
    Mouse*          mouse_;
    Keyboard*       keyboard_;

    // this variable is used for single-click actions:
    int             mouseTimer_ = 0;

    // class variables:
    GameScreens     gameScreens_;
    World           world_;
    Terrain         terrain_;
    Sprite          miniMap_;
    // tells which palette is currently selected in the editor:
    int             activePalette_ = BASIC_TERRAIN_PALETTE;
    int            *paletteListIndexPTR_ = &basicTerrainPaletteIndex_;
    Sprite         *paletteSpritePTR_ = &basicTerrainPalette_;
    std::vector<int> *paletteYvaluesPTR_ = &basicTerrainPaletteYvalues_;
    bool            scrollDownLock_; // whether scrolling down is an option

    Sprite          basicTerrainPalette_;
    Sprite          doodadPalette_;
    // this list keeps track of which palette member icon starts at which y value
    std::vector<int> basicTerrainPaletteYvalues_;
    std::vector<int> doodadPaletteYvalues_;
    // we need to keep track of which palette member is visible:
    int             basicTerrainPaletteIndex_ = 0;
    int             doodadPaletteIndex_ = 0;
    // which icon we are currently drawing with:
    int             basicTerrainSelectedIcon_ = 0;
    int             doodadSelectedIcon_ = 0;

    // specific variables for the terrain editor:
    // the type of terrain we are drawing with now:
    char            terrainType_ = T_DEFAULT;
    bool            isGridVisible_ = true;
    // these coordinates keep track of our location on the map:
    int             TerrainDrawXOrig_ = 0;
    int             TerrainDrawYOrig_ = 0;
    // nr of visible tiles in Horizontal and vertical direction:
    int             visibleTilesX_;
    int             visibleTilesY_;
    // width & height of the tiles in pixels:
    int             tileWidth_;
    int             tileHeight_;
    // these coordinates are used to draw the minimap (the radar screen):
    int             miniMapXOrig_;
    int             miniMapYOrig_;
    Rect            miniMapCursor_;
    int             miniMapZoom_ = ZOOM_ONE_PIXEL_PER_TILE;
};
