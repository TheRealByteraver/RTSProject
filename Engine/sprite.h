#pragma once

#include <fstream>
#include <assert.h>
#include "font.h"
#include "colors.h"
#include "graphics.h"

// forward declarations for stupid linker:
struct Rect;
class Font;

// for changeing the egdes of a bitmap to create a block like effect
#define BLOCK_EDGE_COLOR_DELTA              60
#define BROKEN_IMAGE_SPRITE_WIDTH           16
#define BROKEN_IMAGE_SPRITE_HEIGHT          16

#pragma pack(push)
#pragma pack(1)
struct BitmapHeader
{
    char            charB;
    char            charM;
    unsigned        fileSize;
    unsigned short  reserved[2];
    unsigned        imageDataOffset;
};
struct BitmapInfoHeader
{
    unsigned        headerSize;             
    int             width;
    int             height;
    unsigned short  nrOfColorPlanes;
    unsigned short  bitsPerPixel;
    unsigned        compressionType;
    unsigned        imageSize;
    unsigned        horizontalResolution;
    unsigned        verticalResolution;
    unsigned        nrColorsInPalette;
    unsigned        nrOfImportantColorsUsed;
};
#pragma pack(pop)

class Sprite
{
public:
    static const Color brokenImageSpriteData
        [BROKEN_IMAGE_SPRITE_WIDTH * BROKEN_IMAGE_SPRITE_HEIGHT];
public:
    Sprite() : Sprite( 
        BROKEN_IMAGE_SPRITE_WIDTH,
        BROKEN_IMAGE_SPRITE_HEIGHT,
        brokenImageSpriteData
        ) {}
    Sprite( int width_,int height_,const Color *pixelData_ );
    Sprite( const char *fileName );
    ~Sprite();
    Sprite& operator=( const Sprite& sourceSprite );
    int     getWidth() const { return width; }
    int     getHeight() const { return height; }
    bool    isImagePresent() const { return (pixelData != nullptr); }
    Color  *getPixelData() const { assert( isImagePresent() ); return pixelData; }
    void    unloadSprite();
    int     loadBitmap( const char *fileName );
    //void    loadFromMemory( int width_,int height_,Color *pixelData_ );
    void    captureFromMemory( Rect area,Color *source,const int sourceWidth );
    void    createEmptySprite( int width_,int height_ );
    void    createEmptySprite( int width_,int height_,Color fillColor );
    //int     loadPNG( const char *filename );
    int     loadFromBMP( const char *fileName, Rect area );
    int     saveToBMP( const char *fileName );
    bool    isEmpty( Rect area,Color color );
    void    fill( Color color );
    void    putPixel( int i,Color c );
    void    putPixel( int x,int y,Color c );
    Color   getPixel( int x,int y ) const;
    void    copyFromSprite( const Sprite& source, Rect area );
    void    insertFromSprite( int x,int y,const Sprite& source );
    void    printXY( int x,int y,const char *s,Font *font );
    //void    printXY( void *gfx,int x,int y,const char *s,Font *font );
    //void    printXY( int x,int y,const char *s,Font *font );
    Rect    getSpriteCoordsAt( int x,int y );
    void    makeButtonEdges();
    /*
    int saveBitmapTNTFontFile();
    int saveMonoCTNTFontFile();
    */
private:
    int     width;
    int     height;
    Color   *pixelData;
private:
    void    loadBrokenImageSpriteData();
};





