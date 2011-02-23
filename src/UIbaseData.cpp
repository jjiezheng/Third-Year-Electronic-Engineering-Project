#include "UIbaseData.h"
#include "Math.h"


using namespace de::classes;
using namespace de::graphics;
using namespace de::misc;
using namespace de::events;
using namespace de::theme;
using namespace de::enums;

namespace
{
    static bool GlobleFocusLock(false);
}



textBox::textBox() :
    hoverEvent(false),
    hasFocus(false),
    formFittingBox(false),
    boxLocation( 100, 18, true )
{}

bool textBox::handleEvent( const SDL_Event &_event, de::enums::fboTypes _fbo, bool _actAsButton )
{
    if( GlobleFocusLock && !hasFocus )
    {
        return false;
    }


    bool changeEvent(false);

    if( _event.type == SDL_MOUSEMOTION )
    {
        mousePosition = getMousePosition( _event, true, _fbo );

        if( boxLocation.containsPoint( mousePosition ) )
            hoverEvent = true;
        else
            hoverEvent = false;
    }

    if( _event.type == SDL_MOUSEBUTTONDOWN )
    {
        mousePosition = getMousePosition( _event, false, _fbo );

        if( _event.button.button == SDL_BUTTON_LEFT  )
        {
            if( boxLocation.containsPoint( mousePosition ) )
            {
                //GlobleFocusLock = true;
                hasFocus = true;
            }
            else
            {
                if(hasFocus)
                {
                    GlobleFocusLock = false;
                }
                hasFocus = false;
            }

        }

        mousePosition = getMousePosition( _event, true, _fbo );

        if( boxLocation.containsPoint( mousePosition ) )
            hoverEvent = true;
        else
            hoverEvent = false;
    }

    if( _event.type == SDL_MOUSEBUTTONUP && _actAsButton )
    {
        if( hasFocus && hoverEvent )
        {
            changeEvent = true;
        }

        hasFocus = false;
        hoverEvent = false;
    }

    return changeEvent;
}

void textBox::reLoadTextures()
{
    valueText.reload();
}

void textBox::render( fboTypes _fbo, fboTypes _fbo2, bool _actAsButton )
{
    int height(0), width(0);
    if(formFittingBox)
    {
        std::vector<Sprite>::iterator iter, iterEnd;
        iter = valueText.getSprites().begin();
        iterEnd = valueText.getSprites().end() - 1;

        height = (int)iter->coords.Vertices[3].y - (int)iter->coords.Vertices[0].y;
        width = (int)iterEnd->coords.Vertices[1].x - (int)iter->coords.Vertices[0].x;

        boxLocation.changeRect( width + 5, height + 10 );
    }

    if( _actAsButton )
    {
        if( hasFocus && hoverEvent )
            de::Engine::Graphics().add( boxLocation, DefaultSelectedBoxColour, 10, _fbo );
        else if( hoverEvent )
            de::Engine::Graphics().add( boxLocation, DefaultTextColour, 10, _fbo );
        else
            de::Engine::Graphics().add( boxLocation, DefaultBoxColour, 10, _fbo );
    }
    else
    {
        valueText.render( _fbo2 );
        if( hasFocus )
            de::Engine::Graphics().add( boxLocation, DefaultBoxColour, 10, _fbo );
        else if( !hasFocus )
            de::Engine::Graphics().add( boxLocation, DefaultSelectedBoxColour, 10, _fbo );
    }
}


UIDataEventButtonBF::UIDataEventButtonBF( const std::string &_name,
                                          de::enums::events::SDL_EVENT_CODES _backButton,
                                          de::enums::events::SDL_EVENT_CODES _forwardButton,
                                          const Vector &_position,
                                          const int &_priority, Align _align )
{
    set( _name, _backButton, _forwardButton, _position, _priority, _align );
}

