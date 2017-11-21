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

/******************************************************************************
*                                                                             *
* Start of INI Reader Code                                                    *
*                                                                             *
******************************************************************************/
#define INIFILE_FOLDER      ".\\RTSMedia\\"
#define INIFILE_FILENAME    "RTSProject.ini"
/*
    MAX_LINE_LENGTH has an arbitrary high value, because the code will bug if 
    the length of the line found in the ini file exceeds this number
*/
#define INIFILE_MAX_LINE_LENGTH     255
#define INIFILE_COMMENT_CHAR        ';'

class IniFile
{
public:
    //IniFile() { stringList.clear(); }
    IniFile( const std::string& filename ) { readFile( filename ); }
    bool    isLoaded() const { return iniFileLoaded_; }
    // These functions return non zero on error
    int     getKey( const std::string& section,const std::string& key,std::string& dest ) const;
    int     getKey( const std::string& section,const std::string& key,int& value ) const;
    int     getKey( const std::string& section,const std::string& key,bool& value ) const;
    int     getNextSection( std::string& section );
private:
    std::vector<std::string>  stringList;
private:
    bool    iniFileLoaded_ = false;
    int     currentRow_ = 0;
    int     readFile( const std::string& filename );
    char    *deleteWhiteSpace( char *buf ) const;
};

int IniFile::readFile( const std::string& filename )
{
    // start with a clean empty string list
    stringList.clear();
    iniFileLoaded_ = false;
    // Open the ini file
    std::ifstream iniFile( filename.c_str() );
    if ( !iniFile.is_open() ) return -1;
    // read the whole file into memory
    std::stringstream rawData;
    rawData << iniFile.rdbuf();
    iniFile.close();
    // read the list line by line and remove comments, whitespace and illegal
    // commands
    char strBuf[INIFILE_MAX_LINE_LENGTH];
    bool sectionFound = false;
    for ( ; !rawData.eof(); )
    {
        // this function only works if the line it reads is shorter than 
        // MAX_LINE_LENGTH. If not, the whole function is screwed.
        rawData.getline( strBuf,INIFILE_MAX_LINE_LENGTH );
        deleteWhiteSpace( strBuf );
        // strip any possible comments from the line and check if an equal sign
        // is present
        bool equalSignFound = false;
        bool bracketOpenFound = false;
        bool bracketClosedFound = false;
        for ( char *c = strBuf; *c != '\0'; c++ )
        {
            if ( *c == '[' ) bracketOpenFound = true;
            if ( *c == ']' ) bracketClosedFound = true;
            if ( *c == '=' ) equalSignFound = true;
            if ( *c == INIFILE_COMMENT_CHAR )
            {
                *c = '\0';
                break;
            }
        }
        if ( strlen( strBuf ) == 0 ) continue;
        if ( equalSignFound )
        {
            // ini file should start with a [section]
            if ( bracketOpenFound || bracketClosedFound ||
                (!sectionFound) ) continue;
        } else {
            if ( !(bracketOpenFound && bracketClosedFound) ) continue;
            else {
                if ( (strBuf[0] != '[') || (strBuf[strlen( strBuf ) - 1] != ']') )
                    continue;
                sectionFound = true;
            }
        }
        // put alles in upper case for easy comparing later on
        std::string str( strBuf );
        std::transform( str.begin(),str.end(),str.begin(),::toupper );
        // push the data into the buffer
        stringList.push_back( str );
        /*
        // debug:
        //std::cout << *(stringList.end() - 1);
        std::cout << stringList[stringList.size() - 1].c_str();
        std::cout << std::endl;
        */
    }
    iniFileLoaded_ = true;
    return 0;
}

int IniFile::getNextSection( std::string& section )
{
    for ( ; currentRow_ < (int)stringList.size(); currentRow_++ )
    {
        if ( stringList[currentRow_][0] == '[' ) break;
    }
    if ( currentRow_ < (int)stringList.size() )
    {
        section.clear();
        section = stringList[currentRow_];
        currentRow_++;
        return 0;
    } 
    else return -1;
}
// to be tested
int IniFile::getKey( const std::string& section,const std::string& key,std::string& dest ) const
{
    // Transform the strings to upper case for correct comparison
    std::string sectionStr;
    sectionStr.assign( "[" );
    sectionStr.append( section );
    sectionStr.append( "]" );
    std::transform( sectionStr.begin(),sectionStr.end(),sectionStr.begin(),::toupper );
    std::string keyStr( key );
    std::transform( keyStr.begin(),keyStr.end(),keyStr.begin(),::toupper );
    for ( int line = 0; line < (int)stringList.size(); line++ )
    {
        if ( stringList[line].compare( sectionStr ) != 0 ) continue;
        line++;
        for ( ; line < (int)stringList.size(); line++ )
        {
            if ( stringList[line][0] == '[' ) break; // end of this section
            char strBuf[INIFILE_MAX_LINE_LENGTH];
            const char *src = stringList[line].c_str();
            char *dst = strBuf;
            for ( ; (*src != '=') && (*src != '\0'); ) *dst++ = *src++;
            *dst = '\0';
            if ( strcmp( keyStr.c_str(),strBuf ) == 0 ) // found the key!
            {
                if ( *src == '=' ) dest.assign( src + 1 );
                else dest.clear();
                return 0;
            }
        }
        break;
    }
    return -1;
}

int IniFile::getKey( const std::string& section,const std::string& key,int& value ) const
{
    std::string dest;
    int error = getKey( section,key,dest );
    if ( error != 0 ) return -1;
    value = 0;
    try {
        value = std::stoi( dest );
    }
    /*
    catch ( std::invalid_argument& e ) {
        // if no conversion could be performed
        return -1;
    }
    catch ( std::out_of_range& e ) {
        // if the converted value would fall out of the range of the result type 
        // or if the underlying function (std::strtol or std::strtoull) sets errno 
        // to ERANGE.
        return -1;
    }
    */
    catch ( ... ) {
        // everything else
        return -1;
    }
    return 0;
}

int IniFile::getKey( const std::string& section,const std::string& key,bool& value ) const
{
    value = false;
    std::string dest;
    int error = getKey( section,key,dest );
    if ( error != 0 ) return -1;
    if ( (dest.compare( "TRUE" ) == 0) ||
        (dest.compare( "YES" ) == 0) ||
        (dest.compare( "1" ) == 0) ) value = true;
    else if ( !((dest.compare( "FALSE" ) == 0) ||
        (dest.compare( "NO" ) == 0) ||
        (dest.compare( "0" ) == 0)) ) return -1;
    return 0;
}

char *IniFile::deleteWhiteSpace( char *buf ) const
{
    char *d = buf;
    char *s = buf;
    for ( ; *s != NULL; )
        if ( (*s != ' ') && (*s != '\t') ) *d++ = *s++;
        else s++;
        *d = '\0';
        return buf;
}

