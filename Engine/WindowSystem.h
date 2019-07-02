#pragma once
#include "assert.h"
#include "sprite.h"
#include "Font.h"
#include "GameScreens.h" // for MENU_COLOR
#include "globals.h"     // for Rect class


/*
Source: https://www.youtube.com/watch?v=4Vvc1YurUYA&index=35&list=PLqCJpWy5Fohfil0gvjzgdV4h29R9kDKtZ

Inheritance rules:

- a derived class should always be a kind-of base class, i.e. a Ford Taunus is
  a kind-of Car
- "Private" members of a base class are not accessible to derived versions of
  that class
  "Protected" members of a base class are accessible to derived versions of the
  class but not to users (instantiations) of the class
- A pointer or reference to a derived class can serve as a pointer/ reference 
  to a base class, but not the other way around
- adding a pure virtual member function to a class makes the class "Abstract",
  meaning no instantiations of the base class can exist
- adding the "override" specifier at the end of the function prototype (like 
  const) tells intellisense you are overriding a virtual base class member 
  function, use it for clarity / bug avoidance

- When constructing a derived class, the constructor of the base class is 
  called first, then the constructors of the variables of the base class, and
  finally the constructor of the derived class. The destructor goes the other
  way around

- Making a function virtual causes run-time polymorphism, meaning that a 
  base class-type pointer still calls the derived function if the pointer
  points to a derived class, when dereferencing that pointer that is.
  In this case the base class destructor MUST be declared as virtual!

- dynamic_cast returns nullptr if the base class pointer you do the cast on 
  is in reality pointing to a different derived class type than the derived
  class type you try to cast it towards

- static_cast is used if you are certain to what type of derived class the
  base class pointer is pointing (faster)

- reinterpret_cast is used to change the type (like classic typecasting),
  no limitations (try to avoid if possible)

- const_cast is used to make a const pointer not const basically (avoid it)
source: https://www.youtube.com/watch?v=g-NGBFCn3co&index=37&list=PLqCJpWy5Fohfil0gvjzgdV4h29R9kDKtZ


Syntax:

- include <typeinfo> for RTTI typeid() function
- A derived class is declared as:
    class derived_class : public base_class {}
- A derived member function is declared as:
    void function( int argument ) override;
- A pure virtual function is declared as below (in the base class):
    virtual void function( int argument ) = 0;


===============================================================================

SMART POINTERS:

source: https://www.youtube.com/watch?v=WCTiFVlQFZU&index=39&list=PLqCJpWy5Fohfil0gvjzgdV4h29R9kDKtZ

- Basic rule: always use smart pointers, new & delete are outdated
- As arguments to a function, pass unique pointers by reference, unless you need to 
- transfer ownership, then transfer by value

- include <memory> for unique pointer use
- Create a pointer to a class:
    std::unique_ptr<classtype> pointerToclasstype = std::make_unique<classtype>();

- Create a pointer to an array of a class with a dimension of nElements:
std::unique_ptr<classtype> pointerToclasstype = std::make_unique<classtype[]>( nElements );

- want to return the pointer itself? use pointerToclasstype.get();


- shared_pointer and weak_pointer are advanced topics, usually not needed
*/

class WinDim
{ 
public:
    WinDim() 
    {
        xOrig_ = 0;
        yOrig_ = 0;
        width_ = 0;
        height_ = 0;
    }
    WinDim( int xOrig,int yOrig,int width,int height ) :
        xOrig_(xOrig),
        yOrig_(yOrig),
        width_(width),
        height_(height) 
    {
        assert( width_ > 0 );
        assert( height_ > 0 );
    }
    WinDim( Rect r ) :
        xOrig_( r.x1 ),
        yOrig_( r.y1 ),
        width_( r.width() ),
        height_( r.height() )
    {
        assert( width_ > 0 );
        assert( height_ > 0 );
    }
    void    init( int xOrig,int yOrig,int width,int height )
    {
        assert( width > 0 );
        assert( height > 0 );
        xOrig_ = xOrig;
        yOrig_ = yOrig;
        width_ = width;
        height_ = height;
    }
    void    init( Rect r )
    {
        assert( r.width() > 0 );
        assert( r.height() > 0 );
        xOrig_ = r.x1;
        yOrig_ = r.y1;
        width_ = r.width();
        height_ = r.height();
    }
    void    moveTo( int xOrig,int yOrig )
    {
        xOrig_ = xOrig;
        yOrig_ = yOrig;
    }
    void    reSize( int width,int height )
    {
        assert( width > 0 );
        assert( height > 0 );
        width_ = width;
        height_ = height;
    }
    bool    contains( int x,int y ) const 
    {
        if( (x >= xOrig_) &&
            (y >= yOrig_) &&
            (x < xOrig_ + width_ ) &&
            (y < yOrig_ + height_ ) 
            ) return true;
        return false;
    }
    Rect    rect() const
    {
        return Rect(
            xOrig_,
            yOrig_,
            xOrig_ + width_ - 1,
            yOrig_ + height_ - 1
        );
    }
    int     xOrig() const { return xOrig_; }
    int     yOrig() const { return yOrig_; }
    int     width() const { return width_; }
    int     height() const { return height_; }
private:
    int xOrig_;
    int yOrig_;
    int width_;
    int height_;
};

