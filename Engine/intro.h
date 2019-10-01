#pragma once

#include <array>
#include <stdlib.h>
#include "Globals.h"
#include "Graphics.h"
#include "Sprite.h"

#define TNTLOGOFILENAME         "sprites\\tntlogo.bmp"

#define COLORBAR_BORDER_TOP     100
#define COLORBAR_BORDER_BOTTOM  120
#define COLORBAR_MIN_SPEED      1.6f
#define COLORBAR_MAX_SPEED      1.6f
#define COLORBAR_NR             5
#define COLORBAR_DARKEST_COLOR  0x40
#define COLORBAR_MIN_HEIGHT     30
#define COLORBAR_MAX_HEIGHT     30 

#define SCROLLER_TEXT   "Hello and welcome to this charmingly oldschool" \
" sine scroller. I hacked it together in a few evenings to see how hard it would be to do." \
" Apparently it is quite doable ;) The speed and size of the color bars can be randomized " \
"and I can put just as well 50 of them on the screen by changing a few constants, but it " \
"just does not look all that good . . .    Also it is probably not a good idea either to " \
"use random colors. The colors add up, so they act like lamps if you will. To think that " \
"crackers could make similar intro's on limited machines such as the ATARI ST and the " \
"Commodore Amiga just goes to show how talented they were, and how lazy modern computers " \
"with their ultra fast processors make us . . .     The music you are hearing is called " \
" \"Global Trash v3\" and was made by " \
"Jesper Kyd from the Silents Amiga demo group (they later appeared in the PC scene as well" \
") and dates back to the early nineties I believe. I kinda like its dark vibe so I decided" \
" to use it here for your pleasure. I hope you like it, if not, you can replace it with " \
"another .mod file of your liking (find more on https://modarchive.org/)"



// The bar is always twice the height of the "height" value
class ColorBar {
public:
    ColorBar()
    {
        speed = COLORBAR_MIN_SPEED;
        if ( COLORBAR_MIN_SPEED != COLORBAR_MAX_SPEED )
        speed += (float)(rand() % (int)
                    ((COLORBAR_MAX_SPEED - COLORBAR_MIN_SPEED) * 100.0f)
                / 100.0f);
        height = COLORBAR_MIN_HEIGHT; 
        if ( (COLORBAR_MAX_HEIGHT - COLORBAR_MIN_HEIGHT) != 0 )
            height += rand() % (COLORBAR_MAX_HEIGHT - COLORBAR_MIN_HEIGHT);
        minY = COLORBAR_BORDER_TOP + height - 1;
        maxY = Graphics::ScreenHeight - height - 1 - COLORBAR_BORDER_BOTTOM;
        y = (float)(minY + rand() % (maxY - minY));

        const int colMin = COLORBAR_DARKEST_COLOR;
        const int colVar = 0xFF - colMin;
        color = Color(
            colMin + rand() % colVar,
            colMin + rand() % colVar,
            colMin + rand() % colVar
            );
        colors.resize( height );
        for ( int c = 0; c < height; c++ )
            colors[c] = Color(
                ((int)color.GetR() * c) / height, 
                ((int)color.GetG() * c) / height,
                ((int)color.GetB() * c) / height
                );
    }
    void updateFrame()
    {
        if ( movingUp ) { 
            y += speed;
            if ( (int)y >= maxY ) {
                y = (float)maxY;
                movingUp = false;
            }
        }
        else {
            y -= speed;
            if ( (int)y <= minY ) {
                y = (float)minY;
                movingUp = true;
            }
        }
    }

private:
    bool    movingUp = true;
    int     minY;
    int     maxY;
    Color   color;

public:
    float   speed;
    float   y;
    int     height;
    std::vector< Color > colors;
};


class Intro 
{
public:
    Intro() 
    {
        tntLogo.loadBitmap( GAME_FOLDER TNTLOGOFILENAME );
        lineData = tntLogo.getPixelData();
        blockWidth = tntLogo.getWidth();
        blockHeight = tntLogo.getHeight();
        halfBlockWidth = blockWidth / 2;
        halfBlockHeight = blockHeight / 2;
        x1 = Graphics::ScreenWidth / 2 - halfBlockWidth;
        x2 = x1 + blockWidth - 1;
        y1 = Graphics::ScreenHeight / 2 - halfBlockHeight;

        assert( x1 > 0 );
        assert( y1 > 0 );

        // div by 2 at minimum
        maxDeviation = (Graphics::ScreenWidth - blockWidth) / 4; 

        // precalculate sinus table with deviations included
        Offsets.resize( nrSteps );
        for ( unsigned i = 0; i < Offsets.size(); i++ ) {
            Offsets[i] = (int)(sin( ((float)i * 2.0 * 3.14159265359) / (float)nrSteps )
                * (float)maxDeviation );
        }

        // start the scroller at the right edge of the screen:
        scrollerStartX = Graphics::ScreenWidth;  
        scrollerTextLength = (int)strlen( SCROLLER_TEXT );
    }

