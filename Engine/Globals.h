
#pragma once

// folders:
#define GAME_FOLDER                 "c:\\RTSMedia\\" // should be ".\\"

#define RACES_SUBFOLDER             "Races\\"       // should be loaded from MASTER_INIFILE_FILENAME
#define TERRAINS_SUBFOLDER          "Terrains\\"    // should be loaded from MASTER_INIFILE_FILENAME 
#define SCENARIOS_SUBFOLDER         "Scenarios\\"   // should be loaded from MASTER_INIFILE_FILENAME

// files:
#define MASTER_INIFILE_FILENAME     "RTSProject.ini"
#define TERRAIN_SPRITE_LIB_FILENAME "TerrainSpriteLib.bmp"  // should be loaded from MASTER_INIFILE_FILENAME


#define RESOURCES_FILENAME          "Resources.ini"         // should be loaded from MASTER_INIFILE_FILENAME

// miscellaneous:
#define MIN_SCREENWIDTH             768     // should be loaded from MASTER_INIFILE_FILENAME
#define MIN_SCREENHEIGHT            432     // should be loaded from MASTER_INIFILE_FILENAME
#define MAX_SCREENWIDTH             1920    // should be loaded from MASTER_INIFILE_FILENAME
#define MAX_SCREENHEIGHT            1080    // should be loaded from MASTER_INIFILE_FILENAME

#include "Graphics.h"
#include "Inifile.h" 
#include "string.h"
#include "assert.h"
#include "windows.h"
#include <iostream>
#include <fstream>



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
    const std::string&  racesFolder() { return racesFolder_; }
    const std::string&  terrainsFolder() { return terrainsFolder_; }
    const std::string&  scenariosFolder() { return scenariosFolder_; }
    const std::string&  defaultTerrainFile() { return defaultTerrainFile_; }
public:
    std::ofstream   debugLogFile;
private:
    bool            dirExists( const std::string& dirName_in );
private:
    IniFile         masterIniFile_ = GAME_FOLDER MASTER_INIFILE_FILENAME;
    std::string     debugLogFilename_;
    int             screenWidth_;
    int             screenHeight_;
    int             minScreenWidth_;
    int             minScreenHeight_;
    int             maxScreenWidth_;
    int             maxScreenHeight_;
    std::string     racesFolder_;
    std::string     terrainsFolder_;
    std::string     scenariosFolder_;
    std::string     defaultTerrainFile_;
};// defaults; // declare a global variable of this class


