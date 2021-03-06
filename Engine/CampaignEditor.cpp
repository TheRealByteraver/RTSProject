#include "CampaignEditor.h"

const char *menufiletitles[] =
{
    "New",
    "Open",
    "Save",
    "Save As",
    "Exit",
    nullptr
};

constexpr auto DIMENSION_64x64 = 0;
constexpr auto DIMENSION_96x96 = 1;
constexpr auto DIMENSION_128x128 = 2;
constexpr auto DIMENSION_192x192 = 3;
constexpr auto DIMENSION_256x256 = 4;

const char *terraindimensions[] =
{ 
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",

    /*   
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",
    "64x64",
    "96x96",
    "128x128",
    "192x192",
    "256x256",
    */

    //  "Custom:  256  -+ x  256  -+",
  //    "Custom:                    ", // not for now
    nullptr
};

const char *filemenunewtext[] =
{
    "Please choose a world (environment) and the dimensions of the ",
    "new terrain. Careful, make sure you saved your work so far, as",
    "the program will not ask you to do so (yet)!",
    "Press cancel if you want to save your current work first.",
    nullptr
};

const char *filemenuopentext[] =
{
    "Please choose a terrain to load by clicking on it, or click",
    "cancel to return to the editor.",
    "the program will not ask you to save your current work (yet)!",
    "Press cancel if you want to save your current work first.",
    nullptr
};

const char *filemenuexittext[] =
{
    "Are you sure you want to exit the Campaign Editor?",
    "the program will not ask you to save your current work (yet)!",
    "Press cancel if you want to save your current work first.",
    nullptr
};

constexpr auto BUTTON_OK = 0;
constexpr auto BUTTON_CANCEL = 1;
const char *buttonsokcancel[] =
{
    "  Ok  ",
    "Cancel",
    /*
    "All right this is a very big button now, like way too big and stuff",
    "OK",
    "Cancel",
    "All right this is a very big button now, like way too big and stuff",
    "OK",
    "Cancel",
    "All right this is a very big button now, like way too big and stuff",
    */
    nullptr
};

const char* palettetitles[] = { 
    "<   Terrain    >",
    "<   Doodads    >",
    nullptr
};

void CampaignEditor::init( 
    class MainWindow& wndRef,
    Graphics& gfxRef,
    Font& fontRef,
    Mouse& mouseRef,
    Keyboard& keyboardRef )
{
    // get the shared pointers from the main program
    gfx_ = &gfxRef;
    wnd_ = &wndRef;
    font_ = &fontRef;
    mouse_ = &mouseRef;
    keyboard_ = &keyboardRef;
    assert( gfx_ != nullptr );
    assert( wnd_ != nullptr );
    assert( font_ != nullptr );
    assert( mouse_ != nullptr );
    assert( keyboard_ != nullptr );
    //Graphics& gfx = *gfx_;
    MainWindow& wnd = *wnd_;
    Font& font = *font_;
    // give the screen drawing class a font to write with and draw the screens
    gameScreens_.setFont( &font );  // move font to globals / defaults?

    // create the File menu:
    fileMenu_.init( 
        gameScreens_.menubar_coords.x1 + MENU_FILE_X,
        gameScreens_.menubar_coords.y2,
        menufiletitles,&font
    );

    // load the default world:
    int error = world_.load( defaults.defaultWorld() );
    if ( error != 0 )
    {
        defaults.debugLogFile << "Unable to load default world "
            << terrain_.getWorld() << ", Exiting Program." << std::endl;
        //showErrorAndQuit( defaults.defaultWorld() );
        std::wstring errMsg( L"Unable to open default world gfx data file " );
        for ( char c : defaults.defaultWorld() ) errMsg += c;
        errMsg += L".bmp, exiting Program.";
        wnd.ShowMessageBox( L"Fatal Error",errMsg,MB_OK );
        PostQuitMessage( 0 );
        return;
    }
    // start out with empty terrain
    terrain_.init( defaults.defaultTerrainWidth(),defaults.defaultTerrainHeight() );

    // prepare for doodad editing:
    initDoodadLocationMap();

    // initialize drawing coordinates & // allocate memory for the minimap sprite:
    initMapCoords();

    // create the drawing palettes:
    createBasicTerrainPalette();
    createDoodadPalette();

    // initialize the pointers to the current palette:
    initPalettePointers();

    // draw (prepare) the screens that are used by the CampaignEditor onto sprites 
    // stored in memory:
    gameScreens_.drawScenarioEditor(); 

    // draw the currently active palette:
    redrawPalette();

    // Draw the minimap:
    redrawMiniMap();

    // And we are done:
    isInitialized_ = true;

    // debug: load test terrain
    //error = loadTerrain( "default.ini" );
}
/*
  This function will create a list of filenames of a given extension from a 
  given path. The path should not start with a driveletter or backslash, as
  it will be appended to the default Game directory which should be ".\" by
  default. The path should not end with a backslash either. The extension 
  should be just that, including the dot though, for example ".ini".
*/
void CampaignEditor::populateFileList( const std::string path,const std::string extension )
{
    fileList_.clear();
    std::wstring searchPath;
    for ( char c : std::string( GAME_FOLDER ) ) searchPath += c;
    for ( char c : path ) searchPath += c;
    searchPath.append( L"\\*" );
    for ( char c : extension ) searchPath += c;
    WIN32_FIND_DATA findData;
    HANDLE hFind = ::FindFirstFile( searchPath.c_str(),&findData );
    if ( hFind != INVALID_HANDLE_VALUE )
    {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if ( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                std::string worldName;
                for ( WCHAR *c = findData.cFileName; *c != NULL; c++ ) worldName += (char)*c;
                fileList_.push_back( worldName );
            }
        } while ( ::FindNextFile( hFind,&findData ) );
        ::FindClose( hFind );
    }
}

int CampaignEditor::loadTerrain( const std::string& terrainName )
{
    MainWindow& wnd = *wnd_;
    std::string path( GAME_FOLDER );
    path.append( defaults.terrainsFolder() );
    path.append( "\\" );
    path.append( terrainName );
    int error = terrain_.loadTerrain( path );
    if ( error != 0 )  
    {
        // error loading terrain, create empty terrain:
        terrain_.init(
            defaults.defaultTerrainWidth(),
            defaults.defaultTerrainHeight() );
        // initialize drawing coordinates:
        initMapCoords();
        return -1;
    }
    // load world:
    error = world_.load( terrain_.getWorld() );
    if ( error != 0 )
    {
        defaults.debugLogFile 
            << "Unable to load world " << terrain_.getWorld() << std::endl;        
        //showErrorAndQuit( defaults.defaultWorld() );
        std::wstring errMsg( L"Unable to open world gfx data file " );
        for ( char c : terrain_.getWorld() ) errMsg += c;
        errMsg += L".bmp, using current world instead.";
        wnd.ShowMessageBox( L"Error",errMsg,MB_OK );
        //PostQuitMessage( 0 );
    }
    // Must be done after loading terrain and / or world. Checks if the doodads
    // used in the terrain exist in the world's definition and remove them if 
    // not:
    terrain_.removeUnavailableDoodads( world_.nrOfDoodads() );

    // init the boolean map containing the doodad locations
    initDoodadLocationMap();

    // Must be done each time the terrain is modified. Checks if every single 
    // doodad is still in a valid location:
    checkDoodads();

    // initialize the coordinates for terrain drawing:
    initMapCoords();

    // create the drawing palettes:
    createBasicTerrainPalette();
    createDoodadPalette();

    // initialize the pointers to the current palette:
    initPalettePointers();

    // draw the currently active palette:
    redrawPalette();

    // Draw the minimap:
    redrawMiniMap();

    return error;
}