/******************************************************************************
*                                                                             *
* Start of Terrain class code                                                 *
*                                                                             *
******************************************************************************/
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
                { drawTerrain( j * columns_ + i,terrain ); }
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
                { assign( j * columns_ + i,terrain ); }
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
        return isBasicTerrain( data) &&
            (getMasterTerrainType( data ) == T_LOW);
    }
    bool    isHigh( const char data )
    {
        return isBasicTerrain( data ) &&
            (getMasterTerrainType(data) == T_HIGH);
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
        return (data >= T_RAMPS_LOWER_LIMIT) && ( data <= T_RAMPS_UPPER_LIMIT);
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
            case T_HIGH_WATER       : { return T_HIGH;      break; }
            case T_HIGH             : { return T_LOW;       break; }
            case T_LOW              : { return T_LOW_WATER; break; }
            case T_LOW_WATER        : { return T_LOW_WATER; break; }
            case T_RIGHT_RAMP_MASK  : 
            case T_DOWN_RAMP_MASK   :
            case T_LEFT_RAMP_MASK   :
            case T_UP_RAMP_MASK     : { return T_LOW_WATER; break; }
        }
        return -1;
    }
    char    getHigherTerrain( char data )
    {
        switch ( getMasterTerrainType( data ) )
        {
            case T_HIGH_WATER       : { return T_HIGH_WATER; break; }
            case T_HIGH             : { return T_HIGH_WATER; break; }
            case T_LOW              : { return T_HIGH;       break; }
            case T_LOW_WATER        : { return T_LOW;        break; }
            case T_RIGHT_RAMP_MASK  :
            case T_DOWN_RAMP_MASK   :
            case T_LEFT_RAMP_MASK   :
            case T_UP_RAMP_MASK     : { return T_HIGH;       break; }
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

bool Terrain::isValidLocationFor( int i,char terrain )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    assert( isBasicTerrain( terrain ) || isRamp( terrain ) );
    int y = i / columns_;
    int x = i % columns_;
    switch ( terrain )
    {
        /*
            Ramps are 4x4 big squares (big squares are 2x2 small ones)
            The origin is always the top left, no matter 
            how the ramp is oriented. Conditions:
            - high ground should be two 2x2 squares wide were the ramp 
            is attached
            - there should be one strip of low ground next
            to the ramp (at the bottom)
            - the ramp should not be attached to a corner of high ground
            - high ground may not be adjacent to the sides of the ramp
        */
        case T_RIGHT_RAMP_MASK:
        {
            if ( x < 4 ) return false;
            if ( x > columns_ - 6 ) return false;
            if ( y > rows_ - 4 ) return false;
            if (
                (getMasterTerrainType( i - 2 - columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - 2 + columns_ * 4 ) != T_HIGH) ||
                (getMasterTerrainType( i - 2                ) != T_HIGH) ||
                (getMasterTerrainType( i - 2 + columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - 4                ) != T_HIGH) ||
                (getMasterTerrainType( i - 4 + columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 2     ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 4     ) != T_LOW) ||
                (getMasterTerrainType( i                    ) != T_LOW) ||
                (getMasterTerrainType( i     + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 2                ) != T_LOW) ||
                (getMasterTerrainType( i + 2 + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 4                ) != T_LOW) ||
                (getMasterTerrainType( i + 4 + columns_ * 2 ) != T_LOW) 
                ) return false;
            else return true;
            break;
        }
        case T_DOWN_RAMP_MASK:
        {
            if ( x > columns_ - 4 ) return false;
            if ( y < 4 ) return false;
            if ( y > rows_ - 6 ) return false;
            if (
                (getMasterTerrainType( i - columns_ * 2 - 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 2 + 4 ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 2     ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 2 + 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 4     ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 4 + 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - 2                ) != T_LOW) ||
                (getMasterTerrainType( i + 4                ) != T_LOW) ||
                (getMasterTerrainType( i                    ) != T_LOW) ||
                (getMasterTerrainType( i                + 2 ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 2     ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 2 + 2 ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 4     ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 4 + 2 ) != T_LOW) 
                ) return false;
            else return true;
            break;
        }
        case T_LEFT_RAMP_MASK:
        {
            if ( x > columns_ - 8 ) return false;
            if ( x < 2 ) return false;
            if ( y > rows_ - 4 ) return false;
            if (
                (getMasterTerrainType( i + 4 - columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i + 4 + columns_ * 4 ) != T_HIGH) ||
                (getMasterTerrainType( i + 4                ) != T_HIGH) ||
                (getMasterTerrainType( i + 4 + columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i + 6                ) != T_HIGH) ||
                (getMasterTerrainType( i + 6 + columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i + 2 - columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 2 + columns_ * 4 ) != T_LOW) ||
                (getMasterTerrainType( i                    ) != T_LOW) ||
                (getMasterTerrainType( i     + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 2                ) != T_LOW) ||
                (getMasterTerrainType( i + 2 + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i - 2                ) != T_LOW) ||
                (getMasterTerrainType( i - 2 + columns_ * 2 ) != T_LOW)
                ) return false;
            else return true;
            break;
        }
        case T_UP_RAMP_MASK:
        {
            {
                if ( x > columns_ - 4 ) return false;
                if ( y < 2 ) return false;
                if ( y > rows_ - 8 ) return false;
                if (
                    (getMasterTerrainType( i + columns_ * 4 - 2 ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 4 + 4 ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 4     ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 4 + 2 ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 6     ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 6 + 2 ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 2 - 2 ) != T_LOW) ||
                    (getMasterTerrainType( i + columns_ * 2 + 4 ) != T_LOW) ||
                    (getMasterTerrainType( i                    ) != T_LOW) ||
                    (getMasterTerrainType( i                + 2 ) != T_LOW) ||
                    (getMasterTerrainType( i + columns_ * 2     ) != T_LOW) ||
                    (getMasterTerrainType( i + columns_ * 2 + 2 ) != T_LOW) ||
                    (getMasterTerrainType( i - columns_ * 2     ) != T_LOW) ||
                    (getMasterTerrainType( i - columns_ * 2 + 2 ) != T_LOW)
                    ) return false;
                else return true;
                break;
            }
            break;
        }
        /*
            Other master terrain types have no restrictions
        */
        default:
        {
            return true;
            break;
        }
    }    
}

void Terrain::drawTerrain( int i,char terrain )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    assert( isBasicTerrain( terrain ) || isRamp( terrain ) );
    if ( !isValidLocationFor( i,terrain ) ) return;
    int y = i / columns_;
    int x = i % columns_;
    if ( isRamp( terrain ) )
    {
        drawRamp( i,terrain );
        int x1 = x - 2;
        int y1 = y - 2;
        int x2 = x + 4;
        int y2 = y + 4;
        fixTransitions( x1,y1,x2,y2 );
    } else {
        if ( y > 0 )
        {
            if ( x > 0 ) assign( i - columns_ * 2 - 2,terrain );
            assign( i - columns_ * 2,terrain );
            if ( x < columns_ - 2 ) assign( i - columns_ * 2 + 2,terrain );
        }
        if ( x > 0 ) assign( i - 2,terrain );
        assign( i,terrain );
        if ( x < columns_ - 2 ) assign( i + 2,terrain );
        if ( y < rows_ - 2 )
        {
            if ( x > 0 ) assign( i + columns_ * 2 - 2,terrain );
            assign( i + columns_ * 2,terrain );
            if ( x < columns_ - 2 ) assign( i + columns_ * 2 + 2,terrain );
        }
        adapt( i );
    }
}

