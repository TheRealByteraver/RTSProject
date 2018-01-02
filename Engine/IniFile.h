﻿#pragma once

#include <vector>
#include <string>

/*
MAX_LINE_LENGTH has an arbitrary high value, because the code will bug if
the length of the line found in the ini file exceeds this number
*/
#define INIFILE_MAX_LINE_LENGTH     300
#define INIFILE_COMMENT_CHAR        ';'

/*
A small pair value class for the more complex keys:
*/

#define KEYPAIR_SEPARATOR   ':'
#define KEYPAIR_NEXT        ','
class KeyPair
{
public:
    KeyPair() {}
    KeyPair( std::string toDecode ) { decode( toDecode ); }
    const std::string&  getName() const { assert( isDecoded_ ); return name_; }
    int                 getValue() const { assert( isDecoded_ ); return value_; }
    bool                isDecoded() const { return isDecoded_; }
    int                 decode( std::string toDecode );
    int                 charsDecoded() const { return charsDecoded_; }
private:
    bool                isDecoded_ = false;
    std::string         name_;
    int                 value_ = 0;
    int                 charsDecoded_ = 0;
};

class IniFile
{
public:
    IniFile( const std::string& filename ) { readFile( filename ); }
    IniFile( const std::string& filename,std::ofstream& debugLogFile ) 
    {        
        if ( debugLogFile.is_open() )
        {
            debugLogFile_ = &debugLogFile;
            writeDebugLogFile_ = true;
        }
        readFile( filename ); 
    }
    void    setDebugLog( std::ofstream& debugLogFile )
    {
        if ( debugLogFile.is_open() )
        {
            debugLogFile_ = &debugLogFile;
            writeDebugLogFile_ = true;
        }
    }
    bool    isLoaded() const { return iniFileLoaded_; }
    // These functions return non zero on error
    int     getKeyValue( const std::string& section,const std::string& key,std::string& dest );
    int     getKeyValue( const std::string& section,const std::string& key,int& value );
    int     getKeyValue( const std::string& section,const std::string& key,bool& value );
    int     explodeStringToKeyPairList( const std::string& sourceStr,std::vector<KeyPair>& destList );
    int     explodeStringToStringList( const std::string& sourceStr,std::vector<std::string>& destList );
    int     getNextSection( std::string& section );
    int     getNextKey( std::string& key );
    int     rewind() { currentRow_ = 0; return (iniFileLoaded_ ? 0 : -1); }
private:
    std::vector<std::string>    stringList_;
    std::ofstream*              debugLogFile_;
    std::string                 iniFilename_;   // only used for debugging
    bool                        writeDebugLogFile_ = false;
    bool                        iniFileLoaded_ = false;
    int                         currentRow_ = 0;
private:
    int     readFile( const std::string& filename );
    char    *deleteWhiteSpace( char *buf ) const;
};
