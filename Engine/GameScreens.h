
#pragma once

#include "Colors.h"
#include "Graphics.h"
#include "Globals.h"
#include "Sprite.h"

/*
Start of Game screen drawing class
*/

// miscellaneous constants
#define GRID_COLOR                  Colors::Gray
#define CURSOR_COLOR                Colors::Red

// basic coordinates
#define ANCHOR_SCREEN_LEFT          0
#define ANCHOR_SCREEN_RIGHT         (Graphics::ScreenWidth - 1)
#define ANCHOR_SCREEN_TOP           0
#define ANCHOR_SCREEN_BOTTOM        (Graphics::ScreenHeight - 1)

// general relative coordinates & spaces
#define FRAME_WIDTH                 3
#define FONT_HEIGHT                 16
#define TEXT_OFFSET                 FRAME_WIDTH
#define MENU_BAR_HEIGHT             (FONT_HEIGHT + 2 * FRAME_WIDTH)
#define MINI_MAP_CLIENT_WIDTH       128//MAX_TERRAIN_WIDTH
#define MINI_MAP_CLIENT_HEIGHT      128//MAX_TERRAIN_HEIGHT

// specific coordinates relative to ANCHOR_SCREEN_LEFT,ANCHOR_SCREEN_RIGHT:
#define MENU_BAR_X1                 ANCHOR_SCREEN_LEFT
#define MENU_BAR_X2                 ANCHOR_SCREEN_RIGHT
#define MENU_BAR_Y1                 ANCHOR_SCREEN_TOP
#define MENU_BAR_Y2                 (MENU_BAR_Y1 + MENU_BAR_HEIGHT - 1)

#define SIDEBAR_X1                  (ANCHOR_SCREEN_RIGHT       \
                                     - MINI_MAP_CLIENT_WIDTH   \
                                     - 4 * FRAME_WIDTH + 1)
#define SIDEBAR_X2                  ANCHOR_SCREEN_RIGHT
#define SIDEBAR_Y1                  (MENU_BAR_Y2 + 1)
#define SIDEBAR_Y2                  ANCHOR_SCREEN_BOTTOM
#define SIDEBAR_HEIGHT              (SIDEBAR_Y2 - SIDEBAR_Y1 + 1)

#define MAP_AREA_X1                 ANCHOR_SCREEN_LEFT
#define MAP_AREA_X2                 (SIDEBAR_X1 - 1)
#define MAP_AREA_Y1                 (MENU_BAR_Y2 + 1)
#define MAP_AREA_Y2                 ANCHOR_SCREEN_BOTTOM

// the following coords are relative to the origin SIDEBAR_X1,SIDEBAR_Y1:
#define MINIMAP_X1                  FRAME_WIDTH
#define MINIMAP_X2                  (MINIMAP_X1 + MINI_MAP_CLIENT_WIDTH + \
                                    2 * FRAME_WIDTH - 1)
#define MINIMAP_Y1                  FRAME_WIDTH
#define MINIMAP_Y2                  (MINIMAP_Y1 + MINI_MAP_CLIENT_HEIGHT + \
                                    2 * FRAME_WIDTH - 1)
#define MINIMAP_CLIENT_X1           (MINIMAP_X1 + FRAME_WIDTH)
#define MINIMAP_CLIENT_X2           (MINIMAP_X2 - FRAME_WIDTH)
#define MINIMAP_CLIENT_Y1           (MINIMAP_Y1 + FRAME_WIDTH)
#define MINIMAP_CLIENT_Y2           (MINIMAP_Y2 - FRAME_WIDTH)