void CampaignEditor::draw()
{
    Graphics& gfx = *gfx_;
    Mouse& mouse = *mouse_;
    int mX = mouse.GetPosX();
    int mY = mouse.GetPosY();
    // Draw the terrain editor menu's:
    gfx.paintSprite(
        gameScreens_.menubar_coords.x1,
        gameScreens_.menubar_coords.y1,
        gameScreens_.menuBar() );
    gfx.paintSprite(
        gameScreens_.sidebar_coords.x1,
        gameScreens_.sidebar_coords.y1,
        gameScreens_.sideBar() );
    drawTerrain();
    if ( isGridVisible_ ) drawTerrainGrid();
    drawMiniMap();
    drawMiniMapCursor();
    redrawPalette(); // temp, should be moved to side bar drawing function
    if ( menuFileVisible_ )
    {
        gfx.paintSprite(
            fileMenu_.dimensions().x1,
            fileMenu_.dimensions().y1,
            fileMenu_.getImage()
        );
        if ( (mY >= fileMenu_.dimensions().y1) &&
            (mY <= fileMenu_.dimensions().y2 - FRAME_WIDTH - TEXT_OFFSET) )
        {
            const int menuNr = fileMenu_.getSelectedSubMenu( mY );
            const int y1 = fileMenu_.dimensions().y1 + FRAME_WIDTH + TEXT_OFFSET + menuNr * FONT_HEIGHT;
            gfx.drawBox(
                fileMenu_.dimensions().x1 + FRAME_WIDTH,
                y1,
                fileMenu_.dimensions().x2 - FRAME_WIDTH,
                y1 + FONT_HEIGHT - 1,
                Colors::Red
            );
        }
    } else if ( submenuVisible_ ) 
    {
        gfx.setFrameColor( MENU_COLOR );
        gfx.drawNiceBlock( Rect(
            winElementBarList0_.getDimensions().rect().x1 - FRAME_WIDTH,
            winElementBarList0_.getDimensions().rect().y1 - FRAME_WIDTH,
            winElementBarList0_.getDimensions().rect().x2 + FRAME_WIDTH,
            winElementBarList0_.getDimensions().rect().y2 + FRAME_WIDTH )
        );
        int y = winElementBarList0_.getDimensions().yOrig();
        for ( auto& barElem : winElementBarList0_.winElementBarList() )
        {
            int x = winElementBarList0_.getDimensions().xOrig();
            for ( auto& s : barElem->winElementList() )
            {
                gfx.paintSprite( 
                    x + s->getDimensions().xOrig(),
                    y + s->getDimensions().yOrig(),
                    s->image() 
                );
            }
            y += barElem->getDimensions().height();
        }
    } else {
        if ( doodadMouseCursor_ ) drawDoodadCursorAtLocation();
        else drawTerrainCursor();
    }
    /*
    ScrollBar scrollBar( WinDim(
        0,600,600,60 ) 
    );
    scrollBar.init( 18,600,SCROLL_BAR_HORIZONTAL );
    gfx.paintSprite( 0,600,scrollBar.image() );

    ScrollBar scrollBar2( WinDim(
        0,600,60,600 )
    );
    scrollBar2.init( 18,600,SCROLL_BAR_VERTICAL );
    gfx.paintSprite( 620,60,scrollBar2.image() );
    */
}

void CampaignEditor::handleInput()
{
    Mouse& mouse = *mouse_;
    int mX = mouse.GetPosX();
    int mY = mouse.GetPosY();
    if ( mouseLeftClickDelay_ ) mouseLeftClickDelay_--;    
    if ( !mouse.LeftIsPressed() ) // cancel click delay if left mouse released
    {
        mouseWaitForLeftButtonReleased_ = false;
        mouseLeftClickDelay_ = MOUSE_NO_DELAY;
    }
    if ( menuFileVisible_ )
    {
        if ( !(mouse.isInArea( gameScreens_.menuFileTitle ) ||
            mouse.isInArea( fileMenu_.dimensions() )) )
        {
            menuFileVisible_ = false;
        }
    } else if ( !submenuVisible_ ) {
        handleMapScrollingFunction();
        handlePaletteScrollingFunction();
    }
    /*
    if ( submenuVisible_ )
    {
        //submenuHandleInput();
        //menuFileNewHandleInput( mX,mY ); // temp
        //return;
    }
    */
    if ( mouse.RightIsPressed() )
    {
        doodadMouseCursor_ = false; // right-click cancels doodad cursor
    }
    // mouse left click functions:
    if ( mouse.LeftIsPressed() )
    {
        if ( menuFileVisible_ )
        {
            menuFileHandleInput( mY );
            mouseWaitForLeftButtonReleased_ = true;
        // handle whatever submenu that needs to be handled:
        } else if ( submenuVisible_ )
        {
            if ( mouse.isInArea( winElementBarList0_.getDimensions().rect() ) )                
            {
                submenuHandleInput( mX,mY );
                mouseWaitForLeftButtonReleased_ = true;
            }
        // terrain drawing functions:
        } else if ( mouse.isInArea( gameScreens_.map_coords ) )
        {
            if ( doodadMouseCursor_ )
            {
                if ( !mouseWaitForLeftButtonReleased_ ) 
                    tryDrawDoodad( mX,mY );
            } else
            {
                if ( !mouseWaitForLeftButtonReleased_ ) 
                    drawBasicTerrain( mX,mY );
            }
        // switch to the next palette:
        } else if ( mouse.isInArea( gameScreens_.paletteSelector ) )
        {
            if ( mouseLeftClickDelay_ == MOUSE_NO_DELAY )
            {
                mouseLeftClickDelay_ = MOUSE_LEFT_CLICK_DELAY;
                switchToNextPalette();
            }
        } else if ( mouse.isInArea( gameScreens_.paletteWindow ) )
        {
            paletteHandleInput( mY );
        } else if ( mouse.isInArea( gameScreens_.menuFileTitle ) )
        {
            menuFileVisible_ = true;
        }
    }
    /*
    for ( ;;)
    {
    Mouse::Event event = mouse.Read();
    if ( !event.IsValid() ) break;
    int mX = event.GetPosX();
    int mY = event.GetPosY();
    }
    */
}

void CampaignEditor::handleMapScrollingFunction()
{
    Mouse& mouse = *mouse_;
    bool mouseInScrollArea = false;
    if ( mouse.isInArea( gameScreens_.scrollMapLeft ) && (TerrainDrawXOrig_ > 0) )
    {
        if ( mouseMapScrollDelay_ > 0 ) mouseMapScrollDelay_--;
        else {
            TerrainDrawXOrig_--;
        }
        mouseInScrollArea = true;
    }
    if ( mouse.isInArea( gameScreens_.scrollMapRight ) &&
        (TerrainDrawXOrig_ < terrain_.getColumns() - visibleTilesX_) )
    {
        if ( mouseMapScrollDelay_ > 0 ) mouseMapScrollDelay_--;
        else {
            TerrainDrawXOrig_++;
        }
        mouseInScrollArea = true;
    }
    if ( mouse.isInArea( gameScreens_.scrollMapUp ) && (TerrainDrawYOrig_ > 0) )
    {
        if ( mouseMapScrollDelay_ > 0 ) mouseMapScrollDelay_--;
        else {
            TerrainDrawYOrig_--;
        }
        mouseInScrollArea = true;
    }
    if ( mouse.isInArea( gameScreens_.scrollMapDown ) &&
        (TerrainDrawYOrig_ < terrain_.getRows() - visibleTilesY_) )
    {
        if ( mouseMapScrollDelay_ > 0 ) mouseMapScrollDelay_--;
        else {
            TerrainDrawYOrig_++;
        }
        mouseInScrollArea = true;
    }
    if ( !mouseInScrollArea ) mouseMapScrollDelay_ = MOUSE_SCROLL_START_DELAY;
}

void CampaignEditor::handlePaletteScrollingFunction()
{
    Mouse& mouse = *mouse_;
    bool mouseInScrollArea = false;
    Palette& palette = *palettePTR_;
    if ( mouse.isInArea(
        Rect( gameScreens_.paletteWindow.x1,
            gameScreens_.paletteWindow.y1,
            gameScreens_.paletteWindow.x2,
            gameScreens_.paletteWindow.y1 + 64 ) ) )
    {
        if ( mousePaletteScrollDelay_ > 0 ) mousePaletteScrollDelay_--;
        else {
            if ( mousePaletteScrollSpeed_ > 0 ) mousePaletteScrollSpeed_--;
            else {
                if ( palette.listIndex > 0 ) palette.listIndex--;
                redrawPalette();
                mousePaletteScrollSpeed_ = MOUSE_SCROLL_SPEED;
            }
        }
        mouseInScrollArea = true;
    }
    if ( mouse.isInArea(
        Rect( gameScreens_.paletteWindow.x1,
            gameScreens_.paletteWindow.y2 - 64,
            gameScreens_.paletteWindow.x2,
            gameScreens_.paletteWindow.y2 ) ) )
    {
        if ( mousePaletteScrollDelay_ > 0 ) mousePaletteScrollDelay_--;
        else {
            if ( mousePaletteScrollSpeed_ > 0 ) mousePaletteScrollSpeed_--;
            else {
                if ( !scrollDownLock_ ) palette.listIndex++;
                redrawPalette();
                mousePaletteScrollSpeed_ = MOUSE_SCROLL_SPEED;
            }
        }
        mouseInScrollArea = true;
    }
    if ( !mouseInScrollArea ) mousePaletteScrollDelay_ = MOUSE_SCROLL_START_DELAY;
}

