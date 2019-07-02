#include "Globals.h"

// declare a global variable of this class, for it must have its constructor called
// before anything else:
Defaults defaults; 

Defaults::Defaults()
{
    if ( !masterIniFile_.isLoaded() )
    {
        MessageBox(
            0,
            L"Unable to open " GAME_FOLDER MASTER_INIFILE_FILENAME ", exiting.",
            L"Fatal Error",
            MB_OK
        );
        exit( -1 );
        return;
    }
    debugLogFilename_ = GAME_FOLDER;
    std::string inStr;
    masterIniFile_.getKeyValue( "Files","Debuglog",inStr );
    debugLogFilename_ += inStr;
    remove( debugLogFilename_.c_str() );
    debugLogFile.open( debugLogFilename_ );
    if ( !debugLogFile.is_open() )
    {
        MessageBox(
            0,
            L"Unable to open debuglog for writing, exiting.",
            L"Fatal Error",
            MB_OK );
        exit( -1 );
        return;
    }
    debugLogFile 
        << "Start of RTS Project debug log, defaults loaded." << std::endl;
    int error = 0;
    error += masterIniFile_.getKeyValue( "Main","ScreenWidth",screenWidth_ );
    error += masterIniFile_.getKeyValue( "Main","ScreenHeight",screenHeight_ );
    error += masterIniFile_.getKeyValue( "Main","MinScreenWidth",minScreenWidth_ );
    error += masterIniFile_.getKeyValue( "Main","MinScreenHeight",minScreenHeight_ );
    error += masterIniFile_.getKeyValue( "Main","MaxScreenWidth",maxScreenWidth_ );
    error += masterIniFile_.getKeyValue( "Main","MaxScreenHeight",maxScreenHeight_ );
    if ( (screenWidth_ < minScreenWidth_) ||
        (screenWidth_ > maxScreenWidth_) ||
        (screenHeight_ < minScreenHeight_) ||
        (screenHeight_ > maxScreenHeight_) || (error != 0) )
    {
        debugLogFile 
            << "Error reading window resolution from .ini file, exiting." 
            << std::endl;
        debugLogFile.close();
        exit( -1 );
        return;
    }
    Graphics::ScreenWidth = screenWidth_;
    Graphics::ScreenHeight = screenHeight_;
    debugLogFile << "Preparing to set window resolution to "
        << screenWidth_ << "x" << screenHeight_ << "." << std::endl;
    // initialize default terrain dimensions:
    error = masterIniFile_.getKeyValue( "Main","DefaultTerrainWidth",defaultTerrainWidth_ );
    if ( error != 0 ) defaultTerrainWidth_ = DEFAULT_TERRAIN_WIDTH;
    error = masterIniFile_.getKeyValue( "Main","DefaultTerrainHeight",defaultTerrainHeight_ );
    if ( error != 0 ) defaultTerrainHeight_ = DEFAULT_TERRAIN_HEIGHT;
    // load folders and check if they exist, create them if not:
    checkFolder( "Races",racesFolder_ );
    checkFolder( "Worlds",worldsFolder_ );
    checkFolder( "Terrains",terrainsFolder_ );
    checkFolder( "Scenarios",scenariosFolder_ );
    checkFolder( "Sprites",spritesFolder_ );
    checkFolder( "Media",mediaFolder_ );
    checkFolder( "Sounds",soundsFolder_ );
    checkFolder( "SoundTrack",soundTrackFolder_ );
    // load default font filename. Error checking is done when loading Font
    masterIniFile_.getKeyValue( "Files","smallFontFile",smallfontFile_ );
    // load default race name:
    error = masterIniFile_.getKeyValue( "Main","DefaultRace",defaultRace_ );
    if ( (error != 0) || (defaultRace_.length() == 0) )
    {
        debugLogFile 
            << "Error reading default Race .ini file, exiting." << std::endl;
        debugLogFile.close();
        exit( -1 );
        return;
    }
    // check if the GreenPrairie terrain is present and recreate it if it isn't
    std::string defWorldSpriteLib( GAME_FOLDER );
    defWorldSpriteLib.append( worldsFolder_ );
    defWorldSpriteLib.append( "\\" );
    defWorldSpriteLib.append( GREENPRAIRIE_WORLD_NAME );
    defWorldSpriteLib.append( ".bmp" );
    std::string defWorldIni( GAME_FOLDER );
    defWorldIni.append( worldsFolder_ );
    defWorldIni.append( "\\" );
    defWorldIni.append( GREENPRAIRIE_WORLD_NAME );
    defWorldIni.append( ".ini" );
    bool recreate = false;
    //recreate = true; // debug!!!
    if ( !fileExists( defWorldSpriteLib ) )
    {
        debugLogFile << "Can't open default terrain file " << defWorldSpriteLib 
            << ", recreating world " << GREENPRAIRIE_WORLD_NAME << "." << std::endl;
        recreate = true;
    }
    if ( ! fileExists( defWorldIni ) ) 
    {
        debugLogFile << "Can't open default terrain file " << defWorldIni
            << ", recreating world " << GREENPRAIRIE_WORLD_NAME << "." << std::endl;
        recreate = true;
    }     
    if ( recreate ) CreateDefaultSprites().createGreenPrairieWorld();

    // check if the DesertWorld world is present and create it if not:
    std::string desertWorldSpriteLib( GAME_FOLDER );
    desertWorldSpriteLib.append( worldsFolder_ );
    desertWorldSpriteLib.append( "\\" );
    desertWorldSpriteLib.append( DESERT_WORLD_NAME );
    desertWorldSpriteLib.append( ".bmp" );
    std::string desertWorldIni( GAME_FOLDER );
    desertWorldIni.append( worldsFolder_ );
    desertWorldIni.append( "\\" );
    desertWorldIni.append( DESERT_WORLD_NAME );
    desertWorldIni.append( ".ini" );
    recreate = false;
    if ( !fileExists( desertWorldSpriteLib ) )
    {
        debugLogFile << "Can't open default world file " << desertWorldSpriteLib
            << ", recreating world " << DESERT_WORLD_NAME << "." << std::endl;
        recreate = true;
    }
    if ( !fileExists( desertWorldIni ) )
    {
        debugLogFile << "Can't open default world file " << desertWorldIni
            << ", recreating world " << DESERT_WORLD_NAME << "." << std::endl;
        recreate = true;
    }
    //recreate = true; // debug !!!
    if ( recreate ) CreateDefaultSprites().createDesertWorld();

    // load default world name and check if the world exists:
    error = masterIniFile_.getKeyValue( "Main","DefaultWorld",defaultWorld_ );
    if ( (error != 0) || (defaultWorld_.length() == 0) )
    {
        defaultWorld_ = GREENPRAIRIE_WORLD_NAME;
    } else {
        /*
        std::string defWorldSpriteLib( GAME_FOLDER );
        defWorldSpriteLib.append( worldsFolder_ );
        defWorldSpriteLib.append( "\\" );
        defWorldSpriteLib.append( defaultWorld_ );
        defWorldSpriteLib.append( ".bmp" );
        std::string defWorldIni( GAME_FOLDER );
        defWorldIni.append( worldsFolder_ );
        defWorldIni.append( "\\" );
        defWorldIni.append( defaultWorld_ );
        defWorldIni.append( ".ini" );
        if ( !fileExists( defWorldSpriteLib ) )
        {
            debugLogFile << "Can't open default world file " << defWorldSpriteLib
                << ", loading world " << defaultWorld_ << " instead." << std::endl;
            defaultWorld_ = GREENPRAIRIE_WORLD_NAME;
        }
        if ( !fileExists( defWorldIni ) )
        {
            debugLogFile << "Can't open default world file " << defWorldIni
                << ", loading world " << defaultWorld_ << " instead." << std::endl;
            defaultWorld_ = GREENPRAIRIE_WORLD_NAME;
        }
        */
        std::string defWorldIni( GAME_FOLDER );
        defWorldIni.append( worldsFolder_ );
        defWorldIni.append( "\\" );
        defWorldIni.append( defaultWorld_ );
        defWorldIni.append( ".ini" );
        if ( !fileExists( defWorldIni ) )
        {
            debugLogFile << "Can't open default world file " << defWorldIni
                << ", loading world " << defaultWorld_ << " instead." << std::endl;
            defaultWorld_ = GREENPRAIRIE_WORLD_NAME;
        } else {
            IniFile defaultWorld( defWorldIni );
            std::string defaultWorldBmp;
            error = defaultWorld.getKeyValue( "Main","SourceFile",defaultWorldBmp );
            std::string defaultWorldBmpFullPath( GAME_FOLDER );
            defaultWorldBmpFullPath.append( worldsFolder_ );
            defaultWorldBmpFullPath.append( "\\" );
            defaultWorldBmpFullPath.append( defaultWorldBmp );
            if ( (error != 0) || (!fileExists( defaultWorldBmpFullPath )) )
            {
                debugLogFile << "Can't open default world file " << defWorldSpriteLib
                    << ", loading world " << defaultWorld_ << " instead." << std::endl;
                defaultWorld_ = GREENPRAIRIE_WORLD_NAME;
            }
        }
    }
}