class WinElement
{
public:
    WinElement() {}
    WinElement( WinDim& dimConstraint ) :
        dimConstraint_( dimConstraint )
    {
        dimConstraintInitialized_ = true;
    }
    void            setDimConstraint( WinDim& dimConstraint )
    {
        dimConstraint_ = dimConstraint;
        dimConstraintInitialized_ = true;
    }
    void            setFont( Font* font ) { font_ = font; }
    void            moveTo( int x,int y ) { dim_.moveTo( x,y ); }
    WinDim          getDimensions() const { return dim_; }
    const Sprite&   image() const { return image_; }
    virtual void    handleInput( int absMouseX,int absMouseY ) = 0;
protected:
    bool        dimConstraintInitialized_ = false;
    Font       *font_ = nullptr;
    WinDim      dim_;
    WinDim      dimConstraint_;
    Sprite      image_;
};

class TextPanel : public WinElement
{
public:
    TextPanel() : WinElement() {}
    TextPanel( WinDim& dimConstraint ) : WinElement( dimConstraint ) {}
    void    init( const char *stringList[] )
    {
        std::vector<std::string>    tmpStrList;
        for ( int i = 0; stringList[i] != nullptr; i++ )
            tmpStrList.push_back( std::string( stringList[i] ) );
        init( tmpStrList );
    }
    void    init( std::vector<std::string>& stringList )
    {
        assert( dimConstraintInitialized_ );
        stringList_.clear();
        widestString_ = 0;
        for ( auto s : stringList )
        {
            int slen = (int)s.length();
            if ( slen > widestString_ ) widestString_ = slen;
            stringList_.push_back( s );
        }
        draw();
    }
    void    handleInput( int absMouseX,int absMouseY )
    {
        if ( !dim_.contains( absMouseX,absMouseY ) ) return;
        int mX = absMouseX - dim_.xOrig();
        int mY = absMouseY - dim_.yOrig();
    }
private:
    void    draw()
    {
        assert( font_ != nullptr );
        Font& font = *font_;
        int maxNrLines = (int)stringList_.size();
        int maxNrChars = widestString_;
        int windowFat = TEXT_OFFSET * 2;
        int wdwDesiredWidth = windowFat + maxNrChars * font.width();
        int wdwDesiredHeight = windowFat + maxNrLines * font.height();
        if ( wdwDesiredWidth > dimConstraint_.width() )
        {
            maxNrChars = (dimConstraint_.width() - windowFat) / font.width();
            wdwDesiredWidth = windowFat + maxNrChars * font.width();
        }
        if ( wdwDesiredHeight > dimConstraint_.height() )
        {
            maxNrLines = (dimConstraint_.height() - windowFat) / font.height();
            wdwDesiredHeight = windowFat + maxNrLines * font.height();
        }
        // start drawing:
        dim_.init( 0,0,wdwDesiredWidth,wdwDesiredHeight );
        image_.createEmptySprite( wdwDesiredWidth,wdwDesiredHeight,MENU_COLOR );
        image_.setFont( &font );
        int x = TEXT_OFFSET;
        int y = TEXT_OFFSET;
        for ( int j = 0; j < maxNrLines; j++ )
        {
            if( (int)stringList_[j].length() <= maxNrChars )
                image_.printXY( x,y,stringList_[j].c_str() );
            else
            {
                std::string s( stringList_[j] );
                s.resize( maxNrChars );
                image_.printXY( x,y,s.c_str() );
            }
            y += font.height();
        }
    }
private:
    int                         widestString_;
    std::vector<std::string>    stringList_;
};

