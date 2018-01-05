
#include "assert.h"
#include "CreateDefaultSprites.h"


extern class Defaults defaults;

// aide function for the drawing function 
void CreateDefaultSprites::setDrawCoords(
    int& x1,int& y1,int& x2,int& y2,
    const int spriteNr,const int width,const int height )
{
    const int currentRow = spriteNr / TILES_PER_ROW;
    const int currentColumn = spriteNr % TILES_PER_ROW;
    x1 = TILE_X_OFFSET + currentColumn * (width + TILE_SEPARATOR);
    y1 = TILE_Y_OFFSET + currentRow * (height + TILE_SEPARATOR);
    x2 = x1 + width - 1;
    y2 = y1 + height - 1;
};

/*
[Tiles]
Width=32
Height=32
Separator=1
Offset=1
NrOfTiles=64
SpriteLib=GreenPrairie.bmp

[Doodadds] ; ??

*/
void  CreateDefaultSprites::writeWorldIni()
{
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    path.append( defaults.defaultWorld() );
    path.append( ".ini" );
    std::ofstream earthWorld( path );
    earthWorld << "; This describes the default world spritesheet." << std::endl;
    earthWorld << "; If this file is corrupt then delete it, the game will recreate it." << std::endl;
    earthWorld << ";" << std::endl;
    earthWorld << "[Tiles]" << std::endl;
    earthWorld << "Width=" << WORLD_TILE_DEFAULT_WIDTH << std::endl;
    earthWorld << "Height=" << WORLD_TILE_DEFAULT_WIDTH << std::endl;
    earthWorld << "Separator=" << TILE_SEPARATOR << std::endl;
    earthWorld << "Offset=" << TILE_X_OFFSET << std::endl;
    earthWorld << "NrOfTiles=" << NR_OF_TILES << std::endl;
}

