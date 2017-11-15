#include <Windows.h>
#include <string.h>
#include "Graphics.h"
#include "sprite.h"
#include "font.h"
#include "math.h"
#include <stdlib.h>
#include <assert.h>

#pragma comment ( lib,"gdiplus.lib" )


Color const Sprite::brokenImageSpriteData[BROKEN_IMAGE_SPRITE_WIDTH * BROKEN_IMAGE_SPRITE_HEIGHT] = {
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0xFF0000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,
    0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000
};

const Sprite brokenImageSprite = {
    BROKEN_IMAGE_SPRITE_WIDTH,
    BROKEN_IMAGE_SPRITE_HEIGHT,
    Sprite::brokenImageSpriteData
};
void Sprite::loadBrokenImageSpriteData()
{
    assert( width == 0 );
    assert( height == 0 );
    assert( pixelData == nullptr );
    width = BROKEN_IMAGE_SPRITE_WIDTH;
    height = BROKEN_IMAGE_SPRITE_HEIGHT;
    const int size = width * height;
    pixelData = new Color[size];
    for ( int i = 0; i < size; i++ ) 
        pixelData[i] = Sprite::brokenImageSpriteData[i];
}
/*
    This constructor initializes the sprite by copying the 
    (width * height) -sized image from pixelData_ to itself.
*/
Sprite::Sprite( int width_,int height_,const Color *pixelData_ )
{
    assert( width_ > 0 );
    assert( height_ > 0 );
    assert( pixelData_ != nullptr );
    width = width_;
    height = height_;
    const int size = width * height;
    pixelData = new Color[size];
    for ( int i = 0; i < size; i++ ) pixelData[i] = pixelData_[i];
}
Sprite::Sprite( const char *fileName )
{
    width = 0;
    height = 0;
    pixelData = nullptr;
    loadBitmap( fileName );
    if ( pixelData == nullptr ) loadBrokenImageSpriteData();
}
void Sprite::unloadSprite()
{
    width = 0;
    height = 0;
    if ( pixelData != nullptr ) delete pixelData;
    pixelData = nullptr;
}
Sprite::~Sprite()
{
    unloadSprite();
}
Sprite& Sprite::operator=( const Sprite& sourceSprite )
{
    if ( this != &sourceSprite ) {
        unloadSprite();
        if ( sourceSprite.getWidth() <= 0 ) return *this;
        if ( sourceSprite.getHeight() <= 0 ) return *this;
        if ( sourceSprite.getPixelData() == nullptr ) return *this;
        width = sourceSprite.getWidth();
        height = sourceSprite.getHeight();
        int size = width * height;
        pixelData = new Color[size];
        Color *s = sourceSprite.getPixelData();
        Color *d = pixelData;
        Color *e = pixelData + size;
        for ( ; d < e; ) *d++ = *s++;
    }
    return *this;
}
// It will only load 24 bit uncompressed bitmaps
int Sprite::loadBitmap( const char *fileName )
{
    /*
        Read headers into memory & do some basic checking
    */
    unloadSprite();
    BitmapHeader        header1;
    BitmapInfoHeader    header2;
    std::ifstream       BMPFile;
    BMPFile.open( fileName, std::ios_base::in | std::ios_base::binary );
    if ( ! BMPFile ) return - 1;
    BMPFile.read( (char *)(&header1), sizeof(BitmapHeader) );
    if ( ! ((header1.charB == 'B') && (header1.charM == 'M')) ) return - 1;
    BMPFile.read( (char *)(&header2), sizeof(BitmapInfoHeader) );
    if ( ! ((header2.headerSize      == 40) &&
            (header2.nrOfColorPlanes == 1 ) &&
            (header2.bitsPerPixel    == 24) &&
            (header2.compressionType == 0 ) )) return - 1;
    /*
        read the image data.
        rowSize is expressed in bytes
        dataSize is expressed in integers (32 bit)
    */
    width = header2.width;
    height = header2.height;
    int rowSize = ((width * (unsigned)header2.bitsPerPixel + 31) / 32) * 4;
    int dataSize = abs( width * height );
    int nextLine;
    unsigned char *buf = new unsigned char[rowSize];
    pixelData = new Color[dataSize];
    Color *data;
    if ( height > 0 ) 
    {
        nextLine = - width;
        data = pixelData + dataSize;
    } else {
        nextLine = width;
        data = pixelData - width;
    }        
    BMPFile.seekg( header1.imageDataOffset );
    for ( int row = 0; row < height; row++ )
    {
        data += nextLine;
        BMPFile.read( (char *)buf,rowSize );
        unsigned char *p = buf;
        for ( int x = 0; x < width; x++ ) 
        {
            unsigned b = *p; p++;
            unsigned g = *p; p++;
            unsigned r = *p; p++;
            data[x] = (r << 16) + (g << 8) + b;
        }
    }
    delete buf;
    return 0;
}

