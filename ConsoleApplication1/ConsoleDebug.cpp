/*
    Thoughts:

    - define resources in .ini files (separate ini file?)
    - define which unit can harvest which resource, by name
    - --> create dependencies
*/


//#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> 
#include <algorithm>
#include <iostream>
#include <cctype>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>    // for getch();
#include <windows.h>
#include "TerrainEditor.h"
#include "IniFile.h"

#define FOLDER_RACES            ".\\RTSMedia\\Races\\"
#define FOLDER_TERRAINS         ".\\RTSMedia\\Terrains\\"
#define FILE_TERRAIN_WASTELAND  "testterrain.ini"
#define FILE_RACE_HUMAN         "humans.ini"


/******************************************************************************
*                                                                             *
* Start of the Race class code                                                *
*                                                                             *
******************************************************************************/

/*
    The Unit class describes the basic parameters of a Race element. It can be 
    a unit (soldier, tank, etc) or a building.
*/

// some possible error codes:
#define RACE_NO_ERROR                   0
#define RACE_INIFILE_NOT_LOADED         1
#define RACE_INVALID_DEFAULT_SECTION    2
#define RACE_BUILDING_NOT_FOUND         3
#define RACE_UNIT_NOT_FOUND             4
#define RACE_INVALID_RACE_ELEMENT       5
#define RACE_NR_OF_ERRORS               6 // latest error + 1 please :)

// and there corresponding description strings:
const std::string raceerrorstringlist[]
{ 
    "race parsed successfully",
    "error loading ini file",
    "error loading default race element [Default]",
    "building used in key parameter but not found as section",
    "error in race element description",
    "unit used in key parameter but not found as section"
};

class Unit
{
public:
    Unit( IniFile& iniFile,const std::string& unitName )
    {
        initialized_ = (loadFromIniFile( iniFile,unitName ) == RACE_NO_ERROR);
    }
    bool                isInitialized() { return initialized_; }
    bool                isAUnit() const { return isAUnit_; }
    bool                isABuilding() const { return isABuilding_; }
    void                setID( int id ) { assert( id >= 0 ); id_ = id; }
    int                 getId() const { return id_;  }
    const std::string&  getName() const { return name_; }
    bool                hasRadarCapability() const { return hasRadarCapability_; }
    bool                canMove() const { return canMove_; }
    bool                canFLy() const { return canFLy_; }
    bool                canJump() const { return canJump_; }
    int                 moveSpeed() const { return moveSpeed_; }
    int                 maxHealth() const { return maxHealth_; }
    int                 maxShield() const { return maxShield_; }
    int                 maxEnergy() const { return maxEnergy_; }
    int                 firingRange() const { return firingRange_; }
    int                 antiAirFirePower() const { return antiAirFirePower_; }
    int                 antiGroundFirePower() const { return antiGroundFirePower_; }
    int                 firingRate() const { return firingRate_; }
    int                 neededCargoSpace() const { return neededCargoSpace_; }
    int                 costToProduce() const { return costToProduce_; }
    int                 cargoCapacity() const { return cargoCapacity_; }
    const std::vector <KeyPair>& canHarvest() const { return canHarvest_; }
    const std::vector <std::string>& canProduce() const { return canProduce_; }
    const std::vector <std::string>& requires() const { return requires_; }
    bool                defaultsLoaded() const { return defaultsLoaded_; }
    static void         resetDefaultsLoadedtoFalse() { defaultsLoaded_ = false; }
    int                 getErrorStatus() const { return errorStatus_; }
    const std::string&  getErrorString( int errorNr ) const
    {
        assert( errorNr >= 0 );
        assert( errorNr < RACE_NR_OF_ERRORS );
        return( raceerrorstringlist[errorNr] );
    }
private:
// here we set the default values for each member variable. We also load these 
// values from the ini file. The [Default] section must be present in the 
// ini file that defines the race.    
    static bool         defaultsLoaded_;
    static std::string  nameDefault_;
    static bool         hasRadarCapabilityDefault_; 
    static bool         canMoveDefault_;            
    static bool         canFLyDefault_;             
    static bool         canJumpDefault_;
    static bool         isAllowedToDieDefault_;
    static int          moveSpeedDefault_;          
    static int          maxHealthDefault_;          
    static int          maxShieldDefault_;          
    static int          maxEnergyDefault_;          
    static int          firingRangeDefault_;        
    static int          antiAirFirePowerDefault_;   
    static int          antiGroundFirePowerDefault_;
    static int          firingRateDefault_;         
    static int          neededCargoSpaceDefault_;   
    static int          costToProduceDefault_;      
    static int          cargoCapacityDefault_;      
    static std::vector <std::string> canProduceDefault_;   
    static std::vector <std::string> requiresDefault_;     
private:
    int                 errorStatus_ = RACE_NO_ERROR;
    bool                isAUnit_ = false;
    bool                isABuilding_ = false;
    bool                initialized_ = false;
    int                 id_;                 // index in list of all units / buildings
    std::string         name_;
    bool                hasRadarCapability_; // whether it can detect hidden units
    bool                canMove_;            // whether it can move
    bool                canFLy_;             // whether the unit is an airplane
    bool                canJump_;            // whether it can jump onto higher ground or from a cliff
    bool                isAllowedToDie_;     // Hero units cannot die usually (for scenario consistency)
    int                 moveSpeed_;          // how fast it can move
    int                 maxHealth_;          // its maximum health,can be more than 100
    int                 maxShield_;          // its maximum shield level
    int                 maxEnergy_;          // its maximum energy level
    int                 firingRange_;        // how close units have to be before unit can shoot them
    int                 antiAirFirePower_;   // how effective unit is against enemy aircraft
    int                 antiGroundFirePower_;// how effective unit is against enemy ground units
    int                 firingRate_;         // how fast it shoots
    int                 neededCargoSpace_;   // how much it occupies if transported
    int                 costToProduce_;      // How much money it costs to produce this unit
    int                 cargoCapacity_;      // how much cargo it can carry
    std::vector <KeyPair> canHarvest_;       // list of resources it can harvest, and the amount of it it can carry
    std::vector <std::string> canProduce_;   // Can be used if a unit can evolve maybe ?
    std::vector <std::string> requires_;     // building( s ) required to produce the unit/building
private:
    int                 loadDefaults( IniFile& iniFile );
    int                 loadFromIniFile( IniFile& iniFile, const std::string& unitName );
};