/*
#define TERRAIN_ICON_WIDTH          ((SIDEBAR_X2 - SIDEBAR_X1 + 1 - FRAME_WIDTH * 2) / 4 - FRAME_WIDTH * 2)
#define TERRAIN_ICON1_X1            (FRAME_WIDTH * 2)
#define TERRAIN_ICON1_X2            (TERRAIN_ICON1_X1 + TERRAIN_ICON_WIDTH - 1)
#define TERRAIN_ICON1_Y1            (MINIMAP_Y2 + FRAME_WIDTH * 2 + FONT_HEIGHT + TEXT_OFFSET * 2)
#define TERRAIN_ICON1_Y2            (TERRAIN_ICON1_Y1 + TERRAIN_ICON_WIDTH - 1)

#define TERRAIN_ICON2_X1            (TERRAIN_ICON1_X2 + 1 + 2 * FRAME_WIDTH)
#define TERRAIN_ICON2_X2            (TERRAIN_ICON2_X1 + TERRAIN_ICON_WIDTH - 1)
#define TERRAIN_ICON2_Y1            TERRAIN_ICON1_Y1
#define TERRAIN_ICON2_Y2            TERRAIN_ICON1_Y2

#define TERRAIN_ICON3_X1            (TERRAIN_ICON2_X2 + 1 + 2 * FRAME_WIDTH)
#define TERRAIN_ICON3_X2            (TERRAIN_ICON3_X1 + TERRAIN_ICON_WIDTH - 1)
#define TERRAIN_ICON3_Y1            TERRAIN_ICON1_Y1
#define TERRAIN_ICON3_Y2            TERRAIN_ICON1_Y2

#define TERRAIN_ICON4_X1            (TERRAIN_ICON3_X2 + 1 + 2 * FRAME_WIDTH)
#define TERRAIN_ICON4_X2            (TERRAIN_ICON4_X1 + TERRAIN_ICON_WIDTH - 1)
#define TERRAIN_ICON4_Y1            TERRAIN_ICON1_Y1
#define TERRAIN_ICON4_Y2            TERRAIN_ICON1_Y2
*/
#define PALLETTE_SELECTOR_X1        MINIMAP_X1
#define PALLETTE_SELECTOR_X2        MINIMAP_X2
#define PALLETTE_SELECTOR_Y1        (MINIMAP_Y2 + 1)
#define PALLETTE_SELECTOR_Y2        (PALLETTE_SELECTOR_Y1 + FONT_HEIGHT + 2 * FRAME_WIDTH)

#define PALLETTE_SELECTOR_CLIENT_X1 (PALLETTE_SELECTOR_X1 + FRAME_WIDTH)
#define PALLETTE_SELECTOR_CLIENT_X2 (PALLETTE_SELECTOR_X2 - FRAME_WIDTH)
#define PALLETTE_SELECTOR_CLIENT_Y1 (PALLETTE_SELECTOR_Y1 + FRAME_WIDTH)
#define PALLETTE_SELECTOR_CLIENT_Y2 (PALLETTE_SELECTOR_Y2 - FRAME_WIDTH)

#define PALETTE_WINDOW_X1           MINIMAP_X1           
#define PALETTE_WINDOW_X2           MINIMAP_X2
#define PALETTE_WINDOW_Y1           (PALLETTE_SELECTOR_Y2 + 1)
#define PALETTE_WINDOW_Y2           (SIDEBAR_HEIGHT - FRAME_WIDTH - 1)

#define PALETTE_CLIENT_WINDOW_X1    (PALETTE_WINDOW_X1 + FRAME_WIDTH)
#define PALETTE_CLIENT_WINDOW_X2    (PALETTE_WINDOW_X2 - FRAME_WIDTH)
#define PALETTE_CLIENT_WINDOW_Y1    (PALETTE_WINDOW_Y1 + FRAME_WIDTH)
#define PALETTE_CLIENT_WINDOW_Y2    (PALETTE_WINDOW_Y2 - FRAME_WIDTH)

/*
    coordinates for mouse handling
*/
// first the scrolling area's:
#define SENSITIVE_MARGIN            32

#define SCROLL_MAP_LEFT_X1          MAP_AREA_X1
#define SCROLL_MAP_LEFT_X2          (MAP_AREA_X1 + SENSITIVE_MARGIN)
#define SCROLL_MAP_LEFT_Y1          MAP_AREA_Y1
#define SCROLL_MAP_LEFT_Y2          MAP_AREA_Y2