/*
//  See constructor( int, int, const Color * )
void Sprite::loadFromMemory( int width_,int height_,Color *pixelData_ )
{
    unloadSprite();
    if ( width_ <= 0 ) return;
    if ( height_ <= 0 ) return;
    if ( pixelData_ == nullptr ) return;
    width = width_;
    height = height_;
    int size = width * height;
    pixelData = new Color[size];
    Color *s = pixelData_;
    Color *d = pixelData;
    Color *e = pixelData + size;
    for ( ; d < e; ) *d++ = *s++;
}
*/

/*
    This function is primarily used to take a screenshot from (a part of)
    the screen, although any valid Color* pointer can be a source.
*/
void Sprite::captureFromMemory( Rect area,Color *source,const int sourceWidth )
{
    assert( area.isValid() );
    createEmptySprite( area.width(),area.height() );
    Color *s = source + area.y1 * sourceWidth + area.x1;
    Color *d = pixelData;
    int nextLine = sourceWidth - width;
    for ( int j = 0; j < height; j++ )
    {
        for ( int i = 0; i < width; i++ ) *d++ = *s++;
        s += nextLine;
    }
}
void Sprite::createEmptySprite( int width_,int height_ )
{
    assert( width_ > 0 );
    assert( height_ > 0 );
    unloadSprite();
    width = width_;
    height = height_;
    pixelData = new Color[width_ * height_];
}
void Sprite::createEmptySprite( int width_,int height_,Color fillColor )
{
    createEmptySprite( width_,height_ );
    fill( fillColor );
    /*
    Color *d = pixelData;
    Color *e = pixelData + width_ * height_;
    for ( ; d < e; ) *d++ = fillColor;
    */
}
/*
//  todo: add error checking! Can't be used -> huge memory leak
int Sprite::loadPNG( const char *filename )
{
    std::string sCharStr;
    sCharStr = filename;
    std::wstring wCharStr = std::wstring( sCharStr.begin(),sCharStr.end() );
    const wchar_t *wFilename = wCharStr.c_str();
    Gdiplus::Sprite bitmap( wFilename );
    Gdiplus::Color pixel;
    height = bitmap.GetHeight();
    width = bitmap.GetWidth();
    pixelData = new Color[height * width];
    Color *d = pixelData;
    for( int y = 0; y < height; y++ )
        for( int x = 0; x < width; x++ )
        {
            bitmap.GetPixel( x,y,&pixel );
            *d++ = D3DCOLOR_ARGB( 
                pixel.GetA(),
                pixel.GetR(),
                pixel.GetG(),
                pixel.GetB() );
        }
    return 0;
}
*/

