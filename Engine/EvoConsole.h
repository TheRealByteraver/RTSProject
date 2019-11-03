
#pragma once

#include <string>
#include <vector>
#include <assert.h>

/*
    Notes: uses no new / delete nor unique_ptr / make_unique

    This class simulates a console. It has no graphical functions but
    you can get every line (i.e. a row) from it to display the console 
    anyway you wish in your program. 

    Initialization:

    EvoConsole  console( 80,25 );

    -> This will create a 80 column, 25 row console.

    Write text to the console:

    print( std::string("\nHello world!\n") );

    -> This will go to a new line, print "Hello world" and go to a new 
       line again

    Get all the console's contents:

    for( int row = 0; row < console.getNrRows(); row++ ) {
       std::string fromConsoleStr = console.getTextRow( row );
       // print fromConsoleStr here
    }
*/

class EvoConsole {
public:
    EvoConsole( int nrColumns,int nrRows ) :
        columns_( nrColumns ),
        rows_( nrRows )
    {
        assert( rows_ > 0 );
        assert( columns_ > 0 );
        textRows_.resize( rows_ );
        for ( std::string& row : textRows_ ) 
            row.resize( columns_,'\0' );
    }
    void    Print( const std::string str )
    {
        if ( str.length() == 0 )
            return;
        /*
           We only scroll up when we start printing beyond the last character
           of the row. This way the console will not scroll up the whole
           screen when we print the bottom right character :)
        */
        int currentRowIndex = getCurrentRow();
        for ( const char ch : str ) {
            bool newLineChar = (ch == '\n');
            if ( newLineChar || (xCursor >= columns_) ) {
                textRows_[currentRowIndex][xCursor] = '\0';
                xCursor = 0;
                yCursor++;
                if ( yCursor >= rows_ )
                    ScrollUp();
                currentRowIndex = getCurrentRow();
            }
            if ( !newLineChar ) {
                textRows_[currentRowIndex][xCursor] = ch;
                xCursor++;
                //assert( textRows_[currentRowIndex].length() <= columns_ ); // DEBUG
            }
        }
        textRows_[currentRowIndex][xCursor] = '\0';
    }
    int     getNrColumns() const
    {
        return columns_;
    }
    int     getNrRows() const
    {
        return rows_;
    }
    const std::string& getTextRow( int row ) const
    {
        return textRows_[getRow( row )];
    }
private:
    int     getCurrentRow() const
    {
        return (currentFirstRow_ + yCursor) % rows_;
    }
    int     getRow( int row )  const
    {
        assert( row >= 0 );
        assert( row < rows_ );
        return (currentFirstRow_ + row) % rows_;
    }
    void    ScrollUp()
    {
        textRows_[currentFirstRow_][0] = '\0';
        currentFirstRow_++;
        currentFirstRow_ %= rows_;
        xCursor = 0;
        yCursor = rows_ - 1;
    }
private:
    std::vector<std::string> textRows_;
    int     columns_;
    int     rows_;
    int     currentFirstRow_ = 0;
    int     xCursor = 0;
    int     yCursor = 0;
};