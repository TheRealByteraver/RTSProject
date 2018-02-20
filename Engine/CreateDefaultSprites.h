#pragma once

#include "Terrain.h"
#include "Sprite.h"
#include "Globals.h"

// the default width of one tile used to create the terrain
// change this value to zoom in or out
#define WORLD_TILE_DEFAULT_WIDTH    16

// there are 64 tiles, some of whom are not used (black)
// do not change this value, or program will crash
#define NR_OF_TILES                 64

// the number of pixels between two tiles in the sprite library
#define TILE_SEPARATOR              1

// the initial offset of the first tile in the sprite library
#define TILE_X_OFFSET               1
#define TILE_Y_OFFSET               1

// this defines how wide the sprite .bmp file will be
// do not change this, the create doodad functions rely on it
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


class CreateDefaultSprites
{
public:
    CreateDefaultSprites() { }
    void    createGreenPrairieWorld();
    void    createDesertWorld();
    const Sprite& getSpriteLibrary() { return terrainSpriteLib_; } // for debugging
private:
    Sprite  terrainSpriteLib_;  
    int     width_ = WORLD_TILE_DEFAULT_WIDTH;
    int     height_ = WORLD_TILE_DEFAULT_WIDTH;
private:
    void    createGreenPrairieWorldTiles();
    //void    createGreenPrairieWorldDoodAdds(); // TODO!
    void    createDesertWorldTiles();
    void    createDesertWorldDoodAdds();
    void    createSmoothEdgeDoodAds( 
                int xDest,int yDest,
                const Sprite& tiles,
                int sourceTileRow,
                Color color );
    void    setDrawCoords(
                int& x1,int& y1,int& x2,int& y2,
                const int tileNr,
                const int width,const int height );
};







