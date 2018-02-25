
#include "assert.h"
#include "CreateDefaultSprites.h"


extern class Defaults defaults;


void CreateDefaultSprites::createGreenPrairieWorld()
{
    // Create the common part of the filenames:
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );

    // Create and save the greenprairie (default) world tiles:
    createGreenPrairieWorldTiles();
    terrainSpriteLib_.saveToBMP( (path + GREENPRAIRIE_WORLD_NAME ".bmp").c_str() );

    // Create and save the greenprairie (default) world doodads:
    createGreenPrairieWorldDoodads(); 
    std::string doodadBmp = GREENPRAIRIE_WORLD_NAME DOODAD_SUFFIX ".bmp";
    terrainSpriteLib_.saveToBMP( (path + doodadBmp).c_str() );
   
    // Now create & save the .ini file:
    std::ofstream greenPrairieWorldIni( path + GREENPRAIRIE_WORLD_NAME ".ini" );
    greenPrairieWorldIni << "; This describes the " << GREENPRAIRIE_WORLD_NAME
        << " world spritesheet." << std::endl
        << "; If this file is corrupt then delete it, the game will"
        << " recreate it." << std::endl
        << ";" << std::endl
        << "[Tiles]" << std::endl
        << "Width=" << WORLD_TILE_DEFAULT_WIDTH << std::endl
        << "Height=" << WORLD_TILE_DEFAULT_WIDTH << std::endl
        << "Separator=" << TILE_SEPARATOR << std::endl
        << "Offset=" << TILE_X_OFFSET << std::endl
        << "NrOfTiles=" << NR_OF_TILES << std::endl
        << ";" << std::endl
        << "; This section defines and describes the " GREENPRAIRIE_WORLD_NAME " world doodad sprites." << std::endl
        << "; The 'Walkable' mask describes if the tile can be walked upon by units." << std::endl
        << "; A '1' means yes, a '0' means no." << std::endl
        << "; The Terrain mask is a bit more tricky. It describes what kind of terrain" << std::endl
        << "; the doodad can be placed upon. Each letter corresponds to a particular" << std::endl
        << "; type of basic terrain: low water, high water, low ground, transition tiles, etc." << std::endl
        << ";" << std::endl
        << "[Main]" << std::endl
        << "world=desert" << std::endl
        << ";" << std::endl
        << "[Doodad000]" << std::endl
        << "name=RampRightwards" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
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
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + width_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
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
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 6 + TILE_SEPARATOR) * 2 
        << "     ; in pixels" << std::endl
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
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 6 + TILE_SEPARATOR) * 3 
        << "     ; in pixels" << std::endl
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
        // tar pit smooth edges:
        << ";" << std::endl
        << "[Doodad004]" << std::endl
        << "name=LowTarPitSmoothDiagonalTopLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=G4" << std::endl
        << "TerrainMask01=6F" << std::endl
        << ";" << std::endl

        << "[Doodad005]" << std::endl
        << "name=LowTarPitSmoothDiagonalTopRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + width_ * 2 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=5G" << std::endl
        << "TerrainMask01=E6" << std::endl
        << ";" << std::endl

        << "[Doodad006]" << std::endl
        << "name=LowTarPitSmoothDiagonalBottomLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 2 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=7D" << std::endl
        << "TerrainMask01=G4" << std::endl
        << ";" << std::endl

        << "[Doodad007]" << std::endl
        << "name=LowTarPitSmoothDiagonalBottomRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 3 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=C7" << std::endl
        << "TerrainMask01=5G" << std::endl

        // From high to low terrain smooth edges:
        << ";" << std::endl
        << "[Doodad008]" << std::endl
        << "name=HighTerrainSmoothDiagonalTopLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 4 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=01" << std::endl
        << "TerrainMask00=G&" << std::endl
        << "TerrainMask01=#-" << std::endl
        << ";" << std::endl

        << "[Doodad009]" << std::endl
        << "name=HighTerrainSmoothDiagonalTopRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 5 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=10" << std::endl
        << "TerrainMask00=@G" << std::endl
        << "TerrainMask01=)#" << std::endl
        << ";" << std::endl

        << "[Doodad010]" << std::endl
        << "name=HighTerrainSmoothDiagonalBottomLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 6 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=01" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00='}" << std::endl
        << "TerrainMask01=G&" << std::endl
        << ";" << std::endl

        << "[Doodad011]" << std::endl
        << "name=HighTerrainSmoothDiagonalBottomRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 7 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=10" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00={'" << std::endl
        << "TerrainMask01=@G" << std::endl

        // High Water smooth edges:
        << ";" << std::endl
        << "[Doodad012]" << std::endl
        << "name=HighWaterSmoothDiagonalTopLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 8 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=W~" << std::endl
        << "TerrainMask01=\\>" << std::endl
        << ";" << std::endl

        << "[Doodad013]" << std::endl
        << "name=HighWaterSmoothDiagonalTopRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 9 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=/W" << std::endl
        << "TerrainMask01=<\\" << std::endl
        << ";" << std::endl

        << "[Doodad014]" << std::endl
        << "name=HighWaterSmoothDiagonalBottomLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 10 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=:$" << std::endl
        << "TerrainMask01=W~" << std::endl
        << ";" << std::endl

        << "[Doodad015]" << std::endl
        << "name=HighWaterSmoothDiagonalBottomRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 11 
            << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
            << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=%:" << std::endl
        << "TerrainMask01=/W" << std::endl
        ;
    // close the ini file:
    greenPrairieWorldIni.close();
}

