#include <iostream>
#include <fstream>
#include "TerrainEditor.h"
#include "IniFile.h"


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

/*
    We use our own integer-to-ascii conversion table for the possible terrain
    values as not all characters are safe for use in an ini file, such as:
    = ; [ ]
    It is 64 characters long, as we need the 0 .. 63 range:
    alphabet + decimals + special characters <=> 2 * 26 + 10 + 2 = 64 
    We can't use the alphabet twice because the ini file system is case 
    insensitive. The "!" characters are unused in the code.
*/
const char *Terrain::encodestr = 
//  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ&@#'(^\"!{})-_*+§~/\\:.,?|%$<>";
    "0!!!456789ABCDEFG!!!!!!!!!!!!!!!W!!!&@#'(^\"Z{})-_!!!~/\\:.,?|%$<>";
//   01        10        20        30        40         50         60
bool Terrain::isValidLocationFor( int i,char terrain )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    assert( isBasicTerrain( terrain ) /* || isRamp( terrain ) */ );
    int y = i / columns_;
    int x = i % columns_;
    return true;
    /*
    switch ( terrain )
    {        
        case T_RIGHT_RAMP_MASK:
        {
            if ( x < 4 ) return false;
            if ( x > columns_ - 6 ) return false;
            if ( y > rows_ - 4 ) return false;
            if (
                (getMasterTerrainType( i - 2 - columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - 2 + columns_ * 4 ) != T_HIGH) ||
                (getMasterTerrainType( i - 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - 2 + columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - 4 ) != T_HIGH) ||
                (getMasterTerrainType( i - 4 + columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 4 ) != T_LOW) ||
                (getMasterTerrainType( i ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 2 + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 4 ) != T_LOW) ||
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
                (getMasterTerrainType( i - columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 2 + 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 4 ) != T_HIGH) ||
                (getMasterTerrainType( i - columns_ * 4 + 2 ) != T_HIGH) ||
                (getMasterTerrainType( i - 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 4 ) != T_LOW) ||
                (getMasterTerrainType( i ) != T_LOW) ||
                (getMasterTerrainType( i + 2 ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 2 + 2 ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 4 ) != T_LOW) ||
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
                (getMasterTerrainType( i + 4 ) != T_HIGH) ||
                (getMasterTerrainType( i + 4 + columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i + 6 ) != T_HIGH) ||
                (getMasterTerrainType( i + 6 + columns_ * 2 ) != T_HIGH) ||
                (getMasterTerrainType( i + 2 - columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 2 + columns_ * 4 ) != T_LOW) ||
                (getMasterTerrainType( i ) != T_LOW) ||
                (getMasterTerrainType( i + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 2 ) != T_LOW) ||
                (getMasterTerrainType( i + 2 + columns_ * 2 ) != T_LOW) ||
                (getMasterTerrainType( i - 2 ) != T_LOW) ||
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
                    (getMasterTerrainType( i + columns_ * 4 ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 4 + 2 ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 6 ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 6 + 2 ) != T_HIGH) ||
                    (getMasterTerrainType( i + columns_ * 2 - 2 ) != T_LOW) ||
                    (getMasterTerrainType( i + columns_ * 2 + 4 ) != T_LOW) ||
                    (getMasterTerrainType( i ) != T_LOW) ||
                    (getMasterTerrainType( i + 2 ) != T_LOW) ||
                    (getMasterTerrainType( i + columns_ * 2 ) != T_LOW) ||
                    (getMasterTerrainType( i + columns_ * 2 + 2 ) != T_LOW) ||
                    (getMasterTerrainType( i - columns_ * 2 ) != T_LOW) ||
                    (getMasterTerrainType( i - columns_ * 2 + 2 ) != T_LOW)
                    ) return false;
                else return true;
                break;
            }
            break;
        }
        // Other master terrain types have no restrictions
        default:
        {
            return true;
            break;
        }
    }
    */
}