int Sprite::loadFromBMP( const char *fileName, Rect area )
{
    /*
        Read headers into memory & do some basic checking
    */
    assert( area.x1 >= 0 );
    assert( area.x2 >= 0 );
    assert( area.y1 >= 0 );
    assert( area.y2 >= 0 );
    unloadSprite();
    BitmapHeader        header1;
    BitmapInfoHeader    header2;
    std::ifstream       BMPFile;
    BMPFile.open( fileName, std::ios_base::in | std::ios_base::binary );
    if ( ! BMPFile ) return - 1;
    BMPFile.read( (char *)(&header1), sizeof(BitmapHeader) );
    if ( ! ((header1.charB == 'B') && (header1.charM == 'M')) ) return - 1;
    BMPFile.read( (char *)(&header2), sizeof(BitmapInfoHeader) );
    if ( ! ((header2.headerSize      == 40) &&
            (header2.nrOfColorPlanes == 1 ) &&
            (header2.bitsPerPixel    == 24) &&
            (header2.compressionType == 0 ) )) return - 1;
    /*
        read the image data.
        rowSize is expressed in bytes
        dataSize is expressed in integers (32 bit)
    */
    int destW = area.x2 - area.x1 + 1;
    int destH = area.y2 - area.y1 + 1;
    width = destW;
    height = destH;
    int destSize = destW * destH;
    pixelData = new Color[destSize];
    memset( pixelData,0,destSize );
    if ( area.x1 >= header2.width) return 0;
    if ( area.y1 >= header2.height ) return 0;
    int maxX;
    int maxY;
    if ( area.x2 >= header2.width ) maxX = header2.width - 1;
    else maxX = area.x2; 
    if ( area.y2 >= header2.height ) maxY = header2.height - 1;
    else maxY = area.y2; 
    
    int rowSize = ((header2.width * (unsigned)header2.bitsPerPixel + 31) / 32) * 4;
    int dataSize = abs( header2.width * header2.height );
    int nextLine;
    unsigned char *buf = new unsigned char[rowSize];
    Color *data;
    int yOffset;
    if (header2.height > 0 )
    {
        nextLine = - destW;
        data = pixelData + destSize;
        yOffset = (header2.height - 1 - maxY) * rowSize;
    } else {
        nextLine = destW;
        data = pixelData - destW;
        yOffset = area.y1 * rowSize;
    }        

    BMPFile.seekg( header1.imageDataOffset + yOffset); 
    for ( int row = area.y1; row <= maxY; row++ )
    {
        data += nextLine;
        BMPFile.read( (char *)buf,rowSize ); 
        unsigned char *p = buf + area.x1 * 3;
        for ( int x = area.x1; x <= maxX; x++ ) 
        {
            unsigned b = *p; p++;
            unsigned g = *p; p++;
            unsigned r = *p; p++;
            data[x - area.x1] = (r << 16) + (g << 8) + b;
        }
    }
    delete buf;
    return 0;
}

int Sprite::saveToBMP( const char *fileName )
{
    // Prepare headers
    assert( width > 0 );
    assert( height > 0 );
    assert( pixelData != nullptr );
    BitmapHeader        header1;
    BitmapInfoHeader    header2;
    header2.headerSize = sizeof( BitmapInfoHeader );
    header2.width = width;
    header2.height = height;
    header2.nrOfColorPlanes = 1;
    header2.bitsPerPixel = 24;
    header2.compressionType = 0;
    header2.imageSize = ((width * 3 + 3) / 4) * 4 * height; // calc padding
    header2.horizontalResolution = 2835;
    header2.verticalResolution = 2835;
    header2.nrColorsInPalette = 0;
    header2.nrOfImportantColorsUsed = 0;  
    header1.charB = 'B';
    header1.charM = 'M';
    header1.reserved[0] = 0;
    header1.reserved[1] = 0;
    header1.imageDataOffset = sizeof( BitmapHeader ) + header2.headerSize;
    header1.fileSize = header1.imageDataOffset + header2.imageSize;
    // recreate file
    remove( fileName );
    std::ofstream       BMPFile;
    BMPFile.open( fileName,std::ios_base::out | std::ios_base::binary );
    if ( !BMPFile ) return -1;
    // write headers
    BMPFile.write( (char *)(&header1),sizeof( BitmapHeader ) );
    BMPFile.write( (char *)(&header2),sizeof( BitmapInfoHeader ) );
    // write the image data. We start with the last row (.BMP logic)
    Color *source = pixelData + width * (height - 1);
    int nrPaddingBytes = (4 - ((width * 3) % 4)) % 4;
    char padByte = 0;
    for ( int j = height - 1; j >= 0; j-- )
    {
        for ( int i = 0; i < width; i++ )
        {
            Color pixel = *source++; 
            char c = pixel.GetB();
            BMPFile.write( &c,sizeof( char ) );
            c = pixel.GetG();
            BMPFile.write( &c,sizeof( char ) );
            c = pixel.GetR();
            BMPFile.write( &c,sizeof( char ) );
        }
        for ( int i = 0; i < nrPaddingBytes; i++ ) 
            BMPFile.write( &padByte,sizeof( padByte ) );
        source -= width * 2;
    }
    BMPFile.close();
    return 0;
}

