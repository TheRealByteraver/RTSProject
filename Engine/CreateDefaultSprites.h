#pragma once

#include "Terrain.h"
#include "Sprite.h"
#include "Globals.h"

// the default width of one tile used to create the terrain
// a value of 64 will allocate 1 GB of memory on a 256x256 tile sized terrain lol
#define WORLD_TILE_DEFAULT_WIDTH    32

// there are 64 tiles, most of whom are not used (black)
#define NR_OF_TILES                 64

// the number of pixels between two tiles in the sprite library
#define TILE_SEPARATOR              1

// the initial offset of the first tile in the sprite library
#define TILE_X_OFFSET               1
#define TILE_Y_OFFSET               1

// this defines how wide the sprite .bmp file will be
#define TILES_PER_ROW               8

// the size of the bitmap file containing the sprite library
#define TERRAIN_SPRITE_LIB_WIDTH    (TILE_X_OFFSET + TILES_PER_ROW * \
                                    (WORLD_TILE_DEFAULT_WIDTH + TILE_SEPARATOR))
#if ( NR_OF_TILES % TILES_PER_ROW == 0 )
    #define NR_OF_TILE_ROWS         (NR_OF_TILES / TILES_PER_ROW)
#else 
    #define NR_OF_TILE_ROWS         (NR_OF_TILES / TILES_PER_ROW + 1)
#endif

#define TERRAIN_SPRITE_LIB_HEIGHT   (TILE_Y_OFFSET + NR_OF_TILE_ROWS * \
                                    (WORLD_TILE_DEFAULT_WIDTH + TILE_SEPARATOR))

// some color constants for the greenprairie world drawing function:
#define T_LOW_WATER_COLOR           0x202080
#define T_LOW_COLOR                 0x208020
#define T_HIGH_COLOR                0x20F020
#define T_HIGH_WATER_COLOR          0x2020F0
#define T_TRANS_SHADE_COLOR         0x00D000
#define T_TRANS_DARKSHADE_COLOR     0x00B000
#define T_TRANS_SUN_COLOR           0x40F040
#define T_TRANS_LIGHTSUN_COLOR      0x60FF60

// some color constants for the desert world drawing function:
#define T_LOW_TARPIT_COLOR              0x101010
#define T_DESERT_LOW_COLOR              0xB49999
#define T_DESERT_HIGH_COLOR             0xE7C987
//#define T_HIGH_WATER_COLOR              0x2020F0
#define T_DESERT_TRANS_SHADE_COLOR      0xD7B977
#define T_DESERT_TRANS_DARKSHADE_COLOR  0xC7A967
#define T_DESERT_TRANS_SUN_COLOR        0xFEE09E
#define T_DESERT_TRANS_LIGHTSUN_COLOR   0xFFE29F


class CreateDefaultSprites
{
public:
    CreateDefaultSprites() { }
    // greenprairie world:
    void    drawTerrainSprites();
    void    saveTerrainSprites();
    void    writeWorldIni();
    // desert world:
    void    drawDesertWorldTiles();
    void    saveDesertWorldTiles();
    void    writeDesertWorldIni();
    //const Sprite& getSpriteLibrary() { return terrainSpriteLib; } // for debugging
private:
    Sprite  terrainSpriteLib;
private:
    void    setDrawCoords(
                int& x1,int& y1,int& x2,int& y2,
                const int spriteNr,
                const int width,const int height );
};







