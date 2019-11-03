#include <Windows.h>
#include <string.h>
#include "Graphics.h"
#include "sprite.h"
#include "font.h"
#include "math.h"
#include <stdlib.h>
#include <assert.h>

#pragma comment ( lib,"gdiplus.lib" )

/*
    The constant below is a small image that any uninitialized sprite is 
    initialized with. It is a small black square with a diagonal red cross 
    in it, symbolizing a missing image.
*/
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
    *this = brokenImageSprite;
}
/*
    This constructor initializes the sprite by copying the 
    (width * height) -sized image from pixelData to itself.
*/
Sprite::Sprite( int width,int height,const Color *pixelData )
{
    assert( width > 0 );
    assert( height > 0 );
    assert( pixelData != nullptr );
    width_ = width;
    height_ = height;
    const int size = width_ * height_;
    pixelData_ = new Color[size];
    for ( int i = 0; i < size; i++ ) 
        pixelData_[i] = pixelData[i];
}
/*
    this constructor initializes the sprite with a .bmp bitmap file
*/
Sprite::Sprite( const char *bmpFileName )
{
    width_ = 0;
    height_ = 0;
    pixelData_ = nullptr;
    loadBitmap( bmpFileName );
}
// this function removes the sprite data from memory
void Sprite::unloadSprite()
{
    width_ = 0;
    height_ = 0;
    if ( pixelData_ != nullptr ) delete pixelData_;
    pixelData_ = nullptr;
}
// free data on destruction
Sprite::~Sprite()
{
    unloadSprite();
}
/*
    the assignment operator copies the data and creates an additional 
    copy of the sprite
*/
Sprite& Sprite::operator=( const Sprite& sourceSprite )
{
    if ( this != &sourceSprite ) {
        unloadSprite();
        if ( (sourceSprite.getWidth() <= 0) ||
            (sourceSprite.getHeight() <= 0) ||
            (sourceSprite.getPixelData() == nullptr) ) {
            loadBrokenImageSpriteData();
            return *this;
        }
        width_ = sourceSprite.getWidth();
        height_ = sourceSprite.getHeight();
        int size = width_ * height_;
        pixelData_ = new Color[size];
        memcpy( pixelData_,sourceSprite.getPixelData(),size );
        //Color* s = sourceSprite.getPixelData();
        //Color* d = pixelData_;
        //Color* e = pixelData_ + size;
        //for ( ; d < e; ) 
        //    *d++ = *s++;
    }
    return *this;
}
/*
    It will only load 24 bit uncompressed bitmaps. If an error occurs it 
    creates a sprite with the broken image icon.
*/ 
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
    if ( !BMPFile )
    {
        loadBrokenImageSpriteData();
        return -1;
    }
    BMPFile.read( (char *)(&header1), sizeof(BitmapHeader) );
    if ( ! ((header1.charB == 'B') && (header1.charM == 'M')) )
    {
        loadBrokenImageSpriteData();
        return -1;
    }
    BMPFile.read( (char *)(&header2), sizeof(BitmapInfoHeader) );
    if ( ! ((header2.headerSize      == 40) &&
            (header2.nrOfColorPlanes == 1 ) &&
            (header2.bitsPerPixel    == 24) &&
            (header2.compressionType == 0 ) ))
    {
        loadBrokenImageSpriteData();
        return -1;
    }
    /*
        read the image data.
        rowSize is expressed in bytes
        dataSize is expressed in integers (32 bit)
    */
    width_ = header2.width;
    height_ = header2.height;
    int rowSize = ((width_ * (unsigned)header2.bitsPerPixel + 31) / 32) * 4;
    int dataSize = abs( width_ * height_ );
    int nextLine;
    //unsigned char *buf = new unsigned char[rowSize];
    std::unique_ptr<unsigned char[]> buf = std::make_unique<unsigned char[]>( rowSize );
    pixelData_ = new Color[dataSize];
    Color *data;
    if ( height_ > 0 ) 
    {
        nextLine = - width_;
        data = pixelData_ + dataSize;
    } else {
        nextLine = width_;
        data = pixelData_ - width_;
    }        
    BMPFile.seekg( header1.imageDataOffset );
    for ( int row = 0; row < height_; row++ )
    {
        data += nextLine;
        BMPFile.read( (char *)buf.get(),rowSize );
        unsigned char *p = buf.get();
        for ( int x = 0; x < width_; x++ ) 
        {
            unsigned b = *p; p++;
            unsigned g = *p; p++;
            unsigned r = *p; p++;
            //data[x] = (r << 16) + (g << 8) + b;
            data[x] = Color( r,g,b );
        }
    }
    //delete[] buf;
    return 0;
}

