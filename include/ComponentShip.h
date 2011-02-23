#ifndef COMPONENTSHIP_H
#define COMPONENTSHIP_H

#include "CoreEnumsAndClasses.h"
#include "ControlComponent.h"
#include "GraphicsComponent.h"


namespace Attrition
{
    class ComponentShip
    {
        public:
            /** Default constructor */
            //ComponentShip( spriteInfo, hitboxInfo, control, script );
            ComponentShip();
            ComponentShip( ControlComponent *_control, GraphicsComponent *_graphics );
            /** Default destructor */
            virtual ~ComponentShip();

            void time( const Uint32 &_currentTime );
            void handleEvents( const SDL_Event &_event );
            void logic( const Uint32 &_delta );
            void render();
            void reload();


            glm::vec3 velocity, position;
        protected:
        private:
            bool active;
            ControlComponent *control;
            GraphicsComponent *graphics;
    };
}

#endif // COMPONENTSHIP_H
