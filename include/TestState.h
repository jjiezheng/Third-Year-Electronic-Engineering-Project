#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "State.h"
#include "Haptics.h"
#include "modelObject.h"
/*#include "HL\hl.h"
#include "HD\hd.h"*/

class TestState : public de::state::State
{
    public:
        /** Default constructor */
        TestState();
        /** Default destructor */
        virtual ~TestState();

        bool handleEvents( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks, State* &_nextState, de::state::options &_options );

        void render();

    private:
        void reLoadTextures();
		
		modelObject mesh;
		/*
		HHD deviceHandle;
		HHLRC hapticsContext;
		HLuint hapticsShape;
		*/
};

#endif // TESTSTATE_H