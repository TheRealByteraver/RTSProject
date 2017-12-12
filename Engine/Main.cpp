/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Main.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "ChiliException.h"
/*
    Added code from here onwards:
*/
#include "Inifile.h" 
#include "string.h"

/*
This little piece of code makes it possible to define the resolution in
the main configuration .ini file
*/
class Defaults
{
public:
    Defaults();
    ~Defaults() {}
public:
    int             screenWidth;
    int             screenHeight;
    int             minScreenWidth;
    int             minScreenHeight;
    int             maxScreenWidth;
    int             maxScreenHeight;
    std::string     debugLogFile;
    std::string     racesFolder;
    std::string     terrainsFolder;
    std::string     scenariosFolder;


private:
    IniFile         masterIniFile = GAME_FOLDER MASTER_INIFILE_FILENAME;
} defaults; // declare a global variable of this class

Defaults::Defaults()
{ 
    // todo: create an error log and start writing to it
    MessageBox( 0, L"And text here",L"MessageBox caption",MB_OK ); // handy for errors
    // PostMessage( wnd,WM_CLOSE,0,0 ); does not work so we use PostQuitMessage() instead
    if ( !masterIniFile.isLoaded() ) PostQuitMessage( -1 ); 
    int error;
    error = masterIniFile.getKeyValue( "Main","ScreenWidth",screenWidth );
    if ( error != 0 ) PostQuitMessage( -1 );
    error = masterIniFile.getKeyValue( "Main","ScreenHeight",screenHeight );
    if ( error != 0 ) PostQuitMessage( -1 );
    error = masterIniFile.getKeyValue( "Main","MinScreenWidth",minScreenWidth );
    if ( error != 0 ) PostQuitMessage( -1 );
    error = masterIniFile.getKeyValue( "Main","MinScreenHeight",minScreenHeight );
    if ( error != 0 ) PostQuitMessage( -1 );
    error = masterIniFile.getKeyValue( "Main","MaxScreenWidth",maxScreenWidth );
    if ( error != 0 ) PostQuitMessage( -1 );
    error = masterIniFile.getKeyValue( "Main","MaxScreenHeight",maxScreenHeight );
    if ( error != 0 ) PostQuitMessage( -1 );
    if( (screenWidth < minScreenWidth) ||
        (screenWidth > maxScreenWidth) ||
        (screenHeight < minScreenHeight) ||
        (screenHeight > maxScreenHeight)
        ) PostQuitMessage( -1 );
    Graphics::ScreenWidth = screenWidth;
    Graphics::ScreenHeight = screenHeight;
    // load folders 
    error = masterIniFile.getKeyValue( "Folders","Races",racesFolder );
    if ( error != 0 ) PostQuitMessage( -1 );
    error = masterIniFile.getKeyValue( "Folders","Terrains",terrainsFolder );
    if ( error != 0 ) PostQuitMessage( -1 );
    error = masterIniFile.getKeyValue( "Folders","Scenarios",scenariosFolder );
    if ( error != 0 ) PostQuitMessage( -1 );
    // todo: check if folders exist

}
/*
    provisory end of addition
*/

int WINAPI wWinMain( HINSTANCE hInst,HINSTANCE,LPWSTR pArgs,INT )
{

    try
	{
		MainWindow wnd( hInst,pArgs );		
        try
		{
			Game theGame( wnd );
			while( wnd.ProcessMessage() )
			{
				theGame.Go();
			}
		}
		catch( const ChiliException& e )
		{
			const std::wstring eMsg = e.GetFullMessage() + 
				L"\n\nException caught at Windows message loop.";
			wnd.ShowMessageBox( e.GetExceptionType(),eMsg,MB_ICONERROR );
		}
		catch( const std::exception& e )
		{
			// need to convert std::exception what() string from narrow to wide string
			const std::string whatStr( e.what() );
			const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) + 
				L"\n\nException caught at Windows message loop.";
			wnd.ShowMessageBox( L"Unhandled STL Exception",eMsg,MB_ICONERROR );
		}
		catch( ... )
		{
			wnd.ShowMessageBox( L"Unhandled Non-STL Exception",
				L"\n\nException caught at Windows message loop.",MB_ICONERROR );
		}
	}
	catch( const ChiliException& e )
	{
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
		MessageBox( nullptr,eMsg.c_str(),e.GetExceptionType().c_str(),MB_ICONERROR );
	}
	catch( const std::exception& e )
	{
		// need to convert std::exception what() string from narrow to wide string
		const std::string whatStr( e.what() );
		const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) +
			L"\n\nException caught at main window creation.";
		MessageBox( nullptr,eMsg.c_str(),L"Unhandled STL Exception",MB_ICONERROR );
	}
	catch( ... )
	{
		MessageBox( nullptr,L"\n\nException caught at main window creation.",
			L"Unhandled Non-STL Exception",MB_ICONERROR );
	}

	return 0;
}