void Terrain::drawRamp( int i,char rampType )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    assert( isRamp( rampType ) );
    switch ( getMasterTerrainType( rampType ) )
    {
        case T_RIGHT_RAMP_MASK:
        {
            data_[i                   ] = T_RIGHT_RAMP_TOPSIDE_LTR1;
            data_[i                + 1] = T_RIGHT_RAMP_TOPSIDE_LTR2;
            data_[i                + 2] = T_RIGHT_RAMP_TOPSIDE_LTR3;
            data_[i                + 3] = T_RIGHT_RAMP_TOPSIDE_LTR4;
            data_[i + columns_        ] = T_RIGHT_RAMP_CENTRAL_LTR1;
            data_[i + columns_     + 1] = T_RIGHT_RAMP_CENTRAL_LTR2;
            data_[i + columns_     + 2] = T_RIGHT_RAMP_CENTRAL_LTR3;
            data_[i + columns_     + 3] = T_RIGHT_RAMP_CENTRAL_LTR4;
            data_[i + columns_ * 2    ] = T_RIGHT_RAMP_CENTRAL_LTR1;
            data_[i + columns_ * 2 + 1] = T_RIGHT_RAMP_CENTRAL_LTR2;
            data_[i + columns_ * 2 + 2] = T_RIGHT_RAMP_CENTRAL_LTR3;
            data_[i + columns_ * 2 + 3] = T_RIGHT_RAMP_CENTRAL_LTR4;
            data_[i + columns_ * 3    ] = T_RIGHT_RAMP_BTMSIDE_LTR1;
            data_[i + columns_ * 3 + 1] = T_RIGHT_RAMP_BTMSIDE_LTR2;
            data_[i + columns_ * 3 + 2] = T_RIGHT_RAMP_BTMSIDE_LTR3;
            data_[i + columns_ * 3 + 3] = T_RIGHT_RAMP_BTMSIDE_LTR4;
            break;
        }
        case T_DOWN_RAMP_MASK:
        {
            data_[i                   ] = T_DOWN_RAMP_LEFTSIDE_TTB1;
            data_[i + columns_        ] = T_DOWN_RAMP_LEFTSIDE_TTB2;
            data_[i + columns_ * 2    ] = T_DOWN_RAMP_LEFTSIDE_TTB3;
            data_[i + columns_ * 3    ] = T_DOWN_RAMP_LEFTSIDE_TTB4;
            data_[i                + 1] = T_DOWN_RAMP_CENTRAL_TTB1;
            data_[i + columns_     + 1] = T_DOWN_RAMP_CENTRAL_TTB2;
            data_[i + columns_ * 2 + 1] = T_DOWN_RAMP_CENTRAL_TTB3;
            data_[i + columns_ * 3 + 1] = T_DOWN_RAMP_CENTRAL_TTB4;
            data_[i                + 2] = T_DOWN_RAMP_CENTRAL_TTB1;
            data_[i + columns_     + 2] = T_DOWN_RAMP_CENTRAL_TTB2;
            data_[i + columns_ * 2 + 2] = T_DOWN_RAMP_CENTRAL_TTB3;
            data_[i + columns_ * 3 + 2] = T_DOWN_RAMP_CENTRAL_TTB4;
            data_[i                + 3] = T_DOWN_RAMP_RIGHTSIDE_TTB1;
            data_[i + columns_     + 3] = T_DOWN_RAMP_RIGHTSIDE_TTB2;
            data_[i + columns_ * 2 + 3] = T_DOWN_RAMP_RIGHTSIDE_TTB3;
            data_[i + columns_ * 3 + 3] = T_DOWN_RAMP_RIGHTSIDE_TTB4;
            break;
        }
        case T_LEFT_RAMP_MASK:
        {
            data_[i                + 3] = T_LEFT_RAMP_TOPSIDE_RTL1;
            data_[i                + 2] = T_LEFT_RAMP_TOPSIDE_RTL2;
            data_[i                + 1] = T_LEFT_RAMP_TOPSIDE_RTL3;
            data_[i                   ] = T_LEFT_RAMP_TOPSIDE_RTL4;
            data_[i + columns_     + 3] = T_LEFT_RAMP_CENTRAL_RTL1;
            data_[i + columns_     + 2] = T_LEFT_RAMP_CENTRAL_RTL2;
            data_[i + columns_     + 1] = T_LEFT_RAMP_CENTRAL_RTL3;
            data_[i + columns_        ] = T_LEFT_RAMP_CENTRAL_RTL4;
            data_[i + columns_ * 2 + 3] = T_LEFT_RAMP_CENTRAL_RTL1;
            data_[i + columns_ * 2 + 2] = T_LEFT_RAMP_CENTRAL_RTL2;
            data_[i + columns_ * 2 + 1] = T_LEFT_RAMP_CENTRAL_RTL3;
            data_[i + columns_ * 2    ] = T_LEFT_RAMP_CENTRAL_RTL4;
            data_[i + columns_ * 3 + 3] = T_LEFT_RAMP_BTMSIDE_RTL1;
            data_[i + columns_ * 3 + 2] = T_LEFT_RAMP_BTMSIDE_RTL2;
            data_[i + columns_ * 3 + 1] = T_LEFT_RAMP_BTMSIDE_RTL3;
            data_[i + columns_ * 3    ] = T_LEFT_RAMP_BTMSIDE_RTL4;
            break;
        }
        case T_UP_RAMP_MASK:
        {
            data_[i + columns_ * 3    ] = T_UP_RAMP_LEFTSIDE_BTT1;
            data_[i + columns_ * 2    ] = T_UP_RAMP_LEFTSIDE_BTT2;
            data_[i + columns_        ] = T_UP_RAMP_LEFTSIDE_BTT3;
            data_[i                   ] = T_UP_RAMP_LEFTSIDE_BTT4;
            data_[i + columns_ * 3 + 1] = T_UP_RAMP_CENTRAL_BTT1;
            data_[i + columns_ * 2 + 1] = T_UP_RAMP_CENTRAL_BTT2;
            data_[i + columns_     + 1] = T_UP_RAMP_CENTRAL_BTT3;
            data_[i                + 1] = T_UP_RAMP_CENTRAL_BTT4;
            data_[i + columns_ * 3 + 2] = T_UP_RAMP_CENTRAL_BTT1;
            data_[i + columns_ * 2 + 2] = T_UP_RAMP_CENTRAL_BTT2;
            data_[i + columns_     + 2] = T_UP_RAMP_CENTRAL_BTT3;
            data_[i                + 2] = T_UP_RAMP_CENTRAL_BTT4;
            data_[i + columns_ * 3 + 3] = T_UP_RAMP_RIGHTSIDE_BTT1;
            data_[i + columns_ * 2 + 3] = T_UP_RAMP_RIGHTSIDE_BTT2;
            data_[i + columns_     + 3] = T_UP_RAMP_RIGHTSIDE_BTT3;
            data_[i                + 3] = T_UP_RAMP_RIGHTSIDE_BTT4;
            break;
        }
    }
}

void Terrain::deleteRamp( int i,char rampType  )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    int y = i / columns_;
    int x = i % columns_;

    if ( isRamp( data_[i] ) && (rampType == getMasterTerrainType( data_[i] ))
        ) assign(i, T_LOW /*getLowerTerrain( data_[i] ) */ );
    else return;
    if ( y > 1 )
    {
        if ( x > 0 ) deleteRamp( i - columns_ * 2 - 2,rampType );
        deleteRamp( i - columns_ * 2,rampType );
        if ( x < columns_ - 1 ) deleteRamp( i - columns_ * 2 + 2,rampType );
    }
    if ( x > 0 ) deleteRamp( i - 2,rampType );
    deleteRamp( i,rampType );
    if ( x < columns_ - 1 ) deleteRamp( i + 2,rampType );
    if ( y < rows_ - 1 )
    {
        if ( x > 0 ) deleteRamp( i + columns_ * 2 - 2,rampType );
        deleteRamp( i + columns_ * 2,rampType );
        if ( x < columns_ - 1 ) deleteRamp( i + columns_ * 2 + 2,rampType );
    }
}