void CampaignEditor::switchToNextPalette()
{
    activePalette_++;
    if ( activePalette_ >= NR_OF_PALETTES ) activePalette_ = 0;
    initPalettePointers();
    redrawPalette();
}

void CampaignEditor::paletteHandleInput( int mY )
{
    Palette& palette = *palettePTR_;
    if ( palette.yValues.size() > 0 )
    {
        int y = mY - gameScreens_.paletteWindow.y1;
        int yDelta = palette.yValues[palette.listIndex];
        if ( palette.image.getHeight() - yDelta < y ) return; // below lowest icon
        int index;
        for ( index = palette.listIndex; index < (int)palette.yValues.size(); index++ )
        {
            if ( palette.yValues[index] - yDelta > y ) break;
        }
        int activeItem = index - 1;
        switch ( activePalette_ )
        {
            case BASIC_TERRAIN_PALETTE:
            {
                doodadMouseCursor_ = false;
                // terrainType_ = activeItem << 4;
                terrainType_ = terrain_.getBasicTerrainValue( activeItem );
                break;
            }
            case DOODAD_PALETTE:
            {
                doodadNr_ = activeItem;
                doodadCursorSprite_ = world_.getDoodad( doodadNr_ ).image();
                doodadMouseCursor_ = true;
                break;
            }
        }
    }
}

void CampaignEditor::drawBasicTerrain( int mX,int mY )
{
    // Calculate the absolute tile coordinates at mouse' location:
    int tileX = (mX - gameScreens_.map_coords.x1) / tileWidth_ + TerrainDrawXOrig_;
    int tileY = (mY - gameScreens_.map_coords.y1) / tileHeight_ + TerrainDrawYOrig_;
    if ( (tileX < terrain_.getColumns()) &&
        (tileY < terrain_.getRows()) )
    {
        if ( tileX & 0x1 ) tileX--;
        if ( tileY & 0x1 ) tileY--;
        Rect redraw = terrain_.drawTerrain( tileX,tileY,terrainType_ );
        checkDoodads();
        redrawMiniMap();
        isTerrainSaved_ = false;
    }
}

void CampaignEditor::tryDrawDoodad( int mX,int mY )
{
    // Calculate the absolute tile coordinates at mouse' location:
    int tileX = (mX - gameScreens_.map_coords.x1) / tileWidth_ + TerrainDrawXOrig_;
    int tileY = (mY - gameScreens_.map_coords.y1) / tileHeight_ + TerrainDrawYOrig_;
    const Doodad& doodad = world_.getDoodad( doodadNr_ );
    if ( canPlaceDoodadAtLocation( tileX,tileY,doodad )
        && (!doodadPresentInArea( Rect(
            tileX,
            tileY,
            tileX + doodad.width() - 1,
            tileY + doodad.height() - 1 ) )) )
    {
        terrain_.addDoodad(
            DoodadLocation( tileX,tileY,doodadNr_,true )
        );
        for ( int j = tileY; j < tileY + doodad.height(); j++ )
            for ( int i = tileX; i < tileX + doodad.width(); i++ )
                doodadLocationMap_[j * terrain_.getColumns() + i] = true;
        redrawMiniMap();
        isTerrainSaved_ = false;
    }
}

void CampaignEditor::menuFileHandleInput( int mY )
{
    activeMenuID_ = fileMenu_.getSelectedSubMenu( mY );
    switch ( activeMenuID_ )
    {
        case MENU_FILE_NEW:
        {
            menuFileVisible_ = false;
            submenuVisible_ = true;
            menuFileNewDrawSubmenu();
            break;
        }
        case MENU_FILE_OPEN:
        {
            menuFileVisible_ = false;
            submenuVisible_ = true;
            menuFileOpenDrawSubmenu();
            break;
        }
        case MENU_FILE_SAVE:
        {
            terrain_.setWorld( world_.name() );
            terrain_.saveTerrain( GAME_FOLDER "terrains\\savetest.ini" );
            menuFileVisible_ = false;
            break;
        }
        case MENU_FILE_SAVE_AS:
        {
            menuFileVisible_ = false;
            break;
        }
        case MENU_FILE_EXIT:
        {
            menuFileVisible_ = false;
            submenuVisible_ = true;
            menuFileExitDrawSubmenu();
            break;
        }
    }
}

void CampaignEditor::submenuHandleInput( int mX,int mY )
{
    switch ( activeMenuID_ )
    {
        case MENU_FILE_NEW:
        {
            menuFileNewHandleInput( mX,mY );
            break;
        }
        case MENU_FILE_OPEN:
        {
            menuFileOpenHandleInput( mX,mY );
            break;
        }
        case MENU_FILE_EXIT:
        {
            menuFileExitHandleInput( mX,mY );
            break;
        }
    }
}

void CampaignEditor::menuFileNewDrawSubmenu()
{
    // half the screen width should be enough for the dimensions radiobutton list:
    WinDim dimConstraint( 
        0,
        0,
        Graphics::ScreenWidth / 2,
        Graphics::ScreenHeight - FRAME_WIDTH * 2
    );
    radiobuttonGroup0_.setDimConstraint( dimConstraint );
    radiobuttonGroup0_.setFont( font_ );
    radiobuttonGroup0_.init(
        std::string( " Choose the size: " ),
        terraindimensions 
    );
    // Get a list with the available worlds .ini files:
    populateFileList( defaults.worldsFolder(),".ini" );
    // reset the maximum size constraint for the new radio button group:
    dimConstraint.reSize(
        Graphics::ScreenWidth - FRAME_WIDTH * 2
        - radiobuttonGroup0_.getDimensions().width(),
        Graphics::ScreenHeight - FRAME_WIDTH * 2
    );
    radiobuttonGroup1_.setDimConstraint( dimConstraint );
    radiobuttonGroup1_.setFont( font_ );
    radiobuttonGroup1_.init( std::string( " Choose the world: " ),fileList_ );

    // create the OK-Cancel button window element:
    buttonList0_.setDimConstraint( dimConstraint ); // will be at least half the screen
    buttonList0_.setFont( font_ );
    buttonList0_.init( buttonsokcancel );

    // create the text panel on top:
    dimConstraint.reSize(
        Graphics::ScreenWidth - FRAME_WIDTH * 2,
        Graphics::ScreenHeight - FRAME_WIDTH * 2
    );
    textPanel0_.setDimConstraint( dimConstraint );
    textPanel0_.setFont( font_ );
    textPanel0_.init( filemenunewtext );

    // create a 1st horizontal bar with both radio button win elements we just created:
    winElementBar0_.clear();
    winElementBar0_.addWinElement( radiobuttonGroup1_ );
    winElementBar0_.addWinElement( radiobuttonGroup0_ );

    // create a 2nd bar with the ok+cancel button list win element:
    winElementBar1_.clear();
    // center the buttons to make it a bit more appealing visually:
    int xBtnOfs = winElementBar0_.getDimensions().width() -
                    buttonList0_.getDimensions().width();
    if ( xBtnOfs < 0 ) xBtnOfs = 0;
    buttonList0_.moveTo( xBtnOfs / 2,0 );
    winElementBar1_.addWinElement( buttonList0_ );

    // create a 3rd bar with the text panel win element:
    winElementBar2_.clear();
    winElementBar2_.addWinElement( textPanel0_ );

    // create the final window, meaning a vertical stack of winElementBar's:
    winElementBarList0_.clear();
    winElementBarList0_.addWinElementBar( winElementBar2_ );
    winElementBarList0_.addWinElementBar( winElementBar0_ );
    winElementBarList0_.addWinElementBar( winElementBar1_ );

    // center the final window itself on the screen:
    winElementBarList0_.moveTo(
        (Graphics::ScreenWidth - winElementBarList0_.getDimensions().width()) / 2,
        (Graphics::ScreenHeight - winElementBarList0_.getDimensions().height()) / 2
    );
}

