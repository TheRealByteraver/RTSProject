
#pragma once

#include "Graphics.h"
#include "Inifile.h" 
#include "string.h"
#include "assert.h"
#include "windows.h"
#include "createDefaultSprites.h"
#include <iostream>
#include <fstream>

// folders:
#define GAME_FOLDER                 "c:\\RTSMedia\\" // debug, should be ".\\"
//#define GAME_FOLDER                 ".\\"

// files:
#define MASTER_INIFILE_FILENAME     "RTSProject.ini"
#define GREENPRAIRIE_WORLD_NAME     "GreenPrairie"
#define DESERT_WORLD_NAME           "desert"
#define DOODAD_SUFFIX               "ddd"

constexpr auto WORLD_MAJOR_FILE_REVISION = 1;
constexpr auto WORLD_MINOR_FILE_REVISION = 0;

// map dimension constants
constexpr auto MIN_TERRAIN_WIDTH = 64;
constexpr auto MIN_TERRAIN_HEIGHT = 64;
constexpr auto MAX_TERRAIN_WIDTH = 256;
constexpr auto MAX_TERRAIN_HEIGHT = 256;
constexpr auto DEFAULT_TERRAIN_WIDTH = 96;
constexpr auto DEFAULT_TERRAIN_HEIGHT = 96;


/*
This little piece of code makes it possible to define the resolution in
the main configuration .ini file. It also opens a global debug logfile.
*/
class Defaults
{
public:
    Defaults();
    ~Defaults()
    {
        debugLogFile.close();
    }
    const std::string&  racesFolder() const { return racesFolder_; }
    const std::string&  worldsFolder() const { return worldsFolder_; }
    const std::string&  terrainsFolder() const { return terrainsFolder_; }
    const std::string&  scenariosFolder() const { return scenariosFolder_; }
    const std::string&  spritesFolder() const { return spritesFolder_; }
    const std::string&  mediaFolder() const { return mediaFolder_; }
    const std::string&  soundsFolder() const { return soundsFolder_; }
    const std::string&  soundTrackFolder() const { return soundTrackFolder_; }
    const std::string&  defaultRace() const { return defaultRace_; }
    const std::string&  defaultWorld() const { return defaultWorld_; }
    const std::string&  smallFontFile() const { return smallfontFile_; }
    int                 defaultTerrainWidth() const { return defaultTerrainWidth_; }
    int                 defaultTerrainHeight() const { return defaultTerrainHeight_; }
public:
    std::ofstream   debugLogFile;
private:
    void            checkFolder( const std::string& folderType,std::string& folderName );
    bool            dirExists( const std::string& dirName );
    bool            fileExists( const std::string& fileName );
private:
    IniFile         masterIniFile_ = GAME_FOLDER MASTER_INIFILE_FILENAME;
    std::string     debugLogFilename_;
    int             screenWidth_;
    int             screenHeight_;
    int             minScreenWidth_;
    int             minScreenHeight_;
    int             maxScreenWidth_;
    int             maxScreenHeight_;
    int             defaultTerrainWidth_;
    int             defaultTerrainHeight_;
    std::string     racesFolder_;
    std::string     worldsFolder_;
    std::string     terrainsFolder_;
    std::string     scenariosFolder_;
    std::string     spritesFolder_;
    std::string     mediaFolder_;
    std::string     soundsFolder_;
    std::string     soundTrackFolder_;
    std::string     defaultRace_;
    std::string     defaultWorld_;
    std::string     smallfontFile_;
};