void Defaults::checkFolder( const std::string& folderType,std::string& folderName )
{
    masterIniFile_.getKeyValue( "Folders",folderType,folderName );
    if ( folderName.length() == 0 )
    {
        debugLogFile << "Illegal entry in " << MASTER_INIFILE_FILENAME 
            << " for variable " << folderType 
            << ": a valid folder name must be given." << std::endl;
        debugLogFile.close();
        exit( -1 );
        return;
    }
    if ( !dirExists( GAME_FOLDER + folderName ) )
    {
        debugLogFile << "The " << folderType << " folder " 
            << folderName << " is missing, creating." << std::endl;
        std::string directory( GAME_FOLDER );
        directory.append( folderName );
        if ( !(CreateDirectoryA( directory.c_str(),NULL ) ||
            (ERROR_ALREADY_EXISTS == GetLastError())) )
        {
            debugLogFile << "Unable to create " << folderType << " folder " 
                << folderName << ", exiting." << std::endl;
            debugLogFile.close();
            exit( -1 );
            return;
        }
    }
}

bool Defaults::dirExists( const std::string& dirName )
{
    DWORD ftyp = GetFileAttributesA( dirName.c_str() );
    if ( ftyp == INVALID_FILE_ATTRIBUTES )
        return false;  //something is wrong with your path!
    if ( ftyp & FILE_ATTRIBUTE_DIRECTORY )
        return true;   // this is a directory!
    return false;    // this is not a directory!
}

#include <experimental/filesystem>
bool Defaults::fileExists( const std::string& fileName )
{
    return std::experimental::filesystem::exists( fileName );
    /*
    In MS Visual Studio 2013 this function is available under std::tr2::sys::exists( "helloworld.txt" ); – Constantin Mar 30 '15 at 4:41
        2
        I actually hope it won't be std::exists, that would be quite confusing (think: exists in an STL container like a set). – einpoklum Feb 17 '16 at 15:00
        1
        Also in Visual Studio 2015 : #include <experimental/filesystem> bool file_exists( std::string fn ) { std::experimental::filesystem::exists( "helloworld.txt" ); } – Orwellophile Feb 16 '17 at 12:49

    //std::ifstream infile( fileName );
    //return infile.good();
    */
}
