#include "UIspline.h"

using namespace de::classes;
using namespace de::graphics;
using namespace de::core;
using namespace de::theme;
using namespace de::misc;
using namespace de::events;
using namespace de::enums;

UIspline::UIspline()
    :pointerNotSet(true), draggingEvent(false), addEvent(false), deleteEvent(false), hoverEvent(false), boxLocation( 20, 20, true )
{
    VectorPointsPtr = NULL;
    line.shader = de::Engine::Resources().getShader( "Untextured" );
    line.priority = 6;
}


UIspline::UIspline( std::vector<de::classes::Vector> *_VectorPointsPtr )
    :pointerNotSet(false),draggingEvent(false), addEvent(false), deleteEvent(false), boxLocation( 20, 20, true ), VectorPointsPtr( _VectorPointsPtr)
{
    for( iterVector = (*VectorPointsPtr).begin(); iterVector < (*VectorPointsPtr).end(); ++iterVector )
    {
        boxLocation.transform( -boxLocation.centre + (*iterVector) );
        BoxPoints.push_back( LinedBox( boxLocation ) );
    }
    line.priority = 6;
}



UIspline::~UIspline()
{
    //dtor
}


void UIspline::setPoints( std::vector<de::classes::Vector> *_VectorPointsPtr )
{
    pointerNotSet = false;
    VectorPointsPtr = _VectorPointsPtr;

    BoxPoints.clear();
    for( iterVector = (*VectorPointsPtr).begin(); iterVector < (*VectorPointsPtr).end(); ++iterVector )
    {
        boxLocation.transform( -boxLocation.centre + (*iterVector) );
        BoxPoints.push_back( LinedBox( boxLocation ) );
    }
}



void UIspline::handleEvent( const SDL_Event &_event )
{
    if( pointerNotSet )
        return;

    if( _event.type == SDL_MOUSEMOTION )
    {
        hoverEvent = false;

        mousePosition = getMousePosition( _event, true, FBO_GAME );
        boxLocation.transform( -boxLocation.centre + mousePosition );
        if( !draggingEvent )
        {
            for ( iterLinedBox = BoxPoints.begin(), iterVector = (*VectorPointsPtr).begin(); iterLinedBox < BoxPoints.end(); ++iterLinedBox, ++iterVector )
            {
                if( iterLinedBox->getPoly().containsPoint( mousePosition ) )
                {
                    activeBox = iterLinedBox;
                    activePoint = iterVector;

                    hoverEvent = true;
                }
            }
        }
    }



    if( _event.type == SDL_MOUSEBUTTONDOWN )
    {
        mousePosition = getMousePosition( _event, false, FBO_GAME );

        if( _event.button.button == SDL_BUTTON_LEFT && !draggingEvent )
        {
            for ( iterLinedBox = BoxPoints.begin(), iterVector = (*VectorPointsPtr).begin(); iterLinedBox < BoxPoints.end(); ++iterLinedBox, ++iterVector )
            {
                if( iterLinedBox->getPoly().containsPoint( mousePosition ) )
                {
                    activeBox = iterLinedBox;
                    activePoint = iterVector;

                    draggingEvent = true;
                }
            }
        }



        if( _event.button.button == SDL_BUTTON_RIGHT )
        {
            addEvent = true;
        }

        if( _event.button.button == SDL_BUTTON_MIDDLE )
        {
            deleteEvent = true;
        }



    }

    if( _event.type == SDL_MOUSEBUTTONUP )
    {
        if( _event.button.button == SDL_BUTTON_LEFT )
        {
            draggingEvent = false;
        }
    }
}

bool UIspline::logic( Uint32 _deltaTicks )
{
    bool wasThereChange(false);
    if( pointerNotSet )
        return wasThereChange;



    if( draggingEvent )
    {
        wasThereChange = true;

        activeBox->update( boxLocation );
        (*activePoint) = mousePosition;
    }

    if( addEvent )
    {
        addEvent = false;
        wasThereChange = true;

        boxLocation.transform( -boxLocation.centre + mousePosition );
        (*VectorPointsPtr).push_back( mousePosition );
        BoxPoints.push_back( LinedBox( boxLocation ) );
    }

    if( deleteEvent )
    {
        deleteEvent = false;
        wasThereChange = true;

        for ( iterLinedBox = BoxPoints.begin(), iterVector = (*VectorPointsPtr).begin(); iterLinedBox < BoxPoints.end(); ++iterLinedBox, ++iterVector )
        {
            if( iterLinedBox->getPoly().containsPoint( mousePosition ) )
            {
                iterLinedBox = BoxPoints.erase( iterLinedBox );
                iterVector = (*VectorPointsPtr).erase( iterVector );
            }
        }
    }

    Poly temp = boxLocation;
    for ( iterLinedBox = BoxPoints.begin(), iterVector = (*VectorPointsPtr).begin(); iterLinedBox < BoxPoints.end(); ++iterLinedBox, ++iterVector )
    {
        temp.transform( -temp.centre + (*iterVector) );
        iterLinedBox->update( temp );
    }



    getSplinePath( line, (*VectorPointsPtr) );

    return wasThereChange;
}

void UIspline::reLoadTextures()
{
    if( pointerNotSet )
        return;

    line.shader = de::Engine::Resources().getShader( "Untextured" );
}

void UIspline::render( fboTypes _fbo )
{
    if( pointerNotSet )
        return;


    de::Engine::Graphics().add( line, _fbo );

    if( !(draggingEvent || hoverEvent) )
        de::Engine::Graphics().add( boxLocation, Colour( DefaultSelectedBoxColour.r, DefaultSelectedBoxColour.g, DefaultSelectedBoxColour.b, 0.3f ), 10, _fbo );

    for ( iterLinedBox = BoxPoints.begin(); iterLinedBox < BoxPoints.end(); ++iterLinedBox )
    {
        if( (iterLinedBox == activeBox) && draggingEvent )
            iterLinedBox->render( Colour( DefaultTextColour.r, DefaultTextColour.g, DefaultTextColour.b, 0.6f ), _fbo );
        else if( (iterLinedBox == activeBox) && hoverEvent )
            iterLinedBox->render( Colour( DefaultBoxColour.r, DefaultBoxColour.g, DefaultBoxColour.b, 0.6f ), _fbo );
        else
            iterLinedBox->render( Colour( DefaultSelectedBoxColour.r, DefaultSelectedBoxColour.g, DefaultSelectedBoxColour.b, 0.6f ), _fbo );
    }
}







inline void UIspline::getSplinePath( Line &_path, std::vector<Vector> &_Points )
{
    _path.clear();

    std::vector<Vector>::iterator iterVector = _Points.begin();
    Vector Position;
    float location = 0.0;

    while( iterVector + 3  < _Points.end() )
    {

        if( location > 1 )
        {
            location = 0;
            ++iterVector;
        }

        pointOnCurve( Position, location, (*iterVector), (*(iterVector + 1)), (*(iterVector + 2)), (*(iterVector + 3)) );
        _path.add( Position, Colour( 0,0,0.0,1) );


        location += 0.01f;
    }

}