    void updateFrame( Graphics & gfx,Font& font )
    {
        // draw ColorBars
        /*
        for ( int j = 0; j < COLORBAR_NR; j++ ) {
            for ( int i = 0; i < colorBars[j].height; i++ ) {
                gfx.drawHorLine( 0,j * 40 + i,Graphics::ScreenWidth - 1,colorBars[j].colors[i] );
            }
        }
        */
        
        for ( int y = 0; y < Graphics::ScreenHeight; y++ ) {
            unsigned r = 0;
            unsigned g = 0;
            unsigned b = 0;
            unsigned cnt = 0;
            Color color = 0;
            for ( int iBar = 0; iBar < COLORBAR_NR; iBar++ ) {
                ColorBar& colorBar = colorBars[iBar];
                int deltaY = abs( y - (int)colorBar.y );
                if ( deltaY < colorBar.height ) {
                    deltaY = colorBar.height - 1 - deltaY;
                    cnt++;
                    r += colorBar.colors[deltaY].GetR();
                    g += colorBar.colors[deltaY].GetG();
                    b += colorBar.colors[deltaY].GetB();
                }
            }
            if ( cnt > 0 ) {
                if ( r > 0xFF )
                    r = 0xFF;
                if ( g > 0xFF )
                    g = 0xFF;
                if ( b > 0xFF )
                    b = 0xFF;
                color = Color(
                    r,
                    g,
                    b
                );

                /*
                color = Color(
                    r / cnt,
                    g / cnt,
                    b / cnt
                );
                */
                gfx.drawHorLine( 0,(int)y,Graphics::ScreenWidth - 1,color );
            }
        }
        // now move the color bars:
        for ( int iBar = 0; iBar < COLORBAR_NR; iBar++ )
            colorBars[iBar].updateFrame();

        //gfx.drawBlock( 0,0,Graphics::ScreenWidth - 1,Graphics::ScreenHeight - 1,Colors::Gray );
        
        // draw center TNT Sinus logo:
        Color *logoLines = lineData;
        for ( int i = 0; i < blockHeight; i++ ) {
            int ofs2 = 
                (i * nrSteps / 4) / blockHeight + sineIndex;
            if ( ofs2 >= nrSteps )
                ofs2 %= nrSteps;


            if ( i & 0x1 ) {// draw odd lines
                int X = x1 + Offsets[ofs2];
                int Y = y1 + i;
                for ( int x = X; x < X + blockWidth; x++ ) {
                    if ( *logoLines != Colors::Black )
                        gfx.PutPixel( x,Y,*logoLines ); //Color( 0xFFFFFF - (*logoLines).dword ) );
                    logoLines++;
                }
            }
            else {         // draw even lines               
                ofs2 += nrSteps / 3;
                //ofs2 = maxDeviation / 2;//(i * nrSteps / 4) / blockHeight;
                if ( ofs2 >= nrSteps )
                    ofs2 %= nrSteps;
                    
                int X = x1 + Offsets[ofs2];
                int Y = y1 + i;
                for ( int x = X; x < X + blockWidth; x++ ) {
                    if ( *logoLines != Colors::Black )
                        gfx.PutPixel( x,Y,*logoLines );
                    logoLines++;
                }
            }
        }
        sineIndex++;
        if ( sineIndex >= nrSteps )
            sineIndex = 0;

        // Draw bottom scroller:
        int startX = scrollerStartX;
        scrollerStartX -= 8;
        // restart scroll after a while:
        if ( -scrollerStartX > scrollerTextLength * 40 )
            scrollerStartX = Graphics::ScreenWidth * 2;

        int y = Graphics::ScreenHeight - font.height() * 2;
        
        for ( int i = 0; i < scrollerTextLength; i++ ) {
            Sprite& charSprite = *((Sprite *)(font.getBmpData( SCROLLER_TEXT[i] )));
            gfx.paintBMPClearType( 
                startX,
                y + Offsets[(startX + sineIndex)  % nrSteps] / 4,
                charSprite,
                Colors::Black,
                0x20 );
            startX += charSprite.getWidth();
        }
    }

private:
    Sprite              tntLogo;
    Color              *lineData;
    int                 sineIndex = 0;
    int                 blockWidth;
    int                 blockHeight;
    int                 halfBlockWidth;
    int                 halfBlockHeight;
    int                 x1;
    int                 x2;
    int                 y1;
    int                 maxDeviation;
    const int           nrSteps = 400; // 60 steps == 1 second
    std::vector<int>    Offsets;
    std::array<ColorBar,COLORBAR_NR> colorBars;
    int                 scrollerStartX;
    int                 scrollerTextLength;
};