/*
The adapt function will make sure that there are no inconsistencies in the 
terrain. For example, T_LOW_WATER can not be adjacent with T_HIGH_WATER

      /------------ start at this x level, at the center
      |
      v

* > > > > > *
^ * > > > * v
^ ^ * > * v v
^ ^ ^ i v v v  <---- start at this y level, at the center
^ ^ * < * v v
^ * < < < * v
* < < < < < *

Go round clockwise to correct terrain outwards based on the
new terrain modification.

Since we draw the 2x2 squares in a 3x3 config at a time, we start at iLoop = 4
because only the second 2x2 square outbound might need a change (the center 
and first circle of square we changed in the assign function).

'*' == corner

Also, ramps should be deleted if a change in the terrain makes there position
invalid. ( to be implemented )

*/
void Terrain::adapt( int i )
{
    int y = i / columns_;
    int x = i % columns_;
    int iLoop;
    bool changed = true;
    for ( iLoop = 4; changed ; iLoop += 2 )
    {
        changed = false;
        bool checkTopRightCorner = false;
        bool checkBottomRightCorner = false;
        bool checkBottomLeftCorner = false;
        bool checkTopLeftCorner = false;                
        // we go around the new terrain item clockwise to fix issues outbound
        // right side, going down
        if ( x + iLoop < columns_ - 1 )
        {
            int xStart = x + iLoop;
            int yStart = y - iLoop + 2;
            int yEnd = y + iLoop - 2;
            if ( yStart < 0 ) yStart = 0;
            if ( yStart > 1 )  checkTopRightCorner = true;
            if ( yEnd > rows_ - 2 ) yEnd = rows_ - 2;
            int iSource = yStart * columns_ + xStart - 2;
            if ( checkTopRightCorner )
            {
                int iDest = iSource - columns_ * 2 + 2;
                int srcT = getMasterTerrainType( iSource );
                int dstT = getMasterTerrainType( iDest );
                if ( (srcT > T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
                if ( srcT > getHigherTerrain( dstT ) )
                {
                    assign( iDest,getLowerTerrain( srcT ) );
                    changed = true;
                } else if ( srcT < getLowerTerrain( dstT ) )
                {
                    assign( iDest,getHigherTerrain( srcT ) );
                    changed = true;
                }
            }
            for ( int j = yStart; j <= yEnd; j += 2 )
            {
                int iDest = iSource + 2;
                int srcT = getMasterTerrainType( iSource );
                int dstT = getMasterTerrainType( iDest );
                if ( (srcT != T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
                if ( srcT > getHigherTerrain( dstT ) )
                {
                    assign( iDest,getLowerTerrain( srcT ) );
                    changed = true;
                } else if ( srcT < getLowerTerrain( dstT ) )
                {
                    assign( iDest,getHigherTerrain( srcT ) );
                    changed = true;
                }
                iSource += columns_ * 2;
            }
        }
        // bottom side, going left
        if ( y + iLoop < rows_ - 1 )
        {
            int xStart = x + iLoop - 2;
            int yStart = y + iLoop;
            int xEnd = x - iLoop + 2;
            if ( xStart > columns_ - 2 ) xStart = columns_ - 2;
            if ( xStart < columns_ - 2 ) checkBottomRightCorner = true;
            if ( xEnd < 0 ) xEnd = 0;
            int iSource = (yStart - 2) * columns_ + xStart;
            if ( checkBottomRightCorner )
            {
                int iDest = iSource + columns_ * 2 + 2;
                int srcT = getMasterTerrainType( iSource );
                int dstT = getMasterTerrainType( iDest );
                if ( (srcT > T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
                if ( srcT > getHigherTerrain( dstT ) )
                {
                    assign( iDest,getLowerTerrain( srcT ) );
                    changed = true;
                } else if ( srcT < getLowerTerrain( dstT ) )
                {
                    assign( iDest,getHigherTerrain( srcT ) );
                    changed = true;
                }
            }
            for ( int j = xStart; j >= xEnd; j -= 2 )
            {
                int iDest = iSource + columns_ * 2;
                int srcT = getMasterTerrainType( iSource );
                int dstT = getMasterTerrainType( iDest );
                if ( (srcT != T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
                if ( srcT > getHigherTerrain( dstT ) )
                {
                    assign( iDest,getLowerTerrain( srcT ) );
                    changed = true;
                } else if ( srcT < getLowerTerrain( dstT ) )
                {
                    assign( iDest,getHigherTerrain( srcT ) );
                    changed = true;
                }
                iSource -= 2;
            }
        }
        // left side, going up
        if ( x - iLoop >= 0 )
        {
            int xStart = x - iLoop;
            int yStart = y + iLoop - 2;
            int yEnd = y - iLoop + 2;
            if ( yStart > rows_ - 2 ) yStart = rows_ - 2;
            if ( yStart < rows_ - 2 ) checkBottomLeftCorner = true;
            if ( yEnd < 0 ) yEnd = 0;
            int iSource = yStart * columns_ + xStart + 2;
            if ( checkBottomLeftCorner )
            {
                int iDest = iSource + columns_ * 2 - 2;
                int srcT = getMasterTerrainType( iSource );
                int dstT = getMasterTerrainType( iDest );
                if ( (srcT > T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
                if ( srcT > getHigherTerrain( dstT ) )
                {
                    assign( iDest,getLowerTerrain( srcT ) );
                    changed = true;
                } else if ( srcT < getLowerTerrain( dstT ) )
                {
                    assign( iDest,getHigherTerrain( srcT ) );
                    changed = true;
                }
            }
            for ( int j = yStart; j >= yEnd; j -= 2 )
            {
                int iDest = iSource - 2;
                int srcT = getMasterTerrainType( iSource );
                int dstT = getMasterTerrainType( iDest );
                if ( (srcT != T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
                if ( srcT > getHigherTerrain( dstT ) )
                {
                    assign( iDest,getLowerTerrain( srcT ) );
                    changed = true;
                } else if ( srcT < getLowerTerrain( dstT ) )
                {
                    assign( iDest,getHigherTerrain( srcT ) );
                    changed = true;
                }
                iSource -= columns_ * 2;
            }
        }
        // top side, going right
        if ( y - iLoop >= 0 )
        {
            int xStart = x - iLoop + 2;
            int yStart = y - iLoop;
            int xEnd = x + iLoop - 2;
            if ( xStart < 0 ) xStart = 0;
            if ( xStart > 1 ) checkTopLeftCorner = true;
            if ( xEnd > columns_ - 2 ) xEnd = columns_ - 2;
            int iSource = (yStart + 2) * columns_ + xStart;
            if ( checkTopLeftCorner )
            {
                int iDest = iSource - columns_ * 2 - 2;
                int srcT = getMasterTerrainType( iSource );
                int dstT = getMasterTerrainType( iDest );
                if ( (srcT > T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
                if ( srcT > getHigherTerrain( dstT ) )
                {
                    assign( iDest,getLowerTerrain( srcT ) );
                    changed = true;
                } else if ( srcT < getLowerTerrain( dstT ) )
                {
                    assign( iDest,getHigherTerrain( srcT ) );
                    changed = true;
                }
            }
            for ( int j = xStart; j <= xEnd; j += 2 )
            {
                int iDest = iSource - columns_ * 2;
                int srcT = getMasterTerrainType( iSource );
                int dstT = getMasterTerrainType( iDest );
                if ( (srcT != T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
                if ( srcT > getHigherTerrain( dstT ) )
                {
                    assign( iDest,getLowerTerrain( srcT ) );
                    changed = true;
                } else if ( srcT < getLowerTerrain( dstT ) )
                {
                    assign( iDest,getHigherTerrain( srcT ) );
                    changed = true;
                }
                iSource += 2;
            }
        }
    }
    int x1 = x - iLoop - 2;
    int x2 = x + iLoop + 2;
    int y1 = y - iLoop - 2;
    int y2 = y + iLoop + 2;
    fixTransitions( x1,y1,x2,y2 );
}

void Terrain::fixTransitions( int x1, int y1, int x2, int y2 )
{
    assert( (x1 & 0x1) == 0 );
    assert( (x2 & 0x1) == 0 );
    assert( (y1 & 0x1) == 0 );
    assert( (y2 & 0x1) == 0 );
    if ( x1 < 0 ) x1 = 0;
    if ( y1 < 0 ) y1 = 0;
    if ( x2 > columns_ - 2 ) x2 = columns_ - 2;
    if ( y2 > rows_    - 2 ) y2 = rows_    - 2;
    for ( int j = y1; j <= y2; j += 2 )
    {
        for ( int i = x1; i <= x2; i += 2 )
        {
            char *srcTerrain = data_ + j * columns_ + i;
            if ( isLow( *srcTerrain ) )
            {
                // No transitions inside T_LOW
                srcTerrain += 2;
                continue;
            } else
            if ( isHigh( *srcTerrain ) )
            {
                // Make transitions to low ground if neccessary:
                bool transLeft = false;
                bool transTopLeft = false;
                bool transTop = false;
                bool transTopRight = false;
                bool transRight = false;
                bool transBottomRight = false;
                bool transBottom = false;
                bool transBottomLeft = false;
                // check surrounding 2x2 squares of this 2x2 T_HIGH square:
                if ( j > 0 )
                    {
                        if ( i > 0 )
                            transTopLeft = ! isHighCompatible( srcTerrain[-columns_ - 1] );
                        transTop = ! isHighCompatible( srcTerrain[-columns_] );
                        if ( i < (columns_ - 2) )
                            transTopRight = ! isHighCompatible( srcTerrain[-columns_ + 2] );
                    }
                if ( i > 0 ) transLeft = ! isHighCompatible( srcTerrain[-1] );
                if ( i < (columns_ - 2) ) transRight = ! isHighCompatible( srcTerrain[2] );

                if ( j < (rows_ - 2) )
                {
                    if ( i > 0 )
                        transBottomLeft = ! isHighCompatible( srcTerrain[columns_ * 2 - 1] );
                    transBottom = ! isHighCompatible( srcTerrain[columns_ * 2] );
                    if ( i < (columns_ - 2) )
                        transBottomRight = ! isHighCompatible( srcTerrain[columns_ * 2 + 2] );
                }
                // now evaluate.
                // top left square
                if ( transLeft )
                {
                    if ( transTop ) *srcTerrain = T_HIGH_TOP_LEFT;
                    else *srcTerrain = T_RIGHTWARDS_TO_HIGH;
                } else {
                    if ( transTop ) *srcTerrain = T_DOWNWARDS_TO_HIGH;
                    else {
                        if ( transTopLeft ) *srcTerrain = T_H_HIGH_BOTTOM_RIGHT;
                        else *srcTerrain = T_HIGH;
                    }
                }
                // top right square
                srcTerrain++;
                if ( transRight )
                {
                    if ( transTop ) *srcTerrain = T_HIGH_TOP_RIGHT;
                    else *srcTerrain = T_RIGHTWARDS_FROM_HIGH;
                } else {
                    if ( transTop ) *srcTerrain = T_DOWNWARDS_TO_HIGH;
                    else {
                        if ( transTopRight ) *srcTerrain = T_H_HIGH_BOTTOM_LEFT;
                        else *srcTerrain = T_HIGH;
                    }
                }
                // bottom right square
                srcTerrain += columns_;
                if ( transRight )
                {
                    if ( transBottom ) *srcTerrain = T_HIGH_BOTTOM_RIGHT;
                    else *srcTerrain = T_RIGHTWARDS_FROM_HIGH;
                } else {
                    if ( transBottom ) *srcTerrain = T_DOWNWARDS_FROM_HIGH;
                    else {
                        if ( transBottomRight ) *srcTerrain = T_H_HIGH_TOP_LEFT;
                        else *srcTerrain = T_HIGH;
                    }
                }
                // bottom left square
                srcTerrain--;
                if ( transLeft )
                {
                    if ( transBottom ) *srcTerrain = T_HIGH_BOTTOM_LEFT;
                    else *srcTerrain = T_RIGHTWARDS_TO_HIGH;
                } else {
                    if ( transBottom ) *srcTerrain = T_DOWNWARDS_FROM_HIGH;
                    else {
                        if ( transBottomLeft ) *srcTerrain = T_H_HIGH_TOP_RIGHT;
                        else *srcTerrain = T_HIGH;
                    }
                }
            } else
            if ( isLowWater( *srcTerrain ) )
            {
                // Make transitions to low ground if neccessary
                // (everything except low water needs a transition):
                bool transLeft = false;
                bool transTopLeft = false;
                bool transTop = false;
                bool transTopRight = false;
                bool transRight = false;
                bool transBottomRight = false;
                bool transBottom = false;
                bool transBottomLeft = false;
                // check surrounding 2x2 squares of this 2x2 T_LOW_WATER square:
                if ( j > 0 )
                {
                    if ( i > 0 )
                        transTopLeft = ! isLowWater( srcTerrain[-columns_ - 1] );
                    transTop = ! isLowWater( srcTerrain[-columns_] );
                    if ( i < (columns_ - 2) )
                        transTopRight = ! isLowWater( srcTerrain[-columns_ + 2] );
                }
                if ( i > 0 ) transLeft = ! isLowWater( srcTerrain[-1] );
                if ( i < (columns_ - 2) ) transRight = ! isLowWater( srcTerrain[2] );

                if ( j < (rows_ - 2) )
                {
                    if ( i > 0 )
                        transBottomLeft = ! isLowWater( srcTerrain[columns_ * 2 - 1] );
                    transBottom = ! isLowWater( srcTerrain[columns_ * 2] );
                    if ( i < (columns_ - 2) )
                        transBottomRight = ! isLowWater( srcTerrain[columns_ * 2 + 2] );
                }
                // now evaluate.
                // top left square
                if ( transLeft )
                {
                    if ( transTop ) *srcTerrain = T_LOW_WATER_TOP_LEFT;
                    else *srcTerrain = T_RIGHTWARDS_TO_LOW_WATER;
                } else {
                    if ( transTop ) *srcTerrain = T_DOWNWARDS_TO_LOW_WATER;
                    else {
                        if ( transTopLeft ) *srcTerrain = T_H_LOW_WATER_BOTTOM_RIGHT;
                        else *srcTerrain = T_LOW_WATER;
                    }
                }
                // top right square
                srcTerrain++;
                if ( transRight )
                {
                    if ( transTop ) *srcTerrain = T_LOW_WATER_TOP_RIGHT;
                    else *srcTerrain = T_RIGHTWARDS_FROM_LOW_WATER;
                } else {
                    if ( transTop ) *srcTerrain = T_DOWNWARDS_TO_LOW_WATER;
                    else {
                        if ( transTopRight ) *srcTerrain = T_H_LOW_WATER_BOTTOM_LEFT;
                        else *srcTerrain = T_LOW_WATER;
                    }
                }
                // bottom right square
                srcTerrain += columns_;
                if ( transRight )
                {
                    if ( transBottom ) *srcTerrain = T_LOW_WATER_BOTTOM_RIGHT;
                    else *srcTerrain = T_RIGHTWARDS_FROM_LOW_WATER;
                } else {
                    if ( transBottom ) *srcTerrain = T_DOWNWARDS_FROM_LOW_WATER;
                    else {
                        if ( transBottomRight ) *srcTerrain = T_H_LOW_WATER_TOP_LEFT;
                        else *srcTerrain = T_LOW_WATER;
                    }
                }
                // bottom left square
                srcTerrain--;
                if ( transLeft )
                {
                    if ( transBottom ) *srcTerrain = T_LOW_WATER_BOTTOM_LEFT;
                    else *srcTerrain = T_RIGHTWARDS_TO_LOW_WATER;
                } else {
                    if ( transBottom ) *srcTerrain = T_DOWNWARDS_FROM_LOW_WATER;
                    else {
                        if ( transBottomLeft ) *srcTerrain = T_H_LOW_WATER_TOP_RIGHT;
                        else *srcTerrain = T_LOW_WATER;
                    }
                }
            } else
            if ( isHighWater( *srcTerrain ) ) // no check needed, only option left
            {
                // Make transitions to low ground or high ground if neccessary
                // (basically everything except low water needs a transition):
                bool transLeft = false;
                bool transTopLeft = false;
                bool transTop = false;
                bool transTopRight = false;
                bool transRight = false;
                bool transBottomRight = false;
                bool transBottom = false;
                bool transBottomLeft = false;
                // check surrounding 2x2 squares of this 2x2 T_HIGH_WATER square:
                if ( j > 0 )
                {
                    if ( i > 0 )
                        transTopLeft = !isHighWater( srcTerrain[-columns_ - 1] );
                    transTop = !isHighWater( srcTerrain[-columns_] );
                    if ( i < (columns_ - 2) )
                        transTopRight = !isHighWater( srcTerrain[-columns_ + 2] );
                }
                if ( i > 0 ) transLeft = !isHighWater( srcTerrain[-1] );
                if ( i < (columns_ - 2) ) transRight = !isHighWater( srcTerrain[2] );

                if ( j < (rows_ - 2) )
                {
                    if ( i > 0 )
                        transBottomLeft = !isHighWater( srcTerrain[columns_ * 2 - 1] );
                    transBottom = !isHighWater( srcTerrain[columns_ * 2] );
                    if ( i < (columns_ - 2) )
                        transBottomRight = !isHighWater( srcTerrain[columns_ * 2 + 2] );
                }
                // now evaluate.
                // top left square
                if ( transLeft )
                {
                    if ( transTop ) *srcTerrain = T_HIGH_WATER_TOP_LEFT;
                    else *srcTerrain = T_RIGHTWARDS_TO_HIGH_WATER;
                } else {
                    if ( transTop ) *srcTerrain = T_DOWNWARDS_TO_HIGH_WATER;
                    else {
                        if ( transTopLeft ) *srcTerrain = T_H_HIGH_WATER_BOTTOM_RIGHT;
                        else *srcTerrain = T_HIGH_WATER;
                    }
                }
                // top right square
                srcTerrain++;
                if ( transRight )
                {
                    if ( transTop ) *srcTerrain = T_HIGH_WATER_TOP_RIGHT;
                    else *srcTerrain = T_RIGHTWARDS_FROM_HIGH_WATER;
                } else {
                    if ( transTop ) *srcTerrain = T_DOWNWARDS_TO_HIGH_WATER;
                    else {
                        if ( transTopRight ) *srcTerrain = T_H_HIGH_WATER_BOTTOM_LEFT;
                        else *srcTerrain = T_HIGH_WATER;
                    }
                }
                // bottom right square
                srcTerrain += columns_;
                if ( transRight )
                {
                    if ( transBottom ) *srcTerrain = T_HIGH_WATER_BOTTOM_RIGHT;
                    else *srcTerrain = T_RIGHTWARDS_FROM_HIGH_WATER;
                } else {
                    if ( transBottom ) *srcTerrain = T_DOWNWARDS_FROM_HIGH_WATER;
                    else {
                        if ( transBottomRight ) *srcTerrain = T_H_HIGH_WATER_TOP_LEFT;
                        else *srcTerrain = T_HIGH_WATER;
                    }
                }
                // bottom left square
                srcTerrain--;
                if ( transLeft )
                {
                    if ( transBottom ) *srcTerrain = T_HIGH_WATER_BOTTOM_LEFT;
                    else *srcTerrain = T_RIGHTWARDS_TO_HIGH_WATER;
                } else {
                    if ( transBottom ) *srcTerrain = T_DOWNWARDS_FROM_HIGH_WATER;
                    else {
                        if ( transBottomLeft ) *srcTerrain = T_H_HIGH_WATER_TOP_RIGHT;
                        else *srcTerrain = T_HIGH_WATER;
                    }
                }
            } 
        }
    }
}

void Terrain::show( int curX,int curY )
#define FOREGROUND_LIGHTGRAY    (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define FOREGROUND_WHITE        (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY )
#define FOREGROUND_BROWN        (FOREGROUND_GREEN | FOREGROUND_RED )
#define FOREGROUND_YELLOW       (FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_LIGHTCYAN    (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_LIGHTMAGENTA (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY)
#define BACKGROUND_BROWN        (BACKGROUND_RED | BACKGROUND_GREEN)
#define BACKGROUND_LIGHTBLUE    (BACKGROUND_BLUE | BACKGROUND_INTENSITY )
#define BACKGROUND_LIGHTGREEN   (BACKGROUND_GREEN | BACKGROUND_INTENSITY )
{
    assert( data_ != NULL );
    HANDLE hStdin = GetStdHandle( STD_INPUT_HANDLE );
    HANDLE hStdout = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    COORD dwCursorPosition;
    GetConsoleScreenBufferInfo( hStdout,&csbiInfo );
    dwCursorPosition.X = 0;
    dwCursorPosition.Y = 0;
    SetConsoleCursorPosition( hStdout,dwCursorPosition );
    char *i = data_;

    // ╚╔╩╦╠═╬ ┘┌█▄▀┤╣║╗╝┐└┴┬├─┼«»░▒▓│┤
    for ( int rows = 0; rows < rows_; rows++ )
    {
        for ( int columns = 0; columns < columns_; columns++ )
        {
            if ( 
                (columns ==  curX      && rows ==  curY     ) || 
                (columns == (curX + 1) && rows ==  curY     ) ||
                (columns ==  curX      && rows == (curY + 1)) ||
                (columns == (curX + 1) && rows == (curY + 1)))
            {
                SetConsoleTextAttribute( hStdout,
                    BACKGROUND_RED | BACKGROUND_INTENSITY );
                std::cout << ' ';
            } else {
                switch ( *i )
                {
                    case T_LOW_WATER_TOP_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BLUE );
                        std::cout << '/';
                        break;
                    }
                    case T_LOW_WATER_TOP_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BLUE );
                        std::cout << '\\';
                        break;
                    }
                    case T_LOW_WATER_BOTTOM_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BLUE );
                        std::cout << '\\';
                        break;
                    }
                    case T_LOW_WATER_BOTTOM_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BLUE );
                        std::cout << '/';
                        break;
                    }
                    case T_H_LOW_WATER_TOP_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_BLUE );
                        std::cout << '/';
                        break;
                    }
                    case T_H_LOW_WATER_TOP_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_BLUE );
                        std::cout << '\\';
                        break;
                    }
                    case T_H_LOW_WATER_BOTTOM_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_BLUE );
                        std::cout << '\\';
                        break;
                    }
                    case T_H_LOW_WATER_BOTTOM_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_BLUE );
                        std::cout << '/';
                        break;
                    }

                    case T_HIGH_TOP_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BROWN );
                        std::cout << '/';
                        break;
                    }
                    case T_HIGH_TOP_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BROWN );
                        std::cout << '\\';
                        break;
                    }
                    case T_HIGH_BOTTOM_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BROWN );
                        std::cout << '\\';
                        break;
                    }
                    case T_HIGH_BOTTOM_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BROWN );
                        std::cout << '/';
                        break;
                    }
                    case T_H_HIGH_TOP_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_BROWN );
                        std::cout << '/';
                        break;
                    }
                    case T_H_HIGH_TOP_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_BROWN );
                        std::cout << '\\';
                        break;
                    }
                    case T_H_HIGH_BOTTOM_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_BROWN );
                        std::cout << '\\';
                        break;
                    }
                    case T_H_HIGH_BOTTOM_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_BROWN );
                        std::cout << '/';
                        break;
                    }
                    case T_HIGH_WATER_TOP_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_LIGHTBLUE );
                        std::cout << '/';
                        break;
                    }
                    case T_HIGH_WATER_TOP_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_LIGHTBLUE );
                        std::cout << '\\';
                        break;
                    }
                    case T_HIGH_WATER_BOTTOM_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_LIGHTBLUE );
                        std::cout << '\\';
                        break;
                    }
                    case T_HIGH_WATER_BOTTOM_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_LIGHTBLUE );
                        std::cout << '/';
                        break;
                    }
                    case T_H_HIGH_WATER_TOP_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_LIGHTBLUE );
                        std::cout << '/';
                        break;
                    }
                    case T_H_HIGH_WATER_TOP_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_LIGHTBLUE );
                        std::cout << '\\';
                        break;
                    }
                    case T_H_HIGH_WATER_BOTTOM_LEFT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_LIGHTBLUE );
                        std::cout << '\\';
                        break;
                    }
                    case T_H_HIGH_WATER_BOTTOM_RIGHT:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_WHITE | BACKGROUND_LIGHTBLUE );
                        std::cout << '/';
                        break;
                    }
                    case T_RIGHTWARDS_TO_LOW_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BLUE );
                        std::cout << '>';
                        break;
                    }
                    case T_RIGHTWARDS_FROM_LOW_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BLUE );
                        std::cout << '<';
                        break;
                    }
                    case T_DOWNWARDS_TO_LOW_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BLUE );
                        std::cout << 'v';
                        break;
                    }
                    case T_DOWNWARDS_FROM_LOW_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BLUE );
                        std::cout << '^';
                        break;
                    }
                    case T_RIGHTWARDS_TO_HIGH:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BROWN );
                        std::cout << '<';
                        break;
                    }
                    case T_RIGHTWARDS_FROM_HIGH:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BROWN );
                        std::cout << '>';
                        break;
                    }
                    case T_DOWNWARDS_TO_HIGH:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BROWN );
                        std::cout << '^';
                        break;
                    }
                    case T_DOWNWARDS_FROM_HIGH:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_BROWN );
                        std::cout << 'v';
                        break;
                    }
                    case T_RIGHTWARDS_TO_HIGH_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_LIGHTBLUE );
                        std::cout << '>';
                        break;
                    }
                    case T_RIGHTWARDS_FROM_HIGH_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_LIGHTBLUE );
                        std::cout << '<';
                        break;
                    }
                    case T_DOWNWARDS_TO_HIGH_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_LIGHTBLUE );
                        std::cout << 'v';
                        break;
                    }
                    case T_DOWNWARDS_FROM_HIGH_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            FOREGROUND_LIGHTGRAY | BACKGROUND_LIGHTBLUE );
                        std::cout << '^';
                        break;
                    }
                    case T_LOW:
                    {
                        SetConsoleTextAttribute( hStdout,
                            BACKGROUND_GREEN );
                        std::cout << ' ';
                        break;
                    }
                    case T_HIGH:
                    {
                        SetConsoleTextAttribute( hStdout,
                            BACKGROUND_LIGHTGREEN );
                        std::cout << ' ';
                        break;
                    }
                    case T_LOW_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            BACKGROUND_BLUE );
                        std::cout << ' ';
                        break;
                    }
                    case T_HIGH_WATER:
                    {
                        SetConsoleTextAttribute( hStdout,
                            BACKGROUND_BLUE | BACKGROUND_INTENSITY );
                        std::cout << ' ';
                        break;
                    }
                    case T_RIGHT_RAMP_TOPSIDE_LTR1:     
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTCYAN | BACKGROUND_RED );
                        std::cout << '1';
                        break;
                    }
                    case T_RIGHT_RAMP_TOPSIDE_LTR2:    
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTCYAN | BACKGROUND_RED );
                        std::cout << '2';
                        break;
                    }
                    case T_RIGHT_RAMP_TOPSIDE_LTR3:     
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTCYAN | BACKGROUND_RED );
                        std::cout << '3';
                        break;
                    }
                    case T_RIGHT_RAMP_TOPSIDE_LTR4:    
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTCYAN | BACKGROUND_RED );
                        std::cout << '4';
                        break;
                    }
                    case T_RIGHT_RAMP_BTMSIDE_LTR1:    
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTCYAN | BACKGROUND_RED );
                        std::cout << '1';
                        break;
                    }
                    case T_RIGHT_RAMP_BTMSIDE_LTR2:     
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTCYAN | BACKGROUND_RED );
                        std::cout << '2';
                        break;
                    }
                    case T_RIGHT_RAMP_BTMSIDE_LTR3:     
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTCYAN | BACKGROUND_RED );
                        std::cout << '3';
                        break;
                    }
                    case T_RIGHT_RAMP_BTMSIDE_LTR4:        
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTCYAN | BACKGROUND_RED );
                        std::cout << '4';
                        break;
                    }
                    case T_DOWN_RAMP_LEFTSIDE_TTB1: 
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTMAGENTA | BACKGROUND_RED );
                        std::cout << '1';
                        break;
                    }
                    case T_DOWN_RAMP_LEFTSIDE_TTB2:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTMAGENTA | BACKGROUND_RED );
                        std::cout << '2';
                        break;
                    }
                    case T_DOWN_RAMP_LEFTSIDE_TTB3:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTMAGENTA | BACKGROUND_RED );
                        std::cout << '3';
                        break;
                    }
                    case T_DOWN_RAMP_LEFTSIDE_TTB4:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTMAGENTA | BACKGROUND_RED );
                        std::cout << '4';
                        break;
                    }
                    case T_DOWN_RAMP_RIGHTSIDE_TTB1:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTMAGENTA | BACKGROUND_RED );
                        std::cout << '1';
                        break;
                    }
                    case T_DOWN_RAMP_RIGHTSIDE_TTB2:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTMAGENTA | BACKGROUND_RED );
                        std::cout << '2';
                        break;
                    }
                    case T_DOWN_RAMP_RIGHTSIDE_TTB3:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTMAGENTA | BACKGROUND_RED );
                        std::cout << '3';
                        break;
                    }
                    case T_DOWN_RAMP_RIGHTSIDE_TTB4:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTMAGENTA | BACKGROUND_RED );
                        std::cout << '4';
                        break;
                    }
                    case T_LEFT_RAMP_TOPSIDE_RTL1: 
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_YELLOW | BACKGROUND_RED );
                        std::cout << '1';
                        break;
                    }
                    case T_LEFT_RAMP_TOPSIDE_RTL2:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_YELLOW | BACKGROUND_RED );
                        std::cout << '2';
                        break;
                    }
                    case T_LEFT_RAMP_TOPSIDE_RTL3:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_YELLOW | BACKGROUND_RED );
                        std::cout << '3';
                        break;
                    }
                    case T_LEFT_RAMP_TOPSIDE_RTL4:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_YELLOW | BACKGROUND_RED );
                        std::cout << '4';
                        break;
                    }
                    case T_LEFT_RAMP_BTMSIDE_RTL1:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_YELLOW | BACKGROUND_RED );
                        std::cout << '1';
                        break;
                    }
                    case T_LEFT_RAMP_BTMSIDE_RTL2:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_YELLOW | BACKGROUND_RED );
                        std::cout << '2';
                        break;
                    }
                    case T_LEFT_RAMP_BTMSIDE_RTL3:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_YELLOW | BACKGROUND_RED );
                        std::cout << '3';
                        break;
                    }
                    case T_LEFT_RAMP_BTMSIDE_RTL4:
                    {
                        SetConsoleTextAttribute( hStdout,FOREGROUND_YELLOW | BACKGROUND_RED );
                        std::cout << '4';
                        break;
                    }
                    case T_UP_RAMP_LEFTSIDE_BTT1: 
                    {
                        SetConsoleTextAttribute( 
                            hStdout,FOREGROUND_BROWN | BACKGROUND_RED );
                        std::cout << '1';
                        break;
                    }
                    case T_UP_RAMP_LEFTSIDE_BTT2:
                    {
                        SetConsoleTextAttribute(
                            hStdout,FOREGROUND_BROWN | BACKGROUND_RED );
                        std::cout << '2';
                        break;
                    }
                    case T_UP_RAMP_LEFTSIDE_BTT3:
                    {
                        SetConsoleTextAttribute(
                            hStdout,FOREGROUND_BROWN | BACKGROUND_RED );
                        std::cout << '3';
                        break;
                    }
                    case T_UP_RAMP_LEFTSIDE_BTT4:
                    {
                        SetConsoleTextAttribute(
                            hStdout,FOREGROUND_BROWN | BACKGROUND_RED );
                        std::cout << '4';
                        break;
                    }
                    case T_UP_RAMP_RIGHTSIDE_BTT1:
                    {
                        SetConsoleTextAttribute(
                            hStdout,FOREGROUND_BROWN | BACKGROUND_RED );
                        std::cout << '1';
                        break;
                    }
                    case T_UP_RAMP_RIGHTSIDE_BTT2:
                    {
                        SetConsoleTextAttribute(
                            hStdout,FOREGROUND_BROWN | BACKGROUND_RED );
                        std::cout << '2';
                        break;
                    }
                    case T_UP_RAMP_RIGHTSIDE_BTT3:
                    {
                        SetConsoleTextAttribute(
                            hStdout,FOREGROUND_BROWN | BACKGROUND_RED );
                        std::cout << '3';
                        break;
                    }
                    case T_UP_RAMP_RIGHTSIDE_BTT4:
                    {
                        SetConsoleTextAttribute(
                            hStdout,FOREGROUND_BROWN | BACKGROUND_RED );
                        std::cout << '4';
                        break;
                    }
                    case T_RIGHT_RAMP_CENTRAL_LTR1:
                    case T_RIGHT_RAMP_CENTRAL_LTR2:
                    case T_RIGHT_RAMP_CENTRAL_LTR3:
                    case T_RIGHT_RAMP_CENTRAL_LTR4:
                    case T_LEFT_RAMP_CENTRAL_RTL1:
                    case T_LEFT_RAMP_CENTRAL_RTL2:
                    case T_LEFT_RAMP_CENTRAL_RTL3:
                    case T_LEFT_RAMP_CENTRAL_RTL4:
                    {
                        SetConsoleTextAttribute(
                            hStdout,FOREGROUND_WHITE | BACKGROUND_BROWN );
                        std::cout << '-';
                        break;
                    }
                    case T_DOWN_RAMP_CENTRAL_TTB2:
                    case T_DOWN_RAMP_CENTRAL_TTB3:
                    case T_DOWN_RAMP_CENTRAL_TTB4:
                    case T_UP_RAMP_CENTRAL_BTT1:
                    case T_UP_RAMP_CENTRAL_BTT2:
                    case T_UP_RAMP_CENTRAL_BTT3:
                    case T_UP_RAMP_CENTRAL_BTT4:
                    case T_DOWN_RAMP_CENTRAL_TTB1:
                    {
                        SetConsoleTextAttribute(
                            hStdout, FOREGROUND_WHITE | BACKGROUND_BROWN );
                        std::cout << '|';
                        break;
                    }
                    default:
                    {
                        SetConsoleTextAttribute( hStdout,
                            BACKGROUND_BLUE | BACKGROUND_GREEN | 
                            BACKGROUND_RED | BACKGROUND_INTENSITY );
                        std::cout << '!';
                        break;
                    }
                }
            }
            i++;
        }
        if( columns_ < 80 ) std::cout << std::endl;
    }
    SetConsoleTextAttribute( hStdout, FOREGROUND_LIGHTGRAY );
    std::cout << "a = LOW WATER, z = LOW GROUND, e = HIGH GROUND, r = HIGH WATER" 
              << std::endl << "Navigate with o, k, l, m or 8,5,4,6";
}