void CreateDefaultSprites::drawTerrainSprites()
{
    // initialize our sprite library in memory: create an empty canvas
    terrainSpriteLib.createEmptySprite(
        TERRAIN_SPRITE_LIB_WIDTH,
        TERRAIN_SPRITE_LIB_HEIGHT,
        Colors::Black );
    assert( terrainSpriteLib.getWidth() == TERRAIN_SPRITE_LIB_WIDTH );
    assert( terrainSpriteLib.getHeight() == TERRAIN_SPRITE_LIB_HEIGHT );
    // initialize some useful constants
    double pi = 3.1415926535;
    int width = WORLD_TILE_DEFAULT_WIDTH;    
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
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_LOW_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 + (int)w;
        terrainSpriteLib.drawHorLine( x1,y,xs - 1,T_LOW_COLOR );
        terrainSpriteLib.drawHorLine( xs,y,x2,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_LOW_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 - (int)w;
        terrainSpriteLib.drawHorLine( x1,y,xs - 1,T_LOW_WATER_COLOR );
        terrainSpriteLib.drawHorLine( xs,y,x2,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_LOW_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 + (int)w;
        terrainSpriteLib.drawVerLine( x,y1,ys - 1,T_LOW_COLOR );
        terrainSpriteLib.drawVerLine( x,ys,y2,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_LOW_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 - (int)w;
        terrainSpriteLib.drawVerLine( x,y1,ys - 1,T_LOW_WATER_COLOR );
        terrainSpriteLib.drawVerLine( x,ys,y2,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_LEFT,width,height );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
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
            terrainSpriteLib.drawHorLine( x,j - 1,x2,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,j - 1,x2 - x,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
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
            terrainSpriteLib.drawHorLine( x,y1 + y2 + 1 - j,x2,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
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
            terrainSpriteLib.drawHorLine( x,j - 1,x2,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,j - 1,x2 - x,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
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
            terrainSpriteLib.drawHorLine( x,y1 + y2 + 1 - j,x2,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_WATER_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH,width,height );
    x = x1 + width / 2;
    terrainSpriteLib.drawBlock( x1,y1,x,y2,T_LOW_COLOR );
    terrainSpriteLib.drawBlock( x,y1,x2,y2,T_TRANS_SHADE_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH,width,height );
    x = x1 + width / 2;
    terrainSpriteLib.drawBlock( x1,y1,x,y2,T_TRANS_SUN_COLOR );
    terrainSpriteLib.drawBlock( x,y1,x2,y2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH,width,height );
    y = y1 + height / 2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y,T_LOW_COLOR );
    terrainSpriteLib.drawBlock( x1,y,x2,y2,T_TRANS_SUN_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH,width,height );
    y = y1 + height / 2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y,T_TRANS_SHADE_COLOR );
    terrainSpriteLib.drawBlock( x1,y,x2,y2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    x = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x + i,y2 - i,x2,T_TRANS_SUN_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    yStart = y1 + height / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x1,yStart + i,x1 + i,T_TRANS_LIGHTSUN_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    xStart = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( xStart + i,y1 + i,x2,T_TRANS_DARKSHADE_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_LOW_COLOR );
    x = x1 + width / 2 - 1;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x1,y1 + i,x - i,T_TRANS_SUN_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_TRANS_DARKSHADE_COLOR );
    x = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x + i,y2 - i,x2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_TRANS_DARKSHADE_COLOR );
    yStart = y1 + height / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x1,yStart + i,x1 + i,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_TRANS_LIGHTSUN_COLOR );
    xStart = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( xStart + i,y1 + i,x2,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_TRANS_SUN_COLOR );
    x = x1 + width / 2 - 1;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x1,y1 + i,x - i,T_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 + (int)w;
        terrainSpriteLib.drawHorLine( x1,y,xs - 1,T_HIGH_COLOR );
        terrainSpriteLib.drawHorLine( xs,y,x2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 - (int)w;
        terrainSpriteLib.drawHorLine( x1,y,xs - 1,T_HIGH_WATER_COLOR );
        terrainSpriteLib.drawHorLine( xs,y,x2,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 + (int)w;
        terrainSpriteLib.drawVerLine( x,y1,ys - 1,T_HIGH_COLOR );
        terrainSpriteLib.drawVerLine( x,ys,y2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 - (int)w;
        terrainSpriteLib.drawVerLine( x,y1,ys - 1,T_HIGH_WATER_COLOR );
        terrainSpriteLib.drawVerLine( x,ys,y2,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_LEFT,width,height );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
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
            terrainSpriteLib.drawHorLine( x,j - 1,x2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,j - 1,x2 - x,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_BOTTOM_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
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
            terrainSpriteLib.drawHorLine( x,y1 + y2 + 1 - j,x2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_BOTTOM_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_TOP_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
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
            terrainSpriteLib.drawHorLine( x,j - 1,x2,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_TOP_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,j - 1,x2 - x,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_BOTTOM_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
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
            terrainSpriteLib.drawHorLine( x,y1 + y2 + 1 - j,x2,T_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_BOTTOM_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    /* temp for debugging:
    width = 200;
    height = width;
    const int xTest = terrainSpriteLib.ScreenWidth - width - 1;
    const int yTest = terrainSpriteLib.ScreenHeight - width - 1;
    x2 = xTest + width - 1;
    y2 = yTest + width - 1;
    terrainSpriteLib.drawBox( xTest,yTest,x2,y2,Colors::Red );
    terrainSpriteLib.drawBlock( xTest + 1,yTest + 1,x2 - 1,y2 - 1,T_LOW_WATER_COLOR );
    terrainSpriteLib.drawHorLine( xTest,yTest + width / 2,x2,Colors::Red );
    terrainSpriteLib.drawVerLine( xTest + width / 2,yTest,y2,Colors::Red );
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
            terrainSpriteLib.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_HIGH_COLOR );
    }   
}

void CreateDefaultSprites::saveTerrainSprites()
{
    // now save the sprites to a .bmp file:
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    path.append( defaults.defaultWorld() );
    path.append( ".bmp" );
    terrainSpriteLib.saveToBMP( path.c_str() );
}


