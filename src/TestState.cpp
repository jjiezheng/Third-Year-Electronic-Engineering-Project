#include "TestState.h"
#include "GameServices.h"

using namespace de::enums;

TestState::TestState()
{
	de::Engine::Fonts().load( "GeoSansLight" );

	otherText.set( "catcatcat", "GeoSansLight", de::classes::Vector( 50.0f, 75.0f ) );
	otherText.setColour( de::classes::Colour( 0.67f, 0.16f, 0.16f,0.8f ) );

	textytext.font( "GeoSansLight" ).text( "Caturday" ).shader( "String" );
	textytext.setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 1000.0f ) );
	textytext.setUniform( "View", glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3( 0.0f, 0.0f, -900.0f ) ), 180.0f, glm::vec3( 1.0f, 0.0f, 0.0f) ) );
	textytext.setUniform( "Model", glm::mat4(1.0f) );


	/*
	deviceHandle = hdInitDevice(HD_DEFAULT_DEVICE);
	if (HD_DEVICE_ERROR(hdGetError()))
	{
		de::io::error << "No haptics Device found\n";
		assert(!"No haptics Device found");
	}

	hapticsContext = hlCreateContext(deviceHandle);
	hlMakeCurrent(hapticsContext);
	
	hlMatrixMode(HL_TOUCHWORKSPACE);
	hlLoadIdentity();
	hlWorkspace (-80, -80, -70, 80, 80, 20);
	hlOrtho (0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	hlDisable(HL_USE_GL_MODELVIEW);
	

	hapticsShape = hlGenShapes(1);*/
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

bool TestState::logic( const Uint32 &_deltaTicks, State* &_nextState, de::state::options &_options )
{
	static float rotate = 0;
	rotate += 5;
	textytext.setUniform( "Model", glm::translate( glm::rotate( glm::mat4(1.0f), -rotate, glm::vec3(1.0f) ), textytext.align() ) );
    return false;
}

void TestState::reLoadTextures()
{
	textytext.reload();
}

void TestState::render()
{
	textytext.render();
	otherText.render( de::enums::FBO_AFTER );
}
