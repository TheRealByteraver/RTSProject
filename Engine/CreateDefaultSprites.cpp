
#include "assert.h"
#include "CreateDefaultSprites.h"


extern class Defaults defaults;


void CreateDefaultSprites::createGreenPrairieWorld()
{
    // Create the common part of the filenames:
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    path.append( defaults.defaultWorld() );
    // Create and save the greenprairie (default) world tiles:
    createGreenPrairieWorldTiles();
    terrainSpriteLib.saveToBMP( (path + ".bmp").c_str() );

    /*  // To add later    
    // Create and save the greenprairie (default) world doodadds:
    createGreenPrairieWorldDoodAdds(); 
    std::string doodAddBmp = path + DOODADD_SUFFIX ".bmp";
    terrainSpriteLib.saveToBMP( doodAddBmp.c_str() );
    */

    // Now create & save the .ini file:
    int width = WORLD_TILE_DEFAULT_WIDTH;
    std::ofstream defaultWorldIni( path + ".ini" );
    defaultWorldIni << "; This describes the " << defaults.defaultWorld()
        << " world spritesheet." << std::endl;
    defaultWorldIni
        << "; If this file is corrupt then delete it, the game will"
        << " recreate it." << std::endl;
    defaultWorldIni << ";" << std::endl;
    defaultWorldIni << "[Tiles]" << std::endl;
    defaultWorldIni << "Width=" << WORLD_TILE_DEFAULT_WIDTH << std::endl;
    defaultWorldIni << "Height=" << WORLD_TILE_DEFAULT_WIDTH << std::endl;
    defaultWorldIni << "Separator=" << TILE_SEPARATOR << std::endl;
    defaultWorldIni << "Offset=" << TILE_X_OFFSET << std::endl;
    defaultWorldIni << "NrOfTiles=" << NR_OF_TILES << std::endl;
    defaultWorldIni << ";" << std::endl;
    // to add: doodadd definitions
    defaultWorldIni.close();
}