void CampaignEditor::menuFileNewHandleInput( int mX,int mY )
{
    winElementBarList0_.handleInput( mX,mY );
    if( buttonList0_.hasValidInput() )
    {
        
        if ( buttonList0_.getInput() == BUTTON_OK )
        {
            if ( !(radiobuttonGroup0_.hasValidInput() &&
                radiobuttonGroup1_.hasValidInput()) )
            {
                buttonList0_.invalidateInput();
            } else {
                submenuVisible_ = false;
                // new terrain logic here
                int dimensionChoice = radiobuttonGroup0_.getInput();
                int terrainChoice = radiobuttonGroup1_.getInput();
                int newTerrainWidth = 0;
                int newTerrainHeight = 0;
                switch ( dimensionChoice )
                {
                    case DIMENSION_64x64:
                    {
                        newTerrainWidth = 64;
                        newTerrainHeight = 64;
                        break;
                    }
                    case DIMENSION_96x96:
                    {
                        newTerrainWidth = 96;
                        newTerrainHeight = 96;
                        break;
                    }
                    case DIMENSION_128x128:
                    {
                        newTerrainWidth = 128;
                        newTerrainHeight = 128;
                        break;
                    }
                    case DIMENSION_192x192:
                    {
                        newTerrainWidth = 192;
                        newTerrainHeight = 192;
                        break;
                    }
                    case DIMENSION_256x256:
                    {
                        newTerrainWidth = 256;
                        newTerrainHeight = 256;
                        break;
                    }
                }
                // create empty terrain:
                terrain_.init( newTerrainWidth,newTerrainHeight );

                std::string terrainChoiceName( fileList_[terrainChoice] );
                terrainChoiceName.resize( terrainChoiceName.length() - 4 ); // cut extension

                // load world:
                World oldWorld = world_;
                int error = world_.load( terrainChoiceName );
                if ( error != 0 )
                {
                    defaults.debugLogFile
                        << "Unable to load world " << terrainChoiceName << std::endl;
                    //showErrorAndQuit( defaults.defaultWorld() );
                    std::wstring errMsg( L"Unable to open world gfx data file " );
                    for ( char c : terrainChoiceName ) errMsg += c;
                    errMsg += L".bmp, using current world instead.";
                    world_ = oldWorld;
                }
                // init the boolean map containing the doodad locations
                initDoodadLocationMap();

                // initialize the coordinates for terrain drawing:
                initMapCoords();

                // create the drawing palettes:
                createBasicTerrainPalette();
                createDoodadPalette();

                // initialize the pointers to the current palette:
                initPalettePointers();

                // draw the currently active palette:
                redrawPalette();

                // Draw the minimap:
                redrawMiniMap();

            }                
        } else { // cancel button pressed            
            submenuVisible_ = false;
        }        
    }
}

void CampaignEditor::menuFileOpenDrawSubmenu()
{
    WinDim dimConstraint(
        0,
        0,
        Graphics::ScreenWidth - FRAME_WIDTH * 2,
        (Graphics::ScreenHeight - FRAME_WIDTH * 2) / 4
    );

    textPanel0_.setDimConstraint( dimConstraint );
    textPanel0_.setFont( font_ );
    textPanel0_.init( filemenuopentext );
    
    // take as much space as possible:
    dimConstraint.reSize(
        //Graphics::ScreenWidth - FRAME_WIDTH * 2,
        textPanel0_.getDimensions().width(),
        Graphics::ScreenHeight - FRAME_WIDTH * 2 -
        textPanel0_.getDimensions().height()
    );

    // Get a list with the available terrain .ini files:
    populateFileList( defaults.terrainsFolder(),".ini" );
    fileList_.push_back( "Cancel" );

    // create a list of buttons with terrain .ini files and the cancel button:
    buttonList0_.setDimConstraint( dimConstraint ); 
    buttonList0_.setFont( font_ );
    buttonList0_.init( fileList_ );

    // create a 1st horizontal bar with the informative text:
    winElementBar0_.clear();
    winElementBar0_.addWinElement( textPanel0_ );

    // create a 2nd bar with the filename buttons:
    winElementBar1_.clear();
    winElementBar1_.addWinElement( buttonList0_ );

    // create the final window, meaning a vertical stack of winElementBar's:
    winElementBarList0_.clear();
    winElementBarList0_.addWinElementBar( winElementBar0_ );
    winElementBarList0_.addWinElementBar( winElementBar1_ );

    // center the final window itself on the screen:
    winElementBarList0_.moveTo(
        (Graphics::ScreenWidth - winElementBarList0_.getDimensions().width()) / 2,
        (Graphics::ScreenHeight - winElementBarList0_.getDimensions().height()) / 2
    );
}

void CampaignEditor::menuFileOpenHandleInput( int mX,int mY )
{
    winElementBarList0_.handleInput( mX,mY );
    const int cancelButton = (int)fileList_.size() - 1;
    if ( buttonList0_.hasValidInput() )
    {
        submenuVisible_ = false;
        if ( buttonList0_.getInput() != cancelButton )
        {            
            int terrainChoice = buttonList0_.getInput();
            loadTerrain( fileList_[terrainChoice] );
        }
    }
}

void CampaignEditor::menuFileExitDrawSubmenu()
{
    WinDim dimConstraint(
        0,
        0,
        Graphics::ScreenWidth - FRAME_WIDTH * 2,
        (Graphics::ScreenHeight - FRAME_WIDTH * 2)
    );

    textPanel0_.setDimConstraint( dimConstraint );
    textPanel0_.setFont( font_ );
    textPanel0_.init( filemenuexittext );

    dimConstraint.reSize(
        textPanel0_.getDimensions().width(),
        textPanel0_.getDimensions().height()
    );

    // create the OK-Cancel button window element:
    buttonList0_.setDimConstraint( dimConstraint ); // will be at least half the screen
    buttonList0_.setFont( font_ );
    buttonList0_.init( buttonsokcancel );

    // create a 1st horizontal bar with the text panel we just created:
    winElementBar0_.clear();
    winElementBar0_.addWinElement( textPanel0_ );

    // create a 2nd bar with the ok+cancel button list win element:
    winElementBar1_.clear();
    // center the buttons to make it a bit more appealing visually:
    int xBtnOfs = winElementBar0_.getDimensions().width() -
        buttonList0_.getDimensions().width();
    if ( xBtnOfs < 0 ) xBtnOfs = 0;
    buttonList0_.moveTo( xBtnOfs / 2,0 );
    winElementBar1_.addWinElement( buttonList0_ );

    // create the final window, meaning a vertical stack of winElementBar's:
    winElementBarList0_.clear();
    winElementBarList0_.addWinElementBar( winElementBar0_ );
    winElementBarList0_.addWinElementBar( winElementBar1_ );

    // center the final window itself on the screen:
    winElementBarList0_.moveTo(
        (Graphics::ScreenWidth - winElementBarList0_.getDimensions().width()) / 2,
        (Graphics::ScreenHeight - winElementBarList0_.getDimensions().height()) / 2
    );
}

void CampaignEditor::menuFileExitHandleInput( int mX,int mY )
{
    winElementBarList0_.handleInput( mX,mY );
    if ( buttonList0_.hasValidInput() )
    {
        submenuVisible_ = false;
        if ( buttonList0_.getInput() == BUTTON_OK )
        {
            shutdownCampaignEditor();
        }
    }
}