bool        Unit::defaultsLoaded_;
std::string Unit::nameDefault_;
bool        Unit::hasRadarCapabilityDefault_;
bool        Unit::canMoveDefault_;
bool        Unit::canFLyDefault_;
bool        Unit::canJumpDefault_;
bool        Unit::isAllowedToDieDefault_;
int         Unit::moveSpeedDefault_;
int         Unit::maxHealthDefault_;
int         Unit::maxShieldDefault_;
int         Unit::maxEnergyDefault_;
int         Unit::firingRangeDefault_;
int         Unit::antiAirFirePowerDefault_;
int         Unit::antiGroundFirePowerDefault_;
int         Unit::firingRateDefault_;
int         Unit::neededCargoSpaceDefault_;
int         Unit::costToProduceDefault_;
int         Unit::cargoCapacityDefault_;
std::vector <KeyPair> canHarvestDefault_;
std::vector <std::string> Unit::canProduceDefault_;
std::vector <std::string> Unit::requiresDefault_;

int Unit::loadDefaults( IniFile& iniFile )
{
    if ( !iniFile.isLoaded() ) return RACE_INIFILE_NOT_LOADED;
    int error;
    std::string typeStr;
    std::string unitName( "DEFAULT" );
    nameDefault_ = "DEFAULT";
    error = iniFile.getKeyValue( unitName,"hasRadarCapability",hasRadarCapabilityDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"canMove",canMoveDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"canFLy",canFLyDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"canJump",canJumpDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"moveSpeed",moveSpeedDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"maxHealth",maxHealthDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"maxShield",maxShieldDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"maxEnergy",maxEnergyDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"firingRange",firingRangeDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"antiAirFirePower",antiAirFirePowerDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"antiGroundFirePower",antiGroundFirePowerDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"firingRate",firingRateDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"neededCargoSpace",neededCargoSpaceDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"costToProduce",costToProduceDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    error = iniFile.getKeyValue( unitName,"cargoCapacity",cargoCapacityDefault_ );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    std::string strList;
    error = iniFile.getKeyValue( unitName,"CanHarvest",strList );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    // no error checking for the next fn as it may be an empty list:
    iniFile.explodeStringToKeyPairList( strList,canHarvestDefault_ );
    error = iniFile.getKeyValue( unitName,"CanProduce",strList );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    iniFile.explodeStringToStringList( strList,canProduceDefault_ );
    error = iniFile.getKeyValue( unitName,"Requires",strList );
    if ( error != 0 ) return RACE_INVALID_DEFAULT_SECTION;
    iniFile.explodeStringToStringList( strList,requiresDefault_ );
    defaultsLoaded_ = true;
    return RACE_NO_ERROR;
}

int Unit::loadFromIniFile( IniFile& iniFile,const std::string& unitName )
{
    if ( !iniFile.isLoaded() )
    {
        errorStatus_ = RACE_INIFILE_NOT_LOADED;
        return RACE_INIFILE_NOT_LOADED;
    }
    if ( !defaultsLoaded_ )
    {
        errorStatus_ = loadDefaults( iniFile );        
        if ( errorStatus_ != RACE_NO_ERROR ) return errorStatus_;
    }
    errorStatus_ = RACE_NO_ERROR;
    int error;
    std::string typeStr;
    name_ = unitName;
    error = iniFile.getKeyValue( unitName,"TYPE",typeStr );
    if ( error != 0 ) return RACE_INVALID_RACE_ELEMENT;
    if ( typeStr.compare( "BUILDING" ) == 0 ) isABuilding_ = true;
    else if ( typeStr.compare( "UNIT" ) == 0 ) isAUnit_ = true;
    else {
        errorStatus_ = RACE_INVALID_RACE_ELEMENT;
        return errorStatus_;
    }
    error = iniFile.getKeyValue( unitName,"hasRadarCapability",hasRadarCapability_ );
    if ( error != 0 ) hasRadarCapability_ = hasRadarCapabilityDefault_;
    error = iniFile.getKeyValue( unitName,"canMove",canMove_ );
    if ( error != 0 ) canMove_ = canMoveDefault_;
    error = iniFile.getKeyValue( unitName,"canFLy",canFLy_ );
    if ( error != 0 ) canFLy_ = canFLyDefault_;
    error = iniFile.getKeyValue( unitName,"canJump",canJump_ );
    if ( error != 0 ) canJump_ = canJumpDefault_;
    error = iniFile.getKeyValue( unitName,"IsAllowedToDie",isAllowedToDie_ );
    if ( error != 0 ) isAllowedToDie_ = isAllowedToDieDefault_;
    error = iniFile.getKeyValue( unitName,"moveSpeed",moveSpeed_ );
    if ( error != 0 ) moveSpeed_ = moveSpeedDefault_;
    error = iniFile.getKeyValue( unitName,"maxHealth",maxHealth_ );
    if ( error != 0 ) maxHealth_ = maxHealthDefault_;
    error = iniFile.getKeyValue( unitName,"maxShield",maxShield_ );
    if ( error != 0 ) maxShield_ = maxShieldDefault_;
    error = iniFile.getKeyValue( unitName,"maxEnergy",maxEnergy_ );
    if ( error != 0 ) maxEnergy_ = maxEnergyDefault_;
    error = iniFile.getKeyValue( unitName,"firingRange",firingRange_ );
    if ( error != 0 ) firingRange_ = firingRangeDefault_;
    error = iniFile.getKeyValue( unitName,"antiAirFirePower",antiAirFirePower_ );
    if ( error != 0 ) antiAirFirePower_ = antiAirFirePowerDefault_;
    error = iniFile.getKeyValue( unitName,"antiGroundFirePower",antiGroundFirePower_ );
    if ( error != 0 ) antiGroundFirePower_ = antiGroundFirePowerDefault_;
    error = iniFile.getKeyValue( unitName,"firingRate",firingRate_ );
    if ( error != 0 ) firingRate_ = firingRateDefault_;
    error = iniFile.getKeyValue( unitName,"neededCargoSpace",neededCargoSpace_ );
    if ( error != 0 ) neededCargoSpace_ = neededCargoSpaceDefault_;
    error = iniFile.getKeyValue( unitName,"costToProduce",costToProduce_ );
    if ( error != 0 ) costToProduce_ = costToProduceDefault_;
    error = iniFile.getKeyValue( unitName,"cargoCapacity",cargoCapacity_ );
    if ( error != 0 ) cargoCapacity_ = cargoCapacityDefault_;
    std::string strList;
    iniFile.getKeyValue( unitName,"CanHarvest",strList );    
    iniFile.explodeStringToKeyPairList( strList,canHarvest_ );
    iniFile.getKeyValue( unitName,"Requires",strList );
    iniFile.explodeStringToStringList( strList,requires_ );
    iniFile.getKeyValue( unitName,"CanProduce",strList );
    iniFile.explodeStringToStringList( strList,canProduce_ );
    return errorStatus_;
}

/*
    A race is a collection of units and buildings
*/
class Race
{
public:
    Race( IniFile& iniFile );
    bool                            isInitialized() const { return initialized_; }
    int                             nrUnits() const 
    {
        assert( initialized_ );
        return nrUnits_; 
    }
    int                             nrBuildings() const 
    { 
        assert( initialized_ );
        return nrBuildings_; 
    }
    int                             nrRaceElements() const 
    { 
        assert( initialized_ );
        return nrRaceElements_; 
    }
    const Unit&                     getBuilding( int buildingNr ) const
    {
        assert( initialized_ );
        assert( buildingNr >= 0 );
        assert( buildingNr < nrBuildings_ );
        return raceElements_[buildingNr];
    }
    const Unit&                     getUnit( int unitNr ) const
    { 
        assert( initialized_ );
        assert( unitNr >= 0 );
        assert( unitNr < nrUnits_ );
        return raceElements_[nrBuildings_ + unitNr];
    }
    const std::vector<int>&         getBuildingCanProduceList( int buildingNr ) const
    {
        assert( initialized_ );
        assert( buildingNr >= 0 );
        assert( buildingNr < nrBuildings_ );
        return unitCanProduceList[buildingNr];

    }
    const std::vector<int>&         getBuildingRequiresList( int buildingNr ) const
    {
        assert( initialized_ );
        assert( buildingNr >= 0 );
        assert( buildingNr < nrBuildings_ );
        return unitRequiresList[buildingNr];

    }
    const std::vector<int>&         getUnitCanProduceList( int unitNr ) const
    {
        assert( initialized_ );
        assert( unitNr >= 0 );
        assert( unitNr < nrUnits_ );
        return unitCanProduceList[nrBuildings_ + unitNr];
    }
    const std::vector<int>&         getUnitRequiresList( int unitNr ) const
    {
        assert( initialized_ );
        assert( unitNr >= 0 );
        assert( unitNr < nrUnits_ );
        return unitRequiresList[nrBuildings_ + unitNr];
    }
private:
    bool                            initialized_ = false;
    int                             nrUnits_;
    int                             nrBuildings_;
    int                             nrRaceElements_; // nrUnits_ + nrBuildings_
    std::vector<Unit>               raceElements_;
    std::vector< std::vector<int> > unitCanProduceList;
    std::vector< std::vector<int> > unitRequiresList;
private:
    int                             getRaceElementIDbyName( const std::string unitName ) const; // -1 on error
};

int Race::getRaceElementIDbyName( const std::string unitName ) const
{
    assert( unitName.length() > 0 );
    assert( nrRaceElements_ > 0 );
    for ( int unitNr = 0; unitNr < nrRaceElements_; unitNr++ )
        if ( raceElements_[unitNr].getName().compare( unitName ) == 0 )
            return raceElements_[unitNr].getId();
    return -1;
}
/*
    What this function does:
    - load all buildings
    - load all units
    - set the id's of every unit
    - set the id's of every building
    - set the dependencies
*/
Race::Race( IniFile& iniFile )
{
    if ( !iniFile.isLoaded() ) return;
    // Needed for correct default initialization of the unit member variables. 
    // Default values may vary per race so we have to make sure the unit class
    // loads the defaults of this particular race / ini file.
    Unit::resetDefaultsLoadedtoFalse();
    iniFile.rewind();
    nrBuildings_ = 0;
    for ( ;; )
    {
        std::string section;
        std::string keyValue;
        int error = iniFile.getNextSection( section );
        if ( error != 0 ) break;                              // no sections left
        error = iniFile.getKeyValue( section,"TYPE",keyValue );        
        if ( error != RACE_NO_ERROR ) continue;               // skip invalid unit
        if ( keyValue.compare( "BUILDING" ) == 0 ) 
        {             
            Unit unit( iniFile,section ); 
            if ( unit.getErrorStatus() == RACE_INVALID_DEFAULT_SECTION ) return;  // fatal error, stop
            if ( !unit.isInitialized() ) continue;            // skip invalid unit
            unit.setID( nrBuildings_ );
            raceElements_.push_back( unit );
            nrBuildings_++;
        }
    }
    iniFile.rewind();
    nrUnits_ = 0;
    for ( ;; )
    {
        std::string section;
        std::string keyValue;
        int error = iniFile.getNextSection( section );
        if ( error != 0 ) break;
        error = iniFile.getKeyValue( section,"TYPE",keyValue );
        if ( error != 0 ) continue;
        if ( keyValue.compare( "UNIT" ) == 0 )
        {
            Unit unit( iniFile,section );
            if ( unit.getErrorStatus() == RACE_INVALID_DEFAULT_SECTION ) return;  // fatal error, stop
            if ( !unit.isInitialized() ) continue; 
            unit.setID( nrBuildings_ + nrUnits_ );
            raceElements_.push_back( unit );
            nrUnits_++;
        }
    }
    nrRaceElements_ = nrBuildings_ + nrUnits_;
    for ( int elemNr = 0; elemNr < nrRaceElements_; elemNr++ )
    {
        std::vector<int> canProduceList;
        Unit& unit = raceElements_[elemNr];
        for ( int i = 0; i < (int)unit.canProduce().size(); i++ )
        {
            int unitId = getRaceElementIDbyName( unit.canProduce()[i] );
            if ( unitId == -1 ) return; // required RaceElement not found (fatal error!)
            canProduceList.push_back( unitId );
        }
        std::vector<int> requiresList;
        for ( int i = 0; i < (int)unit.requires().size(); i++ )
        {
            int unitId = getRaceElementIDbyName( unit.requires()[i] );
            if ( unitId == -1 ) return; // required RaceElement not found (fatal error!)
            requiresList.push_back( unitId );
        }
        unitCanProduceList.push_back( canProduceList );
        unitRequiresList.push_back( requiresList );
    }
    initialized_ = true;
}

/*
    A faction is an instance of a race in a specific scenario. The scenario can
    apply limits to the functionality of the race, for instance certain units 
    or buildings might not be available.
*/
class Faction
{
};

/*
    A scenario contains:
    - a terrain
    - a collection of factions with their buildings and units
    - available buildings and units for each faction
    - alliances between factions
    - goals for the player(s)
*/
class Scenario
{
};

int main()
{   
    /*
    std::string testStr("gaz:50,crystals:20,wood:70");
    std::vector<KeyPair> CanHarvestList;
    int strIndex = 0;
    const char *str = testStr.c_str();
    for ( ;; )
    {
        KeyPair keyPair;
        str += keyPair.decode( std::string( str ) );
        if ( ! keyPair.isDecoded() ) break;
        std::cout << "resource: " << keyPair.getName() << std::endl;
        std::cout << "value: " << keyPair.getValue() << std::endl;
        if ( std::string( str ).length() > 2 ) str += 2;
        else break;
    }
    
    
    int val;
    std::string resourceStr;
    KeyPair testRes( testStr );
    std::cout << "resource: " << testRes.getName() << std::endl;
    std::cout << "value: " << testRes.getValue() << std::endl;

    int nextIndex = testRes.charsDecoded() + 2;
    nextIndex += testRes.decode( &(testStr[nextIndex]) );
    std::cout << "resource: " << testRes.getName() << std::endl;
    std::cout << "value: " << testRes.getValue() << std::endl;

    nextIndex += testRes.decode( &(testStr[nextIndex + 2]) );
    std::cout << "resource: " << testRes.getName() << std::endl;
    std::cout << "value: " << testRes.getValue() << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    */

    IniFile iniFile( std::string( FOLDER_RACES FILE_RACE_HUMAN ) );
    Race race( iniFile );
    if ( !race.isInitialized() )
    {
        std::cout << "encountered error during loading of race! " << std::endl;
        _getch();
        return 0;
    }
    std::cout << "Nr of Buildings: " << race.nrBuildings() << std::endl;
    std::cout << "Nr of Units: " << race.nrUnits() << std::endl;
    for ( int buildingNr = 0; buildingNr < race.nrBuildings(); buildingNr++ )
    {
        std::cout << "building nr " << buildingNr << " id = " << race.getBuilding( buildingNr ).getId();
        std::cout << ", " << race.getBuilding( buildingNr ).getName() << std::endl;
    }
    for ( int unitNr = 0; unitNr < race.nrUnits(); unitNr++ )
    {
        std::cout << "Unit nr " << unitNr << " id = " << race.getUnit( unitNr ).getId();
        std::cout << ", " << race.getUnit( unitNr ).getName() << std::endl;
    }
    std::cout << std::endl;
    // show requirements:
    for ( int unitNr = 0; unitNr < race.nrUnits(); unitNr++ )
    {
        std::cout << "Unit nr " << unitNr << " requires: ";
        for ( int r = 0; r < (int)race.getUnitRequiresList( unitNr ).size(); r++ )
            std::cout << race.getUnitRequiresList( unitNr )[r] << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for ( int buildingNr = 0; buildingNr < race.nrBuildings(); buildingNr++ )
    {
        std::cout << "Building nr " << buildingNr << " requires: ";
        for ( int r = 0; r < (int)race.getBuildingRequiresList( buildingNr ).size(); r++ )
            std::cout << race.getBuildingRequiresList( buildingNr )[r] << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for ( int unitNr = 0; unitNr < race.nrUnits(); unitNr++ )
    {
        std::cout << "Unit nr " << unitNr << " can produce: ";
        for ( int r = 0; r < (int)race.getUnitCanProduceList( unitNr ).size(); r++ )
            std::cout << race.getUnitCanProduceList( unitNr )[r] << ", ";
        std::cout << std::endl << " and can harvest: ";
        for ( int resource = 0; resource < (int)race.getUnit( unitNr ).canHarvest().size(); resource++ )
        {
            std::cout << race.getUnit( unitNr ).canHarvest()[resource].getName() << ":";
            std::cout << race.getUnit( unitNr ).canHarvest()[resource].getValue() << ",";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for ( int buildingNr = 0; buildingNr < race.nrBuildings(); buildingNr++ )
    {
        std::cout << "Building nr " << buildingNr << " can produce: ";
        for ( int r = 0; r < (int)race.getBuildingCanProduceList( buildingNr ).size(); r++ )
            std::cout << race.getBuildingCanProduceList( buildingNr )[r] << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    _getch();
    int curX = (COLUMNS / 2) & 0xFFFE;
    int curY = (ROWS / 2) & 0xFFFE;
    Terrain terrain( COLUMNS,ROWS );
    /*
    srand( time( NULL ) );
    for ( int i = 0; i < 800; i++ )
    {
        int x = (rand() % COLUMNS) & 0xFFFFE;
        int y = (rand() % ROWS) & 0xFFFFE;
        switch ( rand() % 4 )
        {
            case 0:
            {
                terrain.assign( x,y,T_LOW );
                break;
            }
            case 1:
            {
                terrain.assign( x,y,T_HIGH );
                break;
            }
            case 2:
            {
                terrain.assign( x,y,T_LOW_WATER );
                break;
            }
            case 3:
            {
                terrain.assign( x,y,T_HIGH_WATER );
                break;
            }
        }
    }
    */
    terrain.show( curX,curY );
    for ( ;; )
    {
        char wait = _getch();
        if ( wait == 'x' ) break;
        else if ( (wait == 'o' || wait == '8') && curY > 1             ) curY -= 2;
        else if ( (wait == 'l' || wait == '5') && curY < (ROWS - 2)    ) curY += 2;
        else if ( (wait == 'k' || wait == '4') && curX > 0             ) curX -= 2;
        else if ( (wait == 'm' || wait == '6') && curX < (COLUMNS - 2) ) curX += 2;
        else if ( wait == 'a' ) terrain.drawTerrain( curX,curY,T_LOW_WATER );
        else if ( wait == 'z' ) terrain.drawTerrain( curX,curY,T_LOW );
        else if ( wait == 'e' ) terrain.drawTerrain( curX,curY,T_HIGH );
        else if ( wait == 'r' ) terrain.drawTerrain( curX,curY,T_HIGH_WATER );
        else if ( wait == 'p' ) terrain.saveTerrain( std::string( FOLDER_TERRAINS FILE_TERRAIN_WASTELAND ) );
        else if ( wait == 'i' ) terrain.loadTerrain( std::string( FOLDER_TERRAINS FILE_TERRAIN_WASTELAND ) );
        /*
        else if ( wait == 'q' ) terrain.drawTerrain( curX,curY,T_RIGHT_RAMP_MASK );
        else if ( wait == 's' ) terrain.drawTerrain( curX,curY,T_DOWN_RAMP_MASK );
        else if ( wait == 'd' ) terrain.drawTerrain( curX,curY,T_LEFT_RAMP_MASK );
        else if ( wait == 'f' ) terrain.drawTerrain( curX,curY,T_UP_RAMP_MASK );
        */
        //else if ( wait == 'c' ) terrain.deleteRamp( curX,curY,terrain.getMasterTerrainType( curX,curY ) );
        else continue;
        terrain.show( curX,curY );
    }
    return 0;
}