void CreateDefaultSprites::createDesertWorld()
{
    // Create the common part of the filenames:
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );

    // Create and save the desert world tiles:
    createDesertWorldTiles();
    terrainSpriteLib_.saveToBMP( (path + DESERT_WORLD_NAME ".bmp").c_str() );

    // Create and save the desert world doodads:
    createDesertWorldDoodads();
    std::string doodadBmp = DESERT_WORLD_NAME DOODAD_SUFFIX ".bmp";
    terrainSpriteLib_.saveToBMP( (path + doodadBmp).c_str() );

    // Now create & save the .ini file:
    std::ofstream desertWorldIni( path + DESERT_WORLD_NAME ".ini" );
    desertWorldIni << "; This describes the " << DESERT_WORLD_NAME 
        << " world spritesheet." << std::endl
        << "; If this file is corrupt then delete it, the game will"
        << " recreate it." << std::endl
        << ";" << std::endl
        << "[Tiles]" << std::endl
        << "Width=" << WORLD_TILE_DEFAULT_WIDTH << std::endl
        << "Height=" << WORLD_TILE_DEFAULT_WIDTH << std::endl
        << "Separator=" << TILE_SEPARATOR << std::endl
        << "Offset=" << TILE_X_OFFSET << std::endl
        << "NrOfTiles=" << NR_OF_TILES << std::endl
        << ";" << std::endl
        << "; This section defines and describes the " DESERT_WORLD_NAME " world doodad sprites." << std::endl
        << "; The 'Walkable' mask describes if the tile can be walked upon by units." << std::endl
        << "; A '1' means yes, a '0' means no." << std::endl
        << "; The Terrain mask is a bit more tricky. It describes what kind of terrain" << std::endl
        << "; the doodad can be placed upon. Each letter corresponds to a particular" << std::endl
        << "; type of basic terrain: low water, high water, low ground, transition tiles, etc." << std::endl
        << ";" << std::endl
        << "[Main]" << std::endl
        << "world=desert" << std::endl
        << ";" << std::endl
        << "[Doodad000]" << std::endl
        << "name=RampRightwards" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
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
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + width_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
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
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 6 + TILE_SEPARATOR) * 2 
        << "     ; in pixels" << std::endl
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
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 6 + TILE_SEPARATOR ) * 3 
        << "     ; in pixels" << std::endl
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

        // tar pit smooth edges:
        << ";" << std::endl
        << "[Doodad004]" << std::endl
        << "name=LowTarPitSmoothDiagonalTopLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=G4" << std::endl
        << "TerrainMask01=6F" << std::endl
        << ";" << std::endl

        << "[Doodad005]" << std::endl
        << "name=LowTarPitSmoothDiagonalTopRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + width_ * 2 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=5G" << std::endl
        << "TerrainMask01=E6" << std::endl
        << ";" << std::endl

        << "[Doodad006]" << std::endl
        << "name=LowTarPitSmoothDiagonalBottomLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 2 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=7D" << std::endl
        << "TerrainMask01=G4" << std::endl
        << ";" << std::endl

        << "[Doodad007]" << std::endl
        << "name=LowTarPitSmoothDiagonalBottomRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 3 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=C7" << std::endl
        << "TerrainMask01=5G" << std::endl

        // From high to low terrain smooth edges:
        << ";" << std::endl
        << "[Doodad008]" << std::endl
        << "name=HighTerrainSmoothDiagonalTopLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 4 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=01" << std::endl
        << "TerrainMask00=G&" << std::endl
        << "TerrainMask01=#-" << std::endl
        << ";" << std::endl

        << "[Doodad009]" << std::endl
        << "name=HighTerrainSmoothDiagonalTopRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 5 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=10" << std::endl
        << "TerrainMask00=@G" << std::endl
        << "TerrainMask01=)#" << std::endl
        << ";" << std::endl

        << "[Doodad010]" << std::endl
        << "name=HighTerrainSmoothDiagonalBottomLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 6 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=01" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00='}" << std::endl
        << "TerrainMask01=G&" << std::endl
        << ";" << std::endl

        << "[Doodad011]" << std::endl
        << "name=HighTerrainSmoothDiagonalBottomRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 7 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=10" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00={'" << std::endl
        << "TerrainMask01=@G" << std::endl

        // High Water smooth edges:
        << ";" << std::endl
        << "[Doodad012]" << std::endl
        << "name=HighWaterSmoothDiagonalTopLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 8 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=W~" << std::endl
        << "TerrainMask01=\\>" << std::endl
        << ";" << std::endl

        << "[Doodad013]" << std::endl
        << "name=HighWaterSmoothDiagonalTopRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 9 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=/W" << std::endl
        << "TerrainMask01=<\\" << std::endl
        << ";" << std::endl

        << "[Doodad014]" << std::endl
        << "name=HighWaterSmoothDiagonalBottomLeft" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 10 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=:$" << std::endl
        << "TerrainMask01=W~" << std::endl
        << ";" << std::endl

        << "[Doodad015]" << std::endl
        << "name=HighWaterSmoothDiagonalBottomRight" << std::endl
        << "SourceFile=" << doodadBmp << std::endl
        << "locationX=" << TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 11 
        << "     ; in pixels" << std::endl
        << "locationY=" << TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
        << "     ; in pixels" << std::endl
        << "horSize=2       ; in tiles" << std::endl
        << "VerSize=2       ; in tiles" << std::endl
        << "WalkableMask00=00" << std::endl
        << "WalkableMask01=00" << std::endl
        << "TerrainMask00=%:" << std::endl
        << "TerrainMask01=/W" << std::endl
            ;
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
    // initialize our sprite library in memory: create an empty canvas
    terrainSpriteLib_.createEmptySprite(
        TERRAIN_SPRITE_LIB_WIDTH,
        TERRAIN_SPRITE_LIB_HEIGHT,
        Colors::Black );
    assert( terrainSpriteLib_.getWidth() == TERRAIN_SPRITE_LIB_WIDTH );
    assert( terrainSpriteLib_.getHeight() == TERRAIN_SPRITE_LIB_HEIGHT );
    // initialize some useful constants
    double pi = 3.1415926535;
    int spriteNr = 0;
    int x1,y1,x2,y2;
    int x,y,xStart,yStart,yEnd,yLast;
    double bigPeriod = 3.0 * pi; // Total distance of wave = 1,5 period = 3PI
    double bigAmp = width_ / 20.0;
    double smallPeriod = (width_ / 40.0) * pi;
    double smallAmp = 1.0 + width_ / 400.0;
    /*
    Start drawing terrain sprites.
    */
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_LOW_WATER,width_,height_ );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int xs = x1 + width_ / 2 + (int)w;
        terrainSpriteLib_.drawHorLine( x1,y,xs - 1,T_GREENPRAIRIE_LOW_COLOR );
        terrainSpriteLib_.drawHorLine( xs,y,x2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_LOW_WATER,width_,height_ );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int xs = x1 + width_ / 2 - (int)w;
        terrainSpriteLib_.drawHorLine( x1,y,xs - 1,T_GREENPRAIRIE_LOW_WATER_COLOR );
        terrainSpriteLib_.drawHorLine( xs,y,x2,T_GREENPRAIRIE_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_LOW_WATER,width_,height_ );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int ys = y1 + height_ / 2 + (int)w;
        terrainSpriteLib_.drawVerLine( x,y1,ys - 1,T_GREENPRAIRIE_LOW_COLOR );
        terrainSpriteLib_.drawVerLine( x,ys,y2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_LOW_WATER,width_,height_ );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int ys = y1 + height_ / 2 - (int)w;
        terrainSpriteLib_.drawVerLine( x,y1,ys - 1,T_GREENPRAIRIE_LOW_WATER_COLOR );
        terrainSpriteLib_.drawVerLine( x,ys,y2,T_GREENPRAIRIE_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_LEFT,width_,height_ );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,j - 1,x2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,j - 1,x2 - x,T_GREENPRAIRIE_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,y1 + y2 + 1 - j,x2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_GREENPRAIRIE_LOW_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,j - 1,x2,T_GREENPRAIRIE_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,j - 1,x2 - x,T_GREENPRAIRIE_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,y1 + y2 + 1 - j,x2,T_GREENPRAIRIE_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_WATER_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_GREENPRAIRIE_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_COLOR );

    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH,width_,height_ );
    x = x1 + width_ / 2;
    terrainSpriteLib_.drawBlock( x1,y1,x,y2,T_GREENPRAIRIE_LOW_COLOR );
    terrainSpriteLib_.drawBlock( x,y1,x2,y2,T_GREENPRAIRIE_LEFT_COLOR );

    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH,width_,height_ );
    x = x1 + width_ / 2;
    terrainSpriteLib_.drawBlock( x1,y1,x,y2,T_GREENPRAIRIE_RIGHT_COLOR );
    terrainSpriteLib_.drawBlock( x,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );

    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH,width_,height_ );
    y = y1 + height_ / 2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y,T_GREENPRAIRIE_LOW_COLOR );
    terrainSpriteLib_.drawBlock( x1,y,x2,y2,T_GREENPRAIRIE_TOP_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH,width_,height_ );
    y = y1 + height_ / 2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y,T_GREENPRAIRIE_BOTTOM_COLOR );
    terrainSpriteLib_.drawBlock( x1,y,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    x = x1 + width_ / 2 + 1;
    for ( int i = 0; i < width_ / 2 - 1; i++ )
        terrainSpriteLib_.drawHorLine( x + i,y2 - i,x2,T_GREENPRAIRIE_TOP_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    yStart = y1 + height_ / 2 + 1;
    for ( int i = 0; i < width_ / 2 - 1; i++ )
        terrainSpriteLib_.drawHorLine( x1,yStart + i,x1 + i,T_GREENPRAIRIE_TOP_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    xStart = x1 + width_ / 2 + 1;
    for ( int i = 0; i < width_ / 2 - 1; i++ )
        terrainSpriteLib_.drawHorLine( xStart + i,y1 + i,x2,T_GREENPRAIRIE_BOTTOM_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_LOW_COLOR );
    x = x1 + width_ / 2 - 1 - 1;
    for ( int i = 0; i < width_ / 2 - 1; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x - i,T_GREENPRAIRIE_BOTTOM_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_BOTTOM_RIGHT_COLOR );
    x = x1 + width_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x + i,y2 - i,x2,T_GREENPRAIRIE_LOW_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1,y2 - i,x1 + i,T_GREENPRAIRIE_HIGH_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_BOTTOM_LEFT_COLOR );
    yStart = y1 + height_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x1,yStart + i,x1 + i,T_GREENPRAIRIE_LOW_COLOR );

    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1 + i,y1 + i,x2,T_GREENPRAIRIE_HIGH_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_TOP_RIGHT_COLOR );
    xStart = x1 + width_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( xStart + i,y1 + i,x2,T_GREENPRAIRIE_LOW_COLOR );

    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x1 + i,T_GREENPRAIRIE_HIGH_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_TOP_LEFT_COLOR );
    x = x1 + width_ / 2 - 1;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x - i,T_GREENPRAIRIE_LOW_COLOR );

    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x2 - i,y1 + i,x2,T_GREENPRAIRIE_HIGH_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH_WATER,width_,height_ );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int xs = x1 + width_ / 2 + (int)w;
        terrainSpriteLib_.drawHorLine( x1,y,xs - 1,T_GREENPRAIRIE_HIGH_COLOR );
        terrainSpriteLib_.drawHorLine( xs,y,x2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH_WATER,width_,height_ );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int xs = x1 + width_ / 2 - (int)w;
        terrainSpriteLib_.drawHorLine( x1,y,xs - 1,T_GREENPRAIRIE_HIGH_WATER_COLOR );
        terrainSpriteLib_.drawHorLine( xs,y,x2,T_GREENPRAIRIE_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH_WATER,width_,height_ );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int ys = y1 + height_ / 2 + (int)w;
        terrainSpriteLib_.drawVerLine( x,y1,ys - 1,T_GREENPRAIRIE_HIGH_COLOR );
        terrainSpriteLib_.drawVerLine( x,ys,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH_WATER,width_,height_ );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int ys = y1 + height_ / 2 - (int)w;
        terrainSpriteLib_.drawVerLine( x,y1,ys - 1,T_GREENPRAIRIE_HIGH_WATER_COLOR );
        terrainSpriteLib_.drawVerLine( x,ys,y2,T_GREENPRAIRIE_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_LEFT,width_,height_ );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_COLOR );
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,j - 1,x2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,j - 1,x2 - x,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,y1 + y2 + 1 - j,x2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_TOP_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,j - 1,x2,T_GREENPRAIRIE_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,j - 1,x2 - x,T_GREENPRAIRIE_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,y1 + y2 + 1 - j,x2,T_GREENPRAIRIE_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    /* temp for debugging:
    width_ = 200;
    height_ = width_;
    const int xTest = terrainSpriteLib_.ScreenWidth - width_ - 1;
    const int yTest = terrainSpriteLib_.ScreenHeight - width_ - 1;
    x2 = xTest + width_ - 1;
    y2 = yTest + width_ - 1;
    terrainSpriteLib_.drawBox( xTest,yTest,x2,y2,Colors::Red );
    terrainSpriteLib_.drawBlock( xTest + 1,yTest + 1,x2 - 1,y2 - 1,T_LOW_WATER_COLOR );
    terrainSpriteLib_.drawHorLine( xTest,yTest + width_ / 2,x2,Colors::Red );
    terrainSpriteLib_.drawVerLine( xTest + width_ / 2,yTest,y2,Colors::Red );
    x1 = xTest;
    y1 = yTest;
    bigPeriod = 3.0 * pi;
    bigAmp = width_ / 40;
    smallPeriod = (width_ / 20) * pi;
    smallAmp = 1.0 + width_ / 400;
    // end debugging */
    yEnd = y1 + height_ / 2;
    y = y2;
    for ( int i = 0; i < width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < y1 ) y = y1;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_GREENPRAIRIE_HIGH_COLOR );
    }  
}