void CampaignEditor::drawTerrain()
{
    Graphics& gfx = *gfx_;
    int tileWidth = tileWidth_;
    int tileHeight = tileHeight_;
    Rect tR = gameScreens_.map_coords;
    int j = TerrainDrawYOrig_;
    tR.x2 = tR.x1 + (tR.width() / tileWidth) * tileWidth - 1;
    tR.y2 = tR.y1 + (tR.height() / tileHeight) * tileHeight - 1;
    int xLeftOver = gameScreens_.map_coords.width() - tR.width();
    int yLeftOver = gameScreens_.map_coords.height() - tR.height();
    int y;
    for ( y = tR.y1; y < tR.y2; y += tileHeight )
    {
        if ( j >= terrain_.getRows() ) break;
        int i = TerrainDrawXOrig_;
        int x;
        for ( x = tR.x1; x < tR.x2; x += tileWidth )
        {
            if ( i >= terrain_.getColumns() ) break;
            Tile tile = terrain_.getElement( i,j );
            //if ( tile != DOODAD_NOT_USED ) // technique is not used in editor
            gfx.paintSprite( x,y,world_.getTile( tile ) );
            i++;
        }
        // draw the cut-off tiles on the right edge of the map editor:
        if ( (xLeftOver > 0) && (i < terrain_.getColumns()) )
        {
            Tile tile = terrain_.getElement( i,j );
            //if ( tile != DOODAD_NOT_USED ) // technique is not used in editor
            gfx.paintSpriteSection(
                x,y,
                Rect( 0,0,xLeftOver - 1,tileHeight - 1 ),
                world_.getTile( tile ) );
        }
        j++;
    }
    // draw the cut-off tiles on the bottom edge of the map editor:
    if ( (yLeftOver > 0) && (j < terrain_.getRows()) )
    {
        int i = TerrainDrawXOrig_;
        int x;
        for ( x = tR.x1; x < tR.x2; x += tileWidth )
        {
            if ( i >= terrain_.getColumns() ) break;
            Tile tile = terrain_.getElement( i,j );
            //if ( tile != DOODAD_NOT_USED ) // technique is not used in editor
            gfx.paintSpriteSection(
                x,y,
                Rect( 0,0,tileWidth - 1,yLeftOver - 1 ),
                world_.getTile( tile ) );
            i++;
        }
        // draw the cut-off tile on the bottom right edge of the map editor:
        if ( (xLeftOver > 0) && (i < terrain_.getColumns()) )
        {
            Tile tile = terrain_.getElement( i,j );
            //if ( tile != DOODAD_NOT_USED ) // technique is not used in editor
            gfx.paintSpriteSection(
                x,y,
                Rect( 0,0,xLeftOver - 1,yLeftOver - 1 ),
                world_.getTile( tile ) );
        }
    }
    // Now Draw the doodadd's.
    // These are the visible tiles on the screen, including the ones 
    // that are only part visible on the right & bottom edges:
    int minX = TerrainDrawXOrig_;
    int minY = TerrainDrawYOrig_;
    int maxX = minX + visibleTilesX_;
    int maxY = minY + visibleTilesY_;
    const std::vector<DoodadLocation>& doodadList = terrain_.getDoodadList();
    for ( int doodadNr = 0; doodadNr < (int)doodadList.size(); doodadNr++ )
    {
        const DoodadLocation doodadLocation = doodadList[doodadNr];
        if ( !doodadLocation.isUsed ) continue; // skip invalid data
        const Doodad& doodad = world_.getDoodad( doodadLocation.doodadNr );
        // The next coords / values are expressed in tiles, not pixels:
        int doodadWidth = doodad.width();
        int doodadHeight = doodad.height();
        int x1 = doodadLocation.x;
        int x2 = x1 + doodadWidth - 1;
        int y1 = doodadLocation.y;
        int y2 = y1 + doodadHeight - 1;
        if ( (x2 < minX) || (x1 > maxX) || (y2 < minY) || (y1 > maxY) ) continue; // tile is not visible
        int xOfs = (x1 < minX) ? minX - x1 : 0;
        int yOfs = (y1 < minY) ? minY - y1 : 0;
        // The next coords are expressed in pixels, not tiles:
        int pixX1 = gameScreens_.map_coords.x1 + (doodadLocation.x - TerrainDrawXOrig_ + xOfs) * tileWidth;
        int pixY1 = gameScreens_.map_coords.y1 + (doodadLocation.y - TerrainDrawYOrig_ + yOfs) * tileHeight;
        int pixWidthMinus1 = (doodadWidth - xOfs) * tileWidth - 1;
        int pixHeightMinus1 = (doodadHeight - yOfs) * tileHeight - 1;
        // make sure we clip to the visible part of the map:
        if ( pixX1 + pixWidthMinus1 > gameScreens_.map_coords.x2 )
            pixWidthMinus1 = gameScreens_.map_coords.x2 - pixX1;
        if ( pixY1 + pixHeightMinus1 > gameScreens_.map_coords.y2 )
            pixHeightMinus1 = gameScreens_.map_coords.y2 - pixY1;
        // convert tile offsets to pixel coords:
        xOfs *= tileWidth;
        yOfs *= tileHeight;
        gfx.paintSpriteSection(
            pixX1,pixY1,
            Rect( xOfs,yOfs,xOfs + pixWidthMinus1,yOfs + pixHeightMinus1 ),
            doodad.image() );
    }
}

void CampaignEditor::drawTerrainGrid()
{
    Graphics& gfx = *gfx_;
    int tileWidth = tileWidth_;
    int tileHeight = tileHeight_;
    //const Rect& map = gameScreens.map_coords;
    for ( int x = gameScreens_.map_coords.x1 + tileWidth - 1;
        x < gameScreens_.map_coords.x2; x += tileWidth )
        gfx.drawVerLine(
            x,
            gameScreens_.map_coords.y1,
            gameScreens_.map_coords.y2,GRID_COLOR );
    for ( int y = gameScreens_.map_coords.y1 + tileHeight - 1;
        y < gameScreens_.map_coords.y2; y += tileHeight )
        gfx.drawHorLine(
            gameScreens_.map_coords.x1,
            y,
            gameScreens_.map_coords.x2,GRID_COLOR );
}

// draw mouse cursor (clipped). This is not the doodad placement masked cursor!
void CampaignEditor::drawTerrainCursor()
{
    Graphics& gfx = *gfx_;
    Mouse& mouse = *mouse_;
    if ( mouse.isInArea( gameScreens_.map_coords ) )
    {
        int curX = (mouse.GetPosX() - gameScreens_.map_coords.x1) / tileWidth_;
        int curY = (mouse.GetPosY() - gameScreens_.map_coords.y1) / tileHeight_;
        if ( (TerrainDrawXOrig_ + curX) & 0x1 ) curX--;
        if ( (TerrainDrawYOrig_ + curY) & 0x1 ) curY--;
        bool drawTop = true;
        bool drawBottom = true;
        bool drawLeft = true;
        bool drawRight = true;
        int x1 = gameScreens_.map_coords.x1 + curX * tileWidth_;
        int x2 = x1 + tileWidth_ * 2 - 1;
        int y1 = gameScreens_.map_coords.y1 + curY * tileHeight_;
        int y2 = y1 + tileHeight_ * 2 - 1;
        if ( x1 < gameScreens_.map_coords.x1 )
        {
            drawLeft = false;
            x1 = gameScreens_.map_coords.x1;
        }
        if ( y1 < gameScreens_.map_coords.y1 )
        {
            drawTop = false;
            y1 = gameScreens_.map_coords.y1;
        }
        int maxX = gameScreens_.map_coords.x1 + (terrain_.getColumns() - TerrainDrawXOrig_) * tileWidth_ - 1;
        if ( maxX > gameScreens_.map_coords.x2 ) maxX = gameScreens_.map_coords.x2;

        if ( x2 > maxX ) {
            x2 = maxX;
            drawRight = false;
        }
        int maxY = gameScreens_.map_coords.y1 + (terrain_.getRows() - TerrainDrawYOrig_) * tileHeight_ - 1;
        if ( maxY > gameScreens_.map_coords.y2 ) maxY = gameScreens_.map_coords.y2;
        if ( y2 > maxY ) {
            y2 = maxY;
            drawBottom = false;
        }
        if ( (x1 < maxX) && (y1 < maxY) )
        {
            if ( drawTop ) gfx.drawHorLine( x1,y1,x2,CURSOR_COLOR );
            if ( drawBottom ) gfx.drawHorLine( x1,y2,x2,CURSOR_COLOR );
            if ( drawLeft ) gfx.drawVerLine( x1,y1,y2,CURSOR_COLOR );
            if ( drawRight ) gfx.drawVerLine( x2,y1,y2,CURSOR_COLOR );
            if ( drawTop ) y1++;
            if ( drawBottom ) y2--;
            if ( drawLeft ) x1++;
            if ( drawRight ) x2--;
            if ( drawTop ) gfx.drawHorLine( x1,y1,x2,CURSOR_COLOR );
            if ( drawBottom ) gfx.drawHorLine( x1,y2,x2,CURSOR_COLOR );
            if ( drawLeft ) gfx.drawVerLine( x1,y1,y2,CURSOR_COLOR );
            if ( drawRight ) gfx.drawVerLine( x2,y1,y2,CURSOR_COLOR );
        }
    } // end draw cursor
}

void CampaignEditor::drawMiniMap()
{
    Graphics& gfx = *gfx_;
    gfx.paintSprite( minimap_.xOrig,minimap_.yOrig,minimap_.image );
}