void UIDataEventButtonBF::set( const std::string &_name,
                               de::enums::events::SDL_EVENT_CODES _backButton,
                               de::enums::events::SDL_EVENT_CODES _forwardButton,
                               const Vector &_position,
                               const int &_priority,
                               Align _align )
{
    pointerNotSet = false;
    ValuePtr = NULL;
    backButtonEvent = _backButton;
    forwardButtonEvent = _forwardButton;

    name.set( _name, "System", _position, _priority, _align );
    name.setColour( DefaultTextColour );

    left.set( "<", "System", _position - offset/2, _priority, _align );
    left.setColour( DefaultTextColour );

    right.set( ">", "System", _position + offset/2, _priority, _align );
    right.setColour( DefaultTextColour );


    box.boxLocation = otherBox.boxLocation = Poly( 18, 18, true );
    box.boxLocation.transform( -box.boxLocation.centre + _position - offset/2 );
    otherBox.boxLocation.transform( -otherBox.boxLocation.centre + _position + offset/2 );


}

void UIDataEventButtonBF::set( const std::string &_name,
                               int *_value,
                               const de::classes::Vector &_position,
                               const int &_priority,
                               de::enums::Align _align,
                               bool _write )
{

}


void UIDataEventButtonBF::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    if( box.handleEvent( _event, FBO_UI, true ) )
    {
        pushEvent(backButtonEvent);
    }

    if( otherBox.handleEvent( _event, FBO_UI, true ) )
    {
        pushEvent(forwardButtonEvent);
    }
}

bool UIDataEventButtonBF::logic( const Uint32 &_deltaTicks )
{
    bool wasThereChange(false);
    if( changeEvent )
    {
        changeEvent = false;
        wasThereChange = true;
    }

    if( box.hasFocus && box.hoverEvent )
        left.setColour( DefaultTextColour );
    else if( box.hoverEvent )
        left.setColour( DefaultBoxColour );
    else
        left.setColour( DefaultTextColour );

    if( otherBox.hasFocus && otherBox.hoverEvent )
        right.setColour( DefaultTextColour );
    else if( otherBox.hoverEvent )
        right.setColour( DefaultBoxColour );
    else
        right.setColour( DefaultTextColour );

    return wasThereChange;
}

void UIDataEventButtonBF::reLoadTextures()
{
    name.reload();
    left.reload();
    right.reload();
}

void UIDataEventButtonBF::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    name.render( _topLayer );
    left.render( _topLayer );
    right.render( _topLayer );

    box.render( _bottomLayer, _topLayer, true );
    otherBox.render( _bottomLayer, _topLayer, true );
}





UIDataBoolButton::UIDataBoolButton( const std::string &_name,
                                    bool *_value,
                                    const de::classes::Vector &_position,
                                    const int &_priority,
                                    de::enums::Align _align,
                                    bool _write )
{
    set( _name, _value, _position, _priority, _align, _write );
}

void UIDataBoolButton::set( const std::string &_name, bool *_value, const Vector &_position, const int &_priority, Align _align, bool _write )
{

    write = _write;


    pointerNotSet = false;
    ValuePtr = _value;
    localValue = (*_value);


    box.valueText.set( _name, "System", _position, _priority, _align );
    box.valueText.setColour( DefaultTextColour );

    if( _align == ALIGN_RIGHT )
        box.boxLocation.transform( -box.boxLocation.centre + _position + offset/4 );
    else if( _align == ALIGN_LEFT )
        box.boxLocation.transform( -box.boxLocation.centre + _position - offset/4 );
    else
        box.boxLocation.transform( -box.boxLocation.centre + _position );

}

void UIDataBoolButton::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    localValue = (*ValuePtr);
    changeEvent = box.handleEvent( _event, FBO_UI, true );
}

bool UIDataBoolButton::logic( const Uint32 &_deltaTicks )
{
    bool wasThereChange(false);
    if( changeEvent )
    {
        (*ValuePtr) = true;
        changeEvent = false;
        wasThereChange = true;
    }
    if( box.hasFocus && box.hoverEvent )
        box.valueText.setColour( DefaultTextColour );
    else if( box.hoverEvent )
        box.valueText.setColour( DefaultBoxColour );
    else
        box.valueText.setColour( DefaultTextColour );

    return wasThereChange;
}