class VerticalRadiobuttonGroup : public WinElement
{
public:
    VerticalRadiobuttonGroup() : WinElement() {}
    VerticalRadiobuttonGroup( WinDim& dimConstraint ) : WinElement( dimConstraint ) {}
    void    init( std::string& optionsTitle,const char *optionsList[] )
    {
        std::vector<std::string>    tmpOptList;
        for ( int i = 0; optionsList[i] != nullptr; i++ )
            tmpOptList.push_back( std::string( optionsList[i] ) );
        init( optionsTitle,tmpOptList );
    }
    void    init( std::string& optionsTitle,std::vector<std::string>& optionsList )
    {
        assert( dimConstraintInitialized_ );
        selectedOption_ = -1; // no default option
        optionsTitle_ = optionsTitle;
        optionsList_.clear();
        for ( auto i : optionsList )
        {
            Option_ option;
            option.name = i;
            optionsList_.push_back( option );
        }
        draw();
        isInitialized_ = true;
    }
    void    handleInput( int absMouseX,int absMouseY )
    {
        assert( isInitialized_ );
        if ( !dim_.contains( absMouseX,absMouseY ) ) return;
        int mX = absMouseX - dim_.xOrig();
        int mY = absMouseY - dim_.yOrig();
        int index = 0;
        for ( auto& s: optionsList_ )
        {
            if( s.mouseBox.contains( mX,mY ) )
            {
                if ( selectedOption_ >= 0 )
                    image_.drawRadioButton(
                        radioBtnX_,
                        radioBtnFirstY_ + selectedOption_ * radioBtnOptionHeight_,
                        radioBtnWidth_,
                        false
                    );
                selectedOption_ = index;
                image_.drawRadioButton(
                    radioBtnX_,
                    radioBtnFirstY_ + selectedOption_ * radioBtnOptionHeight_,
                    radioBtnWidth_,
                    true
                );
                break;
            }
            index++;
        }
    }
    bool    hasValidInput() { return selectedOption_ >= 0; }
    int     getInput() { return selectedOption_; }
private:
    void    draw()
    {
        assert( font_ != nullptr );
        Font& font = *font_;
        const Color boxColor( 0xFF,0xFF,0xFF );
        // space reserved for the line around the options
        // for the line with the title (top):
        const int lineSpaceTop = font.height() + TEXT_OFFSET * 2;
        // for the side & bottom lines:
        const int lineSpaceSideBottom = font.width();
        radioBtnWidth_ = font.width();
        radioBtnOptionHeight_ = font.height() + TEXT_OFFSET;
        const int xTitle = lineSpaceSideBottom + font.width();
        const int yTitle = (lineSpaceTop - font.height()) / 2;
        const int xFirstOption = lineSpaceSideBottom;
        const int yFirstOption = lineSpaceTop;
        // find the longest radiobutton option string:
        int maxStrLength = 0;
        for ( auto s : optionsList_ )
        {
            int sLength = (int)s.name.length();
            if ( sLength > maxStrLength ) maxStrLength = sLength;
        }
        const int titleMaxWidth = xTitle + (int)optionsTitle_.length() * font.width()
            + font.width() + lineSpaceSideBottom;
        const int radioBtnMaxWidth = xFirstOption + radioBtnWidth_ +
            (maxStrLength + 1) * font.width() + lineSpaceSideBottom;
        int wdwDesiredWidth;
        if ( titleMaxWidth > radioBtnMaxWidth ) wdwDesiredWidth = titleMaxWidth;
        else wdwDesiredWidth = radioBtnMaxWidth;
        int wdwDesiredHeight = yFirstOption +
            (int)optionsList_.size() * radioBtnOptionHeight_ +
            lineSpaceSideBottom;
        bool windowTooWide = false;
        bool windowTooHigh = false;
        if ( wdwDesiredWidth > dimConstraint_.width() ) windowTooWide = true;
        if ( wdwDesiredHeight > dimConstraint_.height() ) windowTooHigh = true;
        // check if the entries in the options list are too wide:
        if ( windowTooWide )      
        {
            int windowFat = lineSpaceSideBottom * 2 + radioBtnWidth_;
            maxStrLength = (dimConstraint_.width() - windowFat) / font.width();
            if ( maxStrLength < 0 ) maxStrLength = 0; // fail safe?
            if ( maxStrLength > 7 )
            {
                for ( auto& listItem : optionsList_ )
                    if ( (int)listItem.name.length() > maxStrLength )
                    {
                        listItem.name.resize( maxStrLength - 3 );
                        listItem.name.append( "..." );
                    }
            } else { 
                for ( auto& listItem : optionsList_ )
                    if ( (int)listItem.name.length() > maxStrLength )
                        listItem.name.resize( maxStrLength );
            }
            wdwDesiredWidth = windowFat + maxStrLength * font.width();
            // now check the title width:
            if ( titleMaxWidth > wdwDesiredWidth )
            {
                int titleMaxLength = (wdwDesiredWidth - windowFat) / font.width() - 2;
                if ( titleMaxLength < 0 ) titleMaxLength = 0;
                optionsTitle_.resize( titleMaxLength );
            }
        }
        if ( windowTooHigh )
        {
            int windowFat = wdwDesiredHeight - (int)optionsList_.size() * radioBtnOptionHeight_;
            int maxNrListItems = (dimConstraint_.height() - windowFat) / radioBtnOptionHeight_;
            if ( maxNrListItems < 0 ) maxNrListItems = 0; // fail safe?
            optionsList_._Pop_back_n( optionsList_.size() - maxNrListItems );
            wdwDesiredHeight = windowFat + maxNrListItems * radioBtnOptionHeight_;
        }
        // start drawing:
        dim_.init( 0,0,wdwDesiredWidth,wdwDesiredHeight );
        image_.setFont( &font );
        image_.setFrameColor( MENU_COLOR );
        image_.createEmptySprite( wdwDesiredWidth,wdwDesiredHeight );
        image_.drawBlock( Rect(
            0,
            0,
            wdwDesiredWidth - 1,
            wdwDesiredHeight - 1 ),
            image_.getFrameColor()
        );
        const int yRadioBtnOffset = (font.height() - radioBtnWidth_) / 2 - 1;
        const int x1Line = lineSpaceSideBottom / 2;
        const int y1Line = lineSpaceTop / 2;
        const int x2Line = wdwDesiredWidth - 1 - lineSpaceSideBottom / 2;
        const int y2Line = wdwDesiredHeight - 1 - lineSpaceSideBottom / 2;
        image_.drawBox( x1Line,y1Line,x2Line,y2Line,boxColor );
        int optionNr = 0;
        radioBtnX_ = xFirstOption;
        radioBtnFirstY_ = yFirstOption + yRadioBtnOffset;
        for ( auto& s : optionsList_ )
        {
            int yDelta = optionNr * radioBtnOptionHeight_;
            int xText = xFirstOption + radioBtnWidth_ + 2;
            int yText = yFirstOption + yDelta;
            image_.drawRadioButton( 
                radioBtnX_,
                radioBtnFirstY_ + yDelta,
                radioBtnWidth_,false
            );
            image_.printXY( 
                xText,
                yText,
                s.name.c_str() 
            );
            s.mouseBox = Rect(
                xFirstOption,
                yText,
                xText + (int)s.name.length() * font.width(),
                yText + radioBtnOptionHeight_
            );
            optionNr++;
        }
        image_.drawHorLine(
            xTitle,
            y1Line,
            xTitle + (int)optionsTitle_.length() * font.width(),
            image_.getFrameColor() 
        );
        image_.printXY( xTitle,yTitle,optionsTitle_.c_str() );
    }
private:
    bool                        isInitialized_ = false;
    std::string                 optionsTitle_;
    struct Option_ 
    { 
        std::string name;
        Rect        mouseBox;
    };
    std::vector<Option_>        optionsList_;
    int                         selectedOption_;
    // for radio button drawing:
    int                         radioBtnX_;
    int                         radioBtnFirstY_;
    int                         radioBtnWidth_;
    int                         radioBtnOptionHeight_;
};

