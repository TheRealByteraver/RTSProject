#include <iostream>
#include <fstream>
#include "Terrain.h"

extern class Defaults defaults;

/*
    We use our own integer-to-ascii conversion table for the possible terrain
    values as not all characters are safe for use in an ini file, such as:
    = ; [ ]
    It is 64 characters long, as we need the 0 .. 63 range:
    alphabet + decimals + special characters <=> 2 * 26 + 10 + 2 = 64 
    We can't use the alphabet twice because the ini file system is case 
    insensitive. The "!" characters are unused in the code.
    The '1' is a placeholder for Doodads, albeit only whilst playing, not
    whilst designing the terrain.
*/
const char *Terrain::encodestr_ = 
//  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ&@#'(^\"!{})-_*+§~/\\:.,?|%$<>";
    "01!!456789ABCDEFG!!!!!!!!!!!!!!!W!!!&@#'(^\"Z{})-_!!!~/\\:.,?|%$<>";
//   01        10        20        30        40         50         60

/*
    The .ini reader can only read rows up till 300 bytes long.
    80 columns 80 + 7 = 87 characters, so a map can be 256 chars wide.
    Terrain needs:
    - columns
    - rows
    - tileset
    - actual terrain, comma separated

[Characteristics]
name=wastelands
rows=40
columns=80
world=GreenPrairie

[TerrainData]
row000=8F,15,14,...
row001=8F,34,01,...

*/

int Terrain::loadTerrain( std::string filename )
{
    IniFile iniFile( filename );
    if ( !iniFile.isLoaded() ) return -1;
    int error = 0;
    error += iniFile.getKeyValue( "Characteristics","name",name_ );
    error += iniFile.getKeyValue( "Characteristics","columns",columns_ );
    error += iniFile.getKeyValue( "Characteristics","rows",rows_ );
    error += iniFile.getKeyValue( "Characteristics","world",world_ );
    if ( error != 0 ) return -1;
    // load with default data:
    init( columns_,rows_ );
    // now load the actual data:
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
            for ( i = 0; i < (int)strlen( encodestr_ ); i++ )
            {
                if ( value == encodestr_[i] ) break;
            }
            if ( value != encodestr_[i] ) return -1;
            data_[rowNr * columns_ + columnNr] = (Tile)i;
        }
    }
    // now load the doodad's:
    int doodadNr = 0;
    for ( ;; )
    {
        std::string doodadNrSection( "doodad" );
        if ( doodadNr < 100 ) doodadNrSection += "0";
        if ( doodadNr < 10 ) doodadNrSection += "0";
        doodadNrSection += std::to_string( doodadNr );
        DoodadLocation doodadLocation;
        int error = 0;
        error += iniFile.getKeyValue( doodadNrSection,"LocationX",doodadLocation.x );
        error += iniFile.getKeyValue( doodadNrSection,"LocationY",doodadLocation.y );
        error += iniFile.getKeyValue( doodadNrSection,"doodadNr",doodadLocation.doodadNr );
        if ( error != 0 ) break; // no more doodad's left or illegal doodad found
        if ( (doodadLocation.x < 0) || (doodadLocation.y < 0) ||
            (doodadLocation.x >= columns_) || (doodadLocation.y >= rows_)
            )
        {// error in terrain file
            defaults.debugLogFile << "Error reading doodad nr " << doodadNr
                << " location information in terrain file " << filename << std::endl;
            continue; // don't load this terrain doodad
        }
        doodadLocation.isUsed = true;
        doodadList_.push_back( doodadLocation );
        doodadNr++;
    }   
    return 0;
}

int Terrain::saveTerrain( std::string filename ) const
{
    std::ofstream terrainFile( filename.c_str() );
    if ( !terrainFile.is_open() ) return -1;
    terrainFile << "; terrain saved by editor" << std::endl;
    terrainFile << std::endl;
    terrainFile << "[Characteristics]" << std::endl;
    terrainFile << "name=" << name_ << std::endl;
    terrainFile << "rows=" << rows_ << std::endl;
    terrainFile << "columns=" << columns_ << std::endl;
    terrainFile << "world=" << world_ << std::endl;     
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
            assert( (value < (int)strlen( encodestr_ ) ) && (value >= 0) );
            terrainFile << encodestr_[value];
        }
        terrainFile << std::endl;
    }
    // save the doodad's:
    terrainFile 
        << std::endl
        << "; Now follows a list of doodad's that are present on the terrain:"
        << std::endl;
    int doodadNr = 0;
    for ( int index = 0; index < doodadList_.size(); index++ )
    {
        if ( !doodadList_[index].isUsed ) continue;
        std::string doodadNrSection( "[Doodad" );
        if ( doodadNr < 100 ) doodadNrSection += "0";
        if ( doodadNr < 10 ) doodadNrSection += "0";
        doodadNrSection += std::to_string( doodadNr ) + "]";
        terrainFile << doodadNrSection << std::endl;
        const DoodadLocation& doodadLocation = doodadList_[index];        
        terrainFile << "LocationX=" << doodadLocation.x << std::endl;
        terrainFile << "LocationY=" << doodadLocation.y << std::endl;
        terrainFile << "doodadNr=" << doodadLocation.doodadNr << std::endl;
        terrainFile << std::endl;
        doodadNr++;
    }
    terrainFile.close();
    return 0;
}