/*
//  See constructor( int, int, const Color * )
void Sprite::loadFromMemory( int width,int height,Color *pixelData )
{
    unloadSprite();
    if ( width <= 0 ) return;
    if ( height <= 0 ) return;
    if ( pixelData == nullptr ) return;
    width_ = width;
    height_ = height;
    int size = width_ * height_;
    pixelData_ = new Color[size];
    Color *s = pixelData;
    Color *d = pixelData_;
    Color *e = pixelData_ + size;
    for ( ; d < e; ) *d++ = *s++;
}
*/

/*
    This function is primarily used to take a screenshot from (a part of)
    the screen, although any valid Color* pointer can be a source.
*/
void Sprite::captureFromMemory( Rect area,Color* source,const int sourceWidth )
{
    assert( area.isValid() );
    createEmptySprite( area.width(),area.height() );
    Color* s = source + area.y1 * sourceWidth + area.x1;
    Color* d = pixelData_;
    int nextLine = sourceWidth - width_;
    for ( int j = 0; j < height_; j++ ) {
        for ( int i = 0; i < width_; i++ )
            *d++ = *s++;
        s += nextLine;
    }
}

void Sprite::createEmptySprite( int width,int height )
{
    assert( width > 0 );
    assert( height > 0 );
    unloadSprite();
    width_ = width;
    height_ = height;
    pixelData_ = new Color[width * height];
}

void Sprite::createEmptySprite( int width,int height,Color fillColor )
{
    //createEmptySprite( width,height );
    assert( width > 0 );
    assert( height > 0 );
    unloadSprite();
    width_ = width;
    height_ = height;
    pixelData_ = new Color[width * height];
    fill( fillColor );
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
    height_ = bitmap.GetHeight();
    width_ = bitmap.GetWidth();
    pixelData_ = new Color[height_ * width_];
    Color *d = pixelData_;
    for( int y = 0; y < height_; y++ )
        for( int x = 0; x < width_; x++ )
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

int Sprite::loadFromBMP( const char* fileName,Rect area )
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
    BMPFile.open( fileName,std::ios_base::in | std::ios_base::binary );
    if ( !BMPFile ) {
        loadBrokenImageSpriteData();
        return -1;
    }
    BMPFile.read( (char*)(&header1),sizeof( BitmapHeader ) );
    if ( !((header1.charB == 'B') && (header1.charM == 'M')) ) {
        loadBrokenImageSpriteData();
        return -1;
    }
    BMPFile.read( (char*)(&header2),sizeof( BitmapInfoHeader ) );
    if ( !((header2.headerSize == 40) &&
        (header2.nrOfColorPlanes == 1) &&
        (header2.bitsPerPixel == 24) &&
        (header2.compressionType == 0)) ) {
        loadBrokenImageSpriteData();
        return -1;
    }
    /*
        read the image data.
        rowSize is expressed in bytes
        dataSize is expressed in integers (32 bit)
    */
    int destW = area.x2 - area.x1 + 1;
    int destH = area.y2 - area.y1 + 1;
    width_ = destW;
    height_ = destH;
    int destSize = destW * destH;
    pixelData_ = new Color[destSize];
    fill( Colors::Black );
    //memset( pixelData_,0,destSize );
    if ( area.x1 >= header2.width ) return 0;
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
    //unsigned char *buf = new unsigned char[rowSize];
    std::unique_ptr<unsigned char[]> buf = std::make_unique<unsigned char[]>( rowSize );
    Color* data;
    int yOffset;
    if ( header2.height > 0 ) {
        nextLine = -destW;
        data = pixelData_ + destSize;
        yOffset = (header2.height - 1 - maxY) * rowSize;
    }
    else {
        nextLine = destW;
        data = pixelData_ - destW;
        yOffset = area.y1 * rowSize;
    }
    BMPFile.seekg( header1.imageDataOffset + yOffset );
    for ( int row = area.y1; row <= maxY; row++ ) {
        data += nextLine;
        BMPFile.read( (char*)buf.get(),rowSize );
        unsigned char* p = buf.get() + area.x1 * 3;
        for ( int x = area.x1; x <= maxX; x++ ) {
            unsigned b = *p; p++;
            unsigned g = *p; p++;
            unsigned r = *p; p++;
            //data[x - area.x1] = (r << 16) + (g << 8) + b;
            data[x - area.x1] = Color( r,g,b );
        }
    }
    //delete buf;
    return 0;
}

