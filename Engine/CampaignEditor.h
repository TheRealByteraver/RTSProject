#pragma once

#include <memory>

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
#include "WindowSystem.h"



constexpr auto MENU_FILE_NEW = 0;
constexpr auto MENU_FILE_OPEN = 1;
constexpr auto MENU_FILE_SAVE = 2;
constexpr auto MENU_FILE_SAVE_AS = 3;
constexpr auto MENU_FILE_EXIT = 4;
constexpr auto MENU_FILE_NR_MENUS = 5;

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
    int     xOrig_ = 0;
    int     yOrig_ = 0;
    Font   *font_ = nullptr;
    int     nrOfMenus_ = 0;
    Sprite  image_;
    Rect    dimensions_ = Rect( 0,0,0,0 );
};


// Mouse delay timer constants, in frames (1/60 of a second)
constexpr auto MOUSE_LEFT_CLICK_DELAY = 15;
constexpr auto MOUSE_SCROLL_START_DELAY = 20;
constexpr auto MOUSE_NO_DELAY = 0;
constexpr auto MOUSE_SCROLL_SPEED = 10;  // higher = slower

// keep track of which palette is visible in the side bar:
constexpr auto BASIC_TERRAIN_PALETTE = 0;
constexpr auto DOODAD_PALETTE = 1;
constexpr auto NR_OF_PALETTES = 2;

/*
For the minimap.
A zoom level of 0 means 1 pixel per 4 tiles (2x2), because the minimap is
128 pixels wide and the largest map is 256 tiles wide / high.
A zoom level of 1 means 1 pixel per 1 tile (1x1), ideal for when the map
is 128x128.
A zoom level of 2 means 2x2 pixels per tile (1x1), ideal for when the map
is 64x64 (or smaller)
*/

constexpr auto ZOOM_QUARTER_PIXEL_PER_TILE = 0;
constexpr auto ZOOM_ONE_PIXEL_PER_TILE = 1;
constexpr auto ZOOM_FOUR_PIXELS_PER_TILE = 2;

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
    CampaignEditor() {}
    void    init( 
        MainWindow& wndRef,
        Graphics& gfxRef,
        Font& fontRef,
        Mouse& mouseRef,
        Keyboard& keyboardRef
    );
    /*
    ~CampaignEditor() 
    {
        if ( doodadLocationMap_ != nullptr )
            delete doodadLocationMap_;
    }
    */
    void            draw();
    void            handleInput();
    bool            isInitialized() const { return isInitialized_; }
    void            bootCampaignEditor()
    {
        campaignEditorReadyForShutdown_ = false;
    }
    bool            campaignEditorIsReadyForShutdown()
    {
        return campaignEditorReadyForShutdown_;
    }
private:
    void            populateFileList( const std::string path,const std::string extension );
    void            handleMapScrollingFunction();
    void            handlePaletteScrollingFunction();
    void            switchToNextPalette();
    void            paletteHandleInput( int mY );
    void            drawBasicTerrain( int tileX,int tileY );
    void            tryDrawDoodad( int tileX,int tileY );
    void            menuFileHandleInput( int mY );
    void            submenuHandleInput( int mX,int mY );
    void            menuFileNewDrawSubmenu();
    void            menuFileNewHandleInput( int mX,int mY );
    void            menuFileOpenDrawSubmenu();
    void            menuFileOpenHandleInput( int mX,int mY );
    void            menuFileExitDrawSubmenu();
    void            menuFileExitHandleInput( int mX,int mY );
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
    void            shutdownCampaignEditor()
    {
        campaignEditorReadyForShutdown_ = true;
    }
private:
    bool            campaignEditorReadyForShutdown_;
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

    // This section contains al kinds of variables for the menu system
    // and the windows as well as their input handling:
    // Menu title list and coordinate handling:
    Menu            fileMenu_;
    // keeping track what window or menu is visible (active):
    bool            menuFileVisible_ = false;
    bool            submenuVisible_ = false;

    // this variable contains a list of filenames, used by menu functions
    std::vector<std::string> fileList_;

    // these variables are winElements that can be grouped together in a 
    // horizontal WinElementBar object
    VerticalRadiobuttonGroup    radiobuttonGroup0_;
    VerticalRadiobuttonGroup    radiobuttonGroup1_;
    ButtonList                  buttonList0_;
    ButtonList                  buttonList1_;
    TextPanel                   textPanel0_;
    // a Win Element Bar is a horizontal strip of win Elements such as the ones 
    // declared above
    WinElementBar               winElementBar0_; 
    WinElementBar               winElementBar1_;
    WinElementBar               winElementBar2_;
    // a Win Element Bar List is a vertical list of Win Element Bar's
    WinElementBarList           winElementBarList0_;
    int                         activeMenuID_;

    // the two palettes and a pointer to the currently active palette:
    Palette        basicTerrainPalette_;
    Palette        doodadPalette_;
    Palette        *palettePTR_ = &basicTerrainPalette_;
    // tells which palette is currently selected in the editor:
    int             activePalette_ = BASIC_TERRAIN_PALETTE;
    bool            scrollDownLock_; // whether we reached the end of the palette

    // specific variables for the terrain editor:
    // the type of terrain we are drawing with now:
    char            terrainType_ = T_DEFAULT;
    int             doodadNr_ = 0; // which doodad the user selected
    bool            doodadMouseCursor_; // whether the active doodad is shown on the mouse cursor location
    Sprite          doodadCursorSprite_; // the original-sized doodad without the compatibility overlay
    bool            isGridVisible_ = true;
    bool            isTerrainSaved_ = false;
    // this pointer is a two dimensional boolean map that tells if a given 
    // tile has a doodad on top of it or not:
    //bool           *doodadLocationMap_ = nullptr;
    std::unique_ptr<bool[]> doodadLocationMap_;
    // these coordinates keep track of our location (in tiles) on the map:
    int             TerrainDrawXOrig_;
    int             TerrainDrawYOrig_;
    // nr of visible tiles in Horizontal and vertical direction:
    int             visibleTilesX_;
    int             visibleTilesY_;
    // width & height of the tiles in pixels:
    int             tileWidth_;
    int             tileHeight_;
    // these variables are used to draw the minimap (the radar screen):
    struct Minimap
    {
        int             xOrig = 0;
        int             yOrig = 0;
        Sprite          image;
        Rect            cursorCoords = Rect( 0,0,0,0 );
        int             zoomLevel = ZOOM_ONE_PIXEL_PER_TILE;
    } minimap_;
};