bool Sprite::isEmpty( Rect area,Color color )
{
    if ( pixelData == nullptr ) return true;
    if (area.x1 >= width)  return true;
    if (area.y1 >= height) return true;
    if (area.x2 >= width)  area.x2 = width  - 1;
    if (area.y2 >= height) area.y2 = height - 1;
    for (int j = area.y1; j <= area.y2; j++ )
        for (int i = area.x1; i <= area.x2; i++)
        {
            if (pixelData[j * width + i] != color) return false;
        }
    return true;
}

void Sprite::fill( Color color )
{
    for ( int i = 0; i < width * height; i++ )
        pixelData[i] = color;
}

void Sprite::putPixel( int i,Color c )
{
    assert( i < width * height );
    assert( i >= 0 );
    pixelData[i] = c;
}

void Sprite::putPixel( int x,int y,Color c )
{
    assert( x >= 0 );
    assert( y >= 0 );
    assert( x < width );
    assert( y < height );
    pixelData[y * width + x] = c;
}

Color Sprite::getPixel( int x,int y )  const
{
    assert( x >= 0 );
    assert( y >= 0 );
    assert( x < width );
    assert( y < height );
    /*
    if ( (x >= width) || (y >= height) || (x < 0) || (y < 0) ) 
        return Colors::Black;
    */
    return pixelData[y * width + x];
}

void Sprite::copyFromSprite( const Sprite& source,Rect area ) 
{
    assert( area.x1 >= 0 );
    assert( area.y1 >= 0 );
    assert( area.isValid() );
    unloadSprite();
    /*
    width  = area.x2 - area.x1 + 1;
    height = area.y2 - area.y1 + 1;
    pixelData = new Color[width * height];
    memset( pixelData, 0, width * height );
    */
    createEmptySprite( area.width(),area.height(),Colors::Black );
    if ( (area.x1 >= source.width) || (area.y1 >= source.height) ) return;
    if (area.x2 >= source.width)  area.x2 = source.width  - 1;
    if (area.y2 >= source.height) area.y2 = source.height - 1;
    Color *src = source.pixelData + area.y1 * source.width + area.x1;
    int srcNextLine = source.width - width;
    Color *dst = pixelData;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++) *dst++ = *src++;
        src += srcNextLine;
    }
}

void Sprite::insertFromSprite( int x,int y,const Sprite& source )
{
    assert( x >= 0 );
    assert( y >= 0 );
    assert( pixelData != nullptr );
    if ( x >= width ) return;
    if ( y >= height ) return;
    int xEnd = x + source.width - 1;
    int yEnd = y + source.height - 1;
    if ( xEnd >= width  ) xEnd = width - 1;
    if ( yEnd >= height ) yEnd = height - 1;
    
    Color *src = source.pixelData;
    Color *dst = pixelData + y * width + x;
    int drawWidth = xEnd - x + 1;
    int dstNextLine = width - drawWidth;
    int srcNextLine = source.width - drawWidth;
    for ( int j = y; j <= yEnd; j++ )
    {
        for ( int i = x; i <= xEnd; i++ ) *dst++ = *src++;
        src += srcNextLine;
        dst += dstNextLine;
    }
}



