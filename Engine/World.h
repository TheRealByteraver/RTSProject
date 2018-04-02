#pragma once

#include "Globals.h"
#include "assert.h"
#include "Graphics.h"
#include "Sprite.h"
#include "CreateDefaultSprites.h"
#include "Terrain.h"
#include "IniFile.h"

extern class Defaults defaults;

#define MAX_DOODADS                     1000      // per World
#define DOODAD_MAX_WIDTH                16        // in tiles, not higher than 99 ;)
#define DOODAD_MAX_HEIGHT               16        // in tiles, not higher than 99 ;)
#define DOODAD_NOT_USED                 '1'       // to be removed?
#define DOODAD_SECTION_TITLE            "Doodad"

class Doodad
{
public:
    Doodad()
    {
        avgColors_ = nullptr;
        avgColors_2x2_ = nullptr;
        //height_ = 0;
        for ( int i = 0; i < DOODAD_MAX_WIDTH * DOODAD_MAX_HEIGHT; i++ )
        {
            walkAbleMask_[i] = true;
            terrainMask_[i] = DOODAD_NOT_USED;  // ?
        }
    }
    ~Doodad()
    {
        if ( avgColors_ != nullptr ) delete avgColors_;
        if ( avgColors_2x2_ != nullptr ) delete avgColors_2x2_;        
    }
    Doodad( const Doodad &source )  // copy constructor
    {
        if ( this != &source )
        {
            name_ = source.name_;
            width_ = source.width_;
            height_ = source.height_;
            image_ = source.image_;
            int nrOfTiles = width_ * height_;
            avgColors_ = new Color[nrOfTiles];
            avgColors_2x2_ = new Color[nrOfTiles * 2 * 2];
            for ( int i = 0; i < nrOfTiles; i++ )
                avgColors_[i] = source.avgColors_[i];
            for ( int i = 0; i < nrOfTiles * 2 * 2; i++ )
                avgColors_2x2_[i] = source.avgColors_2x2_[i];
            for ( int i = 0; i < DOODAD_MAX_WIDTH * DOODAD_MAX_HEIGHT; i++ )
            {
                walkAbleMask_[i] = source.walkAbleMask_[i];
                terrainMask_[i] = source.terrainMask_[i];
            }
        }
    }
    int     width() const { return width_; }
    int     height() const { return height_; }
    const std::string& name() const { return name_; }
    const Sprite& image() const { return image_; }
    bool    isCompatible( int x,int y,Tile tile ) const
    {
        assert( x >= 0 );
        assert( y >= 0 );
        assert( x < DOODAD_MAX_WIDTH );
        assert( y < DOODAD_MAX_HEIGHT );
        //defaults.debugLogFile << "terrain = " << tile << ", mask = " << terrainMask_[y * width_ + x] << std::endl;
        return ( terrainMask_[y * width_ + x] == tile );
    }
    bool    isWalkable( int x,int y ) const
    {
        assert( x >= 0 );
        assert( y >= 0 );
        assert( x < DOODAD_MAX_WIDTH );
        assert( y < DOODAD_MAX_HEIGHT );
        return walkAbleMask_[y * width_ + x];
    }
    int     load( IniFile& iniFile,int doodadNr )
    {
        assert( doodadNr >= 0 );
        assert( doodadNr < MAX_DOODADS );
        if ( !iniFile.isLoaded() )
        {
            defaults.debugLogFile << "World .ini file not correctly initialized"
                << ", halting loading doodad nr " << doodadNr << "." 
                << std::endl;
            return -1;
        }
        int tileWidth,tileHeight;
        int error = iniFile.getKeyValue( "Tiles","Width",tileWidth );
        error += iniFile.getKeyValue( "Tiles","Height",tileHeight );
        if ( error != 0 )
        {
            defaults.debugLogFile << "Error reading basic tile width & height from "
                << iniFile.getFilename()
                << ", halting loading this doodad." << std::endl;
            return error;
        }
        std::string sectionStr( DOODAD_SECTION_TITLE );
        if ( doodadNr < 100 ) sectionStr += '0';
        if ( doodadNr < 10 ) sectionStr += '0';
        sectionStr += std::to_string( doodadNr );
        std::string spriteFilename;
        int spriteX,spriteY;
        error = iniFile.getKeyValue( sectionStr,"name",name_ );
        error += iniFile.getKeyValue( sectionStr,"SourceFile",spriteFilename );
        error += iniFile.getKeyValue( sectionStr,"locationX",spriteX );
        error += iniFile.getKeyValue( sectionStr,"locationY",spriteY );
        error += iniFile.getKeyValue( sectionStr,"horSize",width_ );
        error += iniFile.getKeyValue( sectionStr,"VerSize",height_ );
        if ( (error != 0) || (spriteFilename.length() == 0) ||
            (spriteX < 0) || (spriteY < 0) ||
            (width_ < 1) || (height_ < 1) ||
            (width_ > DOODAD_MAX_WIDTH) || (height_ > DOODAD_MAX_HEIGHT) ) 
        {
            defaults.debugLogFile << "Error whilst decoding the description of Doodad nr "
                << doodadNr << " in " << iniFile.getFilename() 
                << ", halting loading this doodad." << std::endl;
            return error;
        }
        std::string spriteFilePath( GAME_FOLDER );
        spriteFilePath += defaults.worldsFolder() + '\\' + spriteFilename;
        std::string walkableStr( "WalkableMask" );
        std::string terrainStr( "TerrainMask" );
        int index = 0;
        for ( int j = 0; j < height_; j++ )
        {
            std::string rowStr;
            if ( j < 10 ) rowStr += '0';
            rowStr += std::to_string( j );
            std::string walkableMaskValue;
            std::string terrainMaskValue;
            iniFile.getKeyValue( sectionStr,walkableStr + rowStr,walkableMaskValue );
            iniFile.getKeyValue( sectionStr,terrainStr + rowStr,terrainMaskValue );
            if( (walkableMaskValue.length() != width_) ||
                (terrainMaskValue.length() != width_) )
            {
                defaults.debugLogFile << "Error whilst decoding the description of Doodad nr "
                    << doodadNr << " in " << iniFile.getFilename() 
                    << ": illegal length of one of the masks"
                    << ", halting loading this doodad." << std::endl;
                return -1;
            }
            //const char *encodestr = Terrain().encodeStr(); // to decode the terrain mask
            for ( int i = 0; i < width_; i++ )
            {
                walkAbleMask_[index] = (walkableMaskValue[i] == '1');
                // convert encoded terrain mask to in-program values:
                Tile element = Terrain().decode( terrainMaskValue[i] );
                if ( (element < T_BASIC_TERRAIN_LOWER_LIMIT) || 
                     (element > T_BASIC_TERRAIN_UPPER_LIMIT) )
                {
                    defaults.debugLogFile << "Error whilst decoding the terrain mask of Doodad nr "
                        << doodadNr << " in " << iniFile.getFilename() 
                        << ": illegal character on row " << rowStr << ", column " << i + 1
                        << ", halting loading this doodad." << std::endl;
                    return -1;
                }
                terrainMask_[index] = element;
                index++;
            }
        }
        // now load the bitmap of the Doodad:
        Rect    bmpSection(
            spriteX,
            spriteY,
            spriteX + width_  * tileWidth - 1,
            spriteY + height_ * tileHeight - 1 
        );
        error = image_.loadFromBMP(
            spriteFilePath.c_str(),
            bmpSection
        );
        if ( (error != 0) || 
            (bmpSection.width() != image_.getWidth()) ||
            (bmpSection.height() != image_.getHeight()) )
        {
            defaults.debugLogFile << "Error whilst loading the sprite of Doodad nr "
                << doodadNr << " in " << iniFile.getFilename()
                << ", halting loading this doodad." << std::endl;
            return -1;
        }

        // to test:

        // Now create tiny versions of the doodad for the minimap:
        int nrOfTiles = width_ * height_;
        avgColors_ = new Color[nrOfTiles];
        avgColors_2x2_ = new Color[nrOfTiles * 2 * 2];
        Color *data = image_.getPixelData();

        int halfWidth = (tileWidth / 2);
        int halfHeight = (tileHeight / 2);
        int tileSize = tileWidth * tileHeight;
        int nrPixels = halfWidth * halfHeight;
        index = 0;
        int tileNr = 0;
        for ( int nrTileRows = 0; nrTileRows < height_; nrTileRows++ )
        {
            for ( int nrTileColumns = 0; nrTileColumns < width_; nrTileColumns++ )
            {
                int R = 0;
                int G = 0;
                int B = 0;
                for ( int y = 0; y < tileHeight; y += halfHeight )
                {
                    for ( int x = 0; x < tileWidth; x += halfWidth )
                    {
                        int r = 0;
                        int g = 0;
                        int b = 0;
                        for ( int j = 0; j < halfHeight; j++ )
                            for ( int i = 0; i < halfWidth; i++ )
                            {
                                //Color c = data[(y + j) * tileHeight + x + i];
                                Color c = data[
                                    (nrTileRows * tileHeight + y + j) * image_.getWidth() 
                                    + nrTileColumns * tileWidth + x + i
                                ];
                                r += c.GetR();
                                g += c.GetG();
                                b += c.GetB();
                            }
                        r /= nrPixels;
                        g /= nrPixels;
                        b /= nrPixels;
                        avgColors_2x2_[index] = Color( r,g,b );
                        R += r;
                        G += g;
                        B += b;
                        index++;
                    }
                }
                R /= 4;
                G /= 4;
                B /= 4;
                avgColors_[tileNr] = Color( R,G,B );
                tileNr++;
            }
        }
        return 0;
    }
    Color   getAvgColor( int i,int j ) const
    {
        assert( i >= 0 );
        assert( j >= 0 );
        assert( (j * width_ + i) < width_ * height_ );
        return getAvgColor( j * width_ + i );
    }
    Color   getAvgColor( int i ) const
    {
        assert( avgColors_ != nullptr );
        assert( i >= 0 );
        assert( i < width_ * height_ );
        return avgColors_[i];
    }
    /*
    Color   getAvgColor2x2( int i,int j ) const
    {
        assert( i >= 0 );
        assert( j >= 0 );
        assert( (j * width_ * 2 + i) < width_ * height_ * 2 * 2 );
        return getAvgColor2x2( j * width_ * 2 + i );
    }
    */
    Color   getAvgColor2x2( int i ) const
    {
        assert( avgColors_2x2_ != nullptr );
        assert( i >= 0 );
        assert( i < width_ * height_ * 2 * 2 );
        return avgColors_2x2_[i];
    }
private:
    std::string     name_;
    int             width_ = 0;   // in tiles
    int             height_ = 0;  // in tiles
    Sprite          image_;
    bool            walkAbleMask_[DOODAD_MAX_WIDTH * DOODAD_MAX_HEIGHT];
    Tile            terrainMask_[DOODAD_MAX_WIDTH * DOODAD_MAX_HEIGHT];
    Color          *avgColors_ = nullptr;
    Color          *avgColors_2x2_ = nullptr;
};

