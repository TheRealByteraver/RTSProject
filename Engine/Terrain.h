#pragma once

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

#include "graphics.h" // for Rect
#include "Sprite.h"
#include "IniFile.h"
#include "globals.h"

//#define DEBUG_MODE

/*
*******************************************************************************
*                                                                             *
* Start of Terrain class code                                                 *
*                                                                             *
*******************************************************************************
*/
#define COLUMNS     80 // must be even, only for testing purposes
#define ROWS        48 // must be even, only for testing purposes
/*
    We add this value to each terrain element so we don't use control 
    characters such as tab, bell, carriage return, and so on:
*/
/*
Transitions from low water to low ground
*/
#define T_RIGHTWARDS_TO_LOW_WATER       4 
#define T_RIGHTWARDS_FROM_LOW_WATER     5 
#define T_DOWNWARDS_TO_LOW_WATER        6  
#define T_DOWNWARDS_FROM_LOW_WATER      7 
/*
For the normal corners the outside of the corner is the foreign terrain, so
only the bottom right part of T_LOW_WATER_TOP_LEFT is "low water".
*/
#define T_LOW_WATER_TOP_LEFT            8
#define T_LOW_WATER_TOP_RIGHT           9
#define T_LOW_WATER_BOTTOM_LEFT         10
#define T_LOW_WATER_BOTTOM_RIGHT        11
/*
For the hollow corners the inside of the corner is the foreign terrain, so
only the bottom right part of T_H_LOW_WATER_TOP_LEFT is "low terrain".
*/
#define T_H_LOW_WATER_TOP_LEFT          12
#define T_H_LOW_WATER_TOP_RIGHT         13
#define T_H_LOW_WATER_BOTTOM_LEFT       14
#define T_H_LOW_WATER_BOTTOM_RIGHT      15
/*
Transitions from low ground to high ground
*/
#define T_RIGHTWARDS_TO_HIGH            36
#define T_RIGHTWARDS_FROM_HIGH          37
#define T_DOWNWARDS_TO_HIGH             38
#define T_DOWNWARDS_FROM_HIGH           39
/*
For the normal corners the outside of the corner is the foreign terrain, so
only the bottom right part of T_HIGH_TOP_LEFT is "high terrain".
*/
#define T_HIGH_TOP_LEFT                 40
#define T_HIGH_TOP_RIGHT                41
#define T_HIGH_BOTTOM_LEFT              42
#define T_HIGH_BOTTOM_RIGHT             43
/*
For the hollow corners the inside of the corner is the foreign terrain, so
only the bottom right part of T_H_HIGH_TOP_LEFT is "low terrain".
*/
#define T_H_HIGH_TOP_LEFT               44
#define T_H_HIGH_TOP_RIGHT              45
#define T_H_HIGH_BOTTOM_LEFT            46
#define T_H_HIGH_BOTTOM_RIGHT           47
/*
Transitions from high ground to high water
*/
#define T_RIGHTWARDS_TO_HIGH_WATER      52
#define T_RIGHTWARDS_FROM_HIGH_WATER    53
#define T_DOWNWARDS_TO_HIGH_WATER       54
#define T_DOWNWARDS_FROM_HIGH_WATER     55
/*
For the normal corners the outside of the corner is the foreign terrain, so
only the bottom right part of T_HIGH_WATER_TOP_LEFT is "high water".
*/
#define T_HIGH_WATER_TOP_LEFT           56
#define T_HIGH_WATER_TOP_RIGHT          57
#define T_HIGH_WATER_BOTTOM_LEFT        58
#define T_HIGH_WATER_BOTTOM_RIGHT       59
/*
For the hollow corners the inside of the corner is the foreign terrain, so
only the bottom right part of T_H_HIGH_WATER_TOP_LEFT is "high terrain".
*/
#define T_H_HIGH_WATER_TOP_LEFT         60
#define T_H_HIGH_WATER_TOP_RIGHT        61
#define T_H_HIGH_WATER_BOTTOM_LEFT      62
#define T_H_HIGH_WATER_BOTTOM_RIGHT     63
/*
Terrain types. If you mask one of the above with T_TYPE_MASK you will get
one of the below master terrain types.
*/
#define T_LOW_WATER                     0     
#define T_DOODAD                        1
#define T_LOW                           16      
#define T_HIGH                          32     
#define T_HIGH_WATER                    48
#define T_BASIC_TERRAIN_TYPE_MASK       (16 + 32)
#define T_BASIC_TERRAIN_LOWER_LIMIT     0 
#define T_BASIC_TERRAIN_UPPER_LIMIT     63

/*
Initialization constants: default terrain
*/
#define T_DEFAULT                       T_LOW
#define T_DEFAULT_CURSORWIDTH           2

typedef char Tile;

class DoodadLocation
{
public:
    int     x;
    int     y;
    int     doodadNr;
    bool    isUsed;
};