/******************************************************************************
*                                                                             *
* Start of the Race class code                                                *
*                                                                             *
******************************************************************************/
/*
// Fied names for units & buildings
const std::vector<std::string> FieldNameStrings =
{
    std::string( "type" ),
    std::string( "HasRadarCapability" ),
    std::string( "CanMove" ),
    std::string( "CanFLy" ),
    std::string( "CanJump" ),
    std::string( "MoveSpeed" ),
    std::string( "MaxHealth" ),
    std::string( "MaxShield" ),
    std::string( "MaxEnergy" ),
    std::string( "FiringRange" ),
    std::string( "AntiAirFirePower" ),
    std::string( "AntiGroundFirePower" ),
    std::string( "FiringRate" ),
    std::string( "NeededCargoSpace" ),
    std::string( "CostToProduce" ),
    std::string( "CargoCapacity" ),
    std::string( "Requires" )
};

// Field values
#define STR_UNIT_TYPE_UNIT      "unit"
#define STR_UNIT_TYPE_BUILDING  "building"
#define STR_BOOLEAN_YES         "yes"
#define STR_BOOLEAN_NO          "no"
#define STR_BOOLEAN_TRUE        "true"
#define STR_BOOLEAN_FALSE       "false"
*/


class Unit
{
public:
    Unit() {}
    int                 loadFromIniFile(
        IniFile& iniFile,
        const std::string& unitName 
    );
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
    int             id_;                 // index in list of all units / buildings
    std::string     name_;
    bool            hasRadarCapability_; // whether it can detect hidden units
    bool            canMove_;            // whether it can move
    bool            canFLy_;             // whether the unit is an airplane
    bool            canJump_;            // whether it can jump onto higher ground or from a cliff
    int             moveSpeed_;          // how fast it can move
    int             maxHealth_;          // its maximum health,can be more than 100
    int             maxShield_;          // its maximum shield level
    int             maxEnergy_;          // its maximum energy level
    int             firingRange_;        // how close units have to be before unit can shoot them
    int             antiAirFirePower_;   // how effective unit is against enemy aircraft
    int             antiGroundFirePower_;// how effective unit is against enemy ground units
    int             firingRate_;         // how fast it shoots
    int             neededCargoSpace_;   // how much it occupies if transported
    int             costToProduce_;      // How much money it costs to produce this unit
    int             cargoCapacity_;      // how much cargo it can carry
    std::vector <std::string> canProduce_;// Can be used if a unit can evolve maybe ?
    std::vector <std::string> requires_; // building( s ) required to produce the unit/building
};

