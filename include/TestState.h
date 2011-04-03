#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "State.h"
#include "Haptics.h"
#include "modelObject.h"


class TestState : public de::state::State
{
    public:
        TestState();
        virtual ~TestState();

        bool handleEvents( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks, State* &_nextState, de::state::options &_options );

        void render();

    private:
        void reLoadTextures();
		

};

#endif // TESTSTATE_H