#ifndef ATTRITIONTESTSTATE_H
#define ATTRITIONTESTSTATE_H

#include <State.h>
#include "OBB.h"
#include "Object3D.h"
#include "VisualHitbox.h"


namespace Attrition
{
    class AttritionTestState : public de::state::State
    {
        public:
            /** Default constructor */
            AttritionTestState();
            /** Default destructor */
            virtual ~AttritionTestState();

            bool handleEvents( const SDL_Event &_event );
            bool logic( const float &_deltaTicks, State* &_nextState, de::state::options &_options );
            void render();
        protected:
        private:
            void reLoadTextures();

            de::collision::OBB hitbox, hitbox2;
            de::collision::VisualHitbox showHitbox, showHitbox2;
            glm::mat4 proView;
            glm::vec3 position;
    };
}

#endif // ATTRITIONTESTSTATE_H