//void Sprite::printXY( void *gfx,int x,int y,const char *s, void *font )
void Sprite::printXY( int x,int y,const char *s,Font *font )
{
    if ( s == nullptr ) return;
    int slen = (int)strlen ( s );
    int fontHeight = font->height ();
    if ( ! font->isBitmap () )
    {
        int fontWidth = font->width ();
        int stringWidth = fontWidth * slen;
        /*
        assert ( (x + stringWidth) < ScreenWidth );
        assert ( (y + fontHeight) < ScreenHeight );
        */
        Color *s1 = pixelData;
        s1 += x + width * y;
        for ( int iChar = 0; iChar < slen; iChar++ )
        {
            Color *s2 = s1;
            char *iData = font->getCharData ( s[iChar] );
            for ( int j = 0; j < fontHeight; j++ )
            {
                int iByte;
                for ( int i = 0; i < fontWidth; i++ )
                {
                    iByte = i % 8;
                    if ( (iByte == 0) && (i > 0) ) iData++;
                    if ( ((*iData) & (1 << (7 - iByte))) != 0 )
                        *s2 = 0xFFFFFF;
                    s2++;
                }
                if ( iByte > 0 ) iData++;
                s2 += width - fontWidth;
            }
            s1 += fontWidth;
        }
    }   
    /*
    // doesn't work
    else {
        int startX = x;
        for ( int i = 0; i < slen; i++ )
        {
            ((D3DGraphics *)gfx)->PaintBMPClearType ( startX,y,*((Sprite *)(font->getBmpData ( s[i] ))),0 );
            startX += ((Sprite *)(font->getBmpData ( s[i] )))->width;
        }
    }     
    */
}
/*
    This function will return the coordinates of the block the player clicked
    on in the image library. It will basically scan round the x,y coordinates
    until it finds black borders.
*/
Rect Sprite::getSpriteCoordsAt( int x,int y )
{
    Rect r;
    r.x1 = -1;
    r.x2 = -1;
    r.y1 = -1;
    r.y2 = -1;
    if ( pixelData == nullptr ) return r;
    assert ( x >= 0 );
    assert ( y >= 0 );
    assert ( x < width );
    assert ( y < height );
    // find a pixel with a color:
    Color *endOfImage = pixelData + width * height;
    Color *o = pixelData + y * width + x;
    Color *s;
    for ( s = o; s < endOfImage; s += width + 1 )
    {
        if ( *s != 0x0 ) break;
    }
    if ( s >= endOfImage )
    {
        for ( s = o; s > pixelData; s -= width + 1 )
        {
            if ( *s != 0x0 ) break;
        }       
        if ( s <= pixelData )
        {
            r.x1 = 0;
            r.x2 = 0;
            r.y1 = 0;
            r.y2 = 0;
            return r;
        }
    }
    r.y1 = r.y2 = (int)((s - pixelData) / width);
    r.x1 = r.x2 = (int)((s - pixelData) % width);
    x = r.x1;
    y = r.y1;
    // find the black frame:
    o = pixelData;
    for ( s = o + y * width + r.x1; *s != 0x00 && r.x1 > 0         ; r.x1--, s-- );
    for ( s = o + y * width + r.x2; *s != 0x00 && r.x2 < width  - 1; r.x2++, s++ );
    for ( s = o + r.y1 * width + x; *s != 0x00 && r.y1 > 0         ; r.y1--, s -= width );
    for ( s = o + r.y2 * width + x; *s != 0x00 && r.y2 < height - 1; r.y2++, s += width );
    r.x1++;
    r.x2--;
    r.y1++;
    r.y2--;
    return r;
}
/*
    Give a little 3D effect to (shiny edges) to a flat bitmap
*/
void Sprite::makeButtonEdges()
{
    int w = width;
    int h = height;
    Color *y1 = pixelData + 1;
    Color *y2 = y1 + (h - 1) * w - 1;
    for ( int i = 1; i < w; i++ )
    {
        Color upColor = *y1;
        Color downColor = *y2;
        int r = upColor.GetR();
        int g = upColor.GetG();
        int b = upColor.GetB();
        r += BLOCK_EDGE_COLOR_DELTA; if ( r > 255 ) r = 255;
        g += BLOCK_EDGE_COLOR_DELTA; if ( g > 255 ) g = 255;
        b += BLOCK_EDGE_COLOR_DELTA; if ( b > 255 ) b = 255;
        *y1 = Color( (unsigned char)r,(unsigned char)g,(unsigned char)b );
        r = downColor.GetR();
        g = downColor.GetG();
        b = downColor.GetB();
        r -= BLOCK_EDGE_COLOR_DELTA; if ( r < 0 ) r = 0;
        g -= BLOCK_EDGE_COLOR_DELTA; if ( g < 0 ) g = 0;
        b -= BLOCK_EDGE_COLOR_DELTA; if ( b < 0 ) b = 0;
        *y2 = Color( (unsigned char)r,(unsigned char)g,(unsigned char)b );
        y1++;
        y2++;
    }
    Color *x1 = pixelData;
    Color *x2 = x1 + w * 2 - 1;
    for ( int j = 1; j < h; j++ )
    {
        Color upColor = *x2;
        Color downColor = *x1;
        int r = upColor.GetR();
        int g = upColor.GetG();
        int b = upColor.GetB();
        r += BLOCK_EDGE_COLOR_DELTA; if ( r > 255 ) r = 255;
        g += BLOCK_EDGE_COLOR_DELTA; if ( g > 255 ) g = 255;
        b += BLOCK_EDGE_COLOR_DELTA; if ( b > 255 ) b = 255;
        *x2 = Color( (unsigned char)r,(unsigned char)g,(unsigned char)b );
        r = downColor.GetR();
        g = downColor.GetG();
        b = downColor.GetB();
        r -= BLOCK_EDGE_COLOR_DELTA; if ( r < 0 ) r = 0;
        g -= BLOCK_EDGE_COLOR_DELTA; if ( g < 0 ) g = 0;
        b -= BLOCK_EDGE_COLOR_DELTA; if ( b < 0 ) b = 0;
        *x1 = Color( (unsigned char)r,(unsigned char)g,(unsigned char)b );
        x1 += w;
        x2 += w;
    }
}

