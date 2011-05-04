#include "TestState.h"
#include "GameServices.h"

using namespace de::enums;

TestState::TestState()
{
	int num_joy, i;
	num_joy=SDL_NumJoysticks();
	de::io::tests << num_joy << " joysticks found\n";

	for(i=0;i<num_joy;i++)
	{
		de::io::tests << "Joystick " << i << ": " << SDL_JoystickName(i) << "\n";
	}
}

TestState::~TestState()
{
}

bool TestState::handleEvents( const SDL_Event &_event )
{
    if( _event.type == SDL_USEREVENT )
    {
        if( _event.user.code == events::OPENGL_RELOAD )
        {
            reLoadTextures();
        }
    }

    return true;
}

bool TestState::logic( const float &_deltaTicks, State* &_nextState, de::state::options &_options )
{

    return false;
}

void TestState::reLoadTextures()
{

}

void TestState::render()
{
}
