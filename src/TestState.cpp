#include "TestState.h"
#include "GameServices.h"

using namespace de::enums;

TestState::TestState()
{


		mesh.load( "world.obj", "GroundFromSpace" );
		mesh.writeToDepth(false).depth(false);
		mesh.setUniform( "Model", glm::mat4(1.0f) );
		mesh.setUniform( "View", glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, -10.0f ) ) );
		mesh.setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 100.0f ) );
		/*
		mesh.setUniform( "AmbientMaterial", glm::vec3( 0.04f, 0.04f, 0.04f ) );
		mesh.setUniform( "DiffuseMaterial", glm::vec3( 0.75f, 0.75f, 0.5f ) );
		mesh.setUniform( "SpecularMaterial", glm::vec3( 0.5f, 0.5f, 0.5f ) );
		mesh.setUniform( "Shininess", 100.0f );

		mesh.setUniform( "NormalMatrix", glm::inverse( glm::transpose( glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, -10.0f ) ) ) ) );
		mesh.setUniform( "LightPosition", glm::vec3(10.0f) );

		*/

		glm::vec3 waveLength( powf(0.650f,4.0f), powf(0.570f,4.0f),powf(0.475f,4.0f) );
		float RayleighConst = 0.0025f;
		float MieConst = 0.0015f;
		float OuterRadius(10.25f), InnerRadius(10.0f);
		float RayleighScaleDepth = 0.25f;
		float MieScaleDepth = 0.1f;

		mesh.setUniform( "v3CameraPos", glm::vec3(0.0f,0.0f,25.0f) );
		mesh.setUniform( "v3LightPos", glm::vec3(10.0f) );
		mesh.setUniform( "v3InvWavelength", 1.0f/waveLength );

		mesh.setUniform( "fCameraHeight", 25.0f );
		mesh.setUniform( "fCameraHeight2", powf(25.0f, 2.0f) );
		mesh.setUniform( "fOuterRadius", OuterRadius );
		mesh.setUniform( "fOuterRadius2", OuterRadius*OuterRadius );
		mesh.setUniform( "fInnerRadius", InnerRadius );
		mesh.setUniform( "fInnerRadius2", InnerRadius*InnerRadius );
		mesh.setUniform( "fKrESun", RayleighConst*15.0f );
		mesh.setUniform( "fKmESun", MieConst*15.0f );
		mesh.setUniform( "fKr4PI", RayleighConst*de::misc::pi() );
		mesh.setUniform( "fKm4PI", MieConst*de::misc::pi() );
		mesh.setUniform( "fScale", 1.0f / (OuterRadius - InnerRadius) );
		mesh.setUniform( "fScaleDepth", RayleighScaleDepth );
		mesh.setUniform( "fScaleOverScaleDepth", ( 1.0f/(OuterRadius-InnerRadius) )/ RayleighScaleDepth );

		mesh.setUniform( "nSamples", 2 );
		mesh.setUniform( "fSamples", 2.0f );
		mesh.setUniform( "g", 0.95f );
		mesh.setUniform( "g2", 0.95f*0.95f );
	

		/*
		de::Engine::Resources().getShader( "SkyFromSpace" );
		de::Engine::Resources().getShader( "GroundFromSpace" );
		*/
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

	mesh.render();
}