void CampaignEditor::redrawMiniMap()
{
    switch ( minimap_.zoomLevel )
    {
        case ZOOM_QUARTER_PIXEL_PER_TILE: // one pixel per 4 tiles
        {
            Sprite buffer;
            buffer.createEmptySprite( terrain_.getColumns(),terrain_.getRows() );
            int t = 0;
            for ( int y = 0; y < terrain_.getRows(); y++ )
                for ( int x = 0; x < terrain_.getColumns(); x++ )
                    buffer.putPixel(
                        x,
                        y,
                        world_.getAvgColor( terrain_.getElement( t++ ) ) );
            // and now the doodads:
            const std::vector<DoodadLocation>& doodadList = terrain_.getDoodadList();
            for ( int iDoodad = 0; iDoodad < (int)doodadList.size(); iDoodad++ )
            {
                const DoodadLocation& doodadLocation = doodadList[iDoodad];
                if ( doodadLocation.isUsed )
                {
                    const Doodad& doodad = world_.getDoodad( doodadLocation.doodadNr );
                    int idx = 0;
                    for ( int j = 0; j < doodad.height(); j++ )
                        for ( int i = 0; i < doodad.width(); i++ )
                            buffer.putPixel(
                                doodadLocation.x + i,
                                doodadLocation.y + j,
                                doodad.getAvgColor( idx++ ) );
                }
            }
            Color * bufPtr = buffer.getPixelData();
            int nextLine = buffer.getWidth() - 1;
            int mmpY = 0;
            for ( int y = 0; y < terrain_.getRows(); y += 2 )
            {
                int mmpX = 0;
                for ( int x = 0; x < terrain_.getColumns(); x += 2 )
                {
                    Color c0 = *bufPtr++;
                    Color c1 = *bufPtr;
                    Color c2 = bufPtr[nextLine];
                    bufPtr++;
                    Color c3 = bufPtr[nextLine];
                    minimap_.image.putPixel( mmpX,mmpY,Color(
                        (c0.GetR() + c1.GetR() + c2.GetR() + c3.GetR()) >> 2,
                        (c0.GetG() + c1.GetG() + c2.GetG() + c3.GetG()) >> 2,
                        (c0.GetB() + c1.GetB() + c2.GetB() + c3.GetB()) >> 2 ) );
                    mmpX++;
                }
                bufPtr += buffer.getWidth();
                mmpY++;
            }
            break;
        }
        case ZOOM_ONE_PIXEL_PER_TILE: // one pixel per tile
        {
            int t = 0;
            for ( int y = 0; y < terrain_.getRows(); y++ )
                for ( int x = 0; x < terrain_.getColumns(); x++ )
                    minimap_.image.putPixel( 
                        x,
                        y,
                        world_.getAvgColor( terrain_.getElement( t++ ) ) 
                    );
            // and now the doodads:
            const std::vector<DoodadLocation>& doodadList = terrain_.getDoodadList();
            for ( int iDoodad = 0; iDoodad < (int)doodadList.size(); iDoodad++ )
            {
                const DoodadLocation& doodadLocation = doodadList[iDoodad];
                if ( doodadLocation.isUsed )
                {
                    const Doodad& doodad = world_.getDoodad( doodadLocation.doodadNr );
                    int idx = 0;
                    for ( int j = 0; j < doodad.height(); j++ )
                        for ( int i = 0; i < doodad.width(); i++ )
                            minimap_.image.putPixel(
                                doodadLocation.x + i,
                                doodadLocation.y + j,
                                doodad.getAvgColor( idx++ )
                            );
                }
            }
            break;
        }
        case ZOOM_FOUR_PIXELS_PER_TILE: // two pixels per tile
        {
            int y = 0;
            for ( int j = 0; j < terrain_.getRows(); j++ )
            {
                int x = 0;
                for ( int i = 0; i < terrain_.getColumns(); i++ )
                {
                    const Color *src = world_.getAvgColors_2x2( terrain_.getElement( i,j ) );
                    minimap_.image.putPixel( x    ,y    ,*src++ );
                    minimap_.image.putPixel( x + 1,y    ,*src++ );
                    minimap_.image.putPixel( x    ,y + 1,*src++ );
                    minimap_.image.putPixel( x + 1,y + 1,*src );
                    x += 2;
                }
                y += 2;
            }
            // and now the doodads:
            const std::vector<DoodadLocation>& doodadList = terrain_.getDoodadList();
            for ( int iDoodad = 0; iDoodad < (int)doodadList.size(); iDoodad++ )
            {
                const DoodadLocation& doodadLocation = doodadList[iDoodad];
                if ( doodadLocation.isUsed )
                {
                    const Doodad& doodad = world_.getDoodad( doodadLocation.doodadNr );
                    int idx = 0;
                    int doubleWidth = doodad.width() * 2;
                    int doubleHeight = doodad.height() * 2;
                    int y = doodadLocation.y * 2;
                    for ( int j = 0; j < doubleHeight; j += 2 )
                    {
                        int x = doodadLocation.x * 2;
                        for ( int i = 0; i < doubleWidth; i += 2 )
                        {
                            minimap_.image.putPixel( x    ,y    ,doodad.getAvgColor2x2( idx++ ) );
                            minimap_.image.putPixel( x + 1,y    ,doodad.getAvgColor2x2( idx++ ) );
                            minimap_.image.putPixel( x    ,y + 1,doodad.getAvgColor2x2( idx++ ) );
                            minimap_.image.putPixel( x + 1,y + 1,doodad.getAvgColor2x2( idx++ ) );
                            x += 2;
                        }
                        y += 2;
                    }
                }
            }
            break;
        }
    }
}

void CampaignEditor::drawMiniMapCursor()
{
    // draw the minimap highlighted area delimiter / cursor:
    Rect& cursor = minimap_.cursorCoords;
    switch ( minimap_.zoomLevel )
    {
        case ZOOM_QUARTER_PIXEL_PER_TILE:
        {
            cursor.x1 = minimap_.xOrig + TerrainDrawXOrig_ / 2;
            cursor.y1 = minimap_.yOrig + TerrainDrawYOrig_ / 2;
            cursor.x2 = cursor.x1 + visibleTilesX_ / 2 - 1;
            cursor.y2 = cursor.y1 + visibleTilesY_ / 2 - 1;
            break;
        }
        case ZOOM_ONE_PIXEL_PER_TILE:
        {
            cursor.x1 = minimap_.xOrig + TerrainDrawXOrig_;
            cursor.y1 = minimap_.yOrig + TerrainDrawYOrig_;
            cursor.x2 = cursor.x1 + visibleTilesX_ - 1;
            cursor.y2 = cursor.y1 + visibleTilesY_ - 1;
            break;
        }
        case ZOOM_FOUR_PIXELS_PER_TILE:
        {
            cursor.x1 = minimap_.xOrig + TerrainDrawXOrig_ * 2;
            cursor.y1 = minimap_.yOrig + TerrainDrawYOrig_ * 2;
            cursor.x2 = cursor.x1 + visibleTilesX_ * 2 - 1;
            cursor.y2 = cursor.y1 + visibleTilesY_ * 2 - 1;
            break;
        }
    }
    Graphics& gfx = *gfx_;
    gfx.drawBox( minimap_.cursorCoords,Colors::White );
}

// This function needs to be called each time a new world or terrain is loaded
void CampaignEditor::initMapCoords()
{
    /*
        A zoom level of 1 means 1 pixel per 4 tiles (2x2), because the minimap
        is 128 pixels wide and the largest map is 256 tiles wide / high.
        A zoom level of 2 means 1 pixel per 1 tile (1x1), ideal for when the
        map is 128x128.
        A zoom level of 3 means 2x2 pixels per tile (1x1), ideal for when the
        map is 64x64 (or smaller)

        We also allocate memory for the minimap here.
    */
    if ( terrain_.getColumns() > 128 )
    {
        minimap_.zoomLevel = ZOOM_QUARTER_PIXEL_PER_TILE;
        minimap_.image.createEmptySprite( terrain_.getColumns() / 2,terrain_.getRows() / 2 );
    } else if ( terrain_.getColumns() > 64 )
    {
        minimap_.zoomLevel = ZOOM_ONE_PIXEL_PER_TILE;
        minimap_.image.createEmptySprite( terrain_.getColumns(),terrain_.getRows() );
    } else
    {
        minimap_.zoomLevel = ZOOM_FOUR_PIXELS_PER_TILE;
        minimap_.image.createEmptySprite( terrain_.getColumns() * 2,terrain_.getRows() * 2 );
    }
    tileWidth_ = world_.tileWidth();
    tileHeight_ = world_.tileHeight();
    GameScreens& gS = gameScreens_; // save some screen real estate here ;)
    minimap_.xOrig = gS.sidebar_coords.x1 + gS.minimapclient_coords.x1 +
        (gS.minimapclient_coords.width() - minimap_.image.getWidth()) / 2;
    minimap_.yOrig = gS.sidebar_coords.y1 + gS.minimapclient_coords.y1 +
        (gS.minimapclient_coords.height() - minimap_.image.getHeight()) / 2;
    // Initialize nr of Grid lines in both directions:
    visibleTilesX_ = gameScreens_.map_coords.width() / tileWidth_;
    visibleTilesY_ = gameScreens_.map_coords.height() / tileHeight_;
    if ( visibleTilesX_ > terrain_.getColumns() ) visibleTilesX_ = terrain_.getColumns();
    if ( visibleTilesY_ > terrain_.getRows() ) visibleTilesY_ = terrain_.getRows();

    // go to top left corner of the screen:
    TerrainDrawXOrig_ = 0;
    TerrainDrawYOrig_ = 0;

    // reset default drawing data:
    terrainType_ = T_DEFAULT;
    doodadNr_ = 0; // which doodad the user selected
    doodadMouseCursor_ = false; // whether the active doodad is shown on the mouse cursor location

    // go to the top of the palette list indices:
    basicTerrainPalette_.listIndex = 0;
    doodadPalette_.listIndex = 0;
}

