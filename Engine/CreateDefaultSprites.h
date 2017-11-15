#pragma once

#include "TerrainEditor.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Globals.h"

#define T_LOW_WATER_COLOR       0x202080
#define T_LOW_COLOR             0x208020
#define T_HIGH_COLOR            0x20F020
#define T_HIGH_WATER_COLOR      0x2020F0
#define T_TRANS_SHADE_COLOR     0x00D000
#define T_TRANS_DARKSHADE_COLOR 0x00B000
#define T_TRANS_SUN_COLOR       0x40F040
#define T_TRANS_LIGHTSUN_COLOR  0x60FF60

class CreateDefaultSprites
{
public:
    CreateDefaultSprites() {}
    void init( Graphics *gfx_ )
    {
        gfxPtr = gfx_;
        Graphics& gfx = *gfxPtr;
        drawTerrainSprites();
        Sprite screenCapture;
        screenCapture.captureFromMemory( 
            Rect( 0,0,gfx.ScreenWidth - 1,gfx.ScreenHeight - 1 ),
            gfx.getpSysBuffer(),
            gfx.ScreenWidth );
        // now save the sprites to a .bmp file:
        screenCapture.saveToBMP( INIFILE_FOLDER TERRAIN_SPRITE_LIB_FILENAME );
    }
    void drawTerrainSprites();
private:
    Graphics *gfxPtr = nullptr;
    void setDrawCoords(
        int& x1,int& y1,int& x2,int& y2,
        const int spriteNr,const int width,const int height );
};