void CreateDefaultSprites::createDesertWorld()
{
    // Create the common part of the filenames:
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    //path.append( DESERT_WORLD_NAME );
    // Create and save the desert world tiles:
    createDesertWorldTiles();
    terrainSpriteLib.saveToBMP( (path + DESERT_WORLD_NAME ".bmp").c_str() );
    // Create and save the desert world doodadds:
    createDesertWorldDoodAdds();
    std::string doodAddBmp = DESERT_WORLD_NAME DOODAD_SUFFIX ".bmp";
    terrainSpriteLib.saveToBMP( (path + doodAddBmp).c_str() );
    // Now create & save the .ini file:
    int width = WORLD_TILE_DEFAULT_WIDTH;
    std::ofstream desertWorldIni( path + DESERT_WORLD_NAME ".ini" );
    desertWorldIni << "; This describes the " << DESERT_WORLD_NAME 
        << " world spritesheet." << std::endl;
    desertWorldIni
        << "; If this file is corrupt then delete it, the game will"
        << " recreate it." << std::endl;
    desertWorldIni << ";" << std::endl;
    desertWorldIni << "[Tiles]" << std::endl;
    desertWorldIni << "Width=" << WORLD_TILE_DEFAULT_WIDTH << std::endl;
    desertWorldIni << "Height=" << WORLD_TILE_DEFAULT_WIDTH << std::endl;
    desertWorldIni << "Separator=" << TILE_SEPARATOR << std::endl;
    desertWorldIni << "Offset=" << TILE_X_OFFSET << std::endl;
    desertWorldIni << "NrOfTiles=" << NR_OF_TILES << std::endl;
    desertWorldIni << ";" << std::endl
        << "; This section defines and describes the " DESERT_WORLD_NAME " world doodadd sprites." << std::endl
        << "; The 'Walkable' mask describes if the tile can be walked upon by units." << std::endl
        << "; A '1' means yes, a '0' means no." << std::endl
        << "; The Terrain mask is a bit more tricky. It describes what kind of terrain" << std::endl
        << "; the doodadd can be placed upon. Each letter corresponds to a particular" << std::endl
        << "; type of basic terrain: low water, high water, low ground, transition tiles, etc." << std::endl
        << "; A " << '1' << " means the doodadd does not cover that part of the terrain. " << std::endl
        << ";" << std::endl
        << "[Main]" << std::endl
        << "world=desert" << std::endl
        << ";" << std::endl
        << "[Doodad000]" << std::endl
        << "name=RampRightwards" << std::endl
        << "SourceFile=" << doodAddBmp << std::endl
        << "locationX=" << TILE_X_OFFSET << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET << "     ; in pixels" << std::endl
        << "horSize=6       ; in tiles" << std::endl
        << "VerSize=6       ; in tiles" << std::endl
        << "WalkableMask00=100001" << std::endl
        << "WalkableMask01=111111" << std::endl
        << "WalkableMask02=111111" << std::endl
        << "WalkableMask03=111111" << std::endl
        << "WalkableMask04=111111" << std::endl
        << "WalkableMask05=100001" << std::endl
        << "TerrainMask00=W@GGGG" << std::endl
        << "TerrainMask01=W@GGGG" << std::endl
        << "TerrainMask02=W@GGGG" << std::endl
        << "TerrainMask03=W@GGGG" << std::endl
        << "TerrainMask04=W@GGGG" << std::endl
        << "TerrainMask05=W@GGGG" << std::endl
        << ";" << std::endl
        << "[Doodad001]" << std::endl
        << "name=RampLeftwards" << std::endl
        << "SourceFile=" << doodAddBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + width * 6 + TILE_SEPARATOR << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET << "     ; in pixels" << std::endl
        << "horSize=6       ; in tiles" << std::endl
        << "VerSize=6       ; in tiles" << std::endl
        << "WalkableMask00=100001" << std::endl
        << "WalkableMask01=111111" << std::endl
        << "WalkableMask02=111111" << std::endl
        << "WalkableMask03=111111" << std::endl
        << "WalkableMask04=111111" << std::endl
        << "WalkableMask05=100001" << std::endl
        << "TerrainMask00=GGGG&W" << std::endl
        << "TerrainMask01=GGGG&W" << std::endl
        << "TerrainMask02=GGGG&W" << std::endl
        << "TerrainMask03=GGGG&W" << std::endl
        << "TerrainMask04=GGGG&W" << std::endl
        << "TerrainMask05=GGGG&W" << std::endl
        << ";" << std::endl
        << "[Doodad002]" << std::endl
        << "name=RampDownwards" << std::endl
        << "SourceFile=" << doodAddBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width * 6 + TILE_SEPARATOR) * 2 << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET << "     ; in pixels" << std::endl
        << "horSize=6       ; in tiles" << std::endl
        << "VerSize=6       ; in tiles" << std::endl
        << "WalkableMask00=111111" << std::endl
        << "WalkableMask01=011110" << std::endl
        << "WalkableMask02=011110" << std::endl
        << "WalkableMask03=011110" << std::endl
        << "WalkableMask04=011110" << std::endl
        << "WalkableMask05=111111" << std::endl
        << "TerrainMask00=WWWWWW" << std::endl
        << "TerrainMask01=''''''" << std::endl
        << "TerrainMask02=GGGGGG" << std::endl
        << "TerrainMask03=GGGGGG" << std::endl
        << "TerrainMask04=GGGGGG" << std::endl
        << "TerrainMask05=GGGGGG" << std::endl
        << ";" << std::endl
        << "[Doodad003]" << std::endl
        << "name=RampUpwards" << std::endl
        << "SourceFile=" << doodAddBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width * 6 + TILE_SEPARATOR ) * 3 << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET << "     ; in pixels" << std::endl
        << "horSize=6       ; in tiles" << std::endl
        << "VerSize=6       ; in tiles" << std::endl
        << "WalkableMask00=111111" << std::endl
        << "WalkableMask01=011110" << std::endl
        << "WalkableMask02=011110" << std::endl
        << "WalkableMask03=011110" << std::endl
        << "WalkableMask04=011110" << std::endl
        << "WalkableMask05=111111" << std::endl
        << "TerrainMask00=GGGGGG" << std::endl
        << "TerrainMask01=GGGGGG" << std::endl
        << "TerrainMask02=GGGGGG" << std::endl
        << "TerrainMask03=GGGGGG" << std::endl
        << "TerrainMask04=######" << std::endl
        << "TerrainMask05=WWWWWW" << std::endl;

    // close the ini file:
    desertWorldIni.close();
}


