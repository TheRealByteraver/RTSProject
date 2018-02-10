
#include "GameScreens.h"

void GameScreens::drawScenarioEditor()
{
    // Draw Menu Bar:
    menuBar_.createEmptySprite( menubar_coords.width(),menubar_coords.height() );
    menuBar_.setFont( font_ );
    menuBar_.setFrameWidth( FRAME_WIDTH );
    menuBar_.setFrameColor( menuColor );
    menuBar_.drawNiceBlock( 
        Rect( 0,0,menubar_coords.width() - 1,menubar_coords.height() - 1 )
    );
    
    menuBar_.printXY(
        MENU_BAR_X1 + TEXT_OFFSET,
        MENU_BAR_Y1 + TEXT_OFFSET,
        "Test String" );
    
    // Draw Side Bar:
    sideBar_.createEmptySprite( sidebar_coords.width(),sidebar_coords.height() );
    sideBar_.setFont( font_ );
    sideBar_.setFrameWidth( FRAME_WIDTH );
    sideBar_.setFrameColor( menuColor );
    sideBar_.drawNiceBlock( 
        Rect( 0,0,sidebar_coords.width() - 1,sidebar_coords.height() - 1 ) 
    );
    sideBar_.drawNiceBlockInv( minimap_coords );

    sideBar_.drawNiceBlockInv( paletteselector_coords );
    sideBar_.drawNiceBlockInv( palettewindow_coords );


    /*
    // draw icons:
    sideBar_.printXY(
        FRAME_WIDTH + TEXT_OFFSET,
        MINIMAP_Y2 + FRAME_WIDTH + TEXT_OFFSET,
        "Select terrain:"
    );

    sideBar_.drawBlock( terrainTypeIcon1,Colors::Black );
    sideBar_.drawBlock( terrainTypeIcon2,Colors::Black );
    sideBar_.drawBlock( terrainTypeIcon3,Colors::Black );
    sideBar_.drawBlock( terrainTypeIcon4,Colors::Black );
    */

}