class World
{
public:
    World() {}
    //World( const World &source )  // copy constructor
    World& operator=( const World& source )
    {
        if ( this != &source )
        {
            worldName_ = source.worldName_;
            tileWidth_ = source.tileWidth_;     
            tileHeight_ = source.tileHeight_; 
            int t2 = 0;
            for ( int t = 0; t < NR_OF_TILES; t++ )
            {
                tileLibrary_[t] = source.tileLibrary_[t];
                AvgColors_[t] = source.AvgColors_[t];
                AvgColors_2x2[t2] = source.AvgColors_2x2[t2]; t2++;
                AvgColors_2x2[t2] = source.AvgColors_2x2[t2]; t2++;
                AvgColors_2x2[t2] = source.AvgColors_2x2[t2]; t2++;
                AvgColors_2x2[t2] = source.AvgColors_2x2[t2]; t2++;
            }
            doodads_.clear();
            for ( int d = 0; d < (int)source.doodads_.size(); d++ )
                doodads_.push_back( source.doodads_[d] );
        }
        return *this;
    }
    int             load( const std::string& worldName )
    {
        if ( worldName.length() == 0 )
        {
            defaults.debugLogFile << "Error: world.load() function called "
                << "without parameter, stopping loading this world." << std::endl;
            return -1;
        }
        worldName_ = worldName;
        int error = loadTiles();
        if ( error != 0 )
        {
            defaults.debugLogFile << "Error loading standard tiles from world " 
                << worldName_ << ", stopping loading this world." << std::endl;
            return error;
        }
        error = loadDoodads();
        if ( error != 0 )
        {
            defaults.debugLogFile << "Error loading doodads from world "
                << worldName_ 
                << ", not all doodads might be loaded for this world."
                << std::endl;
        }
        return error;
    }
    const Sprite&   getTile( const int tileNr ) const
    {
        assert( tileNr >= 0 );
        assert( tileNr < NR_OF_TILES );
        return tileLibrary_[tileNr];
    }
    const Doodad&   getDoodad( const int doodadNr ) const
    {
        assert( doodadNr >= 0 );
        assert( doodadNr < (int)doodads_.size() );
        return doodads_[doodadNr];
    }
    Color           getAvgColor( const int tileNr ) const 
    {
        assert( tileNr >= 0 );
        assert( tileNr < NR_OF_TILES );
        return AvgColors_[tileNr];
    }
    const Color*    getAvgColors_2x2( int tileNr ) const
    {
        assert( tileNr >= 0 );
        assert( tileNr < NR_OF_TILES );
        return &(AvgColors_2x2[tileNr * 4]);
    }
    int             tileWidth() const { return tileWidth_; }
    int             tileHeight() const { return tileHeight_; }
    int             nrOfDoodads() const { return (int)doodads_.size(); }
    const std::string& name() { return worldName_; }
private:
    /*
    This function takes the world name as a parameter as defined in any
    terrain .ini file. This should then be the filename without the .ini
    or the .bmp extension.
    Returns non zero on error.
    It assumes the bmp and .ini files are located in the "Worlds" folder
    as defined in rtsproject.ini.
    */
    int             loadTiles()
    {
        // first load the world .ini file:
        std::string path( GAME_FOLDER );
        path.append( defaults.worldsFolder() );
        path.append( "\\" );
        //path.append( worldName_ );
        IniFile defWorldIni( path + worldName_ + ".ini",defaults.debugLogFile );
        if ( !defWorldIni.isLoaded() ) return -1;
        int separator,offset,nrOfTiles,error = 0;
        std::string bmpSourceFileName;
        error += defWorldIni.getKeyValue( "Tiles","Width",tileWidth_ );
        error += defWorldIni.getKeyValue( "Tiles","Height",tileHeight_ );
        error += defWorldIni.getKeyValue( "Tiles","Separator",separator );
        error += defWorldIni.getKeyValue( "Tiles","Offset",offset );
        error += defWorldIni.getKeyValue( "Tiles","NrOfTiles",nrOfTiles );
        error += defWorldIni.getKeyValue( "Main","SourceFile",bmpSourceFileName );
        if ( error != 0 ) return -1;
        // and now the tile library bitmap:
        //Sprite spriteLib( path + ".bmp" );
        Sprite spriteLib( path + bmpSourceFileName );
        int libWidth = spriteLib.getWidth();
        int libHeight = spriteLib.getHeight();
        if ( nrOfTiles != NR_OF_TILES ) return -1;
        if ( (tileWidth_ <= 0) || (tileHeight_ <= 0) ) return -1;
        // check if sprite library is big enough to contain all tiles:
        int tileWidth = tileWidth_ + separator;
        int tileHeight = tileHeight_ + separator;
        int nrColumns = (libWidth - offset) / tileWidth;
        if ( nrColumns <= 0 ) return -1;
        int minNrRows = nrOfTiles / nrColumns;
        if ( nrOfTiles % nrColumns != 0 ) minNrRows++;
        if ( (libWidth < (offset + tileWidth * nrColumns)) ||
            (libHeight < (offset + tileHeight * minNrRows)) ) return -1;
        // now load the used tiles into separate bitmap sprites:
        //const char *encodestr = Terrain().encodeStr();
        for ( int tileNr = 0; tileNr < NR_OF_TILES; tileNr++ )
        {
            //if ( encodestr[tileNr] != '!' )
            //{
            int x = offset + (tileNr % nrColumns) * tileWidth;
            int y = offset + (tileNr / nrColumns) * tileHeight;
            Rect area(
                x,
                y,
                x + tileWidth - separator - 1,
                y + tileHeight - separator - 1 );
            tileLibrary_[tileNr].createFromSprite( spriteLib,area );
            //}
        }
        // now calculate the average color of each tile for the mini map:
        // one quarter of a tile at a time:
        int halfWidth = (tileWidth_ / 2);
        int halfHeight = (tileHeight_ / 2);
        int nrPixels = halfWidth * halfHeight;
        int index = 0;
        for ( int tileNr = 0; tileNr < nrOfTiles; tileNr++ )
        {
            Color *data = tileLibrary_[tileNr].getPixelData();
            int R = 0;
            int G = 0;
            int B = 0;
            for ( int y = 0; y < tileHeight_; y += halfHeight )
            {
                for ( int x = 0; x < tileWidth_; x += halfWidth )
                {
                    int r = 0;
                    int g = 0;
                    int b = 0;    
                    for ( int j = 0; j < halfHeight; j++ )
                        for ( int i = 0; i < halfWidth; i++ )
                        {
                            Color c = data[(y + j) * tileWidth_ + x + i];
                            r += c.GetR();
                            g += c.GetG();
                            b += c.GetB();
                        }
                    r /= nrPixels;
                    g /= nrPixels;
                    b /= nrPixels;
                    AvgColors_2x2[index] = Color( r,g,b );
                    R += r;
                    G += g;
                    B += b;
                    index++;
                }
            }
            R /= 4;
            G /= 4;
            B /= 4;
            AvgColors_[tileNr] = Color( R,G,B );
        }
        return 0;
    }
    int             loadDoodads()
    {
        assert( worldName_.length() != 0 );
        // first load the world's .ini file:
        std::string path( GAME_FOLDER );
        path.append( defaults.worldsFolder() );
        path.append( "\\" );
        path.append( worldName_ );
        IniFile doodadIni( path + ".ini",defaults.debugLogFile );
        if ( !doodadIni.isLoaded() ) return -1;
        int error = 0;
        doodads_.clear();
        for ( int doodadNr = 0; error == 0; doodadNr++ )
        {
            Doodad doodad;
            error = doodad.load( doodadIni,doodadNr );
            if ( error == 0 ) doodads_.push_back( doodad );
        }
        return 0;
    }
private:
    std::string     worldName_;
    Sprite          tileLibrary_[NR_OF_TILES];
    int             tileWidth_;                 // in pixels    
    int             tileHeight_;                // in pixels
    Color           AvgColors_[NR_OF_TILES];    // remove? // doodads?
    Color           AvgColors_2x2[NR_OF_TILES * 2 * 2];// remove?
    std::vector <Doodad> doodads_;
};



