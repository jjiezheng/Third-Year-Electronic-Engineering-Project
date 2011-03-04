#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "State.h"
#include "Object3D.h"
#include "StringObject.h"
#include "Haptics.h"
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
		/*
		HHD deviceHandle;
		HHLRC hapticsContext;
		HLuint hapticsShape;
		*/
		StringObject textytext;
		de::classes::SpriteString otherText;

        std::vector<std::string> shipNames;
        std::vector<std::string> shaderNames;
};

#endif // TESTSTATE_H