int Sprite::saveToBMP( const char *fileName )
{
    // Prepare headers
    assert( width_ > 0 );
    assert( height_ > 0 );
    assert( pixelData_ != nullptr );
    BitmapHeader        header1;
    BitmapInfoHeader    header2;
    header2.headerSize = sizeof( BitmapInfoHeader );
    header2.width = width_;
    header2.height = height_;
    header2.nrOfColorPlanes = 1;
    header2.bitsPerPixel = 24;
    header2.compressionType = 0;
    header2.imageSize = ((width_ * 3 + 3) / 4) * 4 * height_; // calc padding
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
    // copy data to buffer first
    //char *bmpData = new char[header2.imageSize];
    std::unique_ptr<char[]> bmpData = std::make_unique<char[]>( header2.imageSize );
    Color *source = pixelData_ + width_ * (height_ - 1);
    int nrPaddingBytes = (4 - ((width_ * 3) % 4)) % 4;
    char padByte = 0;
    int index = 0;
    for ( int j = height_ - 1; j >= 0; j-- )
    {
        for ( int i = 0; i < width_; i++ )
        {
            Color pixel = *source++;
            bmpData[index++] = pixel.GetB();
            bmpData[index++] = pixel.GetG();
            bmpData[index++] = pixel.GetR();
        }
        for ( int i = 0; i < nrPaddingBytes; i++ ) bmpData[index++] = padByte;
        source -= width_ * 2;        
    }
    BMPFile.write( bmpData.get(),header2.imageSize );
    BMPFile.close();
    //delete bmpData;
    return 0;
}

bool Sprite::isFilledWith( Rect area,Color color )
{
    if ( pixelData_ == nullptr ) return true;
    if (area.x1 >= width_)  return true;
    if (area.y1 >= height_) return true;
    if (area.x2 >= width_)  area.x2 = width_  - 1;
    if (area.y2 >= height_) area.y2 = height_ - 1;
    for (int j = area.y1; j <= area.y2; j++ )
        for (int i = area.x1; i <= area.x2; i++)
        {
            if (pixelData_[j * width_ + i] != color) return false;
        }
    return true;
}

/*
    Primitive drawing functions:
*/

void Sprite::fill( Color color )
{
    for ( int i = 0; i < width_ * height_; i++ )
        pixelData_[i] = color;
}

void Sprite::putPixel( int i,Color color )
{
    assert( i < width_ * height_ );
    assert( i >= 0 );
    pixelData_[i] = color;
}

void Sprite::putPixel( int x,int y,Color color )
{
    assert( x >= 0 );
    assert( y >= 0 );
    assert( x < width_ );
    assert( y < height_ );
    pixelData_[y * width_ + x] = color;
}

void Sprite::drawHorLine( int x1,int y,int x2,Color color )
{
    for ( int x = x1; x <= x2; x++ ) putPixel( x,y,color );
}

void Sprite::drawVerLine( int x,int y1,int y2,Color color )
{
    for ( int y = y1; y <= y2; y++ ) putPixel( x,y,color );
}

void Sprite::drawCircle( int centerX,int centerY,int radius,Color color )
{
    int rSquared = radius * radius;
    int xPivot = (int)(radius * 0.707107f + 0.5f);
    for ( int x = 0; x <= xPivot; x++ )
    {
        int y = (int)(sqrt( (float)(rSquared - x * x) ) + 0.5f);
        putPixel( centerX + x,centerY + y,color );
        putPixel( centerX - x,centerY + y,color );
        putPixel( centerX + x,centerY - y,color );
        putPixel( centerX - x,centerY - y,color );
        putPixel( centerX + y,centerY + x,color );
        putPixel( centerX - y,centerY + x,color );
        putPixel( centerX + y,centerY - x,color );
        putPixel( centerX - y,centerY - x,color );
    }
}