/*
    MAJOR ISSUE: INI READER CAN ONLY READ ROWS UP TILL 255 BYTES LONG!!!
    80 columns ~~ 250 characters
    Terrain needs:
    - columns
    - rows
    - tileset
    - actual terrain, comma separated

[Characteristics]
name=wastelands
rows=40
columns=80

[TerrainData]
row000=8F,15,14,...
row001=8F,34,01,...

*/
int Terrain::loadTerrain( std::string filename )
{
    IniFile iniFile( filename );
    if ( !iniFile.isLoaded() ) return -1;
    int error = iniFile.getKeyValue( "Characteristics","name",name_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( "Characteristics","columns",columns_ );
    if ( error != 0 ) return -1;
    error = iniFile.getKeyValue( "Characteristics","rows",rows_ );
    if ( error != 0 ) return -1;
    // load with default data:
    init( columns_,rows_ );
    for ( int rowNr = 0; rowNr < rows_; rowNr++ )
    {
        std::string rowValue;
        std::string rowStr = "ROW";
        if ( rowNr < 100 ) rowStr += "0";
        if ( rowNr < 10 ) rowStr += "0";
        rowStr += std::to_string( rowNr );
        int error = iniFile.getKeyValue( "TerrainData",rowStr,rowValue );
        if ( error != 0 ) return -1;
        if ( rowValue.length() != columns_ ) return -1;
        for ( int columnNr = 0; columnNr < columns_; columnNr++ )
        {
            char value = rowValue[columnNr];
            int i;
            for ( i = 0; i < (int)strlen( encodestr ); i++ )
            {
                if ( value == encodestr[i] ) break;
            }
            if ( value != encodestr[i] ) return -1;
            data_[rowNr * columns_ + columnNr] = i;
        }
        /*
        // the version below uses to much space (3 times more) and the ini file
        // wouldn't be able to handle that
        int i = 0;
        int iMax = rowValue.length() - 1;
        char buf[3];
        buf[2] = '\0';
        for ( int columnNr = 0; columnNr < columns_; columnNr++ )
        {
            if ( i > iMax ) return -1;
            int charsCopied = rowValue.copy( buf,2,i );
            if ( charsCopied != 2 ) return -1;
            int v;
            for ( v = 0; v < 16; v++ ) if ( buf[0] == hexstr[v] ) break;
            if ( buf[0] != hexstr[v] ) return -1;
            int value = v << 4;
            for ( v = 0; v < 16; v++ ) if ( buf[1] == hexstr[v] ) break;
            if ( buf[1] != hexstr[v] ) return -1;
            value += v;
            data_[rowNr * columns_ + columnNr] = value;
            i += 3;
        }
        */
    }
    return 0;
}

int Terrain::saveTerrain( std::string filename )
{
    std::ofstream terrainFile( filename.c_str() );
    if ( !terrainFile.is_open() ) return -1;
    terrainFile << "; terrain saved by editor" << std::endl;
    terrainFile << std::endl;
    terrainFile << "[Characteristics]" << std::endl;
    terrainFile << "name=" << name_ << std::endl;
    terrainFile << "rows=" << rows_ << std::endl;
    terrainFile << "columns=" << columns_ << std::endl;
    terrainFile << std::endl;
    terrainFile << "[TerrainData]" << std::endl;
    char buf[3];
    buf[2] = '\0';
    for ( int rowNr = 0; rowNr < rows_; rowNr++ )
    {
        terrainFile << "Row";
        if ( rowNr < 100 ) terrainFile << "0";
        if ( rowNr < 10 ) terrainFile << "0";
        terrainFile << rowNr;
        terrainFile << "=";        
        for ( int columnNr = 0; columnNr < columns_; columnNr++ )
        {
            int value = data_[rowNr * columns_ + columnNr];
            assert( (value < (int)strlen( encodestr ) ) && (value >= 0) );
            terrainFile << encodestr[value];
        }
        /*
        // the version below uses to much space (3 times more) and the ini file
        // wouldn't be able to handle that
        for ( int columnNr = 0; columnNr < columns_; columnNr++ )
        {
            int value = data_[rowNr * columns_ + columnNr];
            assert( (value <= 0xFF) && (value >= 0) );
            buf[0] = hexstr[value >> 4];
            buf[1] = hexstr[value & 0xF];
            terrainFile << buf << ",";
        }
        */
        terrainFile << std::endl;
    }
    terrainFile.close();
    return 0;
}

void Terrain::drawTerrain( int i,char terrain )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    assert( isBasicTerrain( terrain ) /* || isRamp( terrain ) */ );
    if ( !isValidLocationFor( i,terrain ) ) return;
    int y = i / columns_;
    int x = i % columns_;
    /*
    if ( isRamp( terrain ) )
    {
        drawRamp( i,terrain );
        int x1 = x - 2;
        int y1 = y - 2;
        int x2 = x + 4;
        int y2 = y + 4;
        fixTransitions( x1,y1,x2,y2 );
    } 
    else 
    */
    {
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

/*
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
            data_[i] = T_RIGHT_RAMP_TOPSIDE_LTR1;
            data_[i + 1] = T_RIGHT_RAMP_TOPSIDE_LTR2;
            data_[i + 2] = T_RIGHT_RAMP_TOPSIDE_LTR3;
            data_[i + 3] = T_RIGHT_RAMP_TOPSIDE_LTR4;
            data_[i + columns_] = T_RIGHT_RAMP_CENTRAL_LTR1;
            data_[i + columns_ + 1] = T_RIGHT_RAMP_CENTRAL_LTR2;
            data_[i + columns_ + 2] = T_RIGHT_RAMP_CENTRAL_LTR3;
            data_[i + columns_ + 3] = T_RIGHT_RAMP_CENTRAL_LTR4;
            data_[i + columns_ * 2] = T_RIGHT_RAMP_CENTRAL_LTR1;
            data_[i + columns_ * 2 + 1] = T_RIGHT_RAMP_CENTRAL_LTR2;
            data_[i + columns_ * 2 + 2] = T_RIGHT_RAMP_CENTRAL_LTR3;
            data_[i + columns_ * 2 + 3] = T_RIGHT_RAMP_CENTRAL_LTR4;
            data_[i + columns_ * 3] = T_RIGHT_RAMP_BTMSIDE_LTR1;
            data_[i + columns_ * 3 + 1] = T_RIGHT_RAMP_BTMSIDE_LTR2;
            data_[i + columns_ * 3 + 2] = T_RIGHT_RAMP_BTMSIDE_LTR3;
            data_[i + columns_ * 3 + 3] = T_RIGHT_RAMP_BTMSIDE_LTR4;
            break;
        }
        case T_DOWN_RAMP_MASK:
        {
            data_[i] = T_DOWN_RAMP_LEFTSIDE_TTB1;
            data_[i + columns_] = T_DOWN_RAMP_LEFTSIDE_TTB2;
            data_[i + columns_ * 2] = T_DOWN_RAMP_LEFTSIDE_TTB3;
            data_[i + columns_ * 3] = T_DOWN_RAMP_LEFTSIDE_TTB4;
            data_[i + 1] = T_DOWN_RAMP_CENTRAL_TTB1;
            data_[i + columns_ + 1] = T_DOWN_RAMP_CENTRAL_TTB2;
            data_[i + columns_ * 2 + 1] = T_DOWN_RAMP_CENTRAL_TTB3;
            data_[i + columns_ * 3 + 1] = T_DOWN_RAMP_CENTRAL_TTB4;
            data_[i + 2] = T_DOWN_RAMP_CENTRAL_TTB1;
            data_[i + columns_ + 2] = T_DOWN_RAMP_CENTRAL_TTB2;
            data_[i + columns_ * 2 + 2] = T_DOWN_RAMP_CENTRAL_TTB3;
            data_[i + columns_ * 3 + 2] = T_DOWN_RAMP_CENTRAL_TTB4;
            data_[i + 3] = T_DOWN_RAMP_RIGHTSIDE_TTB1;
            data_[i + columns_ + 3] = T_DOWN_RAMP_RIGHTSIDE_TTB2;
            data_[i + columns_ * 2 + 3] = T_DOWN_RAMP_RIGHTSIDE_TTB3;
            data_[i + columns_ * 3 + 3] = T_DOWN_RAMP_RIGHTSIDE_TTB4;
            break;
        }
        case T_LEFT_RAMP_MASK:
        {
            data_[i + 3] = T_LEFT_RAMP_TOPSIDE_RTL1;
            data_[i + 2] = T_LEFT_RAMP_TOPSIDE_RTL2;
            data_[i + 1] = T_LEFT_RAMP_TOPSIDE_RTL3;
            data_[i] = T_LEFT_RAMP_TOPSIDE_RTL4;
            data_[i + columns_ + 3] = T_LEFT_RAMP_CENTRAL_RTL1;
            data_[i + columns_ + 2] = T_LEFT_RAMP_CENTRAL_RTL2;
            data_[i + columns_ + 1] = T_LEFT_RAMP_CENTRAL_RTL3;
            data_[i + columns_] = T_LEFT_RAMP_CENTRAL_RTL4;
            data_[i + columns_ * 2 + 3] = T_LEFT_RAMP_CENTRAL_RTL1;
            data_[i + columns_ * 2 + 2] = T_LEFT_RAMP_CENTRAL_RTL2;
            data_[i + columns_ * 2 + 1] = T_LEFT_RAMP_CENTRAL_RTL3;
            data_[i + columns_ * 2] = T_LEFT_RAMP_CENTRAL_RTL4;
            data_[i + columns_ * 3 + 3] = T_LEFT_RAMP_BTMSIDE_RTL1;
            data_[i + columns_ * 3 + 2] = T_LEFT_RAMP_BTMSIDE_RTL2;
            data_[i + columns_ * 3 + 1] = T_LEFT_RAMP_BTMSIDE_RTL3;
            data_[i + columns_ * 3] = T_LEFT_RAMP_BTMSIDE_RTL4;
            break;
        }
        case T_UP_RAMP_MASK:
        {
            data_[i + columns_ * 3] = T_UP_RAMP_LEFTSIDE_BTT1;
            data_[i + columns_ * 2] = T_UP_RAMP_LEFTSIDE_BTT2;
            data_[i + columns_] = T_UP_RAMP_LEFTSIDE_BTT3;
            data_[i] = T_UP_RAMP_LEFTSIDE_BTT4;
            data_[i + columns_ * 3 + 1] = T_UP_RAMP_CENTRAL_BTT1;
            data_[i + columns_ * 2 + 1] = T_UP_RAMP_CENTRAL_BTT2;
            data_[i + columns_ + 1] = T_UP_RAMP_CENTRAL_BTT3;
            data_[i + 1] = T_UP_RAMP_CENTRAL_BTT4;
            data_[i + columns_ * 3 + 2] = T_UP_RAMP_CENTRAL_BTT1;
            data_[i + columns_ * 2 + 2] = T_UP_RAMP_CENTRAL_BTT2;
            data_[i + columns_ + 2] = T_UP_RAMP_CENTRAL_BTT3;
            data_[i + 2] = T_UP_RAMP_CENTRAL_BTT4;
            data_[i + columns_ * 3 + 3] = T_UP_RAMP_RIGHTSIDE_BTT1;
            data_[i + columns_ * 2 + 3] = T_UP_RAMP_RIGHTSIDE_BTT2;
            data_[i + columns_ + 3] = T_UP_RAMP_RIGHTSIDE_BTT3;
            data_[i + 3] = T_UP_RAMP_RIGHTSIDE_BTT4;
            break;
        }
    }
}
*/

/*
void Terrain::deleteRamp( int i,char rampType )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    int y = i / columns_;
    int x = i % columns_;

    if ( isRamp( data_[i] ) && (rampType == getMasterTerrainType( data_[i] ))
        ) assign( i,T_LOW );
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
*/

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
    for ( iLoop = 4; changed; iLoop += 2 )
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
                //if ( (srcT > T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
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
                //if ( (srcT != T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
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
                //if ( (srcT > T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
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
                //if ( (srcT != T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
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
                //if ( (srcT > T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
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
                //if ( (srcT != T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
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
                //if ( (srcT > T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
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
                //if ( (srcT != T_LOW) && isRamp( dstT ) ) deleteRamp( iDest,dstT );
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

void Terrain::fixTransitions( int x1,int y1,int x2,int y2 )
{
    assert( (x1 & 0x1) == 0 );
    assert( (x2 & 0x1) == 0 );
    assert( (y1 & 0x1) == 0 );
    assert( (y2 & 0x1) == 0 );
    if ( x1 < 0 ) x1 = 0;
    if ( y1 < 0 ) y1 = 0;
    if ( x2 > columns_ - 2 ) x2 = columns_ - 2;
    if ( y2 > rows_ - 2 ) y2 = rows_ - 2;
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
                            transTopLeft = !isHighCompatible( srcTerrain[-columns_ - 1] );
                        transTop = !isHighCompatible( srcTerrain[-columns_] );
                        if ( i < (columns_ - 2) )
                            transTopRight = !isHighCompatible( srcTerrain[-columns_ + 2] );
                    }
                    if ( i > 0 ) transLeft = !isHighCompatible( srcTerrain[-1] );
                    if ( i < (columns_ - 2) ) transRight = !isHighCompatible( srcTerrain[2] );

                    if ( j < (rows_ - 2) )
                    {
                        if ( i > 0 )
                            transBottomLeft = !isHighCompatible( srcTerrain[columns_ * 2 - 1] );
                        transBottom = !isHighCompatible( srcTerrain[columns_ * 2] );
                        if ( i < (columns_ - 2) )
                            transBottomRight = !isHighCompatible( srcTerrain[columns_ * 2 + 2] );
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
                                transTopLeft = !isLowWater( srcTerrain[-columns_ - 1] );
                            transTop = !isLowWater( srcTerrain[-columns_] );
                            if ( i < (columns_ - 2) )
                                transTopRight = !isLowWater( srcTerrain[-columns_ + 2] );
                        }
                        if ( i > 0 ) transLeft = !isLowWater( srcTerrain[-1] );
                        if ( i < (columns_ - 2) ) transRight = !isLowWater( srcTerrain[2] );

                        if ( j < (rows_ - 2) )
                        {
                            if ( i > 0 )
                                transBottomLeft = !isLowWater( srcTerrain[columns_ * 2 - 1] );
                            transBottom = !isLowWater( srcTerrain[columns_ * 2] );
                            if ( i < (columns_ - 2) )
                                transBottomRight = !isLowWater( srcTerrain[columns_ * 2 + 2] );
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
                (columns == curX      && rows == curY) ||
                (columns == (curX + 1) && rows == curY) ||
                (columns == curX      && rows == (curY + 1)) ||
                (columns == (curX + 1) && rows == (curY + 1)) )
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
                    /*
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
                            hStdout,FOREGROUND_WHITE | BACKGROUND_BROWN );
                        std::cout << '|';
                        break;
                    }
                    */
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
        if ( columns_ < 80 ) std::cout << std::endl;
    }
    SetConsoleTextAttribute( hStdout,FOREGROUND_LIGHTGRAY );
    std::cout << "a = LOW WATER, z = LOW GROUND, e = HIGH GROUND, r = HIGH WATER"
        << std::endl << "Navigate with o, k, l, m or 8,5,4,6";
}