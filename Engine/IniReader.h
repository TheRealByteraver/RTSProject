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

//#define DEBUG_MODE

/*
*******************************************************************************
*                                                                             *
* Start of INI Reader Code                                                    *
*                                                                             *
*******************************************************************************
*/
#define INIFILE_FOLDER      "C:\\RTSMedia\\"
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
    IniFile() { stringList.clear(); }
    IniFile( char *filename ) { readFile( filename ); }
    // These functions return non zero on error
    int     readFile( char *filename );      
    int     getKey( const char *section,const char *key,std::string& dest );
    int     getKeyInt( const char *section,const char *key,int& value );
    int     getKeyStatus( const char *section,const char *key,bool& value );
private:
    std::vector<std::string>  stringList;
    char    *deleteWhiteSpace( char *buf );
};

int IniFile::readFile( char * filename )
{
    // Open the ini file
    std::ifstream iniFile;
    iniFile.open( filename );
    if ( !iniFile.is_open() ) return -1;
    // read the whole file into memory
    std::stringstream rawData;
    rawData << iniFile.rdbuf();
    iniFile.close();
    // start with a clean empty string list
    stringList.clear();
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
    return 0;
}

int IniFile::getKey( const char *section,const char *key,std::string& dest )   // to be tested
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

int IniFile::getKeyInt( const char *section,const char *key,int& value )
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

int IniFile::getKeyStatus( const char *section,const char *key,bool& value )
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

char *IniFile::deleteWhiteSpace( char *buf )
{
    char *d = buf;
    char *s = buf;
    for ( ; *s != NULL; )
        if ( (*s != ' ') && (*s != '\t') ) *d++ = *s++;
        else s++;
        *d = '\0';
        return buf;
}

