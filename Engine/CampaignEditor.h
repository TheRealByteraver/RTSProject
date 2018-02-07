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
    void            createDoodadIconList();
    void            drawTerrain();
    void            drawTerrainGrid();
    void            drawTerrainCursor();
    void            drawMiniMap();
    void            drawMiniMapCursor();
private:
    MainWindow*     wnd_;
    Graphics*       gfx_;
    Font*           font_;
    Mouse*          mouse_;
    Keyboard*       keyboard_;
    GameScreens     gameScreens_;
    World           world_;
    Terrain         terrain_;
    Sprite          miniMap_;
    Sprite          doodadIconList_;
    // this keeps track of which doodad starts at which y value 
    // in the above sprite
    std::vector<int> doodadIconListYvalues_;

    bool            isInitialized_ = false;

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
    /* 
        A zoom level of 1 means 1 pixel per 4 tiles (2x2), because the minimap is 
        128 pixels wide and the largest map is 256 tiles wide / high.
        A zoom level of 2 means 1 pixel per 1 tile (1x1), ideal for when the map
        is 128x128.
        A zoom level of 3 means 2x2 pixels per tile (1x1), ideal for when the map
        is 64x64 (or smaller)
    */
    int             miniMapZoom_ = 2;
};
