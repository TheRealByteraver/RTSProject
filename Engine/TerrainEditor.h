#pragma once
/*
Thoughts:

--> DoodAd layer must be used, after each change in the terrain we should
check if all doodads are still in a valid location. Basic terrain & doodads
should be kept separate in the code.
*/

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
#define T_LOW                           16      
#define T_HIGH                          32     
#define T_HIGH_WATER                    48
#define T_BASIC_TERRAIN_TYPE_MASK       (16 + 32)
#define T_BASIC_TERRAIN_LOWER_LIMIT     0 
#define T_BASIC_TERRAIN_UPPER_LIMIT     63
/*
Ramps that let the player go from low to high ground. There are four of
them: to the right, down, to the left and up.
LTR = Left-to-right
TTB = Top-to-bottom
RTL = Right-to-left
BTT = Bottom-to-top
nr 1 is always closest to HIGH terrain
nr 4 is always closest to LOW terrain
*/
#define T_RIGHT_RAMP_MASK               (T_RAMPS_LOWER_LIMIT + 0)
#define T_RIGHT_RAMP_CENTRAL_LTR1       68
#define T_RIGHT_RAMP_CENTRAL_LTR2       69
#define T_RIGHT_RAMP_CENTRAL_LTR3       70
#define T_RIGHT_RAMP_CENTRAL_LTR4       71
#define T_RIGHT_RAMP_TOPSIDE_LTR1       72 
#define T_RIGHT_RAMP_TOPSIDE_LTR2       73 
#define T_RIGHT_RAMP_TOPSIDE_LTR3       74 
#define T_RIGHT_RAMP_TOPSIDE_LTR4       75
#define T_RIGHT_RAMP_BTMSIDE_LTR1       76 
#define T_RIGHT_RAMP_BTMSIDE_LTR2       77 
#define T_RIGHT_RAMP_BTMSIDE_LTR3       78 
#define T_RIGHT_RAMP_BTMSIDE_LTR4       79 

#define T_DOWN_RAMP_MASK                (T_RAMPS_LOWER_LIMIT + 16)
#define T_DOWN_RAMP_CENTRAL_TTB1        84 
#define T_DOWN_RAMP_CENTRAL_TTB2        85 
#define T_DOWN_RAMP_CENTRAL_TTB3        86 
#define T_DOWN_RAMP_CENTRAL_TTB4        87 
#define T_DOWN_RAMP_LEFTSIDE_TTB1       88 
#define T_DOWN_RAMP_LEFTSIDE_TTB2       89 
#define T_DOWN_RAMP_LEFTSIDE_TTB3       90 
#define T_DOWN_RAMP_LEFTSIDE_TTB4       91
#define T_DOWN_RAMP_RIGHTSIDE_TTB1      92 
#define T_DOWN_RAMP_RIGHTSIDE_TTB2      93 
#define T_DOWN_RAMP_RIGHTSIDE_TTB3      94 
#define T_DOWN_RAMP_RIGHTSIDE_TTB4      95 

#define T_LEFT_RAMP_MASK                (T_RAMPS_LOWER_LIMIT + 32)
#define T_LEFT_RAMP_CENTRAL_RTL1        100
#define T_LEFT_RAMP_CENTRAL_RTL2        101
#define T_LEFT_RAMP_CENTRAL_RTL3        102
#define T_LEFT_RAMP_CENTRAL_RTL4        103
#define T_LEFT_RAMP_TOPSIDE_RTL1        104
#define T_LEFT_RAMP_TOPSIDE_RTL2        105
#define T_LEFT_RAMP_TOPSIDE_RTL3        106
#define T_LEFT_RAMP_TOPSIDE_RTL4        107
#define T_LEFT_RAMP_BTMSIDE_RTL1        108
#define T_LEFT_RAMP_BTMSIDE_RTL2        109
#define T_LEFT_RAMP_BTMSIDE_RTL3        110
#define T_LEFT_RAMP_BTMSIDE_RTL4        111