void Sprite::drawDisc( int cx,int cy,int r,Color color )
{
    int rSquared = r * r;
    int xPivot = (int)(r * 0.707107f + 0.5f);
    for ( int x = 0; x <= xPivot; x++ )
    {
        int y = (int)(sqrt( (float)(rSquared - x*x) ) + 0.5f);
        int yHi = cy - y;
        int yLo = cy + y;
        for ( int ix = cx - x; ix <= cx + x; ix++ ) putPixel( ix,yHi,color );
        for ( int ix = cx - x; ix <= cx + x; ix++ ) putPixel( ix,yLo,color );
        yHi = cy - x;
        yLo = cy + x;
        for ( int ix = cx - y; ix <= cx + y; ix++ ) putPixel( ix,yHi,color );
        for ( int ix = cx - y; ix <= cx + y; ix++ ) putPixel( ix,yLo,color );
    }
}

void Sprite::drawBox( const Rect& coords,Color color )
{
    drawBox( coords.x1,coords.y1,coords.x2,coords.y2,color );
}

void Sprite::drawBox( int x1,int y1,int x2,int y2,Color color )
{
    for ( int x = x1; x <= x2; x++ )
    {
        putPixel( x,y1,color );
        putPixel( x,y2,color );
    }
    for ( int y = y1; y <= y2; y++ )
    {
        putPixel( x1,y,color );
        putPixel( x2,y,color );
    }
}

void Sprite::drawBlock( Rect coords,Color color )
{
    drawBlock( coords.x1,coords.y1,coords.x2,coords.y2,color );
}

void Sprite::drawBlock( int x1,int y1,int x2,int y2,Color color )
{
    for ( int y = y1; y <= y2; y++ )
        for ( int x = x1; x <= x2; x++ )
            putPixel( x,y,color );
}

void Sprite::drawNiceBlock( Rect r )
{
    int rd = frameColor_.GetR() / frameWidth_;
    int gd = frameColor_.GetG() / frameWidth_;
    int bd = frameColor_.GetB() / frameWidth_;
    int red = rd;
    int grn = gd;
    int blu = bd;
    int i;
    for ( i = 0; i < frameWidth_; i++ )
    {
        drawBox( r.x1 + i,r.y1 + i,r.x2 - i,r.y2 - i,Color( red,grn,blu ) );
        red += rd;
        grn += gd;
        blu += bd;
    }
    drawBlock( r.x1 + i,r.y1 + i,r.x2 - i,r.y2 - i,frameColor_ );
}

void Sprite::drawNiceBlockInv( Rect r )
{
    int red = frameColor_.GetR();
    int grn = frameColor_.GetG();
    int blu = frameColor_.GetB();
    int rd = red / frameWidth_;
    int gd = grn / frameWidth_;
    int bd = blu / frameWidth_;
    int i;
    Color c;
    for ( i = 0; i < frameWidth_; i++ )
    {
        c = Color( red,grn,blu );
        drawBox( r.x1 + i,r.y1 + i,r.x2 - i,r.y2 - i,c );
        red -= rd;
        grn -= gd;
        blu -= bd;
    }
    drawBlock( r.x1 + i,r.y1 + i,r.x2 - i,r.y2 - i,c );
}

void Sprite::drawButton( Rect r )
{
    drawBox( r.x1,r.y1,r.x2,r.y2,0 );
    int colorDelta = 60;
    int red = frameColor_.GetR();
    int grn = frameColor_.GetG();
    int blu = frameColor_.GetB();
    int rd = red - colorDelta; if ( rd < 0 ) rd = 0;
    int gd = grn - colorDelta; if ( gd < 0 ) gd = 0;
    int bd = blu - colorDelta; if ( bd < 0 ) bd = 0;
    int darkColor = (rd << 16) + (gd << 8) + bd;
    int rl = red + colorDelta; if ( rl > 0xFF ) rl = 0xFF;
    int gl = grn + colorDelta; if ( gl > 0xFF ) gl = 0xFF;
    int bl = blu + colorDelta; if ( bl > 0xFF ) bl = 0xFF;
    int lightColor = (rl << 16) + (gl << 8) + bl;
    for ( int i = 1; i < frameWidth_ - 1; i++ )
    {
        drawHorLine( r.x1 + i,r.y1 + i,r.x2 - i,lightColor );
        drawVerLine( r.x2 - i,r.y1 + i,r.y2 - i,lightColor );
        drawHorLine( r.x1 + i,r.y2 - i,r.x2 - i,darkColor );
        drawVerLine( r.x1 + i,r.y1 + i,r.y2 - i,darkColor );
    }
}

