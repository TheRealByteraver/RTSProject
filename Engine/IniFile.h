#pragma once

#include <vector>
#include <string>

/*
MAX_LINE_LENGTH has an arbitrary high value, because the code will bug if
the length of the line found in the ini file exceeds this number
*/
#define INIFILE_MAX_LINE_LENGTH     300
#define INIFILE_COMMENT_CHAR        ';'

class IniFile
{
public:
    IniFile( const std::string& filename ) { readFile( filename ); }
    bool    isLoaded() const { return iniFileLoaded_; }
    // These functions return non zero on error
    int     getKeyValue( const std::string& section,const std::string& key,std::string& dest );
    int     getKeyValue( const std::string& section,const std::string& key,int& value );
    int     getKeyValue( const std::string& section,const std::string& key,bool& value );
    int     getNextSection( std::string& section );
    int     getNextKey( std::string& key );
    int     rewind() { currentRow_ = 0; return (iniFileLoaded_ ? 0 : -1); }
private:
    std::vector<std::string>  stringList;
private:
    bool    iniFileLoaded_ = false;
    int     currentRow_ = 0;
    int     readFile( const std::string& filename );
    char    *deleteWhiteSpace( char *buf ) const;
};
