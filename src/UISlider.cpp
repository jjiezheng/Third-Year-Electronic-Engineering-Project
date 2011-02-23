#include "UISlider.h"

using namespace de::classes;
using namespace de::graphics;
using namespace de::theme;
using namespace de::misc;
using namespace de::events;
using namespace de::enums;


UISlider::UISlider() :
    slider( 10, 18, true ),
    pointerNotSet(true),
    hoverEvent(false),
    draggingEvent(false),
    value(NULL)
{
    name.setColour( DefaultSelectedBoxColour );
    here.setColour( DefaultSelectedBoxColour );
}

UISlider::~UISlider()
{
    //dtor
}


void UISlider::set( const std::string &_name,
                    int *_value,
                    int *_max,
                    const int &_size,
                    const Vector &_position,
                    const int &_priority )
{
    pointerNotSet = false;
    value = _value;
    localValue = (*_value);

    startPoint = _position.x - _size/2;
    endPoint = _position.x + _size/2;
    length = (float)_size;

    maxValue = _max;


    line.add( Vector(startPoint, _position.y ), DefaultSelectedBoxColour );
    line.add( Vector(endPoint, _position.y ), DefaultSelectedBoxColour );

    slider.transform( -slider.centre + Vector(startPoint, _position.y) );

    name.set( _name, "System", Vector(startPoint-15, _position.y), _priority, ALIGN_LEFT );
    here.set( toString(localValue), "System", Vector(startPoint, _position.y + 20), _priority, ALIGN_CENTRE );
}


void UISlider::handleEvent( const SDL_Event &_event )
{
    if(pointerNotSet)
        return;

    if( _event.type == SDL_MOUSEMOTION )
    {
        hoverEvent = false;
        mousePosition = getMousePosition( _event, true, FBO_UI );

        if( slider.containsPoint( mousePosition ) )
        {
            hoverEvent = true;
        }
    }
    if( _event.type == SDL_MOUSEBUTTONDOWN && _event.button.button == SDL_BUTTON_LEFT )
    {
        mousePosition = getMousePosition( _event, false, FBO_UI );

        if( slider.containsPoint( mousePosition ) )
        {
             draggingEvent = true;
        }
    }
    if( _event.type == SDL_MOUSEBUTTONUP  )
    {
        draggingEvent = false;
    }
}


void UISlider::logic( Uint32 _deltaTicks )
{
    if(pointerNotSet)
        return;

    if(draggingEvent)
    {
        slider.transform( -slider.centre + Vector( mousePosition.x, slider.centre.y ) );

        if( slider.centre.x > endPoint )
        {
            slider.transform( -slider.centre + Vector( endPoint, slider.centre.y ) );
        }
        else if( slider.centre.x < startPoint )
        {
            slider.transform( -slider.centre + Vector( startPoint, slider.centre.y ) );
        }

        float ratio = ( slider.centre.x - startPoint)/length;
        (*value) = (int)(*maxValue)*ratio;
        localValue = (*value);

        here = toString( (*value) );
        here = Vector(slider.centre.x, slider.centre.y + 20);
    }
/*
    if(localValue != (*value) )
    {
        localValue = (*value);
        float ratio = (*maxValue)/localValue;
        Vector position((endPoint - startPoint)*ratio, slider.centre.y );

        slider.transform( -slider.centre + position );

        here = toString( (*value) );
        here = Vector(slider.centre.x, slider.centre.y + 20);
    }
*/
}


void UISlider::reLoadTextures()
{
    if(pointerNotSet)
        return;

    name.reload();
    here.reload();
}

void UISlider::render( fboTypes _fbo, fboTypes _fbo2  )
{
    if(pointerNotSet)
        return;

    name.render( _fbo2 );
    de::Engine::Graphics().add( line, _fbo2 );

    if(hoverEvent && !draggingEvent)
    {
        here.render( _fbo2 );
        de::Engine::Graphics().add( slider, DefaultBoxColour, 10, _fbo );
    }
    else if(draggingEvent)
    {
        here.render( _fbo2 );
        de::Engine::Graphics().add( slider, DefaultTextColour, 10, _fbo );
    }
    else
    {
        de::Engine::Graphics().add( slider, DefaultSelectedBoxColour, 10, _fbo );
    }
}