// aide function for the drawing function 
void CreateDefaultSprites::setDrawCoords(
    int& x1,int& y1,int& x2,int& y2,
    const int tileNr,const int width,const int height )
{
    const int currentRow = tileNr / TILES_PER_ROW;
    const int currentColumn = tileNr % TILES_PER_ROW;
    x1 = TILE_X_OFFSET + currentColumn * (width + TILE_SEPARATOR);
    y1 = TILE_Y_OFFSET + currentRow * (height + TILE_SEPARATOR);
    x2 = x1 + width - 1;
    y2 = y1 + height - 1;
};

void CreateDefaultSprites::createGreenPrairieWorldTiles()
{    
    /*
    // Create the .ini file first:
    [Tiles]
    Width=32
    Height=32
    Separator=1
    Offset=1
    NrOfTiles=64
    SpriteLib=desert.bmp


    */    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    path.append( defaults.defaultWorld() );
    path.append( ".ini" );
    std::ofstream greenPrairieWorldIni( path );
    greenPrairieWorldIni << "; This describes the default world spritesheet."
        << std::endl;
    greenPrairieWorldIni
        << "; If this file is corrupt then delete it, the game will"
        << " recreate it." << std::endl;
    greenPrairieWorldIni << ";" << std::endl;
    greenPrairieWorldIni << "[Tiles]" << std::endl;
    greenPrairieWorldIni << "Width=" << WORLD_TILE_DEFAULT_WIDTH << std::endl;
    greenPrairieWorldIni << "Height=" << WORLD_TILE_DEFAULT_WIDTH << std::endl;
    greenPrairieWorldIni << "Separator=" << TILE_SEPARATOR << std::endl;
    greenPrairieWorldIni << "Offset=" << TILE_X_OFFSET << std::endl;
    greenPrairieWorldIni << "NrOfTiles=" << NR_OF_TILES << std::endl;
    greenPrairieWorldIni.close();

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
    // now save the sprites to a .bmp file:
    
    path.assign( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    path.append( defaults.defaultWorld() );
    path.append( ".bmp" );
    terrainSpriteLib.saveToBMP( path.c_str() );
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void CreateDefaultSprites::createDesertWorldTiles()
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
    int height = width /* * 2 */;
    int spriteNr = 0;
    int x1,y1,x2,y2;
    int x,y,xStart,yEnd,yLast;
    double bigPeriod = 3.0 * pi; // Total distance of wave = 1,5 period = 3PI
    double bigAmp = width / 20.0;
    double smallPeriod = (width / 40.0) * pi;
    double smallAmp = 1.0 + width / 400.0;
    /*
    Start drawing terrain sprites.
    */
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_LOW_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 + (int)w;
        terrainSpriteLib.drawHorLine( x1,y,xs - 1,T_DESERT_LOW_COLOR );
        terrainSpriteLib.drawHorLine( xs,y,x2,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_LOW_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 - (int)w;
        terrainSpriteLib.drawHorLine( x1,y,xs - 1,T_DESERT_LOW_TARPIT_COLOR );
        terrainSpriteLib.drawHorLine( xs,y,x2,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_LOW_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 + (int)w;
        terrainSpriteLib.drawVerLine( x,y1,ys - 1,T_DESERT_LOW_COLOR );
        terrainSpriteLib.drawVerLine( x,ys,y2,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_LOW_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 - (int)w;
        terrainSpriteLib.drawVerLine( x,y1,ys - 1,T_DESERT_LOW_TARPIT_COLOR );
        terrainSpriteLib.drawVerLine( x,ys,y2,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_LEFT,width,height );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
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
            terrainSpriteLib.drawHorLine( x,j - 1,x2,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,j - 1,x2 - x,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
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
            terrainSpriteLib.drawHorLine( x,y1 + y2 + 1 - j,x2,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
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
            terrainSpriteLib.drawHorLine( x,j - 1,x2,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,j - 1,x2 - x,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_LEFT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
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
            terrainSpriteLib.drawHorLine( x,y1 + y2 + 1 - j,x2,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_RIGHT,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH,width,height );
    /*
    x = x1 + width / 2;
    terrainSpriteLib.drawBlock( x1,y1,x,y2,T_DESERT_LOW_COLOR );
    terrainSpriteLib.drawBlock( x,y1,x2,y2,T_DESERT_TRANS_SHADE_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH,width,height );
    /*
    x = x1 + width / 2;
    terrainSpriteLib.drawBlock( x1,y1,x,y2,T_DESERT_TRANS_SUN_COLOR );
    terrainSpriteLib.drawBlock( x,y1,x2,y2,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH,width,height );
    /*
    y = y1 + height / 2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y,T_DESERT_LOW_COLOR );
    terrainSpriteLib.drawBlock( x1,y,x2,y2,T_DESERT_TRANS_SUN_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_TOP_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH,width,height );
    /*
    y = y1 + height / 2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y,T_DESERT_TRANS_SHADE_COLOR );
    terrainSpriteLib.drawBlock( x1,y,x2,y2,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_BOTTOM_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_LEFT,width,height );
    /*
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    x = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x + i,y2 - i,x2,T_DESERT_TRANS_SUN_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i < width; i++ )
        terrainSpriteLib.drawHorLine( x1 + i,y2 - i,x2,T_DESERT_TOP_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_RIGHT,width,height );
    /*
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    yStart = y1 + height / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x1,yStart + i,x1 + i,T_DESERT_TRANS_LIGHTSUN_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i < width; i++ )
        terrainSpriteLib.drawHorLine( x1,y1 + i,x1 + i,T_DESERT_TOP_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_LEFT,width,height );
    /*
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    xStart = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( xStart + i,y1 + i,x2,T_DESERT_TRANS_DARKSHADE_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i < width; i++ )
        terrainSpriteLib.drawHorLine( x1 + i,y1 + i,x2,T_DESERT_BOTTOM_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_RIGHT,width,height );
    /*
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    x = x1 + width / 2 - 1;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x1,y1 + i,x - i,T_DESERT_TRANS_SUN_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i < width; i++ )
        terrainSpriteLib.drawHorLine( x1,y1 + i,x2 - i,T_DESERT_BOTTOM_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_LEFT,width,height );
    /*
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_TRANS_DARKSHADE_COLOR );
    x = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x + i,y2 - i,x2,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i < width; i++ )
        terrainSpriteLib.drawHorLine( x1 + i,y2 - i,x2,T_DESERT_BOTTOM_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_RIGHT,width,height );
    /*
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_TRANS_DARKSHADE_COLOR );
    yStart = y1 + height / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x1,yStart + i,x1 + i,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i < width; i++ )
        terrainSpriteLib.drawHorLine( x1,y1 + i,x1 + i,T_DESERT_BOTTOM_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_LEFT,width,height );
    /*
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_TRANS_LIGHTSUN_COLOR );
    xStart = x1 + width / 2;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( xStart + i,y1 + i,x2,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i < width; i++ )
        terrainSpriteLib.drawHorLine( x1 + i,y1 + i,x2,T_DESERT_TOP_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_RIGHT,width,height );
    /*
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_TRANS_SUN_COLOR );
    x = x1 + width / 2 - 1;
    for ( int i = 0; i < width / 2; i++ )
        terrainSpriteLib.drawHorLine( x1,y1 + i,x - i,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i < width; i++ )
        terrainSpriteLib.drawHorLine( x1,y1 + i,x2 - i,T_DESERT_TOP_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER,width,height );
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_HIGH_WATER_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 + (int)w;
        terrainSpriteLib.drawHorLine( x1,y,xs - 1,T_DESERT_HIGH_COLOR );
        terrainSpriteLib.drawHorLine( xs,y,x2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH_WATER,width,height );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int xs = x1 + width / 2 - (int)w;
        terrainSpriteLib.drawHorLine( x1,y,xs - 1,T_HIGH_WATER_COLOR );
        terrainSpriteLib.drawHorLine( xs,y,x2,T_DESERT_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 + (int)w;
        terrainSpriteLib.drawVerLine( x,y1,ys - 1,T_DESERT_HIGH_COLOR );
        terrainSpriteLib.drawVerLine( x,ys,y2,T_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH_WATER,width,height );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width / 2) ) * smallAmp;
        int ys = y1 + height / 2 - (int)w;
        terrainSpriteLib.drawVerLine( x,y1,ys - 1,T_HIGH_WATER_COLOR );
        terrainSpriteLib.drawVerLine( x,ys,y2,T_DESERT_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_LEFT,width,height );
    xStart = x1 + width / 2 - 1;
    yEnd = y1 + height / 2 - 1;
    y = y2;
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
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
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
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
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
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
    terrainSpriteLib.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
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
            terrainSpriteLib.drawHorLine( x,j - 1,x2,T_DESERT_HIGH_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,j - 1,x2 - x,T_DESERT_HIGH_COLOR );
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
            terrainSpriteLib.drawHorLine( x,y1 + y2 + 1 - j,x2,T_DESERT_HIGH_COLOR );
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
    terrainSpriteLib.drawBlock( xTest + 1,yTest + 1,x2 - 1,y2 - 1,T_LOW_TARPIT_COLOR );
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
            terrainSpriteLib.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_DESERT_HIGH_COLOR );
    }
    /*
    // now save the sprites to a .bmp file:
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    path.append( DESERT_WORLD_NAME );
    path.append( ".bmp" );
    terrainSpriteLib.saveToBMP( path.c_str() );
    */
}

void CreateDefaultSprites::createDesertWorldDoodAdds()
{

    int width = WORLD_TILE_DEFAULT_WIDTH;
    int height = width;
    // 4 tiles wide, four ramps
    int minWidth  = TILE_X_OFFSET + (width * 6 + TILE_SEPARATOR) * 4;
    int minHeight = TILE_Y_OFFSET + height * 6 + TILE_SEPARATOR;
    terrainSpriteLib.createEmptySprite( minWidth,minHeight );
    int x = TILE_X_OFFSET;
    int y = TILE_Y_OFFSET;
    // draw ramp downwards to the right:
    terrainSpriteLib.drawBlock(
        x,
        y,
        x + width - 1,
        y + height * 6 - 1,
        T_DESERT_HIGH_COLOR
    );
    x += width;
    terrainSpriteLib.drawBlock(
        x,
        y,
        x + width * 5 - 1,
        y + height * 6 - 1,
        T_DESERT_LOW_COLOR
    );
    terrainSpriteLib.drawBlock(
        x,
        y + height,
        x + width * 4 - 1,
        y + height * 5 - 1,
        T_DESERT_RIGHT_COLOR
    );
    x += width;
    for ( int i = 0; i < height; i++ )
    {
        terrainSpriteLib.drawHorLine(
            x - width,
            y + i,
            x - i - 1,
            T_DESERT_RIGHT_COLOR
        );
        terrainSpriteLib.drawHorLine(
            x - i,
            y + i,
            x + i * 3,
            T_DESERT_TOP_COLOR
        );
        terrainSpriteLib.drawHorLine(
            x - width,
            y + height * 6 - 1 - i,
            x - i - 1,
            T_DESERT_RIGHT_COLOR
        );
        terrainSpriteLib.drawHorLine(
            x - i,
            y + height * 6 - 1 - i,
            x + i * 3,
            T_DESERT_BOTTOM_COLOR
        );
    }
    x += width * 4 + TILE_SEPARATOR;
    // draw ramp downwards to the left:
    terrainSpriteLib.drawBlock(
        x + width * 5,
        y,
        x + width * 6 - 1,
        y + height * 6 - 1,
        T_DESERT_HIGH_COLOR
    );
    terrainSpriteLib.drawBlock(
        x,
        y,
        x + width * 5 - 1,
        y + height * 6 - 1,
        T_DESERT_LOW_COLOR
    );
    x += width;
    terrainSpriteLib.drawBlock(
        x,
        y + height,
        x + width * 4 - 1,
        y + height * 5 - 1,
        T_DESERT_LEFT_COLOR
    );
    x += width * 3 - 1;
    for ( int i = 0; i < height; i++ )
    {
        terrainSpriteLib.drawHorLine(
            x + i + 1,
            y + i,
            x + width,
            T_DESERT_LEFT_COLOR
        );
        terrainSpriteLib.drawHorLine(
            x - i * 3,
            y + i,
            x + i,
            T_DESERT_TOP_COLOR
        );
        terrainSpriteLib.drawHorLine(
            x + i + 1,
            y + height * 6 - 1 - i,
            x + width,
            T_DESERT_LEFT_COLOR
        );
        terrainSpriteLib.drawHorLine(
            x - i * 3,
            y + height * 6 - 1 - i,
            x + i,
            T_DESERT_BOTTOM_COLOR
        );
    }
    x += width * 3 + 1 + TILE_SEPARATOR;
    // draw ramp downwards to the bottom:
    terrainSpriteLib.drawBlock(
        x - width,
        y,
        x + width * 5 - 1,
        y + height - 1,
        T_DESERT_HIGH_COLOR
    );
    y += height;
    terrainSpriteLib.drawBlock(
        x - width,
        y + height,
        x + width * 5 - 1,
        y + height * 5 - 1,
        T_DESERT_LOW_COLOR
    );
    terrainSpriteLib.drawBlock(
        x,
        y,
        x + width * 4 - 1,
        y + height * 4 - 1,
        T_DESERT_BOTTOM_COLOR
    );
    x -= width;
    y += height;
    for ( int i = 0; i < width; i++ )
    {
        terrainSpriteLib.drawVerLine(
            x + i,
            y - height,
            y - i - 1,
            T_DESERT_BOTTOM_COLOR
        );
        terrainSpriteLib.drawVerLine(
            x + i,
            y - i,
            y + i * 3,
            T_DESERT_LEFT_COLOR
        );
        terrainSpriteLib.drawVerLine(
            x + width * 6 - 1 - i,
            y - height,
            y - i - 1,
            T_DESERT_BOTTOM_COLOR
        );
        terrainSpriteLib.drawVerLine(
            x + width * 6 - 1 - i,
            y - i,
            y + i * 3,
            T_DESERT_RIGHT_COLOR
        );
    }   
    x += width * 7 + TILE_SEPARATOR;
    y -= height;
    // draw ramp downwards to the top:
    terrainSpriteLib.drawBlock(
        x - width,
        y + height * 4,
        x + width * 5 - 1,
        y + height * 5 - 1,
        T_DESERT_HIGH_COLOR
    );
    terrainSpriteLib.drawBlock(
        x - width,
        y - height,
        x + width * 5 - 1,
        y + height * 3 - 1,
        T_DESERT_LOW_COLOR
    );
    terrainSpriteLib.drawBlock(
        x,
        y,
        x + width * 4 - 1,
        y + height * 4 - 1,
        T_DESERT_TOP_COLOR
    );
    x -= width;
    y += height * 3 - 1;
    for ( int i = 0; i < width; i++ )
    {
        terrainSpriteLib.drawVerLine(
            x + i,
            y + i + 1,
            y + height,
            T_DESERT_TOP_COLOR
        );
        terrainSpriteLib.drawVerLine(
            x + i,
            y - i * 3,
            y + i,
            T_DESERT_LEFT_COLOR
        );
        terrainSpriteLib.drawVerLine(
            x + width * 6 - 1 - i,
            y + i + 1,
            y + height,
            T_DESERT_TOP_COLOR
        );
        terrainSpriteLib.drawVerLine(
            x + width * 6 - 1 - i,
            y - i * 3,
            y + i,
            T_DESERT_RIGHT_COLOR
        );
    }
    /*
    // now save the sprites to a .bmp file:
    std::string bmpPath( GAME_FOLDER );
    bmpPath.append( defaults.worldsFolder() );
    bmpPath.append( "\\" );
    std::string bmpFile( DESERT_WORLD_NAME );
    bmpFile.append( DOODADD_SUFFIX );
    bmpFile.append( ".bmp" );
    terrainSpriteLib.saveToBMP( (bmpPath + bmpFile).c_str() );
    */
    /* now write the doodadd .ini file:
    std::string iniPath( GAME_FOLDER );
    iniPath.append( defaults.worldsFolder() );
    iniPath.append( "\\" );
    iniPath.append( DESERT_WORLD_NAME );
    iniPath.append( DOODADD_SUFFIX );
    iniPath.append( ".ini" );
    std::ofstream desertWorldDoodAddIni( iniPath );
    desertWorldDoodAddIni
        << "; This describes the Desert world DoodAdd sprites." << std::endl
        << "; The 'Walkable' mask describes if the tile can be walked upon by units." << std::endl
        << "; A '1' means yes, a '0' means no." << std::endl
        << "; The Terrain mask is a bit more tricky. It describes what kind of terrain" << std::endl
        << "; the doodadd can be placed upon. Each letter corresponds to a particular" << std::endl
        << "; type of basic terrain: low water, high water, low ground, transition tiles, etc." << std::endl
        << "; A space means the Doodadd does not cover that part of the terrain. " << std::endl
        << ";" << std::endl
        << "[Main]" << std::endl
        << "world=desert" << std::endl
        << ";" << std::endl
        << "[DoodAdd000]" << std::endl
        << "name=RampRightwards" << std::endl
        << "SourceFile=" << bmpFile << std::endl
        << "locationX=" << TILE_X_OFFSET << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET << "     ; in pixels" << std::endl
        << "horSize=6       ; in tiles" << std::endl
        << "VerSize=6       ; in tiles" << std::endl
        << "WalkableMask00=100001" << std::endl
        << "WalkableMask01=111111" << std::endl
        << "WalkableMask02=111111" << std::endl
        << "WalkableMask03=111111" << std::endl
        << "WalkableMask04=111111" << std::endl
        << "WalkableMask05=100001" << std::endl
        << "TerrainMask00=W@GGGG" << std::endl
        << "TerrainMask01=W@GGGG" << std::endl
        << "TerrainMask02=W@GGGG" << std::endl
        << "TerrainMask03=W@GGGG" << std::endl
        << "TerrainMask04=W@GGGG" << std::endl
        << "TerrainMask05=W@GGGG" << std::endl
        << ";" << std::endl
        << "[DoodAdd001]" << std::endl
        << "name=RampLeftwards" << std::endl
        << "SourceFile=" << bmpFile << std::endl
        << "locationX=" << TILE_X_OFFSET + width * 6 << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET << "     ; in pixels" << std::endl
        << "horSize=6       ; in tiles" << std::endl
        << "VerSize=6       ; in tiles" << std::endl
        << "WalkableMask00=100001" << std::endl
        << "WalkableMask01=111111" << std::endl
        << "WalkableMask02=111111" << std::endl
        << "WalkableMask03=111111" << std::endl
        << "WalkableMask04=111111" << std::endl
        << "WalkableMask05=100001" << std::endl
        << "TerrainMask00=GGGG&W" << std::endl
        << "TerrainMask01=GGGG&W" << std::endl
        << "TerrainMask02=GGGG&W" << std::endl
        << "TerrainMask03=GGGG&W" << std::endl
        << "TerrainMask04=GGGG&W" << std::endl
        << "TerrainMask05=GGGG&W" << std::endl
        << ";" << std::endl
        << "[DoodAdd002]" << std::endl
        << "name=RampDownwards" << std::endl
        << "SourceFile=" << bmpFile << std::endl
        << "locationX=" << TILE_X_OFFSET + width * 12 << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET << "     ; in pixels" << std::endl
        << "horSize=6       ; in tiles" << std::endl
        << "VerSize=6       ; in tiles" << std::endl
        << "WalkableMask00=111111" << std::endl
        << "WalkableMask01=011110" << std::endl
        << "WalkableMask02=011110" << std::endl
        << "WalkableMask03=011110" << std::endl
        << "WalkableMask04=011110" << std::endl
        << "WalkableMask05=111111" << std::endl
        << "TerrainMask00=WWWWWW" << std::endl
        << "TerrainMask01=''''''" << std::endl
        << "TerrainMask02=GGGGGG" << std::endl
        << "TerrainMask03=GGGGGG" << std::endl
        << "TerrainMask04=GGGGGG" << std::endl
        << "TerrainMask05=GGGGGG" << std::endl
        << ";" << std::endl
        << "[DoodAdd003]" << std::endl
        << "name=RampUpwards" << std::endl
        << "SourceFile=" << bmpFile << std::endl
        << "locationX=" << TILE_X_OFFSET + width * 18 << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET << "     ; in pixels" << std::endl
        << "horSize=6       ; in tiles" << std::endl
        << "VerSize=6       ; in tiles" << std::endl
        << "WalkableMask00=111111" << std::endl
        << "WalkableMask01=011110" << std::endl
        << "WalkableMask02=011110" << std::endl
        << "WalkableMask03=011110" << std::endl
        << "WalkableMask04=011110" << std::endl
        << "WalkableMask05=111111" << std::endl
        << "TerrainMask00=GGGGGG" << std::endl
        << "TerrainMask01=GGGGGG" << std::endl
        << "TerrainMask02=GGGGGG" << std::endl
        << "TerrainMask03=GGGGGG" << std::endl
        << "TerrainMask04=######" << std::endl
        << "TerrainMask05=WWWWWW" << std::endl
        ;
    desertWorldDoodAddIni.close();*/
/*
[Main]
world=desert
;
[DoodAdd000]
name=RampRightwards
SourceFile= <<< bmpPath
locationX=offsetX   ; in pixels
locationY=offsetY   ; in pixels
horSize=6 ; in tiles 
VerSize=6 ; in tiles
WalkableMask00=100001
WalkableMask01=111111
WalkableMask02=111111
WalkableMask03=111111
WalkableMask04=111111
WalkableMask05=100001
TerrainMask00=W@GGGG
TerrainMask01=W@GGGG
TerrainMask02=W@GGGG
TerrainMask03=W@GGGG
TerrainMask04=W@GGGG
TerrainMask05=W@GGGG
;
[DoodAdd001]
name=RampLeftwards
SourceFile= <<< bmpPath
locationX=offsetX   ; in pixels
locationY=offsetY   ; in pixels
horSize=6 ; in tiles
VerSize=6 ; in tiles
WalkableMask00=100001
WalkableMask01=111111
WalkableMask02=111111
WalkableMask03=111111
WalkableMask04=111111
WalkableMask05=100001
TerrainMask00=GGGG&W
TerrainMask01=GGGG&W
TerrainMask02=GGGG&W
TerrainMask03=GGGG&W
TerrainMask04=GGGG&W
TerrainMask05=GGGG&W
;
[DoodAdd002]
name=RampDownwards
SourceFile= <<< bmpPath
locationX=offsetX   ; in pixels
locationY=offsetY   ; in pixels
horSize=6 ; in tiles
VerSize=6 ; in tiles
WalkableMask00=111111
WalkableMask01=011110
WalkableMask02=011110
WalkableMask03=011110
WalkableMask04=011110
WalkableMask05=111111
TerrainMask00=WWWWWW
TerrainMask01=''''''
TerrainMask02=GGGGGG
TerrainMask03=GGGGGG
TerrainMask04=GGGGGG
TerrainMask05=GGGGGG
;
[DoodAdd003]
name=RampUpwards
SourceFile= <<< bmpPath
locationX=offsetX   ; in pixels
locationY=offsetY   ; in pixels
horSize=6 ; in tiles
VerSize=6 ; in tiles
WalkableMask00=111111
WalkableMask01=011110
WalkableMask02=011110
WalkableMask03=011110
WalkableMask04=011110
WalkableMask05=111111
TerrainMask00=GGGGGG
TerrainMask01=GGGGGG
TerrainMask02=GGGGGG
TerrainMask03=GGGGGG
TerrainMask04=######
TerrainMask05=WWWWWW
*/
}

