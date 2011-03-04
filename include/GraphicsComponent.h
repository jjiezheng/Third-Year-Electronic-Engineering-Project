#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include "CoreEnumsAndClasses.h"

namespace Attrition
{
    class ComponentShip;
    class GraphicsComponent
    {
        public:
            /** Default destructor */
            virtual ~GraphicsComponent() {};
            virtual void logic( ComponentShip &_ship, const Uint32 &_delta ) = 0;
            virtual void render( ComponentShip &_ship ) = 0;
        protected:
        private:
    };
}


#endif // GRAPHICSCOMPONENT_H'