int Unit::loadFromIniFile( 
    IniFile& iniFile,
    const std::string& unitName )
{
    int error;
    if ( !iniFile.isLoaded() ) return -1;
    name_ = unitName;
    error = iniFile.getKey( unitName,"hasRadarCapability",hasRadarCapability_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"canMove",canMove_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"canFLy",canFLy_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"canJump",canJump_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"moveSpeed",moveSpeed_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"maxHealth",maxHealth_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"maxShield",maxShield_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"maxEnergy",maxEnergy_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"firingRange",firingRange_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"antiAirFirePower",antiAirFirePower_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"antiGroundFirePower",antiGroundFirePower_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"firingRate",firingRate_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"neededCargoSpace",neededCargoSpace_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"costToProduce",costToProduce_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"cargoCapacity",cargoCapacity_ );
    if ( error != 0 ) return -1;
    // temporary, should make a list of units & buildings and assign them a number
    {
        std::string strList;
        error = iniFile.getKey( unitName,"Requires",strList );
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
    }
    {
        std::string strList;
        error = iniFile.getKey( unitName,"CanProduce",strList );
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
                i = 0;
                canProduce_.push_back( std::string( buf ) );
            } else if ( c[1] == '\0' )
            {
                buf[i] = '\0';
                canProduce_.push_back( std::string( buf ) );
            }
        }
        delete buf;
        strList.clear();
    }
    // end temporary
    return 0;
}

