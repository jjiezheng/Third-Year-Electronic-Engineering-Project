#include "TestState.h"
#include "GameServices.h"

using namespace de::enums;

TestState::TestState()
{

	for(int i = 0; i!=100;++i)
	{
		mesh[i].load( "HeadlessGiant.lwo", "basicVBO" );
		mesh[i].writeToDepth(true).depth(true);
		mesh[i].setUniform( "Model", glm::mat4(1.0f) );
		mesh[i].setUniform( "View", glm::translate( glm::mat4( 1.0f ), glm::vec3( (float)i*0.05f, (float)i*0.05f, -20.0f ) ) );
		mesh[i].setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 100.0f ) );

		mesh[i].setUniform( "AmbientMaterial", glm::vec3( 0.04f, 0.04f, 0.04f ) );
		mesh[i].setUniform( "DiffuseMaterial", glm::vec3( 0.75f, 0.75f, 0.5f ) );
		mesh[i].setUniform( "SpecularMaterial", glm::vec3( 0.5f, 0.5f, 0.5f ) );
		mesh[i].setUniform( "Shininess", 100.0f );

		mesh[i].setUniform( "NormalMatrix", glm::inverse( glm::transpose( glm::translate( glm::mat4( 1.0f ), glm::vec3( (float)i*0.05f, (float)i*0.05f, -20.0f ) ) ) ) );
		mesh[i].setUniform( "LightPosition", glm::vec3(-20.0f) );
	}

	/*
	vbo = de::Engine::Resources().getMesh( "HeadlessGiant.lwo" );

	de::io::tests << "vertexBuffer:" << (int)vbo.vertexBuffer << " elementBuffer:" << (int)vbo.elementBuffer << " meshName:" << vbo.meshName << "\n";
	*/
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
    return false;
}

void TestState::reLoadTextures()
{

}

void TestState::render()
{
	for(int i = 0; i!=100;++i)
	{
		mesh[i].render();
	}
}
