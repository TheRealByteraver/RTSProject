#include <sstream>
#include <fstream> 
#include <algorithm>
//#include <iostream> 
#include <assert.h>
//#include <stdlib.h>
//#include <time.h>
//#include <windows.h>

#include "IniFile.h"


/*
    KeyPair class code:
*/


// transforms the string: "Crystals:50" into the string "Crystals" and the int 50
// There should be no whitespace present in the string or any other characters
int KeyPair::decode( std::string toDecode )
{
    name_.clear();
    isDecoded_ = false;
    value_ = 0;
    charsDecoded_ = 0;
    const char *c = toDecode.c_str();
    char *buf = new char[toDecode.length() + 1];
    for ( ;; )
    {
        if ( (*c == '\0') || (*c == KEYPAIR_SEPARATOR) || (*c == KEYPAIR_NEXT) )
            break;
        name_ += *c;
        charsDecoded_++;
        c++;
    }
    if ( *c == KEYPAIR_SEPARATOR )
    {
        c++;
        std::string valueStr;
        for ( ;; )
        {
            if ( (*c == '\0') ||
                (*c == KEYPAIR_SEPARATOR) ||
                (*c == KEYPAIR_NEXT) ||
                (!isdigit( *c ))
                )
                break;
            valueStr += *c;
            charsDecoded_++;
            c++;
        }
        if ( valueStr.length() > 0 )
        {
            value_ = 0;
            try {
                value_ = std::stoi( valueStr );
            }
            /*
            catch ( std::invalid_argument& e )
            {
            // if no conversion could be performed
            return -1;
            }
            catch ( std::out_of_range& e )
            {
            // if the converted value would fall out of the range of the result type
            // or if the underlying function (std::strtol or std::strtoull) sets errno
            // to ERANGE.
            return -1;
            }
            */
            catch ( ... )
            {
                // everything else
                charsDecoded_ = 0;
                return 0;
            }
        }
    }
    delete buf;
    if ( charsDecoded_ > 0 ) isDecoded_ = true;
    return charsDecoded_;
}

/*
    IniFile code:
*/
int IniFile::readFile( const std::string& filename )
{
    // keep track of the filename for debugging purposes:
    iniFilename_ = filename;
    // start with a clean empty string list
    stringList_.clear();
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
            else if ( *c == ']' ) bracketClosedFound = true;
            else if ( *c == '=' ) equalSignFound = true;
            else if ( *c == INIFILE_COMMENT_CHAR )
            {
                *c = '\0';
                break;
            }
        }
        if ( strlen( strBuf ) == 0 ) continue;
        if ( equalSignFound )
        {
            // ini file should start with a [section]
            if ( /* bracketOpenFound || bracketClosedFound || */
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
        stringList_.push_back( str );
        /*
        // debug:
        //std::cout << *(stringList.end() - 1);
        std::cout << stringList[stringList.size() - 1].c_str();
        std::cout << std::endl;
        */
    }
    rewind();
    iniFileLoaded_ = true;
    return 0;
}

int IniFile::getNextSection( std::string& section )
{
    for ( ; currentRow_ < (int)stringList_.size(); currentRow_++ )
    {
        if ( stringList_[currentRow_][0] == '[' ) break;
    }
    if ( currentRow_ < (int)stringList_.size() )
    {
        section.clear();
        for ( int i = 1; i < (int)stringList_[currentRow_].length() - 1; i++ )
            section += stringList_[currentRow_][i];
        //section = stringList[currentRow_];
        currentRow_++;
        return 0;
    } else return -1;
}

int IniFile::getNextKey( std::string& key )
{
    key.clear();
    if ( currentRow_ >= (int)stringList_.size() ) return -1;
    if ( stringList_[currentRow_][0] == '[' ) return -1;
    key = stringList_[currentRow_];
    currentRow_++;
    return 0;
}

int IniFile::getKeyValue( const std::string& section,const std::string& key,std::string& dest )
{
    std::string sectionStr = "[" + section + "]";
    std::string keyStr( key );
    // Transform the strings to upper case for correct comparison
    std::transform( sectionStr.begin(),sectionStr.end(),sectionStr.begin(),::toupper );
    std::transform( keyStr.begin(),keyStr.end(),keyStr.begin(),::toupper );
    for ( currentRow_ = 0; currentRow_ < (int)stringList_.size(); currentRow_++ )
    {
        if ( stringList_[currentRow_].compare( sectionStr ) != 0 ) continue;
        currentRow_++;
        for ( ; currentRow_ < (int)stringList_.size(); currentRow_++ )
        {
            if ( stringList_[currentRow_][0] == '[' ) break; // end of this section
            char strBuf[INIFILE_MAX_LINE_LENGTH];
            const char *src = stringList_[currentRow_].c_str();
            char *dst = strBuf;
            for ( ; (*src != '=') && (*src != '\0'); ) *dst++ = *src++;
            *dst = '\0';
            if ( strcmp( keyStr.c_str(),strBuf ) == 0 ) // found the key!
            {
                if ( *src == '=' ) dest.assign( src + 1 );
                else dest.clear();
                currentRow_++;
                return 0;
            }
        }
        break;
    }
    if ( writeDebugLogFile_ )
        *debugLogFile_  << "Couldn't find key \"" << key << "\" "
                        << "in section \"" << section << "\" " 
                        << " in .ini file " << iniFilename_ << "\n";
    return -1;
}

int IniFile::getKeyValue( const std::string& section,const std::string& key,int& value )
{
    std::string dest;
    int error = getKeyValue( section,key,dest );
    if ( error != 0 ) return -1;
    value = 0;
    try {
        value = std::stoi( dest );
    }
    /*
    catch ( std::invalid_argument& e ) 
    {
        // if no conversion could be performed
        return -1;
    }
    catch ( std::out_of_range& e ) 
    {
        // if the converted value would fall out of the range of the result type
        // or if the underlying function (std::strtol or std::strtoull) sets errno
        // to ERANGE.
        return -1;
    }
    */
    catch ( ... ) 
    {
        // everything else
        return -1;
    }
    return 0;
}

int IniFile::getKeyValue( const std::string& section,const std::string& key,bool& value )
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
    {
        if ( (*s != ' ') && (*s != '\t') ) *d++ = *s++;
        else s++;
    }
    *d = '\0';
     return buf;
}

int IniFile::explodeStringToKeyPairList( const std::string& sourceStr,std::vector<KeyPair>& destList )
{
    const char *str = sourceStr.c_str();
    int iterations = 0;
    for ( ;; )
    {
        KeyPair keyPair;
        str += keyPair.decode( std::string( str ) );
        if ( !keyPair.isDecoded() )
        {
            return -1;
        }
        destList.push_back( keyPair );
        iterations++;
        if ( std::string( str ).length() > 2 ) str += 2;
        else break;
    }
    if ( iterations > 0 ) return 0;
    else return -1;
}

int IniFile::explodeStringToStringList( const std::string& sourceStr,std::vector<std::string>& destList )
{
    const char *c = sourceStr.c_str();
    char *buf = new char[sourceStr.length() + 1];
    buf[0] = '\0';
    for ( int i = 0; *c != '\0'; c++ )
    {
        buf[i] = *c;
        i++;
        if ( *c == ',' )
        {
            buf[i - 1] = '\0';
            destList.push_back( std::string( buf ) );
            i = 0;
        } else if ( c[1] == '\0' )
        {
            buf[i] = '\0';
            destList.push_back( std::string( buf ) );
        }
    }
    delete buf;
    return 0;
}