class Building
{
public:
    Building() {}
    int                 loadFromIniFile(
        IniFile& iniFile,
        const std::string& unitName
    );
    void                setID( int id ) { assert( id >= 0 ); id_ = id; }
    int                 getId() const { return id_; }
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
    int             id_;                 // index in list of all units / buildings
    std::string     name_;
    bool            hasRadarCapability_; // whether it can detect hidden units
    bool            canMove_;            // whether it can move
    bool            canFLy_;             // whether the unit is an airplane
    bool            canJump_;            // whether it can jump onto higher ground or from a cliff
    int             moveSpeed_;          // how fast it can move
    int             maxHealth_;          // its maximum health,can be more than 100
    int             maxShield_;          // its maximum shield level
    int             maxEnergy_;          // its maximum energy level
    int             firingRange_;        // how close units have to be before unit can shoot them
    int             antiAirFirePower_;   // how effective unit is against enemy aircraft
    int             antiGroundFirePower_;// how effective unit is against enemy ground units
    int             firingRate_;         // how fast it shoots
    int             neededCargoSpace_;   // how much it occupies if transported
    int             costToProduce_;      // How much money it costs to produce this unit
    int             cargoCapacity_;      // how much cargo it can carry
    std::vector <std::string> canProduce_;// Can be used if a unit can evolve maybe ?
    std::vector <std::string> requires_; // building( s ) required to produce the unit/building
};