/*
    Makes a vertical list of icons of the available terrain types.
*/
void CampaignEditor::createBasicTerrainPalette()
{
    basicTerrainPalette_.image.setFont( font_ );
    Sprite buffer;
    const int separator = 1;
    const int iconWidth = 2; // expressed in tiles
    const int spriteWidth = gameScreens_.paletteclientwindow_coords.width();
    const int maxHorTiles = (spriteWidth - (2 * separator)) / tileWidth_;
    const int nrOfTerrainTypes = 4;
    // calculate how high the icon list will have to be:
    int nrOfVerticalTiles = iconWidth * nrOfTerrainTypes; // we will create 4 2x2 tile sized icons
    buffer.createEmptySprite(
        spriteWidth,
        separator + nrOfTerrainTypes * separator + nrOfVerticalTiles * tileHeight_
    );
    basicTerrainPalette_.yValues.clear();
    int yStart = separator;
    Sprite terrainIcon;
    terrainIcon.createEmptySprite(
        separator + tileWidth_ * iconWidth + separator,
        separator + tileHeight_ * iconWidth + separator,
        Colors::Black
    );
    for ( int iTerrainType = 0; iTerrainType < nrOfTerrainTypes; iTerrainType++ )
    {
        int tileIndex = iTerrainType * 16; // only works if tile is in 8 x 8 configuration
        if ( iTerrainType == 1 ) // dirty hack for low terrain
        {
            terrainIcon.insertFromSprite( separator             ,separator              ,world_.getTile( tileIndex ) );
            terrainIcon.insertFromSprite( separator + tileWidth_,separator              ,world_.getTile( tileIndex ) );
            terrainIcon.insertFromSprite( separator             ,separator + tileHeight_,world_.getTile( tileIndex ) );
            terrainIcon.insertFromSprite( separator + tileWidth_,separator + tileHeight_,world_.getTile( tileIndex ) );
        }
        else
        {
            terrainIcon.insertFromSprite( separator             ,separator              ,world_.getTile( tileIndex ) );
            terrainIcon.insertFromSprite( separator + tileWidth_,separator              ,world_.getTile( tileIndex + 5 ) );
            terrainIcon.insertFromSprite( separator             ,separator + tileHeight_,world_.getTile( tileIndex + 7 ) );
            terrainIcon.insertFromSprite( separator + tileWidth_,separator + tileHeight_,world_.getTile( tileIndex + 11 ) );
        }
        int zoomDiv = 1;
        const int origTerrainIconWidth = iconWidth * tileWidth_;
        const int origTerrainIconHeight = iconWidth * tileHeight_;
        int terrainIconWidth = origTerrainIconWidth;
        for ( ; terrainIconWidth > spriteWidth; )
        {
            terrainIconWidth /= 2;
            zoomDiv *= 2;
        }
        const int terrainIconHeight = origTerrainIconHeight / zoomDiv;
        const int zoomDivSq = zoomDiv * zoomDiv;
        // draw smaller version (icon) of the doodad onto the icon list sprite
        const int xOffset = (spriteWidth - terrainIconWidth) / 2;
        int finalY = yStart;
        for ( int y = 0; y < origTerrainIconHeight; y += zoomDiv )
        {
            int finalX = xOffset;
            for ( int x = 0; x < origTerrainIconWidth; x += zoomDiv )
            {
                int r = 0;
                int g = 0;
                int b = 0;
                for ( int j = 0; j < zoomDiv; j++ )
                {
                    for ( int i = 0; i < zoomDiv; i++ )
                    {
                        Color c = terrainIcon.getPixel( x + i,y + j );
                        r += c.GetR();
                        g += c.GetG();
                        b += c.GetB();
                    }
                }
                buffer.putPixel(
                    finalX,
                    finalY,
                    Color( r / zoomDivSq,g / zoomDivSq,b / zoomDivSq )
                );
                finalX++;
            }
            finalY++;
        }
        // draw small tile footprint in upper left corner:
        buffer.drawBlock(
            separator,
            separator + yStart,
            separator + iconWidth * 2,
            separator + yStart + iconWidth * 2,
            Colors::Gray
        );
        for ( int j = 0; j < iconWidth; j++ )
        {
            for ( int i = 0; i < iconWidth; i++ )
            {
                buffer.putPixel(
                    1 + separator + i * 2,
                    1 + separator + yStart + j * 2,
                    Colors::Red
                );
            }
        }
        basicTerrainPalette_.yValues.push_back( yStart );
        yStart += separator + terrainIconHeight;
    }
    basicTerrainPalette_.image.createFromSprite(
        buffer,
        Rect( 0,0,buffer.getWidth() - 1,yStart ) );
}

/*
Makes a vertical list of icons of the available doodads.
Design: list of doodad's on black background, separated by a black line
*/
void CampaignEditor::createDoodadPalette()
/*
    Needs failsafe in case of wrong doodad sprites!!
*/
{
    doodadPalette_.image.setFont( font_ );
    doodadPalette_.yValues.clear();
    Sprite buffer;
    const int yOffset = 1;
    const int separator = 1;
    const int spriteWidth = gameScreens_.paletteclientwindow_coords.width();
    const int maxHorTiles = (spriteWidth - (2 * separator)) / tileWidth_;
    // calculate how high the icon list will have to be:
    int nrOfVerticalTiles = 0;
    for ( int i = 0; i < world_.nrOfDoodads(); i++ )
        nrOfVerticalTiles += world_.getDoodad( i ).height();
    if ( nrOfVerticalTiles == 0 )
    {
        doodadPalette_.image.createEmptySprite( 
            spriteWidth,
            FONT_HEIGHT + TEXT_OFFSET * 2 
        );
        doodadPalette_.image.printXY( TEXT_OFFSET,TEXT_OFFSET,"(Empty)" );
        return;
    }
    buffer.createEmptySprite(
        spriteWidth,
        yOffset + world_.nrOfDoodads() * separator + nrOfVerticalTiles * tileHeight_
    );
    int yStart = yOffset;
    for ( int iDoodad = 0; iDoodad < world_.nrOfDoodads(); iDoodad++ )
    {
        int zoomDiv = 1;
        const int origDoodadWidth = world_.getDoodad( iDoodad ).width() * tileWidth_;
        const int origDoodadHeight = world_.getDoodad( iDoodad ).height() * tileHeight_;
        int doodadWidth = origDoodadWidth;
        for ( ; doodadWidth > spriteWidth;)
        {
            doodadWidth /= 2;
            zoomDiv *= 2;
        }
        const int doodadHeight = origDoodadHeight / zoomDiv;
        const int zoomDivSq = zoomDiv * zoomDiv;
        // draw smaller version (icon) of the doodad onto the icon list sprite
        const int xOffset = (spriteWidth - doodadWidth) / 2;
        int finalY = yStart;
        for ( int y = 0; y < origDoodadHeight; y += zoomDiv )
        {
            int finalX = xOffset;
            for ( int x = 0; x < origDoodadWidth; x += zoomDiv )
            {
                int r = 0;
                int g = 0;
                int b = 0;
                for ( int j = 0; j < zoomDiv; j++ )
                {
                    for ( int i = 0; i < zoomDiv; i++ )
                    {
                        Color c = world_.getDoodad( iDoodad ).image().getPixel(  // causes assertion
                            x + i,y + j
                        );
                        r += c.GetR();
                        g += c.GetG();
                        b += c.GetB();
                    }
                }
                buffer.putPixel(
                    finalX,
                    finalY,
                    Color( r / zoomDivSq,g / zoomDivSq,b / zoomDivSq )
                );
                finalX++;
            }
            finalY++;
        }
        // draw small tile footprint in upper left corner:
        buffer.drawBlock(
            separator,
            separator + yStart,
            separator + world_.getDoodad( iDoodad ).width() * 2,
            separator + yStart + world_.getDoodad( iDoodad ).height() * 2,
            Colors::Gray
        );
        for ( int j = 0; j < world_.getDoodad( iDoodad ).height(); j++ )
        {
            for ( int i = 0; i < world_.getDoodad( iDoodad ).width(); i++ )
            {
                buffer.putPixel(
                    1 + separator + i * 2,
                    1 + separator + yStart + j * 2,
                    Colors::Red
                );
            }
        }
        doodadPalette_.yValues.push_back( yStart );
        yStart += separator + doodadHeight;
    }
    doodadPalette_.image.createFromSprite(
        buffer,
        Rect( 0,0,buffer.getWidth() - 1,yStart )
    );
}