void UIDataBoolButton::reLoadTextures()
{
    box.valueText.reload();
}

void UIDataBoolButton::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    box.valueText.render( _topLayer );
    box.render( _bottomLayer, _topLayer, true );
}




UIDataBool::UIDataBool( const std::string &_name, bool *_value, const Vector &_position, const int &_priority, Align _align, bool _write )
{
    set( _name, _value, _position, _priority, _align, _write );
}

void UIDataBool::set( const std::string &_name, bool *_value, const Vector &_position, const int &_priority, Align _align, bool _write )
{
    pointerNotSet = false;
    write = _write;
    ValuePtr = _value;
    localValue = (*_value);

    name.set( _name, "System", _position, _priority, _align );
    box.valueText.set( toString(localValue), "System", _position + offset, _priority, _align );

    name.setColour( DefaultTextColour );
    box.valueText.setColour( DefaultTextColour );

    box.boxLocation.transform( -box.boxLocation.centre + _position + offset/2 );
}

void UIDataBool::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    localValue = (*ValuePtr);
    box.handleEvent( _event, FBO_UI );


    if( _event.type == SDL_KEYDOWN && box.hasFocus && write )
    {
        changeEvent = handleBoolInput( _event, localValue );
    }
}

bool UIDataBool::logic( const Uint32 &_deltaTicks )
{
    bool wasThereChange(false);
    if( localValue != (*ValuePtr) )
    {
        if( changeEvent )
            (*ValuePtr) = localValue;
        else
            localValue = (*ValuePtr);

        box.valueText = toString(localValue);
    }
    return wasThereChange;
}

void UIDataBool::reLoadTextures()
{
    name.reload();
    box.reLoadTextures();
}

void UIDataBool::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    name.render( _topLayer );
    box.render( _bottomLayer, _topLayer );
}




UIDataInt::UIDataInt( const std::string &_name, int *_value, const Vector &_position, const int &_priority, Align _align, bool _write )
{
    set( _name, _value, _position, _priority, _align, _write );
}

void UIDataInt::set( const std::string &_name, int *_value, const Vector &_position, const int &_priority, Align _align, bool _write )
{
    pointerNotSet = false;
    write = _write;
    ValuePtr = _value;
    localValue = (*_value);

    name.set( _name, "System", _position, _priority, _align );
    box.valueText.set( toString( (*ValuePtr) ), "System", _position + offset, _priority, _align );

    name.setColour( DefaultTextColour );
    box.valueText.setColour( DefaultTextColour );

    box.boxLocation.transform( -box.boxLocation.centre + _position + offset/2 );
}

void UIDataInt::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    localValue = (*ValuePtr);
    box.handleEvent( _event, FBO_UI );

    if( _event.type == SDL_KEYDOWN && box.hasFocus  && write )
    {
        changeEvent = handleIntInput( _event, localValue );
    }
}

bool UIDataInt::logic( const Uint32 &_deltaTicks )
{
    bool wasThereChange(false);
    if( localValue != (*ValuePtr) )
    {
        if( changeEvent )
        {
            changeEvent = false;
            wasThereChange = true;

            (*ValuePtr) = localValue;
            box.valueText = toString( localValue );
        }
        else //if( !change )
        {
            localValue = (*ValuePtr);
            box.valueText = toString( localValue );
        }
    }
    return wasThereChange;
}

void UIDataInt::reLoadTextures()
{
    name.reload();
    box.reLoadTextures();
}

void UIDataInt::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    name.render( _topLayer );
    box.render( _bottomLayer, _topLayer );
}



UIDataFloat::UIDataFloat() :
    decimalPoint(false),
    decimalPosition(0)
{}

UIDataFloat::UIDataFloat( const std::string &_name, float *_value, const Vector &_position, const int &_priority, Align _align, bool _write ) :
    decimalPoint(false),
    decimalPosition(0)
{
    set( _name, _value, _position, _priority, _align, _write );
}