void Sprite::drawButtonPlusMinus( Rect r,int width )
{
    int ySplit = r.y1 + (r.y2 - r.y1) / 2 + 1;
    Rect rUp = r;
    rUp.y2 = ySplit;
    Rect rDown = r;
    rDown.y1 = ySplit;
    drawButton( rUp );
    drawButton( rDown );
    for ( int i = 0; i < width; i++ )
    {
        int x = r.x1 + 5 + i * font_->width();
        printXY( x,r.y1 + 1,"+" );
        printXY( x,ySplit,"-" );
    }
}

void Sprite::drawRadioButton( int x,int y,int width,bool selected )
{
    int r = width / 2;
    assert( r > 3 );
    int cx = x + r;
    int cy = y + r;
    Color backColor( textColor_ );
    Color frameColorSoft = Color(
        (frameColor_.GetR() + 0xFF) / 2,
        (frameColor_.GetG() + 0xFF) / 2,
        (frameColor_.GetB() + 0xFF) / 2
    );
    drawDisc( cx,cy,r,backColor );
    //drawCircle( cx,cy,r,frameColorSoft );
    if ( selected )
    {
        r -= 2;
        drawDisc( cx,cy,r,Colors::Black );
    }
}


/*
    This function creates a sprite by copying a section from a bigger sprite.
    The original contents of the sprite that calls the function is discarded.
*/
void Sprite::createFromSprite( const Sprite& source,Rect area )
{
    assert( area.x1 >= 0 );
    assert( area.y1 >= 0 );
    assert( area.isValid() );
    unloadSprite();
    /*
    width_  = area.x2 - area.x1 + 1;
    height_ = area.y2 - area.y1 + 1;
    pixelData_ = new Color[width_ * height_];
    memset( pixelData_, 0, width_ * height_ );
    */
    createEmptySprite( area.width(),area.height(),Colors::Black );
    if ( (area.x1 >= source.width_) || (area.y1 >= source.height_) ) return;
    if ( area.x2 >= source.width_ )  area.x2 = source.width_ - 1;
    if ( area.y2 >= source.height_ ) area.y2 = source.height_ - 1;
    Color *src = source.pixelData_ + area.y1 * source.width_ + area.x1;
    int srcNextLine = source.width_ - width_;
    Color *dst = pixelData_;
    for ( int j = 0; j < height_; j++ )
    {
        for ( int i = 0; i < width_; i++ ) *dst++ = *src++;
        src += srcNextLine;
    }
}


Color Sprite::getPixel( int x,int y )  const
{
    assert( x >= 0 );
    assert( y >= 0 );
    assert( x < width_ );
    assert( y < height_ );
    /*
    if ( (x >= width_) || (y >= height_) || (x < 0) || (y < 0) ) 
        return Colors::Black;
    */
    return pixelData_[y * width_ + x];
}


void Sprite::insertFromSprite( int x,int y,const Sprite& source )
{
    assert( x >= 0 );
    assert( y >= 0 );
    assert( pixelData_ != nullptr );
    if ( x >= width_ ) return;
    if ( y >= height_ ) return;
    int xEnd = x + source.width_ - 1;
    int yEnd = y + source.height_ - 1;
    if ( xEnd >= width_  ) xEnd = width_ - 1;
    if ( yEnd >= height_ ) yEnd = height_ - 1;
    
    Color *src = source.pixelData_;
    Color *dst = pixelData_ + y * width_ + x;
    int drawWidth = xEnd - x + 1;
    int dstNextLine = width_ - drawWidth;
    int srcNextLine = source.width_ - drawWidth;
    for ( int j = y; j <= yEnd; j++ )
    {
        for ( int i = x; i <= xEnd; i++ ) *dst++ = *src++;
        src += srcNextLine;
        dst += dstNextLine;
    }
}



