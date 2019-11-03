#pragma once

#include <array>
#include <stdlib.h>
#include "Globals.h"
#include "Graphics.h"
#include "Sprite.h"

#define TNTLOGOFILENAME     "sprites\\tntlogo.bmp"
#define CHILILOGOFILENAME   "sprites\\chililogo.bmp"

constexpr auto CHILI_SPEED = 8;

constexpr auto COLORBAR_BORDER_TOP = 200;
constexpr auto COLORBAR_BORDER_BOTTOM = 200;
constexpr auto COLORBAR_MIN_SPEED = 0.6f;
constexpr auto COLORBAR_MAX_SPEED = 0.6f;
constexpr auto COLORBAR_NR = 8;
constexpr auto COLORBAR_DARKEST_COLOR = 0x40;
constexpr auto COLORBAR_MIN_HEIGHT = 10;
constexpr auto COLORBAR_MAX_HEIGHT = 10 ;
constexpr auto COLORBAR_SPACER = 20;

constexpr auto SCROLLER_TEXT = "" "SOOOOO you thought I was dead and buried and over but you were wrong." " From the man who brought you the AMAZING (ahem) but shameless Arkanoid " "ripoff two years ago, please put your hands together for the awesome crap " "that is currently gracing your " "screen. And destroying your ears, probably, but you'll have to blame " "Jesper Kyd for that. " "So a warm hello and welcome to this charmingly oldschool" " sine scroller. I hacked it together in a few evenings to see how hard it " "would be to make my own little cracktro." " Apparently it is quite doable ;) What is more difficult is to make it look" " aesthetic. I'm not really satisfied with the rolling bars but I decided " "I spent enough time fucking around with it. " "To think that " "crackers could make similar intro's on limited machines such as the " "ATARI ST and the Commodore Amiga just goes to show how talented they were" ", and how lazy modern computers " "with their ultra fast processors make us . . .     The music you are " "hearing is called \"Global Trash 3 v2.0\" and was made by " "Jesper Kyd from the Silents Amiga demo group (they later appeared in the " "PC scene as well) and dates back to october 1991. I kinda " "like its dark vibe so I decided" " to use it here for your pleasure. I hope you like it, if not, you can " "replace it with " "another .mod file of your liking (find more on https://modarchive.org/)." " Due to hardware limitations on the Commodore Amiga you'll never hear " "more than 4 instruments at once. " " It is reproduced " "here using my own replay routines of which I'm very proud. Cubic " "interpolation and volume" " ramping give you a sound that is - hopefully - smoother than a baby's " "ass and clickfree. " "It can play other variants as .S3M, .IT and .XM as well, although volume " "and panning " "envelopes have yet to be implemented. So .IT and .XM will sound awkward " "and just plain bad." " Also, since I have not yet implemented double buffered replay, this intro " "requires about " "halve a gigabyte of RAM or so :D. Take that Google Chrome lol. Yeah, huge " "amounts of RAM makes us lazy too. Or rather, " "it makes ME lazy. It takes one second to precalculate 16' of music on my " "core i5. As you now know, the little intro is just called by " "the music replay program if it gets started without parameters. If you " "want to test it with your own mods, just drop them onto the \"modplay.exe\"" " file from Windows explorer and off you go. It is a listen-only affair for" " now though, it will not show anything except for some debug info. . .    " "        If you are wondering what this thing really is, search on youtube " "for \"demoscene\" or \"cracktro\". It should be a fullfilling experience !!" " Since nobody was asking what I was up to I thought I'd bore you with it " "anyway. Between my work schedule and my friend who tries to turn me into " "a web developer (yuck) there has not been much time to devote to c++ " "programming. I still managed to do a few things: I kind of got into .svg " "(scalable vector graphics) files as I discovered these are basically " "text files with instructions on how to draw the vector art, in .xml " "format. So I made an .xml parser, because I love a little challenge, " "especially if I can reinvent the wheel while doing it. I guess I'm curious " "\"if I could do that as well\". Must be a familiar feeling around here, " "that is how we learn right? So to be able to draw vector art I needed a " "few graphic routines, so far I made a function that draws square & cubic" " Bezier curves, and a lame floodfill algorithm. I'm now trying to figure " "out how to draw thick bezier curves, I guess I'll need to make a " "rasterizer (\"triangle draw machine\" ;) for that. . .          " "I thought it would be cool to be able to use vector graphics in the RTS " "game that I am making / dreaming about (pick one), so I can do some 2D " "zoom or something.      So that is where we stand today! It is basically " "time to wave goodbye at this point. So keep up the good work, and till " "next time. At this rate my stuff will be finished around 2035 or something" ". Don't get your hopes up though. . .                  Source is as always " " on github, so have a look at github.com/TheRealByteraver ... if your " "stomach can handle it.                          Peace out";





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
            0x2F,
            0x20,
            0xFF
            
        );

        /*
        color = Color(
            colMin + rand() % colVar,
            colMin + rand() % colVar,
            colMin + rand() % colVar
            );
        */
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
            y -= speed;
            if ( (int)y <= minY ) {
                y = (float)minY + 1;
                movingUp = false;
            }
        }
        else {
            y += speed;
            if ( (int)y >= maxY ) {
                y = (float)maxY - 1;
                movingUp = true;
            }
        }
    }
    /*
    void setHeight( int yValue )
    {
        assert( yValue >= height );
        assert( yValue < Graphics::ScreenHeight - height );
        y = yValue;
    }
    void setWidth( int width )
    {
        assert( width < Graphics::ScreenHeight / 2 );
        height = width / 2;
    }
    */

