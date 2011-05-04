#include "TestState.h"
#include "GameServices.h"

using namespace de::enums;

TestState::TestState()
{
	int num_joy, i;
	num_joy=SDL_NumJoysticks();
	de::io::tests << num_joy << " joysticks found\n";

	deviceHandle = hdInitDevice(HD_DEFAULT_DEVICE);
	if (HD_DEVICE_ERROR(hdGetError()))
	{
		de::io::error << "No haptics Device found\n";
		//assert(!"No haptics Device found");
	}

	hapticsContext = hlCreateContext(deviceHandle);
	hlMakeCurrent(hapticsContext);
	
	hlMatrixMode(HL_TOUCHWORKSPACE);
	hlLoadIdentity();
	hlWorkspace (-80, -80, -70, 80, 80, 20);
	hlOrtho (0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	hlDisable(HL_USE_GL_MODELVIEW);

	HLuint viscous = hlGenEffects(1);
	hlBeginFrame();
	hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.8);
	hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 0.9);
	hlStartEffect(HL_EFFECT_VISCOUS, viscous);
	hlEndFrame();



	vender.font( "visitor" ).shader( "String" ).text( (char*)hlGetString( HL_VENDOR ) );
	vender.setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 1000.0f ) );
	vender.setUniform( "View", glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3( 0.0f, 345.0f, -950.0f) ), 180.0f, glm::vec3( 1.0f,0.0f,0.0f) ) );
	vender.setUniform( "Colour", glm::vec4( 0.67f, 0.16f, 0.16f, 0.8f ));

	version.font( "visitor" ).shader( "String" ).text( (char*)hlGetString( HL_VERSION ) );
	version.setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 1000.0f ) );
	version.setUniform( "View", glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3( 0.0f, 325.0f, -950.0f) ), 180.0f, glm::vec3( 1.0f,0.0f,0.0f) ) );
	version.setUniform( "Colour", glm::vec4( 0.67f, 0.16f, 0.16f, 0.8f ));


	using namespace de::misc;
	double pos[3];
    hlGetDoublev(HL_DEVICE_POSITION, pos);
	posText.font( "visitor" ).shader( "String" ).text( "Pos: x:" + toString(pos[0]) + " y:" + toString(pos[1]) + " z:" + toString(pos[2]) );
	posText.setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 1000.0f ) );
	posText.setUniform( "View", glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3( 0.0f, 305.0f, -950.0f) ), 180.0f, glm::vec3( 1.0f,0.0f,0.0f) ) );
	posText.setUniform( "Colour", glm::vec4( 0.67f, 0.16f, 0.16f, 0.8f ));


	hlGetString( HL_VERSION );
	hapticsShape = hlGenShapes(1);
}

TestState::~TestState()
{
	hlBeginFrame();
	hlStopEffect(viscous);
	hlEndFrame();
	hlDeleteEffects(viscous, 1);
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
	using namespace de::misc;
	double pos[3];
    hlGetDoublev(HL_DEVICE_POSITION, pos);

	posText.text( "Pos: x:" + toString(pos[0]) + " y:" + toString(pos[1]) + " z:" + toString(pos[2]) );
	vender.text( (char*)hlGetString( HL_VENDOR ) );
	version.text( (char*)hlGetString( HL_VERSION ) );

    return false;
}

void TestState::reLoadTextures()
{

}

void TestState::render()
{
	hlBeginFrame();
	hlEndFrame();
	posText.render();
	vender.render();
	version.render();
	mesh.render();

}