int Building::loadFromIniFile(
    IniFile& iniFile,
    const std::string& unitName )
{
    int error;
    if ( !iniFile.isLoaded() ) return -1;
    /*
    int keyNr = 0;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],name_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],hasRadarCapability_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],canMove_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],canFLy_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],canJump_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],moveSpeed_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],maxHealth_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],maxShield_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],maxEnergy_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],firingRange_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],antiAirFirePower_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],antiGroundFirePower_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],firingRate_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],neededCargoSpace_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],costToProduce_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],cargoCapacity_ );
    if ( error != 0 ) return -1;
    std::string reqList;
    error = iniFile.getKey( unitName,FieldNameStrings[keyNr++],reqList );
    if ( error != 0 ) return -1;
    */
    name_ = unitName;
    error = iniFile.getKey( unitName,"hasRadarCapability",hasRadarCapability_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"canMove",canMove_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"canFLy",canFLy_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"canJump",canJump_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"moveSpeed",moveSpeed_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"maxHealth",maxHealth_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"maxShield",maxShield_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"maxEnergy",maxEnergy_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"firingRange",firingRange_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"antiAirFirePower",antiAirFirePower_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"antiGroundFirePower",antiGroundFirePower_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"firingRate",firingRate_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"neededCargoSpace",neededCargoSpace_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"costToProduce",costToProduce_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKey( unitName,"cargoCapacity",cargoCapacity_ );
    if ( error != 0 ) return -1;
    // temporary, should make a list of units & buildings and assign them a number
    {
        std::string strList;
        error = iniFile.getKey( unitName,"Requires",strList );
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
    }
    {
        std::string strList;
        error = iniFile.getKey( unitName,"CanProduce",strList );
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
                i = 0;
                canProduce_.push_back( std::string( buf ) );
            } else if ( c[1] == '\0' )
            {
                buf[i] = '\0';
                canProduce_.push_back( std::string( buf ) );
            }
        }
        delete buf;
        strList.clear();
    }
    // end temporary
    return 0;
}


/*
    A race is a collection of units and buildings
*/
class Race
{
public:
private:
};

/*
    A faction is an instance of a race in a specific scenario. The scenario can
    apply limits to the functionality of the race, for instance certain units 
    or buildings might not be available.
*/
class Faction
{
};

/*
    A scenario contains a terrain, a collection of factions with their 
    buildings and units, alliances between factions, goals for the player(s)
*/
class Scenario
{
};

int main()
{
    
    IniFile iniFile( std::string( INIFILE_FOLDER INIFILE_FILENAME ) );

    std::string result;
    int error = iniFile.getKey( "HeavyFactory","CanProduce",result );

    std::cout << "[" << "HeavyFactory" << "]" << ", " << "CanProduce" << "='"
              << result << "'" << std::endl;
    std::cout << "Error: " << error << std::endl;
    std::cout << "---------------------------------------------" << std::endl;

    int intResult;
    error = iniFile.getKey( "RadarHouse","MaxHealth",intResult );
    if( error != 0 ) std::cout << "Error " << error << " occured" << std::endl;
    else std::cout << "MaxHealth = " << intResult << std::endl;

    bool boolResult;
    error = iniFile.getKey( "RadarHouse","HasRadarCapability",boolResult );
    if ( error != 0 ) std::cout << "Error " << error << " occured" << std::endl;
    else std::cout << "HasRadarCapability = " << boolResult << std::endl;   
    std::cout << std::endl;
    Unit unit;
    unit.loadFromIniFile( iniFile,std::string( "HEAVYFACTORY" ) );

    for ( const std::string& s : unit.requires() ) std::cout << s << std::endl;
    std::cout << std::endl;
    for ( const std::string& s : unit.canProduce() ) std::cout << s << std::endl;
    std::cout << std::endl;
    std::string dst;
    while ( iniFile.getNextSection( dst ) == 0 )
    {
        std::cout << dst << std::endl;
        //_getch();
    }
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
        else if ( wait == 'q' ) terrain.drawTerrain( curX,curY,T_RIGHT_RAMP_MASK );
        else if ( wait == 's' ) terrain.drawTerrain( curX,curY,T_DOWN_RAMP_MASK );
        else if ( wait == 'd' ) terrain.drawTerrain( curX,curY,T_LEFT_RAMP_MASK );
        else if ( wait == 'f' ) terrain.drawTerrain( curX,curY,T_UP_RAMP_MASK );
        //else if ( wait == 'c' ) terrain.deleteRamp( curX,curY,terrain.getMasterTerrainType( curX,curY ) );
        else continue;
        terrain.show( curX,curY );
    }
    return 0;
}



