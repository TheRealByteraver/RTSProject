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
#include <windows.h>

/******************************************************************************
*                                                                             *
* Start of INI Reader Code                                                    *
*                                                                             *
******************************************************************************/
/*
MAX_LINE_LENGTH has an arbitrary high value, because the code will bug if
the length of the line found in the ini file exceeds this number
*/
#define INIFILE_MAX_LINE_LENGTH     255
#define INIFILE_COMMENT_CHAR        ';'

class IniFile
{
public:
    IniFile( const std::string& filename ) { readFile( filename ); }
    bool    isLoaded() const { return iniFileLoaded_; }
    // These functions return non zero on error
    int     getKeyValue( const std::string& section,const std::string& key,std::string& dest ) const;
    int     getKeyValue( const std::string& section,const std::string& key,int& value ) const;
    int     getKeyValue( const std::string& section,const std::string& key,bool& value ) const;
    int     getNextSection( std::string& section );
    int     getNextKey( std::string& section );
    int     rewind() { currentRow_ = 0; return (iniFileLoaded_ ? 0 : -1); }
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
        // put everything in upper case for easy comparing later on
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
        for ( int i = 1; i < (int)stringList[currentRow_].length() - 1; i++ )
            section += stringList[currentRow_][i];
        //section = stringList[currentRow_];
        currentRow_++;
        return 0;
    } else return -1;
}

int IniFile::getNextKey( std::string& section )
{
    section.clear();
    if ( currentRow_ >= (int)stringList.size() ) return -1;
    if ( stringList[currentRow_][0] == '[' ) return -1;
    section = stringList[currentRow_];
    currentRow_++;
    return 0;
}

int IniFile::getKeyValue( const std::string& section,const std::string& key,std::string& dest ) const
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

int IniFile::getKeyValue( const std::string& section,const std::string& key,int& value ) const
{
    std::string dest;
    int error = getKeyValue( section,key,dest );
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

int IniFile::getKeyValue( const std::string& section,const std::string& key,bool& value ) const
{
    value = false;
    std::string dest;
    int error = getKeyValue( section,key,dest );
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