void CreateDefaultSprites::createGreenPrairieWorldDoodads()
{
    // 4 ramps size 6x6
    // 12 squares 2x2
    int minWidth = TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 12;
    int minHeight = TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR
        + (height_ * 2 + TILE_SEPARATOR);
    terrainSpriteLib_.createEmptySprite( minWidth,minHeight );
    int x = TILE_X_OFFSET;
    int y = TILE_Y_OFFSET;
    int halfWidth = width_ / 2;
    int halfHeight = height_ / 2;
    // draw ramp downwards to the right:
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ - 1,
        y + height_ * 6 - 1,
        T_GREENPRAIRIE_HIGH_COLOR
    );
    x += width_;
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ * 5 - 1,
        y + height_ * 6 - 1,
        T_GREENPRAIRIE_LOW_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x,
        y + height_,
        x + width_ * 4 - 1,
        y + height_ * 5 - 1,
        T_GREENPRAIRIE_RIGHT_COLOR
    );
    x += width_;    
    for ( int i = 0; i < height_; i++ )
    {
        terrainSpriteLib_.drawHorLine(
            x - width_,
            y + i,
            x - halfWidth - i / 2 - 1,
            T_GREENPRAIRIE_RIGHT_COLOR
        );        
        terrainSpriteLib_.drawHorLine(
            x - halfWidth - i / 2,
            y + i,
            x - halfWidth + (i * 7) / 2,
            T_GREENPRAIRIE_TOP_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x - width_,
            y + height_ * 6 - 1 - i,
            x - halfWidth - i / 2 - 1,
            T_GREENPRAIRIE_RIGHT_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x - halfWidth - i / 2,
            y + height_ * 6 - 1 - i,
            x - halfWidth + (i * 7) / 2,
            T_GREENPRAIRIE_BOTTOM_COLOR
        );
    }    
    x += width_ * 4 + TILE_SEPARATOR;
    // draw ramp downwards to the left:
    terrainSpriteLib_.drawBlock(
        x + width_ * 5,
        y,
        x + width_ * 6 - 1,
        y + height_ * 6 - 1,
        T_GREENPRAIRIE_HIGH_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ * 5 - 1,
        y + height_ * 6 - 1,
        T_GREENPRAIRIE_LOW_COLOR
    );
    x += width_;
    terrainSpriteLib_.drawBlock(
        x,
        y + height_,
        x + width_ * 4 - 1,
        y + height_ * 5 - 1,
        T_GREENPRAIRIE_LEFT_COLOR
    );
    x += width_ * 3 - 1;
    for ( int i = 0; i < height_; i++ )
    {
        terrainSpriteLib_.drawHorLine(
            x + halfWidth + i / 2 + 1,
            y + i,
            x + width_,
            T_GREENPRAIRIE_LEFT_COLOR
        );        
        terrainSpriteLib_.drawHorLine(
            x + halfWidth - (i * 7) / 2,
            y + i,
            x + halfWidth + i / 2,
            T_GREENPRAIRIE_TOP_COLOR
        );        
        terrainSpriteLib_.drawHorLine(
            x + halfWidth + i / 2 + 1,
            y + height_ * 6 - 1 - i,
            x + width_,
            T_GREENPRAIRIE_LEFT_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x + halfWidth - (i * 7) / 2,
            y + height_ * 6 - 1 - i,
            x + halfWidth + i / 2,
            T_GREENPRAIRIE_BOTTOM_COLOR
        );
    }
    x += width_ * 3 + 1 + TILE_SEPARATOR;
    // draw ramp downwards to the bottom:
    terrainSpriteLib_.drawBlock(
        x - width_,
        y,
        x + width_ * 5 - 1,
        y + height_ - 1,
        T_GREENPRAIRIE_HIGH_COLOR
    );
    y += height_;
    terrainSpriteLib_.drawBlock(
        x - width_,
        y,
        x + width_ * 5 - 1,
        y + height_ * 5 - 1,
        T_GREENPRAIRIE_LOW_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ * 4 - 1,
        y + height_ * 4 - 1,
        T_GREENPRAIRIE_BOTTOM_COLOR
    );
    x -= width_;
    y += height_;
    for ( int i = 0; i < width_; i++ )
    {
        terrainSpriteLib_.drawVerLine(
            x + i,
            y - height_,
            y - halfHeight - i / 2 - 1,
            T_GREENPRAIRIE_BOTTOM_COLOR
        );
        
        terrainSpriteLib_.drawVerLine(
            x + i,
            y - halfHeight - i / 2,
            y - halfHeight + (i * 7) / 2,
            T_GREENPRAIRIE_LEFT_COLOR
        );
        
        terrainSpriteLib_.drawVerLine(
            x + width_ * 6 - 1 - i,
            y - height_,
            y - halfHeight - i / 2 - 1,
            T_GREENPRAIRIE_BOTTOM_COLOR
        );
        terrainSpriteLib_.drawVerLine(
            x + width_ * 6 - 1 - i,
            y - halfHeight - i / 2,
            y - halfHeight + (i * 7) / 2,
            T_GREENPRAIRIE_RIGHT_COLOR
        );
    }
    x += width_ * 7 + TILE_SEPARATOR;
    y -= height_;
    // draw ramp downwards to the top:
    terrainSpriteLib_.drawBlock(
        x - width_,
        y + height_ * 4,
        x + width_ * 5 - 1,
        y + height_ * 5 - 1,
        T_GREENPRAIRIE_HIGH_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x - width_,
        y - height_,
        x + width_ * 5 - 1,
        y + height_ * 4 - 1,
        T_GREENPRAIRIE_LOW_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ * 4 - 1,
        y + height_ * 4 - 1,
        T_GREENPRAIRIE_TOP_COLOR
    );
    x -= width_;
    y += height_ * 3 - 1;
    for ( int i = 0; i < width_; i++ )
    {
        terrainSpriteLib_.drawVerLine(
            x + i,
            y + halfHeight + i / 2 + 1,
            y + height_,
            T_GREENPRAIRIE_TOP_COLOR
        );
        
        terrainSpriteLib_.drawVerLine(
            x + i,
            y + halfHeight - (i * 7) / 2,
            y + halfHeight + i / 2,
            T_GREENPRAIRIE_LEFT_COLOR
        );
        
        terrainSpriteLib_.drawVerLine(
            x + width_ * 6 - 1 - i,
            y + halfHeight + i / 2 + 1,
            y + height_,
            T_GREENPRAIRIE_TOP_COLOR
        );
        terrainSpriteLib_.drawVerLine(
            x + width_ * 6 - 1 - i,
            y + halfHeight - (i * 7) / 2,
            y + halfHeight + i / 2,
            T_GREENPRAIRIE_RIGHT_COLOR
        );
    }
    // Now start drawing the smooth diagonal edges. First we do
    // the LOW WATER terrain to LOW terrain edges. We use the previously
    // created basic terrain tiles for this.
    //Top left corner:
    x = TILE_X_OFFSET;
    y = TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR;
    // Create the common part of the filenames:
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    // Load the desert world tiles:
    Sprite tiles( (path + GREENPRAIRIE_WORLD_NAME ".bmp").c_str() );
    Sprite tmp;
    int x1 = TILE_X_OFFSET;
    int y1 = TILE_Y_OFFSET + height_ + TILE_SEPARATOR;
    
    // Now we do the LOW WATER to LOW TERRAIN edges. 
    createSmoothEdgeDoodads( x,y,tiles,1,T_GREENPRAIRIE_LOW_WATER_COLOR );
    // Now we do the HIGH TERRAIN to LOW TERRAIN edges. 
    x += (width_ * 2 + TILE_SEPARATOR) * 4;
    createSmoothEdgeDoodads( x,y,tiles,5,T_GREENPRAIRIE_HIGH_COLOR );
    // Now we do the HIGH WATER to HIGH TERRAIN edges. 
    x += (width_ * 2 + TILE_SEPARATOR) * 4;
    createSmoothEdgeDoodads( x,y,tiles,7,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    
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
    terrainSpriteLib_.createEmptySprite(
        TERRAIN_SPRITE_LIB_WIDTH,
        TERRAIN_SPRITE_LIB_HEIGHT,
        Colors::Black );
    assert( terrainSpriteLib_.getWidth() == TERRAIN_SPRITE_LIB_WIDTH );
    assert( terrainSpriteLib_.getHeight() == TERRAIN_SPRITE_LIB_HEIGHT );
    // initialize some useful constants
    double pi = 3.1415926535;
    int spriteNr = 0;
    int x1,y1,x2,y2;
    int x,y,xStart,yEnd,yLast;
    double bigPeriod = 3.0 * pi; // Total distance of wave = 1,5 period = 3PI
    double bigAmp = width_ / 20.0;
    double smallPeriod = (width_ / 40.0) * pi;
    double smallAmp = 1.0 + width_ / 400.0;
    /*
    Start drawing terrain sprites.
    */
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_LOW_WATER,width_,height_ );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int xs = x1 + width_ / 2 + (int)w;
        terrainSpriteLib_.drawHorLine( x1,y,xs - 1,T_DESERT_LOW_COLOR );
        terrainSpriteLib_.drawHorLine( xs,y,x2,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_LOW_WATER,width_,height_ );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int xs = x1 + width_ / 2 - (int)w;
        terrainSpriteLib_.drawHorLine( x1,y,xs - 1,T_DESERT_LOW_TARPIT_COLOR );
        terrainSpriteLib_.drawHorLine( xs,y,x2,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_LOW_WATER,width_,height_ );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int ys = y1 + height_ / 2 + (int)w;
        terrainSpriteLib_.drawVerLine( x,y1,ys - 1,T_DESERT_LOW_COLOR );
        terrainSpriteLib_.drawVerLine( x,ys,y2,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_LOW_WATER,width_,height_ );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int ys = y1 + height_ / 2 - (int)w;
        terrainSpriteLib_.drawVerLine( x,y1,ys - 1,T_DESERT_LOW_TARPIT_COLOR );
        terrainSpriteLib_.drawVerLine( x,ys,y2,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_LEFT,width_,height_ );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,j - 1,x2,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,j - 1,x2 - x,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,y1 + y2 + 1 - j,x2,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW_WATER_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_DESERT_LOW_TARPIT_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,j - 1,x2,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,j - 1,x2 - x,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,y1 + y2 + 1 - j,x2,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_LOW_WATER_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_TARPIT_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_DESERT_LOW_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_LOW,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH,width_,height_ );
    /*
    x = x1 + width_ / 2;
    terrainSpriteLib_.drawBlock( x1,y1,x,y2,T_DESERT_LOW_COLOR );
    terrainSpriteLib_.drawBlock( x,y1,x2,y2,T_DESERT_TRANS_SHADE_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH,width_,height_ );
    /*
    x = x1 + width_ / 2;
    terrainSpriteLib_.drawBlock( x1,y1,x,y2,T_DESERT_TRANS_SUN_COLOR );
    terrainSpriteLib_.drawBlock( x,y1,x2,y2,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH,width_,height_ );
    /*
    y = y1 + height_ / 2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y,T_DESERT_LOW_COLOR );
    terrainSpriteLib_.drawBlock( x1,y,x2,y2,T_DESERT_TRANS_SUN_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_TOP_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH,width_,height_ );
    /*
    y = y1 + height_ / 2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y,T_DESERT_TRANS_SHADE_COLOR );
    terrainSpriteLib_.drawBlock( x1,y,x2,y2,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_BOTTOM_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_LEFT,width_,height_ );
    /*
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    x = x1 + width_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x + i,y2 - i,x2,T_DESERT_TRANS_SUN_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1 + i,y2 - i,x2,T_DESERT_TOP_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_TOP_RIGHT,width_,height_ );
    /*
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    yStart = y1 + height_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x1,yStart + i,x1 + i,T_DESERT_TRANS_LIGHTSUN_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x1 + i,T_DESERT_TOP_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_LEFT,width_,height_ );
    /*
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    xStart = x1 + width_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( xStart + i,y1 + i,x2,T_DESERT_TRANS_DARKSHADE_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1 + i,y1 + i,x2,T_DESERT_BOTTOM_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_BOTTOM_RIGHT,width_,height_ );
    /*
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    x = x1 + width_ / 2 - 1;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x - i,T_DESERT_TRANS_SUN_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_LOW_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x2 - i,T_DESERT_BOTTOM_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_LEFT,width_,height_ );
    /*
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_TRANS_DARKSHADE_COLOR );
    x = x1 + width_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x + i,y2 - i,x2,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1 + i,y2 - i,x2,T_DESERT_BOTTOM_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_TOP_RIGHT,width_,height_ );
    /*
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_TRANS_DARKSHADE_COLOR );
    yStart = y1 + height_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x1,yStart + i,x1 + i,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x1 + i,T_DESERT_BOTTOM_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_LEFT,width_,height_ );
    /*
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_TRANS_LIGHTSUN_COLOR );
    xStart = x1 + width_ / 2;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( xStart + i,y1 + i,x2,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1 + i,y1 + i,x2,T_DESERT_TOP_RIGHT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_BOTTOM_RIGHT,width_,height_ );
    /*
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_TRANS_SUN_COLOR );
    x = x1 + width_ / 2 - 1;
    for ( int i = 0; i < width_ / 2; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x - i,T_DESERT_LOW_COLOR );
    */
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i < width_; i++ )
        terrainSpriteLib_.drawHorLine( x1,y1 + i,x2 - i,T_DESERT_TOP_LEFT_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_TO_HIGH_WATER,width_,height_ );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int xs = x1 + width_ / 2 + (int)w;
        terrainSpriteLib_.drawHorLine( x1,y,xs - 1,T_DESERT_HIGH_COLOR );
        terrainSpriteLib_.drawHorLine( xs,y,x2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_RIGHTWARDS_FROM_HIGH_WATER,width_,height_ );
    for ( int y = y1; y <= y2; y++ )
    {
        double w = sin( (double)(y - y1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(y - y1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int xs = x1 + width_ / 2 - (int)w;
        terrainSpriteLib_.drawHorLine( x1,y,xs - 1,T_GREENPRAIRIE_HIGH_WATER_COLOR );
        terrainSpriteLib_.drawHorLine( xs,y,x2,T_DESERT_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_TO_HIGH_WATER,width_,height_ );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int ys = y1 + height_ / 2 + (int)w;
        terrainSpriteLib_.drawVerLine( x,y1,ys - 1,T_DESERT_HIGH_COLOR );
        terrainSpriteLib_.drawVerLine( x,ys,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_DOWNWARDS_FROM_HIGH_WATER,width_,height_ );
    for ( int x = x1; x <= x2; x++ )
    {
        double w = sin( (double)(x - x1) * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)(x - x1) * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        int ys = y1 + height_ / 2 - (int)w;
        terrainSpriteLib_.drawVerLine( x,y1,ys - 1,T_GREENPRAIRIE_HIGH_WATER_COLOR );
        terrainSpriteLib_.drawVerLine( x,ys,y2,T_DESERT_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_LEFT,width_,height_ );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,j - 1,x2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,j - 1,x2 - x,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,y1 + y2 + 1 - j,x2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_HIGH_WATER_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_DESERT_HIGH_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_TOP_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,j - 1,x2,T_DESERT_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_TOP_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,j - 1,x2 - x,T_DESERT_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_BOTTOM_LEFT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    xStart = x1 + width_ / 2 - 1;
    yEnd = y1 + height_ / 2 - 1;
    y = y2;
    for ( int i = 0; i <= width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = xStart + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < xStart ) x = xStart;
        if ( y < yEnd ) y = yEnd;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x,y1 + y2 + 1 - j,x2,T_DESERT_HIGH_COLOR );
    }
    setDrawCoords( x1,y1,x2,y2,T_H_HIGH_WATER_BOTTOM_RIGHT,width_,height_ );
    terrainSpriteLib_.drawBlock( x1,y1,x2,y2,T_GREENPRAIRIE_HIGH_WATER_COLOR );
    /* temp for debugging:
    width_ = 200;
    height_ = width_;
    const int xTest = terrainSpriteLib_.ScreenWidth - width_ - 1;
    const int yTest = terrainSpriteLib_.ScreenHeight - width_ - 1;
    x2 = xTest + width_ - 1;
    y2 = yTest + width_ - 1;
    terrainSpriteLib_.drawBox( xTest,yTest,x2,y2,Colors::Red );
    terrainSpriteLib_.drawBlock( xTest + 1,yTest + 1,x2 - 1,y2 - 1,T_LOW_TARPIT_COLOR );
    terrainSpriteLib_.drawHorLine( xTest,yTest + width_ / 2,x2,Colors::Red );
    terrainSpriteLib_.drawVerLine( xTest + width_ / 2,yTest,y2,Colors::Red );
    x1 = xTest;
    y1 = yTest;
    bigPeriod = 3.0 * pi;
    bigAmp = width_ / 40;
    smallPeriod = (width_ / 20) * pi;
    smallAmp = 1.0 + width_ / 400;
    // end debugging */
    yEnd = y1 + height_ / 2;
    y = y2;
    for ( int i = 0; i < width_ / 2; i++ )
    {
        yLast = y;
        double w = sin( (double)i * bigPeriod / (double)(width_ / 2) ) * bigAmp
            + sin( (double)i * smallPeriod / (double)(width_ / 2) ) * smallAmp;
        x = width_ / 2 - 1 + i - (int)w;
        y = y2 - i - (int)w;
        if ( x < 0 ) x = 0;
        if ( y < y1 ) y = y1;
        for ( int j = yLast + 1; j >= y; j-- )
            terrainSpriteLib_.drawHorLine( x1,y1 + y2 + 1 - j,x2 - x,T_DESERT_HIGH_COLOR );
    }
}

void CreateDefaultSprites::createDesertWorldDoodads()
{

    /*
    // 4 tiles wide, four ramps
    int minWidth  = TILE_X_OFFSET + (width_ * 6 + TILE_SEPARATOR) * 4;
    int minHeight = TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR;
    */
    // 4 ramps size 6x6
    // 12 squares 2x2
    int minWidth = TILE_X_OFFSET + (width_ * 2 + TILE_SEPARATOR) * 12;
    int minHeight = TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR 
                                    + (height_ * 2 + TILE_SEPARATOR);
    terrainSpriteLib_.createEmptySprite( minWidth,minHeight );
    int x = TILE_X_OFFSET;
    int y = TILE_Y_OFFSET;
    // draw ramp downwards to the right:
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ - 1,
        y + height_ * 6 - 1,
        T_DESERT_HIGH_COLOR
    );
    x += width_;
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ * 5 - 1,
        y + height_ * 6 - 1,
        T_DESERT_LOW_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x,
        y + height_,
        x + width_ * 4 - 1,
        y + height_ * 5 - 1,
        T_DESERT_RIGHT_COLOR
    );
    x += width_;
    for ( int i = 0; i < height_; i++ )
    {
        terrainSpriteLib_.drawHorLine(
            x - width_,
            y + i,
            x - i - 1,
            T_DESERT_RIGHT_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x - i,
            y + i,
            x + i * 3,
            T_DESERT_TOP_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x - width_,
            y + height_ * 6 - 1 - i,
            x - i - 1,
            T_DESERT_RIGHT_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x - i,
            y + height_ * 6 - 1 - i,
            x + i * 3,
            T_DESERT_BOTTOM_COLOR
        );
    }
    x += width_ * 4 + TILE_SEPARATOR;
    // draw ramp downwards to the left:
    terrainSpriteLib_.drawBlock(
        x + width_ * 5,
        y,
        x + width_ * 6 - 1,
        y + height_ * 6 - 1,
        T_DESERT_HIGH_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ * 5 - 1,
        y + height_ * 6 - 1,
        T_DESERT_LOW_COLOR
    );
    x += width_;
    terrainSpriteLib_.drawBlock(
        x,
        y + height_,
        x + width_ * 4 - 1,
        y + height_ * 5 - 1,
        T_DESERT_LEFT_COLOR
    );
    x += width_ * 3 - 1;
    for ( int i = 0; i < height_; i++ )
    {
        terrainSpriteLib_.drawHorLine(
            x + i + 1,
            y + i,
            x + width_,
            T_DESERT_LEFT_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x - i * 3,
            y + i,
            x + i,
            T_DESERT_TOP_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x + i + 1,
            y + height_ * 6 - 1 - i,
            x + width_,
            T_DESERT_LEFT_COLOR
        );
        terrainSpriteLib_.drawHorLine(
            x - i * 3,
            y + height_ * 6 - 1 - i,
            x + i,
            T_DESERT_BOTTOM_COLOR
        );
    }
    x += width_ * 3 + 1 + TILE_SEPARATOR;
    // draw ramp downwards to the bottom:
    terrainSpriteLib_.drawBlock(
        x - width_,
        y,
        x + width_ * 5 - 1,
        y + height_ - 1,
        T_DESERT_HIGH_COLOR
    );
    y += height_;
    terrainSpriteLib_.drawBlock(
        x - width_,
        y + height_,
        x + width_ * 5 - 1,
        y + height_ * 5 - 1,
        T_DESERT_LOW_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ * 4 - 1,
        y + height_ * 4 - 1,
        T_DESERT_BOTTOM_COLOR
    );
    x -= width_;
    y += height_;
    for ( int i = 0; i < width_; i++ )
    {
        terrainSpriteLib_.drawVerLine(
            x + i,
            y - height_,
            y - i - 1,
            T_DESERT_BOTTOM_COLOR
        );
        terrainSpriteLib_.drawVerLine(
            x + i,
            y - i,
            y + i * 3,
            T_DESERT_LEFT_COLOR
        );
        terrainSpriteLib_.drawVerLine(
            x + width_ * 6 - 1 - i,
            y - height_,
            y - i - 1,
            T_DESERT_BOTTOM_COLOR
        );
        terrainSpriteLib_.drawVerLine(
            x + width_ * 6 - 1 - i,
            y - i,
            y + i * 3,
            T_DESERT_RIGHT_COLOR
        );
    }   
    x += width_ * 7 + TILE_SEPARATOR;
    y -= height_;
    // draw ramp downwards to the top:
    terrainSpriteLib_.drawBlock(
        x - width_,
        y + height_ * 4,
        x + width_ * 5 - 1,
        y + height_ * 5 - 1,
        T_DESERT_HIGH_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x - width_,
        y - height_,
        x + width_ * 5 - 1,
        y + height_ * 3 - 1,
        T_DESERT_LOW_COLOR
    );
    terrainSpriteLib_.drawBlock(
        x,
        y,
        x + width_ * 4 - 1,
        y + height_ * 4 - 1,
        T_DESERT_TOP_COLOR
    );
    x -= width_;
    y += height_ * 3 - 1;
    for ( int i = 0; i < width_; i++ )
    {
        terrainSpriteLib_.drawVerLine(
            x + i,
            y + i + 1,
            y + height_,
            T_DESERT_TOP_COLOR
        );
        terrainSpriteLib_.drawVerLine(
            x + i,
            y - i * 3,
            y + i,
            T_DESERT_LEFT_COLOR
        );
        terrainSpriteLib_.drawVerLine(
            x + width_ * 6 - 1 - i,
            y + i + 1,
            y + height_,
            T_DESERT_TOP_COLOR
        );
        terrainSpriteLib_.drawVerLine(
            x + width_ * 6 - 1 - i,
            y - i * 3,
            y + i,
            T_DESERT_RIGHT_COLOR
        );
    }
    // Now start drawing the smooth diagonal edges. First we do
    // the LOW WATER terrain to LOW terrain edges. We use the previously
    // created basic terrain tiles for this.
    //Top left corner:
    x = TILE_X_OFFSET;
    y = TILE_Y_OFFSET + height_ * 6 + TILE_SEPARATOR;
    // Create the common part of the filenames:
    std::string path( GAME_FOLDER );
    path.append( defaults.worldsFolder() );
    path.append( "\\" );
    // Load the desert world tiles:
    Sprite tiles( (path + DESERT_WORLD_NAME ".bmp").c_str() );
    Sprite tmp;
    int x1 = TILE_X_OFFSET;
    int y1 = TILE_Y_OFFSET + height_ + TILE_SEPARATOR;
    // Now we do the LOW WATER to LOW TERRAIN edges. 
    createSmoothEdgeDoodads( x,y,tiles,1,T_DESERT_LOW_TARPIT_COLOR );
    // Now we do the HIGH TERRAIN to LOW TERRAIN edges. 
    x += (width_ * 2 + TILE_SEPARATOR) * 4;
    createSmoothEdgeDoodads( x,y,tiles,5,T_DESERT_HIGH_COLOR );
    // Now we do the HIGH WATER to HIGH TERRAIN edges. 
    x += (width_ * 2 + TILE_SEPARATOR) * 4;
    createSmoothEdgeDoodads( x,y,tiles,7,T_DESERT_HIGH_WATER_COLOR );
}

