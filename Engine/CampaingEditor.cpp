#include "CampaignEditor.h"

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

    // initialize drawing coordinates & // allocate memory for the minimap sprite:
    initMapCoords();

    // draw (prepare) the screens that are used by the CampaignEditor onto sprites 
    // stored in memory:
    gameScreens_.drawScenarioEditor(); 
    // And we are done:
    isInitialized_ = true;

    // debug: load test terrain
    error = loadTerrain( "testterrain.ini" );
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
    // Must be done each time the terrain is modified. Checks if every single 
    // doodad is still in a valid location:
    checkDoodads();
    initMapCoords();
    createDoodadIconList();
    return error;
}

void CampaignEditor::draw()
{
    Graphics& gfx = *gfx_;
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
    drawTerrainCursor();
    if ( isGridVisible_ ) drawTerrainGrid();
    drawMiniMap();

    /*
    // debug:
    gfx.paintSprite(
        gameScreens_.terrainTypeIcon1AbsCoords.x1,
        gameScreens_.terrainTypeIcon1AbsCoords.y1,
        world_.getTile( T_LOW_WATER )
    );
    gfx.paintSprite(
        gameScreens_.terrainTypeIcon2AbsCoords.x1,
        gameScreens_.terrainTypeIcon2AbsCoords.y1,
        world_.getTile( T_LOW )
    );
    gfx.paintSprite(
        gameScreens_.terrainTypeIcon3AbsCoords.x1,
        gameScreens_.terrainTypeIcon3AbsCoords.y1,
        world_.getTile( T_HIGH )
    );
    gfx.paintSprite(
        gameScreens_.terrainTypeIcon4AbsCoords.x1,
        gameScreens_.terrainTypeIcon4AbsCoords.y1,
        world_.getTile( T_HIGH_WATER )
    );
    */
    /*
    gfx.drawBox( gameScreens.terrainTypeIcon1AbsCoords,Colors::Blue );
    gfx.drawBox( gameScreens.terrainTypeIcon2AbsCoords,Colors::Green );
    gfx.drawBox( gameScreens.terrainTypeIcon3AbsCoords,Colors::LightGreen );
    gfx.drawBox( gameScreens.terrainTypeIcon4AbsCoords,Colors::LightBlue );
    */
    /* debug:
    std::string s( "# of doodadd's: " );
    s += std::to_string( world.nrOfDoodads() );
    gfx.printXY( 300,3,s.c_str() );
    */
}