void UIDataFloat::set( const std::string &_name, float *_value, const Vector &_position, const int &_priority, Align _align, bool _write )
{
    pointerNotSet = false;
    write = _write;
    ValuePtr = _value;
    localValue = (*_value);

    name.set( _name, "System", _position, _priority, _align );
    box.valueText.set( toString( (*ValuePtr) ), "System", _position + offset, _priority, _align );

    name.setColour( DefaultTextColour );
    box.valueText.setColour( DefaultTextColour );

    box.boxLocation.transform( -box.boxLocation.centre + _position + offset/2 );
}

void UIDataFloat::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    localValue = (*ValuePtr);
    box.handleEvent( _event, FBO_UI );

    if( _event.type == SDL_KEYDOWN && box.hasFocus && write )
    {
        changeEvent = handleFloatInput( _event, localValue, decimalPoint, decimalPosition );
    }
}

bool UIDataFloat::logic( const Uint32 &_deltaTicks )
{
    bool wasThereChange(false);
    if( localValue != (*ValuePtr) )
    {
        if( changeEvent )
        {
            changeEvent = false;
            wasThereChange = true;

            (*ValuePtr) = localValue;
            box.valueText = toString( localValue );
        }
        else //if( !changeEvent )
        {
            localValue = (*ValuePtr);
            box.valueText = toString( localValue );
        }
    }
    return wasThereChange;
}

void UIDataFloat::reLoadTextures()
{
    name.reload();
    box.reLoadTextures();
}

void UIDataFloat::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    name.render( _topLayer );
    box.render( _bottomLayer, _topLayer );
}



UIDataVector::UIDataVector() :
    decimalPointX(false),
    decimalPointY(false),
    decimalPositionX(0),
    decimalPositionY(0)
{}

UIDataVector::UIDataVector( const std::string &_name, Vector *_value, const Vector &_position, const int &_priority, Align _align, bool _write ) :
    decimalPointX(false),
    decimalPointY(false),
    decimalPositionX(0),
    decimalPositionY(0)
{
    set( _name, _value, _position, _priority, _align, _write );
}

void UIDataVector::set( const std::string &_name, Vector *_value, const Vector &_position, const int &_priority, Align _align, bool _write )
{
    pointerNotSet = false;
    write = _write;
    ValuePtr = _value;
    localValue = (*_value);

    name.set( _name, "System", _position, _priority, _align );
    box.valueText.set( toString( (*ValuePtr).x ), "System", _position + offset, _priority, _align );
    boxY.valueText.set( toString( (*ValuePtr).y ), "System", _position + offset*2, _priority, _align );

    name.setColour( DefaultTextColour );
    box.valueText.setColour( DefaultTextColour );
    boxY.valueText.setColour( DefaultTextColour );

    box.boxLocation.transform( -box.boxLocation.centre + _position + offset/2 );
    boxY.boxLocation.transform( -boxY.boxLocation.centre + _position + offset/2*3 );
}

void UIDataVector::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    localValue = (*ValuePtr);

    box.handleEvent( _event, FBO_UI );
    boxY.handleEvent( _event, FBO_UI );

    if( _event.type == SDL_KEYDOWN && box.hasFocus && write )
    {
        changeEvent = handleFloatInput( _event, localValue.x, decimalPointX, decimalPositionX );
    }
    else if( _event.type == SDL_KEYDOWN && boxY.hasFocus && write )
    {
        changeEvent = handleFloatInput( _event, localValue.y, decimalPointY, decimalPositionY );
    }
}

bool UIDataVector::logic( const Uint32 &_deltaTicks )
{
    bool wasThereChange(false);
    if( localValue != (*ValuePtr) )
    {
        if( changeEvent )
        {
            changeEvent = false;
            wasThereChange = true;

            (*ValuePtr) = localValue;
            box.valueText = toString( localValue.x );
            boxY.valueText = toString( localValue.y );
        }
        else //if( !changeEvent )
        {
            localValue = (*ValuePtr);
            box.valueText = toString( localValue.x );
            boxY.valueText = toString( localValue.y );
        }
    }
    return wasThereChange;
}