#define T_UP_RAMP_MASK                  (T_RAMPS_LOWER_LIMIT + 48)
#define T_UP_RAMP_CENTRAL_BTT1          116
#define T_UP_RAMP_CENTRAL_BTT2          117
#define T_UP_RAMP_CENTRAL_BTT3          118
#define T_UP_RAMP_CENTRAL_BTT4          119
#define T_UP_RAMP_LEFTSIDE_BTT1         120
#define T_UP_RAMP_LEFTSIDE_BTT2         121
#define T_UP_RAMP_LEFTSIDE_BTT3         122
#define T_UP_RAMP_LEFTSIDE_BTT4         123
#define T_UP_RAMP_RIGHTSIDE_BTT1        124
#define T_UP_RAMP_RIGHTSIDE_BTT2        125
#define T_UP_RAMP_RIGHTSIDE_BTT3        126
#define T_UP_RAMP_RIGHTSIDE_BTT4        127

#define T_RAMPS_TYPE_MASK               (T_RAMPS_LOWER_LIMIT + 16 + 32)
#define T_RAMPS_WALKABLE_MASK           
#define T_RAMPS_LOWER_LIMIT             64
#define T_RAMPS_UPPER_LIMIT             127
/*
Initialization constants: default terrain
*/
#define T_DEFAULT                       T_LOW


class Terrain
{
public:
    Terrain()
    {
        data_ = NULL;
        columns_ = 0;
        rows_ = 0;
        size_ = 0;
    }
    Terrain( int columns,int rows )
    {
        data_ = NULL;
        init( columns,rows );
    }
    Terrain( int columns,int rows,char terrain )
    {
        data_ = NULL;
        init( columns,rows,terrain );
    }
    ~Terrain()
    {
        if ( data_ != NULL ) delete data_;
    }
    void    init( int columns,int rows,char terrain = T_DEFAULT )
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
        if ( data_ != NULL ) delete data_;
        data_ = new char[size_];
        std::fill_n( data_,size_,terrain );
    }
    void    drawTerrain( int i,char terrain );
    void    drawTerrain( int i,int j,char terrain )
    {
        drawTerrain( j * columns_ + i,terrain );
    }
    //  Can only be used to assign basic terrain:
    void    assign( int i,char terrain )
    {
        assert( i < (size_ - columns_ - 1) );
        assert( i >= 0 );
        assert( data_ != NULL );
        assert( isBasicTerrain( terrain ) );
        assert( (i & 0x1) == 0 ); // i must be even and start on an even row
        assert( ((i / columns_) & 0x1) == 0 );
        char *t = data_ + i;
        *t = terrain;
        t++;
        *t = terrain;
        t += columns_;
        *t = terrain;
        t--;
        *t = terrain;
    }
    void    assign( int i,int j,char terrain )
    {
        assign( j * columns_ + i,terrain );
    }
    void    show( int curX,int curY );
