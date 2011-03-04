#ifndef PIXELSTORM_H_
#define PIXELSTORM_H_


#include "State.h"
#include "Overlay.h"

#include "CoreEngine.h"
#include "PerfTimer.h"


namespace Attrition
{
    class DifferenceEngine
    {
        public:
            de::state::State *currentState, *nextState;
            de::state::Overlay *overlay;

            DifferenceEngine();
            ~DifferenceEngine();

            void changeState();
            void setNextState( de::state::State *_nextState );
            bool running();
            void runCurrentState();

        private:
            void initBase();

            void localHandleEvent( const SDL_Event &_event );
            void handleEvents();
            void logic();
            void render();

        private:
            lua_State *myLuaState;
            SDL_Event event;
            de::time::MainTimer Delta;
            de::time::PerfTimer perfTimer;
            std::vector<SDL_Joystick*> Sticks;
            de::state::options Option;

            std::string lastError;
            bool failBit, exit;
    };

    extern DifferenceEngine *Game;
}

#endif //PIXELSTORM_H_
