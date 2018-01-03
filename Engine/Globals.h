
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
#define GAME_FOLDER                 "c:\\RTSMedia\\" // should be ".\\"

// files:
#define MASTER_INIFILE_FILENAME     "RTSProject.ini"
#define RESOURCES_FILENAME          "Resources.ini"         // should be loaded from MASTER_INIFILE_FILENAME

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
    const std::string&  worldsFolder() { return worldsFolder_; }
    const std::string&  terrainsFolder() { return terrainsFolder_; }
    const std::string&  scenariosFolder() { return scenariosFolder_; }
    const std::string&  spritesFolder() { return spritesFolder_; }
    const std::string&  mediaFolder() { return mediaFolder_; }
    const std::string&  defaultWorld() { return defaultWorld_; }
    const std::string&  smallFontFile() { return smallfontFile_;  }
public:
    std::ofstream   debugLogFile;
private:
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
    std::string     racesFolder_;
    std::string     worldsFolder_;
    std::string     terrainsFolder_;
    std::string     scenariosFolder_;
    std::string     spritesFolder_;
    std::string     mediaFolder_;
    std::string     defaultWorld_;
    std::string     smallfontFile_;
};