void CampaignEditor::initPalettePointers()
{
    switch ( activePalette_ )
    {
        case BASIC_TERRAIN_PALETTE:
        {
            palettePTR_ = &basicTerrainPalette_;
            break;
        }
        case DOODAD_PALETTE:
        {
            palettePTR_ = &doodadPalette_;
            break;
        }
    }
}

void CampaignEditor::redrawPalette()
{
    Graphics& gfx = *gfx_;
    // print title:
    gfx.printXY(
        gameScreens_.sidebar_coords.x1 + gameScreens_.paletteselector_coords.x1 + TEXT_OFFSET,
        gameScreens_.sidebar_coords.y1 + gameScreens_.paletteselector_coords.y1 + TEXT_OFFSET,
        palettetitles[activePalette_]
    );
    // draw palette:
    Palette& palette = *palettePTR_;
    // make sure we do not index an empty vector list:
    if ( palette.yValues.size() == 0 )
    {
        gfx.paintSprite(
            gameScreens_.sidebar_coords.x1 + gameScreens_.paletteclientwindow_coords.x1,
            gameScreens_.sidebar_coords.y1 + gameScreens_.paletteclientwindow_coords.y1,
            palette.image );
        return;
    }
    scrollDownLock_ = false;
    int paletteSpriteMaxHeight = gameScreens_.paletteclientwindow_coords.height();
    if ( paletteSpriteMaxHeight >= palette.image.getHeight() - palette.yValues[palette.listIndex] )
    {
        paletteSpriteMaxHeight = palette.image.getHeight() - palette.yValues[palette.listIndex] - 1;
        scrollDownLock_ = true;
    }
    gfx.paintSpriteSection(
        gameScreens_.sidebar_coords.x1 + gameScreens_.paletteclientwindow_coords.x1,
        gameScreens_.sidebar_coords.y1 + gameScreens_.paletteclientwindow_coords.y1,
        Rect(
            0,
            palette.yValues[palette.listIndex] - 1,
            palette.image.getWidth() - 1,
            palette.yValues[palette.listIndex] - 1 + paletteSpriteMaxHeight - 1
        ),
        palette.image
    );
}

void CampaignEditor::drawDoodadCursorAtLocation()
{
    Graphics& gfx = *gfx_;
    Mouse& mouse = *mouse_;
    int xTileStart = (mouse.GetPosX() - gameScreens_.map_coords.x1) / tileWidth_;
    int yTileStart = (mouse.GetPosY() - gameScreens_.map_coords.y1) / tileHeight_;
    if ( (xTileStart >= visibleTilesX_) || (xTileStart < 0) ) return;
    if ( (yTileStart >= visibleTilesY_) || (yTileStart < 0) ) return;
    int doodadWidth = world_.getDoodad( doodadNr_ ).width();
    int doodadHeight = world_.getDoodad( doodadNr_ ).height();
    int xDoodadOrig = gameScreens_.map_coords.x1 + xTileStart * tileWidth_;
    int yDoodadOrig = gameScreens_.map_coords.y1 + yTileStart * tileHeight_;
    int yTile = yTileStart;
    for ( int j = 0; j < doodadHeight; j++ )
    {
        if ( yTile < visibleTilesY_ )
        {
            int xTile = xTileStart;
            for ( int i = 0; i < doodadWidth; i++ )
            {
                if ( xTile < visibleTilesX_ )
                {
                    Tile terrainElement = terrain_.getElement(
                        TerrainDrawXOrig_ + xTile,
                        TerrainDrawYOrig_ + yTile
                    );
                    bool doodadAlreadyThere = doodadLocationMap_
                        [(TerrainDrawYOrig_ + yTile) * terrain_.getColumns() + TerrainDrawXOrig_ + xTile];
                    Color rasterColor;
                    if ( world_.getDoodad( doodadNr_ ).isCompatible( i,j,terrainElement ) 
                        && (!doodadAlreadyThere) )
                        rasterColor = Colors::Green;
                    else rasterColor = Colors::Red;
                    int xTileOffset = i * tileWidth_;
                    int yTileOffset = j * tileHeight_;
                    gfx.paintSpriteSection(
                        xDoodadOrig + xTileOffset,
                        yDoodadOrig + yTileOffset,
                        Rect(
                            xTileOffset,
                            yTileOffset,
                            xTileOffset + tileWidth_ - 1,
                            yTileOffset + tileHeight_ - 1
                        ),
                        doodadCursorSprite_
                    );
                    // draw raster in red / green color:
                    for ( int n = 0; n < tileHeight_; n++ )
                    {
                        int x = xDoodadOrig + xTileOffset + (n & 0x1);
                        int y = yDoodadOrig + yTileOffset + n;
                        for ( int m = x; m < x + tileWidth_ - 1; m += 2 ) 
                            gfx.PutPixel( m,y,rasterColor );
                    }
                }
                xTile++;
            }
        }
        yTile++;
    }
}

// This function will tell you if a doodad can be placed at a certain location
// on the terrain (if it is compatible). This function does NOT check if another
// doodad is already present though!
bool CampaignEditor::canPlaceDoodadAtLocation( int x,int y,const Doodad& doodad ) const
{
    if ( x + doodad.width() > terrain_.getColumns() ) return false;
    if ( y + doodad.height() > terrain_.getRows() ) return false;
    for ( int j = 0; j < doodad.height(); j++ )
        for ( int i = 0; i < doodad.width(); i++ )
        {
            if ( 
                (!doodad.isCompatible( i,j,terrain_.getElement( x + i,y + j ) )) 
//                || (!doodadLocationMap_[(y + j) * terrain_.getColumns() + x + i]) 
                )
                return false;
        }
    return true;
}

/*
    The terrain & world must be loaded before this function is called.
*/
void CampaignEditor::initDoodadLocationMap()
{
    //if ( doodadLocationMap_ != nullptr ) delete doodadLocationMap_;
    int size = terrain_.getRows() * terrain_.getColumns();
    //doodadLocationMap_ = new bool[size];
    doodadLocationMap_ = std::make_unique<bool[]> ( size );
    for ( int i = 0; i < size; i++ ) doodadLocationMap_[i] = false;
    // now put the doodads in there:
    const std::vector<DoodadLocation>& doodadList = terrain_.getDoodadList();
    for ( int iDoodad = 0; iDoodad < (int)doodadList.size(); iDoodad++ )
    {
        const DoodadLocation& doodadLocation = doodadList[iDoodad];
        if ( !doodadLocation.isUsed ) continue;
        const Doodad& doodad = world_.getDoodad( doodadList[iDoodad].doodadNr );
        int x2 = doodadList[iDoodad].x + doodad.width() - 1;
        int y2 = doodadList[iDoodad].y + doodad.height() - 1;
        for ( int j = doodadList[iDoodad].y; j <= y2; j++ )
            for ( int i = doodadList[iDoodad].x; i <= x2; i++ )
                doodadLocationMap_[j * terrain_.getColumns() + i] = true;
    }
}

bool CampaignEditor::doodadPresentInArea( Rect area )
{
    assert( doodadLocationMap_ != nullptr );
    assert( (area.x1 >= 0) && (area.y1 >= 0) );
    assert( area.x2 < terrain_.getColumns() );
    assert( area.y2 < terrain_.getRows() );
    bool doodadPresent = false;
    for ( int y = area.y1; y <= area.y2; y++ )
        for ( int x = area.x1; x <= area.x2; x++ )
            doodadPresent |= doodadLocationMap_[y * terrain_.getColumns() + x];
    return doodadPresent;
}

/*
This function checks for each doodad if the newly modified terrain is still
compatible with the current doodad and it's location, and disables the
doodad if it can not be placed anymore there were it used to be.
*/
void CampaignEditor::checkDoodads()
{
    const std::vector<DoodadLocation>& doodadList = terrain_.getDoodadList();
    for ( int iDoodad = 0; iDoodad < (int)doodadList.size(); iDoodad++ )
    {
        const DoodadLocation& doodadLocation = doodadList[iDoodad];
        if ( !doodadLocation.isUsed ) continue;
        if ( !canPlaceDoodadAtLocation(
            doodadLocation.x,
            doodadLocation.y,
            world_.getDoodad( doodadLocation.doodadNr ) ) )
        {
            const Doodad& doodad = world_.getDoodad( doodadLocation.doodadNr );
            for ( int j = doodadLocation.y; j < doodadLocation.y + doodad.height(); j++ )
                for ( int i = doodadLocation.x; i < doodadLocation.x + doodad.width(); i++ )
                    doodadLocationMap_[j * terrain_.getColumns() + i] = false;
            terrain_.removeDoodad( iDoodad );
        }
    }
}
