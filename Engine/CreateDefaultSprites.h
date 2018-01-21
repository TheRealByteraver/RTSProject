#pragma once

#include "Terrain.h"
#include "Sprite.h"
#include "Globals.h"

// the default width of one tile used to create the terrain
// a value of 64 will allocate 1 GB of memory on a 256x256 tile sized terrain lol
#define WORLD_TILE_DEFAULT_WIDTH    48

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
#define T_DESERT_LOW_TARPIT_COLOR       0x101010
#define T_DESERT_LOW_COLOR              0xE7C987
#define T_DESERT_HIGH_COLOR             0xCD8F6D
#define T_DESERT_HIGH_WATER_COLOR       0x2020F0
// hill colors from Brightest to darkest:
#define T_DESERT_TOP_RIGHT_COLOR        0xFABC9A     // 1
#define T_DESERT_TOP_COLOR              0xEAAC8A     // 2
#define T_DESERT_RIGHT_COLOR            0xDA9C7A     // 3
#define T_DESERT_TOP_LEFT_COLOR         0xCA8C6A     // 4
#define T_DESERT_BOTTOM_RIGHT_COLOR     0xBA7C5A     // 5
#define T_DESERT_BOTTOM_COLOR           0xAA6C4A     // 6
#define T_DESERT_LEFT_COLOR             0x9A5C3A     // 7
#define T_DESERT_BOTTOM_LEFT_COLOR      0x8A4C2A     // 8
/*
#define T_DESERT_TRANS_SHADE_COLOR      0xD7B977
#define T_DESERT_TRANS_DARKSHADE_COLOR  0xC7A967
#define T_DESERT_TRANS_SUN_COLOR        0xFEE09E
#define T_DESERT_TRANS_LIGHTSUN_COLOR   0xFFE29F
*/


class CreateDefaultSprites
{
public:
    CreateDefaultSprites() { }
    void    createGreenPrairieWorldTiles();
    //void    createGreenPrairieWorldDoodAdds(); // TODO!
    void    createDesertWorldTiles();
    void    createDesertWorldDoodAdds();
    const Sprite& getSpriteLibrary() { return terrainSpriteLib; } // for debugging
private:
    Sprite  terrainSpriteLib;                                     // for debugging declared here
private:
    void    setDrawCoords(
                int& x1,int& y1,int& x2,int& y2,
                const int tileNr,
                const int width,const int height );
};