#define SCROLL_MAP_RIGHT_X1         (MAP_AREA_X2 - SENSITIVE_MARGIN)
#define SCROLL_MAP_RIGHT_X2         MAP_AREA_X2
#define SCROLL_MAP_RIGHT_Y1         MAP_AREA_Y1
#define SCROLL_MAP_RIGHT_Y2         MAP_AREA_Y2

#define SCROLL_MAP_UP_X1            MAP_AREA_X1
#define SCROLL_MAP_UP_X2            MAP_AREA_X2
#define SCROLL_MAP_UP_Y1            ANCHOR_SCREEN_TOP // MAP_AREA_Y1
#define SCROLL_MAP_UP_Y2            (MAP_AREA_Y1 + SENSITIVE_MARGIN)

#define SCROLL_MAP_DOWN_X1          MAP_AREA_X1
#define SCROLL_MAP_DOWN_X2          MAP_AREA_X2
#define SCROLL_MAP_DOWN_Y1          (MAP_AREA_Y2 - SENSITIVE_MARGIN)
#define SCROLL_MAP_DOWN_Y2          MAP_AREA_Y2

#define PALETTE_SELECTOR_X1         (SIDEBAR_X1 + PALLETTE_SELECTOR_CLIENT_X1)
#define PALETTE_SELECTOR_X2         (SIDEBAR_X1 + PALLETTE_SELECTOR_CLIENT_X2)
#define PALETTE_SELECTOR_Y1         (SIDEBAR_Y1 + PALLETTE_SELECTOR_CLIENT_Y1)
#define PALETTE_SELECTOR_Y2         (SIDEBAR_Y1 + PALLETTE_SELECTOR_CLIENT_Y2)

#define PALETTE_WINDOW_ABS_X1       (SIDEBAR_X1 + PALETTE_CLIENT_WINDOW_X1)
#define PALETTE_WINDOW_ABS_X2       (SIDEBAR_X1 + PALETTE_CLIENT_WINDOW_X2)
#define PALETTE_WINDOW_ABS_Y1       (SIDEBAR_Y1 + PALETTE_CLIENT_WINDOW_Y1)
#define PALETTE_WINDOW_ABS_Y2       (SIDEBAR_Y1 + PALETTE_CLIENT_WINDOW_Y2)

/*
#define TERRAIN_ICON1ABS_X1         (SIDEBAR_X1 + TERRAIN_ICON1_X1) 
#define TERRAIN_ICON1ABS_X2         (TERRAIN_ICON1ABS_X1 + TERRAIN_ICON_WIDTH - 1)
#define TERRAIN_ICON1ABS_Y1         (SIDEBAR_Y1 + TERRAIN_ICON1_Y1)
#define TERRAIN_ICON1ABS_Y2         (TERRAIN_ICON1ABS_Y1 + TERRAIN_ICON_WIDTH - 1)

#define TERRAIN_ICON2ABS_X1         (SIDEBAR_X1 + TERRAIN_ICON2_X1)
#define TERRAIN_ICON2ABS_X2         (TERRAIN_ICON2ABS_X1 + TERRAIN_ICON_WIDTH - 1)
#define TERRAIN_ICON2ABS_Y1         TERRAIN_ICON1ABS_Y1
#define TERRAIN_ICON2ABS_Y2         TERRAIN_ICON1ABS_Y2

#define TERRAIN_ICON3ABS_X1         (SIDEBAR_X1 + TERRAIN_ICON3_X1)
#define TERRAIN_ICON3ABS_X2         (TERRAIN_ICON3ABS_X1 + TERRAIN_ICON_WIDTH - 1)
#define TERRAIN_ICON3ABS_Y1         TERRAIN_ICON1ABS_Y1
#define TERRAIN_ICON3ABS_Y2         TERRAIN_ICON1ABS_Y2

#define TERRAIN_ICON4ABS_X1         (SIDEBAR_X1 + TERRAIN_ICON4_X1)
#define TERRAIN_ICON4ABS_X2         (TERRAIN_ICON4ABS_X1 + TERRAIN_ICON_WIDTH - 1)
#define TERRAIN_ICON4ABS_Y1         TERRAIN_ICON1ABS_Y1
#define TERRAIN_ICON4ABS_Y2         TERRAIN_ICON1ABS_Y2
*/


