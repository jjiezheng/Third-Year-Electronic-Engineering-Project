#ifndef CONTROLCOMPONENT_H
#define CONTROLCOMPONENT_H

#include "CoreEnumsAndClasses.h"


namespace Attrition
{
    class ComponentShip;
    class ControlComponent
    {
        public:
            /** Default destructor */
            virtual ~ControlComponent() {};
            virtual void handleEvents( ComponentShip &_ship, const SDL_Event &_event ) = 0;
            virtual void logic( ComponentShip &_ship, const Uint32 &_delta ) = 0;
        protected:
        private:
    };
}

#endif // CONTROLCOMPONENT_H