void CampaignEditor::handleInput()
{
    Graphics& gfx = *gfx_;
    Mouse& mouse = *mouse_;
    int mX = mouse.GetPosX();
    int mY = mouse.GetPosY();
    /*
    // debug:
    std::stringstream s;
    s << "Mouse is at pos " << mX << "," << mY;
    gfx.printXY( 100,3,s.str().c_str() );
    // end debug
    */

    // Scrolling function:
    if ( mouse.isInArea( gameScreens_.scrollMapLeft ) && (TerrainDrawXOrig_ > 0) )
        TerrainDrawXOrig_--;
    if ( mouse.isInArea( gameScreens_.scrollMapRight ) &&
        (TerrainDrawXOrig_ < terrain_.getColumns() - visibleTilesX_) )
        TerrainDrawXOrig_++;
    if ( mouse.isInArea( gameScreens_.scrollMapUp ) && (TerrainDrawYOrig_ > 0) )
        TerrainDrawYOrig_--;
    if ( mouse.isInArea( gameScreens_.scrollMapDown ) &&
        (TerrainDrawYOrig_ < terrain_.getRows() - visibleTilesY_) )
        TerrainDrawYOrig_++;
    // drawing function:
    if ( mouse.LeftIsPressed() )
    {
        if ( mouse.isInArea( gameScreens_.map_coords ) )
        {
            int tileX = (mX - gameScreens_.map_coords.x1) / tileWidth_ + TerrainDrawXOrig_;
            int tileY = (mY - gameScreens_.map_coords.y1) / tileHeight_ + TerrainDrawYOrig_;
            if ( (tileX < terrain_.getColumns()) &&
                (tileY < terrain_.getRows()) )
            {
                if ( tileX & 0x1 ) tileX--;
                if ( tileY & 0x1 ) tileY--;
                Rect redraw = terrain_.drawTerrain( tileX,tileY,terrainType_ );
                checkDoodads();
                /*
                // redraw modified terrain (on minimap):
                for ( int y = redraw.y1; y < redraw.y2; y++ )
                {
                    for ( int x = redraw.x1; x < redraw.x2; x++ )
                    {
                        
                        //gfxTerrain.insertFromSprite(
                        //x * world.tileWidth(),
                        //y * world.tileHeight(),
                        //world.getTile( terrain.getElement( x,y ) ) );
                        //miniMap_.putPixel( x,y,world_.getAvgColor( terrain_.getElement( x,y ) ) );
                    }
                }
                */
            }
        } else if ( mouse.isInArea( gameScreens_.terrainTypeIcon1AbsCoords ) )
        {
            terrainType_ = T_LOW_WATER;
        } else if ( mouse.isInArea( gameScreens_.terrainTypeIcon2AbsCoords ) )
        {
            terrainType_ = T_LOW;
        } else if ( mouse.isInArea( gameScreens_.terrainTypeIcon3AbsCoords ) )
        {
            terrainType_ = T_HIGH;
        } else if ( mouse.isInArea( gameScreens_.terrainTypeIcon4AbsCoords ) )
        {
            terrainType_ = T_HIGH_WATER;
        }
    }
    // debug:
    /*
    CreateDefaultSprites test;
    test.createDesertWorld();
    //gfx.paintSprite( -1,21,test.getSpriteLibrary() );
    gfx.paintSprite( 10,31 + 400,test.getSpriteLibrary() );
    */

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
    for ( int doodadNr = 0; doodadNr < doodadList.size(); doodadNr++ )
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
    int mX = mouse.GetPosX();
    int mY = mouse.GetPosY();
    if ( mouse.isInArea( gameScreens_.map_coords ) )
    {
        int curX = (mX - gameScreens_.map_coords.x1) / tileWidth_;
        int curY = (mY - gameScreens_.map_coords.y1) / tileHeight_;
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
    switch ( miniMapZoom_ )
    {
        case 1: // one pixel per 4 tiles
        {
            int mmpY = 0;
            for ( int y = 0; y < terrain_.getRows(); y += 2 )
            {
                int mmpX = 0;
                for ( int x = 0; x < terrain_.getColumns(); x += 2 )
                {
                    Color c0 = world_.getAvgColor( terrain_.getElement( x    ,y     ) );
                    Color c1 = world_.getAvgColor( terrain_.getElement( x + 1,y     ) );
                    Color c2 = world_.getAvgColor( terrain_.getElement( x    ,y + 1 ) );
                    Color c3 = world_.getAvgColor( terrain_.getElement( x + 1,y + 1 ) );
                    miniMap_.putPixel( mmpX,mmpY,Color( 
                        (c0.GetR() + c1.GetR() + c2.GetR() + c3.GetR()) >> 2,
                        (c0.GetG() + c1.GetG() + c2.GetG() + c3.GetG()) >> 2,
                        (c0.GetB() + c1.GetB() + c2.GetB() + c3.GetB()) >> 2 ) );
                    mmpX++;
                }
                mmpY++;
            }
            break;
        }
        case 2: // one pixel per tile
        {
            int i = 0;
            for ( int y = 0; y < terrain_.getRows(); y++ )
            {
                for ( int x = 0; x < terrain_.getColumns(); x++ )
                {
                    miniMap_.putPixel( x,y,world_.getAvgColor( terrain_.getElement( i ) ) );
                    i++;
                }
            }
            break;
        }
        case 3: // two pixels per tile
        {
            for ( int j = 0; j < terrain_.getRows(); j++ )
                for ( int i = 0; i < terrain_.getColumns(); i++ )
                    for ( int p = 0; p < 4; p++ )
                        miniMap_.putPixel(
                            (i << 1) + (p & 1),
                            (j << 1) + (p >> 1),
                            world_.getAvgColors_2x2( terrain_.getElement( i,j ) )[p] );
            break;
        }
    }
    Graphics& gfx = *gfx_;
    gfx.paintSprite( miniMapXOrig_,miniMapYOrig_,miniMap_ );
    drawMiniMapCursor();
}
// draws mini map cursor
void CampaignEditor::drawMiniMapCursor()
{
    // draw the minimap highlighted area delimiter / cursor:
    switch ( miniMapZoom_ )
    {
        case 1:
        {
            miniMapCursor_.x1 = miniMapXOrig_ + TerrainDrawXOrig_ / 2;
            miniMapCursor_.y1 = miniMapYOrig_ + TerrainDrawYOrig_ / 2;
            miniMapCursor_.x2 = miniMapCursor_.x1 + visibleTilesX_ / 2 - 1;
            miniMapCursor_.y2 = miniMapCursor_.y1 + visibleTilesY_ / 2 - 1;
            break;
        }
        case 2:
        {
            miniMapCursor_.x1 = miniMapXOrig_ + TerrainDrawXOrig_;
            miniMapCursor_.y1 = miniMapYOrig_ + TerrainDrawYOrig_;
            miniMapCursor_.x2 = miniMapCursor_.x1 + visibleTilesX_ - 1;
            miniMapCursor_.y2 = miniMapCursor_.y1 + visibleTilesY_ - 1;
            break;
        }
        case 3:
        {
            miniMapCursor_.x1 = miniMapXOrig_ + TerrainDrawXOrig_ * 2;
            miniMapCursor_.y1 = miniMapYOrig_ + TerrainDrawYOrig_ * 2;
            miniMapCursor_.x2 = miniMapCursor_.x1 + visibleTilesX_ * 2 - 1;
            miniMapCursor_.y2 = miniMapCursor_.y1 + visibleTilesY_ * 2 - 1;
            break;
        }
    }
    Graphics& gfx = *gfx_;
    gfx.drawBox( miniMapCursor_,Colors::White );
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
        miniMapZoom_ = 1;
        miniMap_.createEmptySprite( terrain_.getColumns() / 2,terrain_.getRows() / 2 );
    } else if ( terrain_.getColumns() > 64 )
    {
        miniMapZoom_ = 2;
        miniMap_.createEmptySprite( terrain_.getColumns(),terrain_.getRows() );
    } else
    {
        miniMapZoom_ = 3;
        miniMap_.createEmptySprite( terrain_.getColumns() * 2,terrain_.getRows() * 2 );
    }
    tileWidth_ = world_.tileWidth();
    tileHeight_ = world_.tileHeight();
    GameScreens& gS = gameScreens_; // save some screen real estate here ;)
    miniMapXOrig_ = gS.sidebar_coords.x1 + gS.minimapclient_coords.x1 +
        (gS.minimapclient_coords.width() - miniMap_.getWidth()) / 2;
    miniMapYOrig_ = gS.sidebar_coords.y1 + gS.minimapclient_coords.y1 +
        (gS.minimapclient_coords.height() - miniMap_.getHeight()) / 2;
    // Initialize nr of Grid lines in both directions:
    visibleTilesX_ = gameScreens_.map_coords.width() / tileWidth_;
    visibleTilesY_ = gameScreens_.map_coords.height() / tileHeight_;
    if ( visibleTilesX_ > terrain_.getColumns() ) visibleTilesX_ = terrain_.getColumns();
    if ( visibleTilesY_ > terrain_.getRows() ) visibleTilesY_ = terrain_.getRows();
}