class GameScreens
{
public:
    GameScreens() { }
    void            drawScenarioEditor();
    const Sprite&   menuBar() { return menuBar_; }
    const Sprite&   sideBar() { return sideBar_; }
    void            setFont( Font *font ) { font_ = font; }
    Font            *getFont() { return font_; }
public:
    // absolute coordinates for drawing and mouse:
    const Color menuColor = Color( 226,193,170 );
    const Rect screen_coords = { ANCHOR_SCREEN_LEFT,ANCHOR_SCREEN_TOP,ANCHOR_SCREEN_RIGHT,ANCHOR_SCREEN_BOTTOM };
    const Rect menubar_coords = { MENU_BAR_X1,MENU_BAR_Y1,MENU_BAR_X2,MENU_BAR_Y2 };
    const Rect sidebar_coords = { SIDEBAR_X1,SIDEBAR_Y1,SIDEBAR_X2,SIDEBAR_Y2 };
    const Rect map_coords = { MAP_AREA_X1,MAP_AREA_Y1,MAP_AREA_X2,MAP_AREA_Y2 };

    // coordinates for drawing only (relative coordinates)
    const Rect minimap_coords = { MINIMAP_X1,MINIMAP_Y1,MINIMAP_X2,MINIMAP_Y2 };
    const Rect minimapclient_coords = { MINIMAP_CLIENT_X1,MINIMAP_CLIENT_Y1,MINIMAP_CLIENT_X2,MINIMAP_CLIENT_Y2 };
    const Rect paletteselector_coords = { PALLETTE_SELECTOR_X1,PALLETTE_SELECTOR_Y1,PALLETTE_SELECTOR_X2,PALLETTE_SELECTOR_Y2 };
    const Rect paletteselectorclient_coords = { PALLETTE_SELECTOR_CLIENT_X1,PALLETTE_SELECTOR_CLIENT_Y1,PALLETTE_SELECTOR_CLIENT_X2,PALLETTE_SELECTOR_CLIENT_Y2 };
    const Rect palettewindow_coords = { PALETTE_WINDOW_X1,PALETTE_WINDOW_Y1,PALETTE_WINDOW_X2,PALETTE_WINDOW_Y2 };
    const Rect paletteclientwindow_coords = { PALETTE_CLIENT_WINDOW_X1,PALETTE_CLIENT_WINDOW_Y1,PALETTE_CLIENT_WINDOW_X2,PALETTE_CLIENT_WINDOW_Y2 };

    // mouse sensitive area's for map scrolling action (absolute coordinates, for the mouse):
    const Rect scrollMapLeft = { SCROLL_MAP_LEFT_X1,SCROLL_MAP_LEFT_Y1,SCROLL_MAP_LEFT_X2,SCROLL_MAP_LEFT_Y2 };
    const Rect scrollMapRight = { SCROLL_MAP_RIGHT_X1,SCROLL_MAP_RIGHT_Y1,SCROLL_MAP_RIGHT_X2,SCROLL_MAP_RIGHT_Y2 };
    const Rect scrollMapUp = { SCROLL_MAP_UP_X1,SCROLL_MAP_UP_Y1,SCROLL_MAP_UP_X2,SCROLL_MAP_UP_Y2 };
    const Rect scrollMapDown = { SCROLL_MAP_DOWN_X1,SCROLL_MAP_DOWN_Y1,SCROLL_MAP_DOWN_X2,SCROLL_MAP_DOWN_Y2 };
    const Rect paletteSelector = { PALETTE_SELECTOR_X1,PALETTE_SELECTOR_Y1,PALETTE_SELECTOR_X2,PALETTE_SELECTOR_Y2 };
    const Rect paletteWindow = { PALETTE_WINDOW_ABS_X1,PALETTE_WINDOW_ABS_Y1,PALETTE_WINDOW_ABS_X2,PALETTE_WINDOW_ABS_Y2 };

private:
    Sprite      menuBar_,sideBar_;
    Font*       font_;
};



