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



#define MENU_FILE_NEW               0
#define MENU_FILE_OPEN              1
#define MENU_FILE_SAVE              2
#define MENU_FILE_SAVE_AS           3
#define MENU_FILE_EXIT              4
#define MENU_FILE_NR_MENUS          5

const char *menufiletitles[];
/*
const char *menutitles[] =
{
    "New",
    "Open",
    "Save",
    "Save As",
    "Exit",
    nullptr
};
*/

class Menu
{
public:
    Menu() {}
    void        init( int xOrig,int yOrig,const char *menus[],Font* fontPTR )
    {
        assert( menus[0] != nullptr );
        assert( fontPTR != nullptr );
        assert( xOrig >= 0 );
        assert( yOrig >= 0 );
        font_ = fontPTR;
        xOrig_ = xOrig;
        yOrig_ = yOrig;
        int maxLength = 0;
        for ( nrOfMenus_ = 0; menus[nrOfMenus_] != nullptr; nrOfMenus_++ )
        {
            int s = (int)strlen( menus[nrOfMenus_] );
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
        dimensions_ = Rect(
            xOrig_,
            yOrig_,
            xOrig_ + image_.getWidth() - 1,
            yOrig_ + image_.getHeight() - 1
        );
    }
    Sprite&     getImage() { return image_; }
    Rect        dimensions() { return dimensions_; }
    int         getSelectedSubMenu( int mouseY )
    {
        assert( mouseY >= 0 );
        assert( font_ != nullptr );
        Font& font = *font_;
        if ( (mouseY < dimensions_.y1) || (mouseY > dimensions_.y2) )
            return -1;
        return (mouseY - dimensions_.y1 - FRAME_WIDTH - TEXT_OFFSET) 
            / font.height();
    }
private:
    int     xOrig_;
    int     yOrig_;
    Font   *font_ = nullptr;
    int     nrOfMenus_;
    Sprite  image_;
    Rect    dimensions_ = Rect( 0,0,0,0 );
};


// Mouse delay timer constants, in frames (1/60 of a second)
#define MOUSE_LEFT_CLICK_DELAY          15
#define MOUSE_SCROLL_START_DELAY        20
#define MOUSE_NO_DELAY                  0
#define MOUSE_SCROLL_SPEED              10  // higher = slower

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

class Palette
{
public:
    Palette() 
    {
        listIndex = 0;
    }
    // we need to keep track of which palette member is visible:
    int                 listIndex;
    Sprite              image;
    // this list keeps track of which palette member icon starts at which y value
    std::vector<int>    yValues;
};

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
    void            populateWorldsList();
    void            handleMapScrollingFunction();
    void            handlePaletteScrollingFunction();
    void            switchToNextPalette();
    void            paletteHandleInput( int mY );
    void            drawBasicTerrain( int tileX,int tileY );
    void            tryDrawDoodad( int tileX,int tileY );
    void            menuFileHandleInput( int mY );
    void            menuFileNewHandleInput( int mX,int mY );
    void            menuFileNewDrawSubmenu();
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
    void            drawDoodadCursorAtLocation();
    void            drawTerrain();
    void            drawTerrainGrid();
    void            drawTerrainCursor();
    void            drawMiniMap();
    void            redrawMiniMap();
    void            drawMiniMapCursor();
private:
    bool            isInitialized_ = false;
    // main system components we got from the constructor in the Game class:
    MainWindow*     wnd_;
    Graphics*       gfx_;
    Font*           font_;
    Mouse*          mouse_;
    Keyboard*       keyboard_;

    // this variables are used for mouse control:
    bool            mouseWaitForLeftButtonReleased_ = false;
    int             mouseLeftClickDelay_ = MOUSE_NO_DELAY;
    int             mouseMapScrollDelay_ = MOUSE_SCROLL_START_DELAY;
    int             mousePaletteScrollDelay_ = MOUSE_SCROLL_START_DELAY;
    int             mousePaletteScrollSpeed_ = MOUSE_SCROLL_SPEED;

    // class variables:
    GameScreens     gameScreens_;
    World           world_;
    Terrain         terrain_;
    Menu            fileMenu_;

    // the two palettes and a pointer to the currently active palette:
    Palette        basicTerrainPalette_;
    Palette        doodadPalette_;
    Palette        *palettePTR_ = &basicTerrainPalette_;
    // tells which palette is currently selected in the editor:
    int             activePalette_ = BASIC_TERRAIN_PALETTE;
    bool            scrollDownLock_; // whether we reached the end of the palette

    // For the menu: if the menu is shown or not
    bool            menuFileVisible_ = false;
    bool            submenuVisible_ = false;
    Sprite          submenuImage_;
    Rect            submenuCoords_;

    // contains the file list of all the worlds, is only read once since new
    // worlds can't be created with the campaign editor
    std::vector<std::string> worldsList_;


    // specific variables for the terrain editor:
    // the type of terrain we are drawing with now:
    char            terrainType_ = T_DEFAULT;
    int             doodadNr_ = 0; // which doodad the user selected
    bool            doodadMouseCursor_ = false; // whether the active doodad is shown on the mouse cursor location
    Sprite          doodadCursorSprite_; // the original-sized doodad without the compatibility overlay
    bool            isGridVisible_ = true;
    bool            isTerrainSaved_ = false;
    // this pointer is a two dimensional boolean map that tells if a given 
    // tile has a doodad on top of it or not:
    bool           *doodadLocationMap_ = nullptr;
    // these coordinates keep track of our location (in tiles) on the map:
    int             TerrainDrawXOrig_ = 0;
    int             TerrainDrawYOrig_ = 0;
    // nr of visible tiles in Horizontal and vertical direction:
    int             visibleTilesX_;
    int             visibleTilesY_;
    // width & height of the tiles in pixels:
    int             tileWidth_;
    int             tileHeight_;
    // these variables are used to draw the minimap (the radar screen):
    struct Minimap
    {
        int             xOrig;
        int             yOrig;
        Sprite          image;
        Rect            cursorCoords;
        int             zoomLevel = ZOOM_ONE_PIXEL_PER_TILE;
    } minimap_;
};