private:
    int     minY;
    int     maxY;
    Color   color;

public:
    bool    movingUp = true;
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
        chiliLogo.loadBitmap( GAME_FOLDER CHILILOGOFILENAME );
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

        chiliX = 0;
        chiliGoingRight = true;

        // div by 2 at minimum
        maxDeviation = (Graphics::ScreenWidth - blockWidth) / 4; 

        // precalculate sinus table with deviations included
        Offsets.resize( nrSteps );
        assert( Offsets.size() >= (unsigned)nrSteps );

        for ( unsigned i = 0; i < Offsets.size(); i++ ) {
            Offsets[i] = (int)(sin( ((float)i * 2.0 * 3.14159265359) / (float)nrSteps )
                * (float)maxDeviation );
        }

        // prepare the color bars:
        for ( unsigned i = 0; i < colorBars.size(); i++ ) {
            colorBars[i].y = (float)(COLORBAR_BORDER_TOP +
                ((colorBars[i].height * 2 + COLORBAR_SPACER) * i));
        }


        // start the scroller at the right edge of the screen:
        scrollerStartX = Graphics::ScreenWidth;  
        scrollerTextLength = (int)strlen( SCROLLER_TEXT );
    }

    void updateFrame( Graphics & gfx,Font& font )
    {
        // draw ColorBars and background
        for ( int y = 0; y < Graphics::ScreenHeight; y++ ) {
            unsigned r = 0;
            unsigned g = 0;
            unsigned b = 0;
            unsigned cnt = 0;
            for ( int iBar = 0; iBar < COLORBAR_NR; iBar++ ) {
                ColorBar& colorBar = colorBars[iBar];



                int heightDelta = Graphics::ScreenHeight - 
                    (COLORBAR_BORDER_TOP + COLORBAR_BORDER_BOTTOM);


                float deviation = (float)(sin(
                    (colorBar.y - COLORBAR_BORDER_TOP) * 3.14159265359 / heightDelta ) * 10.0);

                colorBar.speed = 1.5f + deviation / 3.0f;

                if ( colorBar.movingUp ) {
                    colorBar.height = (int)(16.0 - deviation);
                }
                else {
                    colorBar.height = 16 + (int)deviation;
                }



                int deltaY = abs( y - (int)colorBar.y );
                if ( deltaY < colorBar.height ) {
                    deltaY = colorBar.height - 1 - deltaY;
                    cnt++;
                    r += (0x80 * deltaY) / colorBar.height;
                    g += (0x20 * deltaY) / colorBar.height;
                    b += (0xA0 * deltaY) / colorBar.height;
                    /*
                    r += colorBar.colors[deltaY].GetR();
                    g += colorBar.colors[deltaY].GetG();
                    b += colorBar.colors[deltaY].GetB();
                    */
                }
            }
            //if ( false ) {
            if ( cnt > 0 ) { // original code
                if ( r > 0xFF )
                    r = 0xFF;
                if ( g > 0xFF )
                    g = 0xFF;
                if ( b > 0xFF )
                    b = 0xFF;
                gfx.drawHorLine( 0,(int)y,Graphics::ScreenWidth - 1,Color( r,g,b ) );
            }
            // draw checkerboard background
            else { 
                const int squareSize = 64;//8 + abs( Offsets[sineIndex] );//(y / 2 + 1); // power of two please :D
                const int yCol = y & (0xFFFF - (squareSize - 1));
                const Color colorA = Color( 
                    (0x30 * (Graphics::ScreenHeight - yCol)) / Graphics::ScreenHeight,
                    (0xE0 * (Graphics::ScreenHeight - yCol)) / Graphics::ScreenHeight,
                    (0xE0 * (Graphics::ScreenHeight - yCol)) / Graphics::ScreenHeight);
                const Color colorB = Color( 
                    (0xA0 * (yCol)) / Graphics::ScreenHeight,
                    (0x10 * (yCol)) / Graphics::ScreenHeight,
                    (0xA0 * (yCol)) / Graphics::ScreenHeight );
                Color col1,col2;
                int nrBlocks = Graphics::ScreenWidth / squareSize;
                if ( (y / squareSize) & 0x1 ) {
                //if ( y & squareSize ) {
                    col1 = colorB;
                    col2 = colorA;
                }
                else { 
                    col1 = colorA;
                    col2 = colorB;
                }
                int ofs = 0;
                for ( int i = 0; i < nrBlocks; i++ ) {
                    if ( i & 0x1 )
                        gfx.drawHorLine( ofs,y,ofs + squareSize - 1,col2 );
                    else
                        gfx.drawHorLine( ofs,y,ofs + squareSize - 1,col1 );
                    ofs += squareSize;
                }
                if ( nrBlocks & 0x1 )
                    gfx.drawHorLine( ofs,y,Graphics::ScreenWidth - 1,col2 );
                else
                    gfx.drawHorLine( ofs,y,Graphics::ScreenWidth - 1,col1 );
            }
            // finished with drawing checkerboard
        }
        // now move the color bars:
        for ( int iBar = 0; iBar < COLORBAR_NR; iBar++ )
            colorBars[iBar].updateFrame();

        // draw top Chili logo:
        gfx.paintSpriteKeyed( chiliX,10,chiliLogo,0 );
        if ( chiliGoingRight ) {
            chiliX += CHILI_SPEED;
            if ( chiliX >= Graphics::ScreenWidth - chiliLogo.getWidth() ) {
                chiliGoingRight = false;
                chiliX -= CHILI_SPEED;
            }
        } 
        else { 
            chiliX -= CHILI_SPEED;
            if ( chiliX <= 0 ) { 
                chiliGoingRight = true;
                chiliX += CHILI_SPEED;
            }
        }

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
                        gfx.PutPixel( x,Y,*logoLines ); 
                    logoLines++;
                }
            }
            else {          // draw even lines               
                ofs2 += nrSteps / 3;
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

        /* // debug
        for ( int s = 0; s < Graphics::ScreenWidth; s++ ) {
            gfx.drawDisc(
                s,
                Graphics::ScreenHeight / 2 + Offsets[(s * nrSteps) / Graphics::ScreenWidth],
                4,
                Colors::White
            );
        } 
        */

        // Draw bottom scroller:
        const int ampDivide = 4;
        int startX = scrollerStartX;
        scrollerStartX -= 6; // 8 == scrolling speed
        // restart scroll after a while:
        if ( -scrollerStartX > scrollerTextLength * 45 )
            scrollerStartX = Graphics::ScreenWidth * 2;

        int y = Graphics::ScreenHeight - 1 - font.height() - maxDeviation / ampDivide;
        for ( int i = 0; i < scrollerTextLength; i++ ) {
            //Sprite& charSprite = *((Sprite *)(font.getBmpData( SCROLLER_TEXT[i] )));
            const Sprite& charSprite = font.getBmpData( SCROLLER_TEXT[i] );

            int Y = startX % nrSteps;
            if ( Y > -50 )
            {
                if ( Y < 0 )
                    Y = 0;
                Y = Offsets[Y] / ampDivide;

                gfx.paintBMPClearTypeColor(
                    startX,
                    y + Y,
                    y - maxDeviation / ampDivide,
                    y + font.height() + maxDeviation / ampDivide,
                    charSprite,
                    Colors::Black,
                    0x0 );
            }
            startX += charSprite.getWidth();
        }
        /*
        gfx.drawHorLine( 0,y - maxDeviation / ampDivide,Graphics::ScreenWidth - 1,Colors::White );
        gfx.drawHorLine( 0,y + font.height() + maxDeviation / ampDivide,Graphics::ScreenWidth - 1,Colors::White );
        */
    }

private:
    Sprite              tntLogo;
    Sprite              chiliLogo;
    bool                chiliGoingRight;
    Color              *lineData;
    int                 sineIndex = 0;
    int                 blockWidth;
    int                 blockHeight;
    int                 halfBlockWidth;
    int                 halfBlockHeight;
    int                 x1;
    int                 x2;
    int                 y1;
    int                 chiliX;
    int                 maxDeviation;
    const int           nrSteps = 400; // 60 steps == 1 second
    std::vector<int>    Offsets;
    std::array<ColorBar,COLORBAR_NR> colorBars;
    int                 scrollerStartX;
    int                 scrollerTextLength;
};
