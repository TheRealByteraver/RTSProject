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

const char *menutitles[];

class Menu
{
public:
    Menu() {}
    void    init( const char *menus[],Font* fontPTR )
    {
        assert( menus[0] != nullptr );
        assert( font != nullptr );
        font_ = fontPTR;
        int maxLength = 0;
        for ( nrOfMenus_ = 0; menus[nrOfMenus_] != nullptr; nrOfMenus_++ )
        {
            int s = strlen( menus[nrOfMenus_] );
            if ( s > maxLength ) maxLength = s;
        }
        // Create graphical representation of menu:
        image_.setFont( font_ );
        Font& font = *font_;
        image_.createEmptySprite(
            maxLength * font.width() + 2 * (FRAME_WIDTH + TEXT_OFFSET),
            nrOfMenus_ * font.height() + 2 * (FRAME_WIDTH + TEXT_OFFSET)
        );
        image_.setFrameColor( MENU_COLOR );
        image_.drawNiceBlock( 
            Rect( 0,0,image_.getWidth() - 1,image_.getHeight() - 1 ) 
        );
        for ( int menuNr = 0; menuNr < nrOfMenus_; menuNr++ )
        {
            image_.printXY(
                FRAME_WIDTH + TEXT_OFFSET,
                FRAME_WIDTH + TEXT_OFFSET + menuNr * font.height(),
                menus[menuNr]
            );
        }
    }
    Sprite& getImage() { return image_; }
private:
    Font   *font_;
    int     nrOfMenus_;
    Sprite  image_;
};


// keep track of which palette is visible in the side bar:
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
    ~CampaignEditor() 
    {
        if ( doodadLocationMap_ != nullptr )
            delete doodadLocationMap_;
    }
    void            draw();
    void            handleInput();
    bool            isInitialized() const { return isInitialized_; }
private:
    int             loadTerrain( const std::string& terrainName );
    bool            canPlaceDoodadAtLocation( int x,int y,const Doodad& doodad ) const;
    void            initDoodadLocationMap();
    bool            doodadPresentInArea( Rect area );
    void            checkDoodads();
    void            initMapCoords();
    void            createBasicTerrainPalette();
    void            createDoodadPalette();
    void            initPalettePointers();
    void            redrawPalette();
    void            drawDoodadCursorAtLocation( /*int xTile,int yTile*/ );
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
    Menu            fileMenu_;

    // tells which palette is currently selected in the editor:
    int             activePalette_ = BASIC_TERRAIN_PALETTE;
    int            *paletteListIndexPTR_ = &basicTerrainPaletteIndex_;
    Sprite         *paletteSpritePTR_ = &basicTerrainPalette_;
    std::vector<int> *paletteYvaluesPTR_ = &basicTerrainPaletteYvalues_;
    bool            scrollDownLock_; // whether we reached the end of the palette

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
    int             doodadNr_ = 0; // which doodad the user selected
    bool            doodadMouseCursor_ = false; // whether the active doodad is shown on the mouse cursor location
    Sprite          doodadCursorSprite_; // the original-sized doodad without the compatibility overlay
    bool            isGridVisible_ = true;
    bool           *doodadLocationMap_ = nullptr;
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
