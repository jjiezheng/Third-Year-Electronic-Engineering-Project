#ifndef SHIPGRAPHICS_H
#define SHIPGRAPHICS_H
#include "GraphicsComponent.h"
#include "SpriteInstance.h"

namespace Attrition
{
    class ShipGraphics : public GraphicsComponent
    {
        public:
            /** Default constructor */
            ShipGraphics( SpriteInstance *_root );
            /** Default destructor */
            virtual ~ShipGraphics();
            void logic( ComponentShip &_ship, const Uint32 &_delta );
            void render( ComponentShip &_ship );
        protected:
        private:
            OpaqueSprite sprite;
    };
}

#endif // SHIPGRAPHICS_H
