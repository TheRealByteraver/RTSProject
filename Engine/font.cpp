#pragma once

#include <fstream>
#include <assert.h>
#include "font.h"

int Font::loadFont( const std::string& fileName )
{
    TftHeader       header;
    std::ifstream   fontFile;
    fontFile.open( fileName,std::ios_base::in | std::ios_base::binary );
    if ( !fontFile ) 
        return -1;
    // do some basic error checking:
    fontFile.read( (char*)(&header),sizeof( TftHeader ) );
    if ( (header.isCompressed) ||
        (header.isPaletteStored) ||
        (header.nrScanLines > MAX_SCANLINES) ||
        (header.nrCharacters > NR_OF_CHARS) ) 
        return -1;

    width_ = header.horizontalRes;
    height_ = header.verticalRes;
    isBitmap_ = header.isBitmap != 0;

    if ( !isBitmap_ ) {
        int dataSize = height_ * header.nrBytesScanline;
        for ( int i = 0; i < NR_OF_CHARS; i++ ) {
            charData_[i].resize( dataSize,0 );
        }
        for ( int iChar = 0; iChar < header.nrCharacters; iChar++ ) {
            unsigned char index;
            fontFile.read( (char*)(&index),sizeof( char ) );
            fontFile.read( &(charData_[index][0]),dataSize );
        }
    }
    else {
         // Now load the bitmapped font:
        for ( int iChar = 0; iChar < header.nrCharacters; iChar++ ) {
            unsigned char ascii;
            unsigned short charHeight;
            unsigned short charWidth;
            fontFile.read( (char*)(&ascii),sizeof( char ) );
            fontFile.read( (char*)(&charWidth),sizeof( unsigned short ) );
            fontFile.read( (char*)(&charHeight),sizeof( unsigned short ) );
            if ( ascii >= NR_OF_CHARS ) 
                return -1;

            bmpData_[ascii].createEmptySprite( charWidth,charHeight );
            for ( int c = 0; c < charHeight * charWidth; c++ ) {
                unsigned char r,g,b;
                fontFile.read( (char*)(&r),sizeof( char ) );
                fontFile.read( (char*)(&g),sizeof( char ) );
                fontFile.read( (char*)(&b),sizeof( char ) );
                bmpData_[ascii].putPixel( c,Color( r,g,b ) );
            }
        }
    }
    return 0;
}