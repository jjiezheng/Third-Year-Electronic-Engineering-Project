#include "ComponentShip.h"



namespace Attrition
{
    ComponentShip::ComponentShip()
    {
        active = false;
        control = NULL;
        graphics = NULL;
    }
    ComponentShip::ComponentShip( ControlComponent *_control, GraphicsComponent *_graphics)
    {
        active = true;
        control = _control;
        graphics = _graphics;
    }

    ComponentShip::~ComponentShip()
    {
        if(active)
        {
            //if(control != NULL) delete control;
            //if(graphics != NULL) delete graphics;
        }
    }

    void ComponentShip::time( const Uint32 &_currentTime )
    {

    }

    void ComponentShip::handleEvents( const SDL_Event &_event )
    {
        control->handleEvents( *this, _event );
    }

    void ComponentShip::logic( const Uint32 &_delta )
    {
        control->logic( *this, _delta );
        graphics->logic( *this, _delta );
    }

    void ComponentShip::render()
    {
        graphics->render(*this);
    }

    void ComponentShip::reload()
    {
    }
}
