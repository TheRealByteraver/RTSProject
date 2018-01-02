#pragma once

#include "TerrainEditor.h"
//#include "Graphics.h"
#include "Sprite.h"
#include "Globals.h"

#define TERRAIN_SPRITE_LIB_FILENAME "GreenPrairie.bmp"  // should be loaded from MASTER_INIFILE_FILENAME
#define TERRAIN_WORLD_FILENAME      "GreenPrairie.ini"  // should be loaded from MASTER_INIFILE_FILENAME

// the default width of one tile used to create the terrain
#define WORLD_TILE_DEFAULT_WIDTH    32

// the size of the bitmap file containing the sprite library
#define TERRAIN_SPRITE_LIB_WIDTH    1280

// there are 64 tiles, most of whom are not used (black)
#define NR_OF_TILES                 64

// the number of pixels between two tiles in the sprite library
#define TILE_SEPARATOR              1

// the initial offset of the first tile in the sprite library
#define TILE_X_OFFSET               1
#define TILE_Y_OFFSET               1

#define TILES_PER_ROW               ((TERRAIN_SPRITE_LIB_WIDTH - TILE_X_OFFSET) / \
                                    (WORLD_TILE_DEFAULT_WIDTH + TILE_SEPARATOR))
#define NR_OF_TILE_ROWS             (NR_OF_TILES / TILES_PER_ROW + 1)
#define TERRAIN_SPRITE_LIB_HEIGHT   (TILE_Y_OFFSET + NR_OF_TILE_ROWS * \
                                    (WORLD_TILE_DEFAULT_WIDTH + TILE_SEPARATOR))

// some color constants for the drawing functions
#define T_LOW_WATER_COLOR           0x202080
#define T_LOW_COLOR                 0x208020
#define T_HIGH_COLOR                0x20F020
#define T_HIGH_WATER_COLOR          0x2020F0
#define T_TRANS_SHADE_COLOR         0x00D000
#define T_TRANS_DARKSHADE_COLOR     0x00B000
#define T_TRANS_SUN_COLOR           0x40F040
#define T_TRANS_LIGHTSUN_COLOR      0x60FF60

class CreateDefaultSprites
{
public:
    CreateDefaultSprites() { drawTerrainSprites(); }
    const Sprite& getSpriteLibrary() { return terrainSpriteLib; }
private:
    Sprite  terrainSpriteLib;
private:
    void    drawTerrainSprites();
    void    setDrawCoords(
                int& x1,int& y1,int& x2,int& y2,
                const int spriteNr,const int width,const int height );
    void    writeWorldIni();
};