/*
    Warning: the program will crash if there is not enough space to put all
    the buttons
*/
class ButtonList : public WinElement
{
public:
    ButtonList() : WinElement() {}
    ButtonList( WinDim& dimConstraint ) : WinElement( dimConstraint ) {}
    void    init( const char *buttonList[] )
    {
        std::vector<std::string>    tmpBtnList;
        for ( int i = 0; buttonList[i] != nullptr; i++ )
            tmpBtnList.push_back( std::string( buttonList[i] ) );
        init( tmpBtnList );
    }
    void    init( std::vector<std::string>& buttonList )
    {
        assert( dimConstraintInitialized_ );
        clickedButton_ = -1;
        buttonList_.clear();
        for ( auto i : buttonList )
        {
            Button_ button;
            button.name = i;
            buttonList_.push_back( button );
        }
        draw();
        isInitialized_ = true;
    }
    void    handleInput( int absMouseX,int absMouseY )
    {
        assert( isInitialized_ );        
        int mX = absMouseX - dim_.xOrig();
        int mY = absMouseY - dim_.yOrig();
        int index = 0;
        for ( auto& s : buttonList_ )
        {
            if ( s.dim.contains( mX,mY ) )
            {
                // maybe show a pushed down button animation here
                // image_.drawBox( s.dim,Colors::Red );
                clickedButton_ = index;
                break;
            }
            index++;
        }
    }
    bool    hasValidInput() 
    { 
        assert( isInitialized_ );
        return clickedButton_ >= 0; 
    }
    int     getInput()
    { 
        assert( hasValidInput() );
        return clickedButton_; 
    }
    void    invalidateInput() { clickedButton_ = -1; }
private:
    void    draw()
    {
        assert( buttonList_.size() > 0 );
        assert( font_ != nullptr );
        Font& font = *font_;
        const int btnFrameWidth = FRAME_WIDTH;
        const int interBtnSpace = FRAME_WIDTH * 2;
        const int textSpace = TEXT_OFFSET;
        const int btnHeight = (btnFrameWidth + textSpace) * 2 + font.height();
        const int btnMinWidth = (btnFrameWidth + textSpace) * 2;
        const int btnMaxStrLength = 
            (dimConstraint_.width() - btnMinWidth - 2 * interBtnSpace) / font.width();
        assert( btnMaxStrLength > 1 );
        if ( btnMaxStrLength <= 1 ) return; // panic
        int totalWidth = interBtnSpace;
        int index = 0;
        int nrBtnRows = 1;
        int nrBtnForThisRow = 0;
        int widestBtnRowWidth = 0;
        std::vector<int>    btnWidths( buttonList_.size() );
        std::vector<int>    btnsPerRow;         
        std::vector<int>    RowLengths;
        bool sizePanic = false;
        for ( auto& btn : buttonList_ )
        {
            int btnStrLength = (int)btn.name.length();
            if ( btnStrLength > btnMaxStrLength )
            {
                btn.name.resize( btnMaxStrLength );
                btnStrLength = btnMaxStrLength;
            }
            int btnSize = btnMinWidth + btnStrLength * font.width();
            btnWidths[index++] = btnSize;
            if ( totalWidth > widestBtnRowWidth ) widestBtnRowWidth = totalWidth;
            totalWidth += btnSize + interBtnSpace;            
            if ( totalWidth > dimConstraint_.width() )
            {
                btnsPerRow.push_back( nrBtnForThisRow );
                RowLengths.push_back( totalWidth - (btnSize + interBtnSpace) );
                totalWidth = btnSize + interBtnSpace * 2;
                nrBtnForThisRow = 1;
                nrBtnRows++;
            } else { 
                nrBtnForThisRow++;
            }
        }
        if ( nrBtnForThisRow > 0 )
        {
            btnsPerRow.push_back( nrBtnForThisRow );
            RowLengths.push_back( totalWidth );
            if ( totalWidth > widestBtnRowWidth ) widestBtnRowWidth = totalWidth;
        }
        const int wdwDesiredWidth = widestBtnRowWidth;
        const int wdwDesiredHeight = interBtnSpace + (btnHeight + interBtnSpace) * nrBtnRows;
        if ( wdwDesiredWidth > dimConstraint_.width() ||
            wdwDesiredHeight > dimConstraint_.height() ) sizePanic = true;
        assert( !sizePanic );
        if ( sizePanic ) return; // panic
        // start drawing:
        dim_.init( 0,0,wdwDesiredWidth,wdwDesiredHeight );
        image_.setFont( &font );
        image_.setFrameColor( MENU_COLOR );
        image_.createEmptySprite( wdwDesiredWidth,wdwDesiredHeight );
        image_.drawBlock( Rect(  
            0,
            0,
            wdwDesiredWidth - 1,
            wdwDesiredHeight - 1 ),
            image_.getFrameColor()
        );
        index = 0;
        int y = interBtnSpace;
        for ( int rowNr = 0; rowNr < nrBtnRows; rowNr++ )
        {
            int x = (wdwDesiredWidth - RowLengths[rowNr]) / 2 + interBtnSpace;
            for ( int btnNr = 0; btnNr < btnsPerRow[rowNr]; btnNr++ )
            {
                int x1 = x;
                x += btnWidths[index];
                int x2 = x - 1;
                x += interBtnSpace;
                buttonList_[index].dim = Rect( x1,y,x2,y + btnHeight );
                image_.drawButton( buttonList_[index].dim );
                image_.printXY(
                    buttonList_[index].dim.x1 + FRAME_WIDTH + TEXT_OFFSET - 1,
                    buttonList_[index].dim.y1 + FRAME_WIDTH + TEXT_OFFSET,
                    buttonList_[index].name.c_str()
                );
                index++;
            }
            y += btnHeight + interBtnSpace;
        }
    }
private:
    bool                        isInitialized_ = false;    
    struct Button_
    {
        std::string name;
        Rect        dim;
    };
    std::vector<Button_>        buttonList_;
    int                         clickedButton_;
};

