
#include "assert.h"
#include "CreateDefaultSprites.h"

// aide function for the drawing function 
void CreateDefaultSprites::setDrawCoords(
    int& x1,int& y1,int& x2,int& y2,
    const int spriteNr,const int width,const int height )
{
    assert( gfxPtr != nullptr );
    Graphics& gfx = *gfxPtr;
    const int spritesPerRow = (gfx.ScreenWidth - 1) / (width + 1);
    const int currentRow = spriteNr / spritesPerRow;
    const int currentColumn = spriteNr % spritesPerRow;
    x1 = 1 + currentColumn * (width + 1);
    y1 = 1 + currentRow * (height + 1);
    x2 = x1 + width - 1;
    y2 = y1 + height - 1;
};

void CreateDefaultSprites::drawTerrainSprites()
{
    assert( gfxPtr != nullptr );
    Graphics& gfx = *gfxPtr;
    double pi = 3.1415926535;
    int width = 32;    
    int height = width;
    int spriteNr = 0;
    int x1,y1,x2,y2;
    int x,y,xStart,yStart,yEnd,yLast;
    double bigPeriod = 3.0 * pi; // Total distance of wave = 1,5 period = 3PI
    double bigAmp = width / 20.0;
    double smallPeriod = (width / 40.0) * pi;
    double smallAmp = 1.0 + width / 400.0;
    /*
    Start drawing terrain sprites.
    */
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_LOW_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 + (int)w;
        gfx.drawHorLine( x1,y,xs - 1,T_LOW_COLOR );
        gfx.drawHorLine( xs,y,x2,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_LOW_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 - (int)w;
        gfx.drawHorLine( x1,y,xs - 1,T_LOW_WATER_COLOR );
        gfx.drawHorLine( xs,y,x2,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_LOW_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 + (int)w;
        gfx.drawVerLine( x,y1,ys - 1,T_LOW_COLOR );
        gfx.drawVerLine( x,ys,y2,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_LOW_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 - (int)w;
        gfx.drawVerLine( x,y1,ys - 1,T_LOW_WATER_COLOR );
        gfx.drawVerLine( x,ys,y2,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_LEFT,width,height );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x,j - 1,x2,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = width / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x1,j - 1,x2 - x,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x,y1 + y2 + 1 - j,x2,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = width / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x,j - 1,x2,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = width / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x1,j - 1,x2 - x,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x,y1 + y2 + 1 - j,x2,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = width / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH,width,height );
    x = x1 + width / 2;
    gfx.drawBlock( x1,y1,x,y2,T_LOW_COLOR );
    gfx.drawBlock( x,y1,x2,y2,T_TRANS_SHADE_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH,width,height );
    x = x1 + width / 2;
    gfx.drawBlock( x1,y1,x,y2,T_TRANS_SUN_COLOR );
    gfx.drawBlock( x,y1,x2,y2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH,width,height );
    y = y1 + height / 2;
    gfx.drawBlock( x1,y1,x2,y,T_LOW_COLOR );
    gfx.drawBlock( x1,y,x2,y2,T_TRANS_SUN_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH,width,height );
    y = y1 + height / 2;
    gfx.drawBlock( x1,y1,x2,y,T_TRANS_SHADE_COLOR );
    gfx.drawBlock( x1,y,x2,y2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    x = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        gfx.drawHorLine( x + i,y2 - i,x2,T_TRANS_SUN_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    yStart = y1 + height / 2;
    for ( int i = 0; i < width / 2; i++ )
        gfx.drawHorLine( x1,yStart + i,x1 + i,T_TRANS_LIGHTSUN_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    xStart = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        gfx.drawHorLine( xStart + i,y1 + i,x2,T_TRANS_DARKSHADE_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    x = x1 + width / 2 - 1;
    for ( int i = 0; i < width / 2; i++ )
        gfx.drawHorLine( x1,y1 + i,x - i,T_TRANS_SUN_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_TRANS_DARKSHADE_COLOR );
    x = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        gfx.drawHorLine( x + i,y2 - i,x2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_TRANS_DARKSHADE_COLOR );
    yStart = y1 + height / 2;
    for ( int i = 0; i < width / 2; i++ )
        gfx.drawHorLine( x1,yStart + i,x1 + i,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_TRANS_LIGHTSUN_COLOR );
    xStart = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        gfx.drawHorLine( xStart + i,y1 + i,x2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_TRANS_SUN_COLOR );
    x = x1 + width / 2 - 1;
    for ( int i = 0; i < width / 2; i++ )
        gfx.drawHorLine( x1,y1 + i,x - i,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 + (int)w;
        gfx.drawHorLine( x1,y,xs - 1,T_HIGH_COLOR );
        gfx.drawHorLine( xs,y,x2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 - (int)w;
        gfx.drawHorLine( x1,y,xs - 1,T_HIGH_WATER_COLOR );
        gfx.drawHorLine( xs,y,x2,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 + (int)w;
        gfx.drawVerLine( x,y1,ys - 1,T_HIGH_COLOR );
        gfx.drawVerLine( x,ys,y2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 - (int)w;
        gfx.drawVerLine( x,y1,ys - 1,T_HIGH_WATER_COLOR );
        gfx.drawVerLine( x,ys,y2,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_LEFT,width,height );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x,j - 1,x2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = width / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x1,j - 1,x2 - x,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_BOTTOM_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x,y1 + y2 + 1 - j,x2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_BOTTOM_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = width / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_TOP_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x,j - 1,x2,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_TOP_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = width / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x1,j - 1,x2 - x,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_BOTTOM_LEFT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x,y1 + y2 + 1 - j,x2,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_BOTTOM_RIGHT,width,height );
    gfx.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    /* temp for debugging:
    width = 200;
    height = width;
    const int xTest = gfx.ScreenWidth - width - 1;
    const int yTest = gfx.ScreenHeight - width - 1;
    x2 = xTest + width - 1;
    y2 = yTest + width - 1;
    gfx.drawBox( xTest,yTest,x2,y2,Colors::Red );
    gfx.drawBlock( xTest + 1,yTest + 1,x2 - 1,y2 - 1,T_LOW_WATER_COLOR );
    gfx.drawHorLine( xTest,yTest + width / 2,x2,Colors::Red );
    gfx.drawVerLine( xTest + width / 2,yTest,y2,Colors::Red );
    x1 = xTest;
    y1 = yTest;
    bigPeriod = 3.0 * pi;
    bigAmp = width / 40;
    smallPeriod = (width / 20) * pi;
    smallAmp = 1.0 + width / 400;
    // end debugging */
    yEnd = y1 + height / 2;
    y = y2;
    for ( int i = 0; i < width / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width / 2) ) * smallAmp;
        x = width / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < y1 ) y = y1;
        for ( int j = yLast + 1; j >= y; j-- )
            gfx.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_HIGH_COLOR );
    }
    /*
    // draw block "High ground sloping down rightwards:"
    x1 = x1 + width + 4;
    y1 = 100;
    width = 400;
    height = width;
    x2 = x1 + width - 1;
    y2 = y1 + height - 1;
    xStart = x1;
    yStart = y1;
    xEnd = x2;
    yEnd = y2;
    bigPeriod = pi;

    // draw green background:
    for ( int j = y1; j <= y2; j++ )
    for ( int i = x1; i <= x2; i++ )
    gfx.PutPixel( i,j,T_LOW_COLOR );
    //draw error checking bound frame:
    for ( int i = x1 - 1; i <= x2 + 1; i++ )
    {
    gfx.PutPixel( i,y1 - 1,FRAME_COLOR );
    gfx.PutPixel( i,y1 + height / 2,FRAME_COLOR );
    gfx.PutPixel( i,y2 + 1,FRAME_COLOR );
    }
    for ( int j = y1 - 1; j <= y2 + 1; j++ )
    {
    gfx.PutPixel( x1 - 1,j,FRAME_COLOR );
    gfx.PutPixel( x2 + 1,j,FRAME_COLOR );
    gfx.PutPixel( x1 + width / 2,j,FRAME_COLOR );
    }
    // draw hill slope:
    int rs = (T_HIGH_COLOR & 0xFF0000) >> 16;
    int gs = (T_HIGH_COLOR & 0xFF00) >> 8;
    int bs = (T_HIGH_COLOR & 0xFF);
    int rd = (LIGHT_BROWN & 0xFF0000) >> 16;
    int gd = (LIGHT_BROWN & 0xFF00) >> 8;
    int bd = (LIGHT_BROWN & 0xFF);
    int rDelta = rd - rs;
    int gDelta = gd - gs;
    int bDelta = bd - bs;
    int delta = width / 2;
    delta /= 10;
    for ( int i = 0; i < delta; i++ )
    {
    //double w = (1.0 - sin( (double)i * bigPeriod / delta )) / 2.0;
    int r = rs + ((i * rDelta) / delta);
    int g = gs + ((i * gDelta) / delta);
    int b = bs + ((i * bDelta) / delta);
    int c = (r << 16) | (g << 8) | b;
    for ( int j = y1; j <= y2; j++ ) gfx.PutPixel( xStart + i,j,c );
    }
    rs = (LIGHT_BROWN & 0xFF0000) >> 16;
    gs = (LIGHT_BROWN & 0xFF00) >> 8;
    bs = (LIGHT_BROWN & 0xFF);
    rd = (BROWN_COLOR & 0xFF0000) >> 16;
    gd = (BROWN_COLOR & 0xFF00) >> 8;
    bd = (BROWN_COLOR & 0xFF);
    rDelta = rd - rs;
    gDelta = gd - gs;
    bDelta = bd - bs;
    xStart += delta;
    delta *= 8;
    for ( int i = 0; i < delta; i++ )
    {
    //double w = (1.0 - sin( (double)i * bigPeriod / delta )) / 2.0;
    int r = rs + ((i * rDelta) / delta);
    int g = gs + ((i * gDelta) / delta);
    int b = bs + ((i * bDelta) / delta);
    int c = (r << 16) | (g << 8) | b;
    for ( int j = y1; j <= y2; j++ ) gfx.PutPixel( xStart + i,j,c );
    }

    rs = (BROWN_COLOR & 0xFF0000) >> 16;
    gs = (BROWN_COLOR & 0xFF00) >> 8;
    bs = (BROWN_COLOR & 0xFF);
    rd = (T_LOW_COLOR & 0xFF0000) >> 16;
    gd = (T_LOW_COLOR & 0xFF00) >> 8;
    bd = (T_LOW_COLOR & 0xFF);
    rDelta = rd - rs;
    gDelta = gd - gs;
    bDelta = bd - bs;
    xStart += delta;
    delta /= 8;
    for ( int i = 0; i < delta; i++ )
    {
    //double w = (1.0 - sin( (double)i * bigPeriod / delta )) / 2.0;
    int r = rs + ((i * rDelta) / delta);
    int g = gs + ((i * gDelta) / delta);
    int b = bs + ((i * bDelta) / delta);
    int c = (r << 16) | (g << 8) | b;
    for ( int j = y1; j <= y2; j++ ) gfx.PutPixel( xStart + i,j,c );
    }

    // -------------------------------

    rs = (BROWN_COLOR & 0xFF0000) >> 16;
    gs = (BROWN_COLOR & 0xFF00) >> 8;
    bs = (BROWN_COLOR & 0xFF);
    rd = (LOW_TERRAIN_COLOR & 0xFF0000) >> 16;
    gd = (LOW_TERRAIN_COLOR & 0xFF00) >> 8;
    bd = (LOW_TERRAIN_COLOR & 0xFF);
    rDelta = rd - rs;
    gDelta = gd - gs;
    bDelta = bd - bs;
    xStart = x1;
    delta = width;

    for ( int i = 0; i < delta; i++ )
    {
    //double w = (1.0 - sin( (double)i * bigPeriod / delta )) / 2.0;
    int r = rs + ((i * rDelta) / delta);
    int g = gs + ((i * gDelta) / delta);
    int b = bs + ((i * bDelta) / delta);
    int c = (r << 16) | (g << 8) | b;
    for ( int j = y1; j <= y2; j++ ) gfx.PutPixel( xStart + i,j,c );
    }

    // -------------------------------

    int w = 56;
    int jC = gfx.ScreenHeight / w;
    int iC = gfx.ScreenWidth / w;
    for( int j = 0; j < jC; j++ )
    for ( int i = 0; i < iC; i++ )
    {
    for ( int c = 0; c < gfx.ScreenWidth; c++ )
    gfx.PutPixel( c, j * w, 0xFF1010 );
    for ( int c = 0; c < gfx.ScreenHeight; c++ )
    gfx.PutPixel( i * w,c,0xFF1010 );
    }

    gfx.setFont( &font );
    gfx.printXY( 0,0,"Hello in standard text" );
    gfx.setFont( &neuropolXBMP );
    gfx.printXY( 0,50,"Hello in NeuropolX" );
    */
}