class Terrain
{
public:
    Terrain()
    {
        /*
        // done in variable definition
        data_ = nullptr;
        columns_ = 0;
        rows_ = 0;
        size_ = 0;
        */
    }
    Terrain( int columns,int rows )
    {
        data_ = nullptr;
        init( columns,rows );
    }
    Terrain( int columns,int rows,Tile terrain )
    {
        data_ = nullptr;
        init( columns,rows,terrain );
    }
    Terrain( const std::string& filename ) { loadTerrain( filename ); }
    ~Terrain()
    {
        if ( data_ != nullptr ) delete data_;
    }
    void    init( int columns,int rows,Tile terrain = T_DEFAULT )
    {
        // terrain must be wider than 1 & have even width & height
        assert( columns > 1 );
        assert( rows > 1 );
        assert( (columns & 0x1) == 0 );
        assert( (rows & 0x1) == 0 );
        assert( isBasicTerrain( terrain ) );
        columns_ = columns;
        rows_ = rows;
        size_ = columns_ * rows_;
        if ( data_ != nullptr ) delete data_;
        data_ = new Tile[size_];
        std::fill_n( data_,size_,terrain );
        doodadList_.clear();
    }
    int     loadTerrain( std::string filename );
    int     saveTerrain( std::string filename ) const;
    Rect    drawTerrain( int i,Tile terrain );
    Rect    drawTerrain( int i,int j,Tile terrain )
    {
        return drawTerrain( j * columns_ + i,terrain );
    }
    //  Can only be used to assign basic terrain:
    void    assign( int i,Tile terrain );
    void    assign( int i,int j,Tile terrain )
    {
        assign( j * columns_ + i,terrain );
    }
    const Tile getElement( const int i ) const 
    {
        assert( i >= 0 );
        assert( i < size_ );
        assert( data_ != nullptr );
        return data_[i];
    }
    const Tile getElement( const int x,const int y ) const 
    {
        assert( x >= 0 );
        assert( y >= 0 );
        assert( x < columns_ );
        assert( y < rows_ );
        assert( data_ != nullptr );
        return data_[y * columns_ + x];
    }
    const std::string& getName() const { return name_; }
    void    setName( const std::string& name )
    {
        assert( name.length() > 0 );
        name_ = name;
    }
    const std::string& getWorld() const { return world_; }
    const int getColumns() const { return columns_; }
    const int getRows()  const { return rows_; }
    // convert from ini file encoding to in-program number:
    Tile    decode( const char terrainElement )
    {
        int c;
        for ( c = 0; c < strlen( encodestr_ ); c++ )
        {
            if ( terrainElement == encodestr_[c] ) break;
        }        
        if ( terrainElement != encodestr_[c] ) return -1;
        return (Tile)c;
    }
    // convert from in-program number to ini-compatible character:
    char    encode( Tile terrainElement )
    {
        assert( terrainElement >= T_BASIC_TERRAIN_LOWER_LIMIT );
        assert( terrainElement <= T_BASIC_TERRAIN_UPPER_LIMIT );
        assert( encodestr_[terrainElement] != '!' );
        return encodestr_[terrainElement];
    }
    //const char *encodeStr()  const { return encodestr_; }
    const std::vector<DoodadLocation>& getDoodadList() const { return doodadList_; }
    void    removeDoodad( int doodAdNr )
    {
        assert( doodAdNr >= 0 );
        assert( doodAdNr < doodadList_.size() );
        doodadList_[doodAdNr].isUsed = false;
    }
    void    removeUnavailableDoodads( int highestDoodad )
    {
        // remove illegal doodad's:
        for ( int iDoodad = 0; iDoodad < doodadList_.size();iDoodad++ )
            if ( doodadList_[iDoodad].doodadNr >= highestDoodad )
                removeDoodad( iDoodad );
    }
    // for debugging in text mode only:
    void    show( int curX,int curY ) const;
private:
    static const char *encodestr_;
    std::string world_; // which environment the terrain was made with: desert, greenprairie, etc
    std::string name_;
    int     cursorWidth_ = T_DEFAULT_CURSORWIDTH;
    int     columns_ = 0;
    int     rows_ = 0;
    int     size_ = 0;
    Tile    *data_ = nullptr;
    std::vector<DoodadLocation> doodadList_;
private:
    bool    isBasicTerrain( const Tile data )
    {
        return (data >= T_BASIC_TERRAIN_LOWER_LIMIT) &&
            (data <= T_BASIC_TERRAIN_UPPER_LIMIT);
    }
    bool    isLow( const Tile data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType( data ) == T_LOW);
    }
    bool    isHigh( const Tile data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType( data ) == T_HIGH);
    }
    bool    isLowWater( const Tile data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType( data ) == T_LOW_WATER);
    }
    bool    isHighWater( const Tile data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType( data ) == T_HIGH_WATER);
    }
    bool    isHighCompatible( const Tile data )
    {
        return isHigh( data ) || isHighWater( data );
    }
    Tile    getMasterTerrainType( const Tile data )
    {
        if ( isBasicTerrain( data ) ) return data & T_BASIC_TERRAIN_TYPE_MASK;
        return -1;
    }
    Tile    getMasterTerrainType( const int i )
    {
        assert( i >= 0 );
        assert( i < size_ );
        return getMasterTerrainType( data_[i] );
    }
    Tile    getMasterTerrainType( int i,int j )
    {
        return getMasterTerrainType( j * columns_ + i );
    }
    Tile    getLowerTerrain( Tile data )
    {
        switch ( getMasterTerrainType( data ) )
        {
            case T_HIGH_WATER: { return T_HIGH;      break; }
            case T_HIGH: { return T_LOW;       break; }
            case T_LOW: { return T_LOW_WATER; break; }
            case T_LOW_WATER: { return T_LOW_WATER; break; }
        }
        return -1;
    }
    Tile    getHigherTerrain( Tile data )
    {
        switch ( getMasterTerrainType( data ) )
        {
            case T_HIGH_WATER: { return T_HIGH_WATER; break; }
            case T_HIGH: { return T_HIGH_WATER; break; }
            case T_LOW: { return T_HIGH;       break; }
            case T_LOW_WATER: { return T_LOW;        break; }
        }
        return -1;
    }
    Rect    adapt( int i );
    void    fixTransitions( int x1,int y1,int x2,int y2 );
};