#define SCROLL_BAR_MIN_SIZE                 (3 + (4 + FRAME_WIDTH) * 2)
#define SCROLL_BAR_HORIZONTAL               true
#define SCROLL_BAR_VERTICAL                 false
#define SCROLL_BAR_DEFAULT_BUTTON_WIDTH     16

class ScrollBar : public WinElement
{
public:
    ScrollBar( WinDim& dimConstraint ) : WinElement( dimConstraint ) {}
    void    init( int width,int size,bool isHorizontal )
    {
        assert( size >= SCROLL_BAR_MIN_SIZE * 2 );
        assert( width >= SCROLL_BAR_MIN_SIZE );
        if ( isHorizontal )
        {
            assert( width <= dimConstraint_.height() );
            assert( size <= dimConstraint_.width() );
        } else { 
            assert( width <= dimConstraint_.width() );
            assert( size <= dimConstraint_.height() );
        }
        isHorizontal_ = isHorizontal;
        size_ = size;
        width_ = width;
        draw();
    }
private:
    void    draw()
    {
        int frameSize = 1;
        int btnWidth = width_ - frameSize * 2;
        if ( btnWidth > SCROLL_BAR_DEFAULT_BUTTON_WIDTH )
            btnWidth = SCROLL_BAR_DEFAULT_BUTTON_WIDTH;
        width_ = btnWidth + frameSize * 2;
        if ( isHorizontal_ ) dim_.init( 0,0,size_,width_ );
        else                 dim_.init( 0,0,width_,size_ );

        image_.createEmptySprite( dim_.width(),dim_.height(),MENU_COLOR );
        image_.setFrameColor( MENU_COLOR );
        image_.drawBox(
            0,
            0,
            dim_.width() - 1,
            dim_.height() - 1,
            Colors::Black
        );
        
        image_.drawNiceBlock( Rect(
            frameSize,
            frameSize,
            frameSize + btnWidth - 1,
            frameSize + btnWidth - 1 )
        );
        image_.drawNiceBlock( Rect(
            dim_.width() - btnWidth - frameSize,
            dim_.height() - btnWidth - frameSize,
            dim_.width() - 1 - frameSize,
            dim_.height() - 1 - frameSize )
        );

        
        int buttonOffset = frameSize;
        cursorSize_ = cursorAreaSize_ * size_ / clientWindowSize_;

    }
private:
    // "size" rather than width or height cause scrollbar can be 
    // both horizontally and vertically oriented
    int     size_;
    int     width_;
    bool    isHorizontal_;
    int     clientWindowSize_; // the window we are navigating w/ the scroll bar
    int     cursorAreaSize_;   // basically width_ or height_ minus the point buttons
                               // maybe make scroll bar without point buttons? --> No
    int     cursorSize_;
    bool    isCursorVisible_;
/*
    cursorSize_ = cursorAreaSize_ * size_  / clientWindowSize_;

*/
};