void UIDataVector::reLoadTextures()
{
    name.reload();
    box.reLoadTextures();
    boxY.reLoadTextures();
}

void UIDataVector::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    name.render( _topLayer );

    box.render( _bottomLayer, _topLayer );
    boxY.render( _bottomLayer, _topLayer );
}



UIDataEnum::UIDataEnum( const std::string &_name,
                            int *_value,
                            const std::vector<int> &_Options,
                            const std::vector<std::string> &_OptionsNames,
                            const Vector &_position,
                            const int &_priority,
                            Align _align,
                            bool _write )
{
    set( _name, _value, _Options, _OptionsNames, _position, _priority, _align, _write );
}

void UIDataEnum::set( const std::string &_name,
                      int *_value,
                      const std::vector<int> &_Options,
                      const std::vector<std::string> &_OptionsNames,
                      const Vector &_position,
                      const int &_priority,
                      Align _align,
                      bool _write )
{
    pointerNotSet = false;

    write = _write;
    ValuePtr = _value;
    localValue = (*_value);

    name.set( _name, "System", _position, _priority, _align );
    name.setColour( DefaultTextColour );

    box.boxLocation.transform( -box.boxLocation.centre + _position + offset/2 );

    Options = _Options;
    OptionsNames = _OptionsNames;



    if( Options.size() != OptionsNames.size() )
        de::io::error << "Impending crash in UIDataEnum... \n";

    for( iterOptions = Options.begin(), iterOptionsNames = OptionsNames.begin(); iterOptions < Options.end(); ++iterOptions, ++iterOptionsNames )
    {
        if( (*iterOptions) == localValue )
        {
            box.valueText.set( (*iterOptionsNames), "System", _position + offset, _priority, _align );
            box.valueText.setColour( DefaultTextColour );
            return;
        }
    }
    box.valueText.set( "", "System", _position + offset, _priority, _align );
    box.valueText.setColour( DefaultTextColour );




    iterOptions = Options.begin();
    iterOptionsNames = OptionsNames.begin();

}

void UIDataEnum::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    localValue = (*ValuePtr);
    box.handleEvent( _event, FBO_UI );


    if( _event.type == SDL_KEYDOWN && box.hasFocus && write )
    {
        //If the key is a space
        if( _event.key.keysym.unicode == (Uint16)'+' )
        {
            if( iterOptions < Options.end() - 1 )
            {
                ++iterOptions;
                ++iterOptionsNames;
            }

            changeEvent = true;
            localValue = (*iterOptions );
        }
        else if( _event.key.keysym.unicode == (Uint16)'-' )
        {
            if( iterOptions > Options.begin() )
            {
                --iterOptions;
                --iterOptionsNames;
            }

            changeEvent = true;
            localValue = (*iterOptions );
        }
    }
}

bool UIDataEnum::logic( const Uint32 &_deltaTicks )
{
    bool wasThereChange(false);
    if( localValue != (*ValuePtr) )
    {
        if( changeEvent )
        {
            changeEvent = false;
            wasThereChange = true;

            (*ValuePtr) = localValue;
            box.valueText = (*iterOptionsNames);
        }
        else //if( !change )
        {
            localValue = (*ValuePtr);
            box.valueText = (*iterOptionsNames);
        }
    }
    return wasThereChange;
}

void UIDataEnum::reLoadTextures()
{
    name.reload();
    box.reLoadTextures();
}

void UIDataEnum::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    name.render( _topLayer );
    box.render( _bottomLayer, _topLayer );
}




UIDataString::UIDataString() :
    usingOptions(false)
{}

UIDataString::UIDataString( const std::string &_name,
                            std::string *_string,
                            const Vector &_position,
                            const int &_priority,
                            Align _align,
                            bool _write )
{
    set( _name, _string, _position, _priority, _align, _write );
}

UIDataString::UIDataString( const std::string &_name,
                            std::string *_string,
                            const std::vector<std::string> &_Options,
                            const std::vector<std::string> &_OptionsNames,
                            const Vector &_position,
                            const int &_priority,
                            Align _align,
                            bool _write )
{
    set( _name, _string, _Options, _OptionsNames, _position, _priority, _align, _write );
}