/*
    Makes a vertical list of icons of the available doodads.
    Design: list of doodad's on black background, separated by a black line,
    surrounded by a white frame?
*/
void CampaignEditor::createDoodadIconList()
{
    const int spriteWidth = (gameScreens_.sideBar().getWidth() - FRAME_WIDTH * 2);
    const int maxHorTiles = spriteWidth / tileWidth_;
    const int yOffset = 1;
    const int separator = 1;
    // calculate how high the icon list will have to be:
    int nrOfVerticalTiles = 0;
    for ( int i = 0; i < world_.nrOfDoodads(); i++ )
        nrOfVerticalTiles += world_.getDoodad( i ).height();
    doodadIconList_.createEmptySprite(
        spriteWidth,
        yOffset + world_.nrOfDoodads() + nrOfVerticalTiles * tileHeight_
    );
    doodadIconListYvalues_.clear();
    int yStart = yOffset;
    for ( int iDoodad = 0; iDoodad < world_.nrOfDoodads(); iDoodad++ )
    {
        int zoomDiv = 1;
        int doodadWidth = world_.getDoodad( iDoodad ).width() * tileWidth_;
        for ( ; doodadWidth > spriteWidth ;)
        {
            doodadWidth /= 2;
            zoomDiv *= 2;
        }

        // ...
        // draw smaller version (icon) of the doodad onto the icon list sprite
        // ...

        doodadIconListYvalues_.push_back( yStart );
        yStart += separator + (world_.getDoodad( iDoodad ).height() * tileHeight_) / zoomDiv;
    }
}

// This function will tell you if a doodad can be placed at a certain location
// on the terrain (if it is compatible):
bool CampaignEditor::canPlaceDoodadAtLocation( int x,int y,const Doodad& doodad ) const
{
    if ( x + doodad.width() >= terrain_.getColumns() ) return false;
    if ( y + doodad.height() >= terrain_.getRows() ) return false;
    for ( int j = 0; j < doodad.height(); j++ )
        for ( int i = 0; i < doodad.width(); i++ )
            if ( !doodad.isCompatible( i,j,terrain_.getElement( x + i,y + j ) ) )
                return false;
    return true;
}
/*
This function checks for each doodad if the newly modified terrain is still
compatible with the current doodad and it's location, and disables the
doodad if it can not be placed anymore there were it used to be.
*/
void CampaignEditor::checkDoodads()
{
    const std::vector<DoodadLocation>& doodadList = terrain_.getDoodadList();
    for ( int iDoodad = 0; iDoodad < doodadList.size(); iDoodad++ )
    {
        const DoodadLocation& doodadLocation = doodadList[iDoodad];
        if ( !doodadLocation.isUsed ) continue;
        if ( !canPlaceDoodadAtLocation(
            doodadLocation.x,
            doodadLocation.y,
            world_.getDoodad( doodadLocation.doodadNr ) ) )
        {
            terrain_.removeDoodad( iDoodad );
        }
    }
}
