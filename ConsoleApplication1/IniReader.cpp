/*
    Thoughts:

    - define resources in .ini files (separate ini file?)
    - define which unit can harvest which resource, by name
    - --> create dependencies
*/


#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> 
#include <algorithm>
#include <iostream> 
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
class Unit
{
public:
    Unit( IniFile& iniFile,const std::string& unitName );
    bool                isInitialized() { return initialized_; }
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
    const std::vector <std::string>& canProduce() const { return canProduce_; }
    const std::vector <std::string>& requires() const { return requires_; }
private:
    bool                initialized_ = false;
    int                 id_;                 // index in list of all units / buildings
    std::string         name_;
    bool                hasRadarCapability_; // whether it can detect hidden units
    bool                canMove_;            // whether it can move
    bool                canFLy_;             // whether the unit is an airplane
    bool                canJump_;            // whether it can jump onto higher ground or from a cliff
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
    std::vector <std::string> canProduce_;   // Can be used if a unit can evolve maybe ?
    std::vector <std::string> requires_;     // building( s ) required to produce the unit/building
private:
    int                 loadFromIniFile( IniFile& iniFile, const std::string& unitName );
};

Unit::Unit( IniFile& iniFile,const std::string& unitName )
{ 
    initialized_ = ( loadFromIniFile( iniFile,unitName ) == 0 );
}

int Unit::loadFromIniFile( IniFile& iniFile,const std::string& unitName )
{
    int error;
    if ( !iniFile.isLoaded() ) return -1;
    name_ = unitName;
    error = iniFile.getKeyValue( unitName,"hasRadarCapability",hasRadarCapability_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"canMove",canMove_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"canFLy",canFLy_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"canJump",canJump_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"moveSpeed",moveSpeed_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"maxHealth",maxHealth_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"maxShield",maxShield_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"maxEnergy",maxEnergy_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"firingRange",firingRange_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"antiAirFirePower",antiAirFirePower_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"antiGroundFirePower",antiGroundFirePower_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"firingRate",firingRate_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"neededCargoSpace",neededCargoSpace_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"costToProduce",costToProduce_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"cargoCapacity",cargoCapacity_ );
    if ( error != 0 ) return -1;
    /*
    error = iniFile.getKeyValue( unitName,"CanProduce",canProduce_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( unitName,"Requires",requires_ );
    if ( error != 0 ) return -1;
    */   
    std::string strList;
    error = iniFile.getKeyValue( unitName,"Requires",strList );
    if ( error != 0 ) return -1;
    const char *c = strList.c_str();
    char *buf = new char[strList.length() + 1];
    buf[0] = '\0';
    for ( int i = 0; *c != '\0'; c++ )
    {
        buf[i] = *c;
        i++;
        if ( *c == ',' )
        {
            buf[i - 1] = '\0';
            requires_.push_back( std::string( buf ) );
            i = 0;
        } else if ( c[1] == '\0' )
        {
            buf[i] = '\0';
            requires_.push_back( std::string( buf ) );
        }
    }
    delete buf;
    strList.clear();
    error = iniFile.getKeyValue( unitName,"CanProduce",strList );
    if ( error != 0 ) return -1;
    c = strList.c_str();
    buf = new char[strList.length() + 1];
    buf[0] = '\0';
    for ( int i = 0; *c != '\0'; c++ )
    {
        buf[i] = *c;
        i++;
        if ( *c == ',' )
        {
            buf[i - 1] = '\0';
            i = 0;
            canProduce_.push_back( std::string( buf ) );
        } else if ( c[1] == '\0' )
        {
            buf[i] = '\0';
            canProduce_.push_back( std::string( buf ) );
        }
    }
    delete buf;
    return 0;
}

/*
    A race is a collection of units and buildings
*/
class Race
{
public:
    Race( IniFile& iniFile );
    bool                            isInitialized() const { return initialized_; }
    int                             nrUnits() const { return nrUnits_; }
    int                             nrBuildings() const { return nrBuildings_; }
    const Unit&                     getBuilding( int buildingNr ) const
    {
        assert( buildingNr >= 0 );
        assert( buildingNr < nrBuildings_ );
        return raceElements_[buildingNr];
    }
    const Unit&                     getUnit( int unitNr ) const
    { 
        assert( unitNr >= 0 );
        assert( unitNr < nrUnits_ );
        return raceElements_[nrBuildings_ + unitNr];
    }
    const std::vector<int>&         getBuildingCanProduceList( int buildingNr ) const
    {
        assert( buildingNr >= 0 );
        assert( buildingNr < nrBuildings_ );
        return unitCanProduceList[buildingNr];

    }
    const std::vector<int>&         getBuildingRequiresList( int buildingNr ) const
    {
        assert( buildingNr >= 0 );
        assert( buildingNr < nrBuildings_ );
        return unitRequiresList[buildingNr];

    }
    const std::vector<int>&         getUnitCanProduceList( int unitNr ) const
    {
        assert( unitNr >= 0 );
        assert( unitNr < nrUnits_ );
        return unitCanProduceList[nrBuildings_ + unitNr];

    }
    const std::vector<int>&         getUnitRequiresList( int unitNr ) const
    {
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
    for ( int u = 0; u < nrRaceElements_; u++ )
        if ( raceElements_[u].getName().compare( unitName ) == 0 )
            return raceElements_[u].getId();
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
    iniFile.rewind();
    nrBuildings_ = 0;
    for ( ;; )
    {
        std::string section;
        std::string keyValue;
        int error = iniFile.getNextSection( section );
        if ( error != 0 ) break;
        error = iniFile.getKeyValue( section,"TYPE",keyValue );
        if ( error != 0 ) break;        
        if ( keyValue.compare( "BUILDING" ) == 0 ) 
        {             
            Unit unit( iniFile,section );
            if ( !unit.isInitialized() ) continue; // skip invalid unit
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
        if ( error != 0 ) break;
        if ( keyValue.compare( "UNIT" ) == 0 )
        {
            Unit unit( iniFile,section );
            if ( !unit.isInitialized() ) continue; // skip invalid unit
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
            if ( unitId == -1 ) continue; // required unit not found (fatal error!)
            canProduceList.push_back( unitId );
        }
        std::vector<int> requiresList;
        for ( int i = 0; i < (int)unit.requires().size(); i++ )
        {
            int unitId = getRaceElementIDbyName( unit.requires()[i] );
            if ( unitId == -1 ) continue; // required unit not found (fatal error!)
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
    IniFile iniFile( std::string( FOLDER_RACES FILE_RACE_HUMAN ) );
    Race race( iniFile );
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