void UIDataString::set( const std::string &_name,
                       std::string *_string,
                       const Vector &_position,
                       const int &_priority,
                       Align _align,
                       bool _write )
{
    pointerNotSet = false;
    write = _write;
    ValuePtr = _string;
    localValue = (*_string);

    name.set( _name, "System", _position, _priority, _align );
    box.valueText.set( (*ValuePtr), "System", _position + offset, _priority, _align );

    name.setColour( DefaultTextColour );
    box.valueText.setColour( DefaultTextColour );

    box.boxLocation.transform( -box.boxLocation.centre + _position + offset/2 );
}

void UIDataString::set( const std::string &_name,
                        std::string *_string,
                        const std::vector<std::string> &_Options,
                        const std::vector<std::string> &_OptionsNames,
                        const Vector &_position,
                        const int &_priority,
                        Align _align,
                        bool _write )
{
    set( _name, _string, _position, _priority, _align, _write );

    Options = _Options;
    usingOptions = true;

    for( iterOptions = Options.begin(); iterOptions < Options.end(); ++iterOptions )
    {
        if( (*iterOptions) == (*ValuePtr) )
            return;
    }

    iterOptions = Options.begin();

}

void UIDataString::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    localValue = (*ValuePtr);
    box.handleEvent( _event, FBO_UI );


    if( _event.type == SDL_KEYDOWN && box.hasFocus && !usingOptions && write )
    {
        changeEvent = handleStringInput( _event, localValue );
    }

    else if( _event.type == SDL_KEYDOWN && box.hasFocus && usingOptions && write )
    {
        //If the key is a space
        if( _event.key.keysym.unicode == (Uint16)'+' )
        {
            if( iterOptions < Options.end() - 1 )
                ++iterOptions;

            changeEvent = true;
            localValue = (*iterOptions );
        }
        else if( _event.key.keysym.unicode == (Uint16)'-' )
        {
            if( iterOptions > Options.begin() )
                --iterOptions;

            changeEvent = true;
            localValue = (*iterOptions );
        }
    }
}

bool UIDataString::logic( const Uint32 &_deltaTicks )
{
    bool wasThereChange(false);
    if( localValue != (*ValuePtr) )
    {
        if( changeEvent )
        {
            changeEvent = false;
            wasThereChange = true;

            (*ValuePtr) = localValue;
            box.valueText = localValue;
        }
        else //if( !changeEvent )
        {
            localValue = (*ValuePtr);
            box.valueText = localValue;
        }
    }
    return wasThereChange;
}

void UIDataString::reLoadTextures()
{
    name.reload();
    box.reLoadTextures();
}

void UIDataString::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    name.render( _topLayer );
    box.render( _bottomLayer, _topLayer );
}






UIContainer::UIContainer()
{
    end = Objects.end();
}

UIContainer::~UIContainer()
{
    Objects.clear();
}

void UIContainer::add( UIInterface *_baseData )
{
    Objects.push_back( _baseData );
    end = Objects.end();
}

void UIContainer::clear()
{
    Objects.clear();
}

unsigned int UIContainer::size()
{
    return Objects.size();
}

void UIContainer::handleEvent( const SDL_Event &_event )
{
    for ( iter = Objects.begin(); iter < end; ++iter )
        iter->handleEvent( _event );
}

bool UIContainer::logic( const Uint32 &_deltaTicks )
{
    bool returnValue(false);
    for ( iter = Objects.begin(); iter < end; ++iter )
        returnValue = returnValue | iter->logic( _deltaTicks );

    return returnValue;
}

void UIContainer::reLoadTextures()
{
    for ( iter = Objects.begin(); iter < end; ++iter )
        iter->reLoadTextures();
}

void UIContainer::render( fboTypes _bottomLayer, fboTypes _topLayer )
{
    for ( iter = Objects.begin(); iter < end; ++iter )
        iter->render( _bottomLayer, _topLayer );
}