// ============================================================================
// end of "primitive" WinElement classes
// start of composite WinElement classes
// ============================================================================

// this class is basically a horizontal bar of WinElement ptr type variables
class WinElementBar : public WinElement
{
public:
    WinElementBar() {}
    void        clear() 
    { 
        winElementList_.clear(); 
        dim_.moveTo( 0,0 );
        dim_.reSize( 1,1 ); // cannot resize to 0,0
    }
    void        addWinElement( WinElement& winElement )
    {
        assert( &winElement != nullptr );
        winElementList_.push_back( &winElement );
        int index = (int)winElementList_.size() - 1;
        if ( index <= 0 )
        {
            dim_.reSize(
                winElementList_[0]->getDimensions().xOrig() +
                winElementList_[0]->getDimensions().width(),
                winElementList_[0]->getDimensions().yOrig() +
                winElementList_[0]->getDimensions().height()
            );
        } else {
            winElementList_[index]->moveTo(
                winElementList_[index - 1]->getDimensions().rect().x2 + 1 +
                winElementList_[index]->getDimensions().xOrig(),
                winElementList_[index]->getDimensions().yOrig()
            );
            int nH = winElement.getDimensions().yOrig() +
                     winElement.getDimensions().height();
            if ( nH < dim_.height() ) nH = dim_.height();
            dim_.reSize( winElement.getDimensions().rect().x2 + 1,nH );
        }
    }
    void        handleInput( int absMouseX,int absMouseY )
    {
        assert( winElementList_.size() > 0 );
        int mX = absMouseX - dim_.xOrig();
        int mY = absMouseY - dim_.yOrig();
        for ( auto& s : winElementList_ )
        {
            if ( s->getDimensions().contains( mX,mY ) )
            {
                s->handleInput( mX,mY );
                break;
            }
        }
    }
    const std::vector<WinElement *>& winElementList()
    {
        return winElementList_;
    }
private:
    std::vector<WinElement *> winElementList_;
};

