#include "ShipGraphics.h"
#include "ComponentShip.h"



namespace Attrition
{
    ShipGraphics::ShipGraphics( SpriteInstance *_root )
    {
        sprite = OpaqueSprite( _root, "bullets.bullet1", "SpriteInstance" );
    }

    ShipGraphics::~ShipGraphics()
    {
        //dtor
    }

    void ShipGraphics::logic( ComponentShip &_ship, const Uint32 &_delta )
    {

    }

    void ShipGraphics::render( ComponentShip &_ship )
    {
        sprite.position = _ship.position;
        sprite.position.z -= 1000;
        sprite.addThisFrame();
    }
}