void Terrain::assign( int i,Tile terrain )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( isBasicTerrain( terrain ) );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    Tile *t = data_ + i;
    *t = terrain;
    t++;
    *t = terrain;
    t += columns_;
    *t = terrain;
    t--;
    *t = terrain;
}

Rect Terrain::drawTerrain( int i,Tile terrain )
{
    assert( i < (size_ - columns_ - 1) );
    assert( i >= 0 );
    assert( data_ != NULL );
    assert( (i & 0x1) == 0 ); // i must be even and start on an even row
    assert( ((i / columns_) & 0x1) == 0 );
    assert( isBasicTerrain( terrain ) /* || isRamp( terrain ) */ );
    assert( cursorWidth_ >= 1 );
    int x = i % columns_;
    int y = i / columns_;
    /*
       we go around the cursor location clockwise to draw the new terrain
       until we reach the thickness of the cursor. A cursor width of 1
       means we'll draw 1x1 of 2x2 terrain squares. A cursor width of 2
       means we'll draw 3x3 of 2x2 terrain squares. A cursor width of 3
       means we'll draw 5x5 of 2x2 terrain squares. And so on.
       right side, going down
    */
    assign( i,terrain );
    for ( int iLoop = 2; iLoop < cursorWidth_ * 2; iLoop += 2 )
    {
        bool checkTopRightCorner = false;
        bool checkBottomRightCorner = false;
        bool checkBottomLeftCorner = false;
        bool checkTopLeftCorner = false;
        if ( x + iLoop < columns_ - 1 )
        {
            int xStart = x + iLoop;
            int yStart = y - iLoop + 2;
            int yEnd = y + iLoop - 2;
            if ( yStart < 0 ) yStart = 0;
            if ( yStart > 1 )  checkTopRightCorner = true;
            if ( yEnd > rows_ - 2 ) yEnd = rows_ - 2;
            int iSource = yStart * columns_ + xStart;
            if ( checkTopRightCorner ) assign( iSource - columns_ * 2,terrain );
            for ( int j = yStart; j <= yEnd; j += 2 )
            {
                assign( iSource,terrain );
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
            if ( checkBottomRightCorner ) assign( iSource + columns_ * 2 + 2,terrain );
            for ( int j = xStart; j >= xEnd; j -= 2 )
            {
                assign( iSource + columns_ * 2,terrain );
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
            int iSource = yStart * columns_ + xStart;
            if ( checkBottomLeftCorner ) assign( iSource + columns_ * 2,terrain );
            for ( int j = yStart; j >= yEnd; j -= 2 )
            {
                assign( iSource,terrain );
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
            if ( checkTopLeftCorner ) assign( iSource - columns_ * 2 - 2,terrain );
            for ( int j = xStart; j <= xEnd; j += 2 )
            {
                assign( iSource - columns_ * 2,terrain );
                iSource += 2;
            }
        }
    }
    return adapt( i );
    
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
Rect Terrain::adapt( int i )
{
    int y = i / columns_;
    int x = i % columns_;
    int iLoop;
    bool changed = true;
    for ( iLoop = cursorWidth_ * 2; changed; iLoop += 2 )    
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
    if ( x1 < 0 ) x1 = 0;
    if ( y1 < 0 ) y1 = 0;
    if ( x2 > columns_ - 2 ) x2 = columns_ - 2;
    if ( y2 > rows_ - 2 ) y2 = rows_ - 2;
    fixTransitions( x1,y1,x2,y2 );
    return Rect( x1,y1,x2 + 2,y2 + 2 );
}

void Terrain::fixTransitions( int x1,int y1,int x2,int y2 )
{
    assert( (x1 & 0x1) == 0 );
    assert( (x2 & 0x1) == 0 );
    assert( (y1 & 0x1) == 0 );
    assert( (y2 & 0x1) == 0 );
    // range checking moved to adapt function
    assert( x1 >= 0 );
    assert( x2 >= 0 );
    assert( y1 >= 0 );
    assert( y1 >= 0 );
    assert( x1 <= columns_ - 2 );
    assert( x2 <= columns_ - 2 );
    assert( y1 <= rows_ - 2 );
    assert( y2 <= rows_ - 2 );
    for ( int j = y1; j <= y2; j += 2 )
    {
        for ( int i = x1; i <= x2; i += 2 )
        {
            Tile *srcTerrain = data_ + j * columns_ + i;
            if ( isLow( *srcTerrain ) )
            {
                // No transitions inside T_LOW
                srcTerrain += 2;
                continue;
            } else {
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
                } else {
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
                    } else {
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
        }
    }
}


void Terrain::show( int curX,int curY ) const
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
    Tile *i = data_;

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