// this class is basically a vertical list of WinElementBar ptr's
class WinElementBarList : public WinElement
{
public:
    WinElementBarList() {}
    void        clear()
    {
        winElementBarList_.clear();
        dim_.moveTo( 0,0 );
        dim_.reSize( 1,1 ); // cannot resize to 0,0
    }
    void        addWinElementBar( WinElementBar& winElementBar )
    {
        assert( &winElementBar != nullptr );
        winElementBarList_.push_back( &winElementBar );
        int index = (int)winElementBarList_.size() - 1;
        if ( index <= 0 )
        {
            dim_.reSize(
                winElementBarList_[0]->getDimensions().xOrig() +
                winElementBarList_[0]->getDimensions().width(),
                winElementBarList_[0]->getDimensions().yOrig() +
                winElementBarList_[0]->getDimensions().height()
            );
        } else {
            winElementBarList_[index]->moveTo(                
                winElementBarList_[index]->getDimensions().xOrig(),
                winElementBarList_[index - 1]->getDimensions().rect().y2 + 1 +
                winElementBarList_[index]->getDimensions().yOrig()
            );
            int nW = winElementBar.getDimensions().xOrig() +
                     winElementBar.getDimensions().width();
            if ( nW < dim_.width() ) nW = dim_.width();
            dim_.reSize( nW,winElementBar.getDimensions().rect().y2 + 1 );
        }
    }
    void        handleInput( int absMouseX,int absMouseY )
    {
        assert( winElementBarList_.size() > 0 );
        int mX = absMouseX - dim_.xOrig();
        int mY = absMouseY - dim_.yOrig();
        for ( auto& s : winElementBarList_ )
        {
            if ( s->getDimensions().contains( mX,mY ) )
            {
                s->handleInput( mX,mY );
                break;
            }
        }
    }
    const std::vector<WinElementBar *>& winElementBarList()
    {
        return winElementBarList_;
    }
private:
    std::vector<WinElementBar *> winElementBarList_;
};