/*
int Sprite::saveBitmapTNTFontFile()
{
    if ( ! pixelData ) return - 1;

    const int charColums = 10;
    const int charRows = 10;
    float ofsX = 61.0f;
    float ofsY = 57.0f;
    float spX  = 79.2f;
    float spY  = 62.73f;
    float endX = ofsX + charColums * spX;
    float endY = ofsY + charRows * spY;
    int keyColor = 0; // background color
    unsigned char firstChar = 32;

    TftHeader   fontHeader;
    strcpy(fontHeader.id  , "TNT FONT FILE 1.00: ");
    strcpy(fontHeader.name, "Neuropol X by Raymond Larabie");
    fontHeader.asciiz           = 0;
    fontHeader.eofSign          = 26;
    fontHeader.fVersion         = 257; // version 1.1
    fontHeader.cVersion         = 257; // version 1.1
    fontHeader.isBitmap         = (char)true;
    fontHeader.nrCharacters     = 95;
    fontHeader.nrBytesScanline  = 0; // not used for bitmap fonts
    fontHeader.nrScanLines      = 0; // not used for bitmap fonts
    fontHeader.horizontalRes    = (char)((int)spX); 
    fontHeader.verticalRes      = (char)((int)spY); 
    fontHeader.isCompressed	    = 0;
    memset( fontHeader.reserved,0,30 );
    fontHeader.isPaletteStored  = (char)false;

    std::ofstream   fontFile;
    fontFile.open( "neuropolX.tft", 
        std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );
    if ( ! fontFile ) return - 1;
    fontFile.write( (char *)(&fontHeader),sizeof(TftHeader) );
    for ( int iChar = 0; iChar < fontHeader.nrCharacters; iChar++ )
    {
        int yBase = iChar / charRows;
        int xBase = iChar % charColums;
        int x1 = (int)(ofsX + (float)xBase * spX);
        int y1 = (int)(ofsY + (float)yBase * spY);
        int x2 = x1 + (int)spX;
        int y2 = y1 + (int)spY;

        int mostRight = 0;
        int charHeight = 0;
        for ( int j = 0; j < (int)spY; j++ )
            for ( int i = 0; i < (int)spX; i++ )
                if ( pixelData[(y1 + j) * width + x1 + i] != keyColor ) 
                {
                    if ( i > mostRight ) mostRight = i;
                    charHeight = j;
                }
        if ( mostRight == 0 ) mostRight++; 
        charHeight++;
        unsigned char nrCharInAscii = firstChar + iChar;
        if ( nrCharInAscii == CHAR_SPACE ) mostRight = (int)spX / 2;
        fontFile.write( (char *)(&nrCharInAscii),sizeof(char) );
        unsigned packedSize = (charHeight << 16) + mostRight;
        fontFile.write( (char *)(&packedSize),sizeof(packedSize) );
        int *pD = pixelData + x1 + y1 * width;
        for (int j = 0; j < charHeight; j++ )
        {
            for (int i = 0; i < mostRight; i++ )
            {
                int color = pD[i];
                unsigned char r = (color & 0xFF0000) >> 16;
                unsigned char g = (color & 0xFF00  ) >>  8;
                unsigned char b = (color & 0xFF    );
                fontFile.write( (char *)(&r),sizeof(char) );
                fontFile.write( (char *)(&g),sizeof(char) );
                fontFile.write( (char *)(&b),sizeof(char) );
            }
            pD += width;
        }
    }        
    fontFile.close();
    return 0;
}
int Sprite::saveMonoCTNTFontFile()
{
    if ( ! pixelData ) return - 1;
    const int charColums = 10;
    const int charRows = 10;
    float ofsX = 61.0f;
    float ofsY = 57.0f;
    float spX  = 79.2f;
    float spY  = 62.73f;
    float endX = ofsX + charColums * spX;
    float endY = ofsY + charRows * spY;
    int keyColor = 0; // background color
    unsigned char firstChar = 32;

    TftHeader   fontHeader;
    strcpy(fontHeader.id  , "TNT FONT FILE 1.00: ");
    strcpy(fontHeader.name, "Neuropol X by Raymond Larabie");
    fontHeader.asciiz           = 0;
    fontHeader.eofSign          = 26;
    fontHeader.fVersion         = 257; // version 1.1
    fontHeader.cVersion         = 257; // version 1.1
    fontHeader.isBitmap         = (char)false;
    fontHeader.nrCharacters     = 95;
    fontHeader.horizontalRes    = (int)spX - 8; 
    fontHeader.nrBytesScanline  = (fontHeader.horizontalRes + 7) / 8; 
    fontHeader.nrScanLines      = (int)spY; 
    fontHeader.verticalRes      = fontHeader.nrScanLines; 
    fontHeader.isCompressed	    = 0;
    memset( fontHeader.reserved,0,30 );
    fontHeader.isPaletteStored  = (char)false;

    std::ofstream   fontFile;
    fontFile.open( "neuropolXMono.tft", 
        std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );
    if ( ! fontFile ) return - 1;
    fontFile.write( (char *)(&fontHeader),sizeof(TftHeader) );
    char *buf = new char[fontHeader.nrBytesScanline];
    for ( int iChar = 0; iChar < fontHeader.nrCharacters; iChar++ )
    {            
        int yBase = iChar / charRows;
        int xBase = iChar % charColums;
        int x1 = (int)(ofsX + (float)xBase * spX);
        int y1 = (int)(ofsY + (float)yBase * spY);
        int x2 = x1 + (int)spX;
        int y2 = y1 + (int)spY;
        unsigned char nrCharInAscii = firstChar + iChar;
        fontFile.write( (char *)(&nrCharInAscii),sizeof(char) );

        int *pD = pixelData + x1 + y1 * width;
        for (int j = 0; j < fontHeader.nrScanLines; j++ )
        {
            memset( buf,0,fontHeader.nrBytesScanline );
            char t = 0;
            int iBuf = 0;
            for ( int i = 0; i < fontHeader.horizontalRes; i++ )
            {
                int iByte = i % 8;
                if ( (iByte == 0) && (i > 0) )
                {
                    buf[iBuf] = t;
                    iBuf++;
                    t = 0;
                }
                if ( pD[i] == 0xFFFFFF ) t += 1 << (7 - iByte);
            }
            buf[iBuf] = t;
            pD += width;
            fontFile.write( buf,fontHeader.nrBytesScanline );
        }
    }        
    delete buf;
    fontFile.close();
    return 0;
}
*/