//void Sprite::printXY( void *gfx,int x,int y,const char *s, void *font )
void Sprite::printXY( int x,int y,const char* s )
{
    assert( s != nullptr );
    assert( font_ != nullptr );
    int slen = (int)strlen( s );
    int fontHeight = font_->height();
    if ( !font_->isBitmap() ) {
        int fontWidth = font_->width();
        int stringWidth = fontWidth * slen;
        /*
        assert ( (x + stringWidth) < ScreenWidth );
        assert ( (y + fontHeight) < ScreenHeight );
        */
        Color* s1 = pixelData_;
        s1 += x + width_ * y;
        for ( int iChar = 0; iChar < slen; iChar++ ) {
            Color* s2 = s1;
            //const char *iData = font_->getCharData ( s[iChar] );
            //const std::vector<char>& iData = font_->getCharData( s[iChar] );
            std::vector<char>::const_iterator iData = font_->getCharData( s[iChar] ).cbegin();

            for ( int j = 0; j < fontHeight; j++ ) {
                int iByte;
                for ( int i = 0; i < fontWidth; i++ ) {
                    iByte = i % 8;
                    if ( (iByte == 0) && (i > 0) ) iData++;
                    if ( ((*iData) & (1 << (7 - iByte))) != 0 )
                        *s2 = 0xFFFFFF;
                    s2++;
                }
                if ( iByte > 0 ) iData++;
                s2 += width_ - fontWidth;
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
            startX += ((Sprite *)(font->getBmpData ( s[i] )))->width_;
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
    if ( pixelData_ == nullptr ) return r;
    assert ( x >= 0 );
    assert ( y >= 0 );
    assert ( x < width_ );
    assert ( y < height_ );
    // find a pixel with a color:
    Color *endOfImage = pixelData_ + width_ * height_;
    Color *o = pixelData_ + y * width_ + x;
    Color *s;
    for ( s = o; s < endOfImage; s += width_ + 1 )
    {
        if ( *s != 0x0 ) break;
    }
    if ( s >= endOfImage )
    {
        for ( s = o; s > pixelData_; s -= width_ + 1 )
        {
            if ( *s != 0x0 ) break;
        }       
        if ( s <= pixelData_ )
        {
            r.x1 = 0;
            r.x2 = 0;
            r.y1 = 0;
            r.y2 = 0;
            return r;
        }
    }
    r.y1 = r.y2 = (int)((s - pixelData_) / width_);
    r.x1 = r.x2 = (int)((s - pixelData_) % width_);
    x = r.x1;
    y = r.y1;
    // find the black frame:
    o = pixelData_;
    for ( s = o + y * width_ + r.x1; *s != 0x00 && r.x1 > 0         ; r.x1--, s-- );
    for ( s = o + y * width_ + r.x2; *s != 0x00 && r.x2 < width_  - 1; r.x2++, s++ );
    for ( s = o + r.y1 * width_ + x; *s != 0x00 && r.y1 > 0         ; r.y1--, s -= width_ );
    for ( s = o + r.y2 * width_ + x; *s != 0x00 && r.y2 < height_ - 1; r.y2++, s += width_ );
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
    int w = width_;
    int h = height_;
    Color *y1 = pixelData_ + 1;
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
    Color *x1 = pixelData_;
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
    if ( ! pixelData_ ) return - 1;

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
                if ( pixelData_[(y1 + j) * width_ + x1 + i] != keyColor ) 
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
        int *pD = pixelData_ + x1 + y1 * width_;
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
            pD += width_;
        }
    }        
    fontFile.close();
    return 0;
}
int Sprite::saveMonoCTNTFontFile()
{
    if ( ! pixelData_ ) return - 1;
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

        int *pD = pixelData_ + x1 + y1 * width_;
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
            pD += width_;
            fontFile.write( buf,fontHeader.nrBytesScanline );
        }
    }        
    delete buf;
    fontFile.close();
    return 0;
}
*/