private:
    int     columns_;
    int     rows_;
    int     size_;
    char    *data_;
    bool    isBasicTerrain( const char data )
    {
        return (data >= T_BASIC_TERRAIN_LOWER_LIMIT) &&
            (data <= T_BASIC_TERRAIN_UPPER_LIMIT);
    }
    bool    isLow( const char data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType( data ) == T_LOW);
    }
    bool    isHigh( const char data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType( data ) == T_HIGH);
    }
    bool    isLowWater( const char data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType( data ) == T_LOW_WATER);
    }
    bool    isHighWater( const char data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType( data ) == T_HIGH_WATER);
    }
    bool    isHighCompatible( const char data )
    {
        return
            isHigh( data ) ||
            isHighWater( data ) ||
            (data == T_RIGHT_RAMP_TOPSIDE_LTR1) ||
            (data == T_RIGHT_RAMP_CENTRAL_LTR1) ||
            (data == T_RIGHT_RAMP_BTMSIDE_LTR1) ||
            (data == T_DOWN_RAMP_LEFTSIDE_TTB1) ||
            (data == T_DOWN_RAMP_CENTRAL_TTB1) ||
            (data == T_DOWN_RAMP_RIGHTSIDE_TTB1) ||
            (data == T_LEFT_RAMP_TOPSIDE_RTL1) ||
            (data == T_LEFT_RAMP_CENTRAL_RTL1) ||
            (data == T_LEFT_RAMP_BTMSIDE_RTL1) ||
            (data == T_UP_RAMP_LEFTSIDE_BTT1) ||
            (data == T_UP_RAMP_CENTRAL_BTT1) ||
            (data == T_UP_RAMP_RIGHTSIDE_BTT1);
    }
    bool    isRamp( const char data )
    {
        return (data >= T_RAMPS_LOWER_LIMIT) && (data <= T_RAMPS_UPPER_LIMIT);
    }
    bool    isRightRamp( const char data )
    {
        return isRamp( data ) &&
            ((data & T_RAMPS_TYPE_MASK) == T_RIGHT_RAMP_MASK);
    }
    bool    isDownRamp( const char data )
    {
        return isRamp( data ) &&
            ((data & T_RAMPS_TYPE_MASK) == T_DOWN_RAMP_MASK);
    }
    bool    isLeftRamp( const char data )
    {
        return isRamp( data ) &&
            ((data & T_RAMPS_TYPE_MASK) == T_LEFT_RAMP_MASK);
    }
    bool    isUpRamp( const char data )
    {
        return isRamp( data ) &&
            ((data & T_RAMPS_TYPE_MASK) == T_UP_RAMP_MASK);
    }
    char    getMasterTerrainType( const char data )
    {
        if ( isRamp( data ) ) return data & T_RAMPS_TYPE_MASK;
        if ( isBasicTerrain( data ) ) return data & T_BASIC_TERRAIN_TYPE_MASK;
        return -1;
    }
    char    getMasterTerrainType( const int i )
    {
        assert( i >= 0 );
        assert( i < size_ );
        return getMasterTerrainType( data_[i] );
    }
    char    getMasterTerrainType( int i,int j )
    {
        return getMasterTerrainType( j * columns_ + i );
    }
    char    getLowerTerrain( char data )
    {
        switch ( getMasterTerrainType( data ) )
        {
            case T_HIGH_WATER: { return T_HIGH;      break; }
            case T_HIGH: { return T_LOW;       break; }
            case T_LOW: { return T_LOW_WATER; break; }
            case T_LOW_WATER: { return T_LOW_WATER; break; }
            case T_RIGHT_RAMP_MASK:
            case T_DOWN_RAMP_MASK:
            case T_LEFT_RAMP_MASK:
            case T_UP_RAMP_MASK: { return T_LOW_WATER; break; }
        }
        return -1;
    }
    char    getHigherTerrain( char data )
    {
        switch ( getMasterTerrainType( data ) )
        {
            case T_HIGH_WATER: { return T_HIGH_WATER; break; }
            case T_HIGH: { return T_HIGH_WATER; break; }
            case T_LOW: { return T_HIGH;       break; }
            case T_LOW_WATER: { return T_LOW;        break; }
            case T_RIGHT_RAMP_MASK:
            case T_DOWN_RAMP_MASK:
            case T_LEFT_RAMP_MASK:
            case T_UP_RAMP_MASK: { return T_HIGH;       break; }
        }
        return -1;
    }
    void    deleteRamp( int i,char rampType );
    void    deleteRamp( int i,int j,char rampType )
    {
        deleteRamp( i + j * columns_,rampType );
    }
    void    drawRamp( int i,char rampType );
    void    drawRamp( int i,int j,char rampType )
    {
        drawRamp( j * columns_ + i,rampType );
    }
    void    adapt( int i );
    void    fixTransitions( int x1,int y1,int x2,int y2 );
    bool    isValidLocationFor( int i,char terrain );
};