void CreateDefaultSprites::createSmoothEdgeDoodads(
    int xDest,int yDest,
    const Sprite& tiles,
    int sourceTileRow,
    Color color )
{
    //Top left corner:
    int x = xDest;
    int y = yDest;
    int x1 = TILE_X_OFFSET;
    int y1 = TILE_Y_OFFSET + (height_ + TILE_SEPARATOR) * sourceTileRow;
    Rect source;
    Sprite tmp;
    source.x1 = x1;
    source.y1 = y1;
    source.x2 = source.x1 + width_ - 1;
    source.y2 = y1 + height_ - 1;
    tmp.createFromSprite( tiles,source );
    terrainSpriteLib_.insertFromSprite( x,y,tmp );
    source.x1 = TILE_X_OFFSET + (width_ + TILE_SEPARATOR) * 7;
    source.x2 = source.x1 + width_ - 1;
    tmp.createFromSprite( tiles,source );
    terrainSpriteLib_.insertFromSprite( x + width_,y,tmp );
    terrainSpriteLib_.insertFromSprite( x,y + height_,tmp );
    terrainSpriteLib_.drawBlock(
        x + width_,y + height_,
        x + width_ * 2 - 1,y + height_ * 2 - 1,
        color );
    // top right corner:
    x += width_ * 2 + TILE_SEPARATOR;
    source.x1 = TILE_X_OFFSET + width_ + TILE_SEPARATOR;
    source.x2 = source.x1 + width_ - 1;
    tmp.createFromSprite( tiles,source );
    terrainSpriteLib_.insertFromSprite( x + width_,y,tmp );
    source.x1 = TILE_X_OFFSET + (width_ + TILE_SEPARATOR) * 6;
    source.x2 = source.x1 + width_ - 1;
    tmp.createFromSprite( tiles,source );
    terrainSpriteLib_.insertFromSprite( x,y,tmp );
    terrainSpriteLib_.insertFromSprite( x + width_,y + height_,tmp );
    terrainSpriteLib_.drawBlock(
        x,y + height_,
        x + width_ - 1,y + height_ * 2 - 1,
        color );
    // bottom left corner:
    x += width_ * 2 + TILE_SEPARATOR;
    source.x1 = TILE_X_OFFSET + (width_ + TILE_SEPARATOR) * 2;
    source.x2 = source.x1 + width_ - 1;
    tmp.createFromSprite( tiles,source );
    terrainSpriteLib_.insertFromSprite( x,y + height_,tmp );
    source.x1 = TILE_X_OFFSET + (width_ + TILE_SEPARATOR) * 5;
    source.x2 = source.x1 + width_ - 1;
    tmp.createFromSprite( tiles,source );
    terrainSpriteLib_.insertFromSprite( x,y,tmp );
    terrainSpriteLib_.insertFromSprite( x + width_,y + height_,tmp );
    terrainSpriteLib_.drawBlock(
        x + width_,y,
        x + width_ * 2 - 1,y + height_ - 1,
        color );
    // bottom right corner:
    x += width_ * 2 + TILE_SEPARATOR;
    source.x1 = TILE_X_OFFSET + (width_ + TILE_SEPARATOR) * 3;
    source.x2 = source.x1 + width_ - 1;
    tmp.createFromSprite( tiles,source );
    terrainSpriteLib_.insertFromSprite( x + width_,y + height_,tmp );
    source.x1 = TILE_X_OFFSET + (width_ + TILE_SEPARATOR) * 4;
    source.x2 = source.x1 + width_ - 1;
    tmp.createFromSprite( tiles,source );
    terrainSpriteLib_.insertFromSprite( x + width_,y,tmp );
    terrainSpriteLib_.insertFromSprite( x,y + height_,tmp );
    terrainSpriteLib_.drawBlock(
        x,y,
        x + width_ - 1,y + height_ - 1,
        color );
}
