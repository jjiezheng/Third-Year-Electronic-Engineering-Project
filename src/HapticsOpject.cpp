#include "HapticsOpject.h"
#include "Haptics.h"
#include "GameServices.h"
#include "openGL.h"


hapticsOpject::hapticsOpject(void)
{
	hapticShape = hlGenShapes(1);
}


hapticsOpject::~hapticsOpject(void)
{
}



hapticsOpject& hapticsOpject::writeToDepth( bool _writeToDepth )
{
	mesh.writeToDepth( _writeToDepth );
	return *this;
}

hapticsOpject& hapticsOpject::depth( bool _depth )
{
	mesh.depth( _depth );
	return *this;
}

hapticsOpject& hapticsOpject::alpha( bool _alpha )
{
	mesh.alpha( _alpha );
	return *this;
}

hapticsOpject& hapticsOpject::blend( bool _blend )
{
	mesh.blend( _blend );
	return *this;
}

hapticsOpject& hapticsOpject::setType( const std::string &_type )
{
	mesh.setType( _type );
	return *this;
}

hapticsOpject& hapticsOpject::load( const std::string &_mesh, const std::string &_shader )
{
	mesh.load( _mesh, _shader );
	return *this;
}

hapticsOpject& hapticsOpject::setTexture( const std::string &_texture, const std::string &_value )
{
	mesh.setTexture( _texture, _value );
	return *this;
}


void hapticsOpject::reload()
{
	mesh.reload();
}

void hapticsOpject::render()
{
	de::Engine::Graphics().add( this );
	de::Engine::Graphics().addHaptics( this );
}

void hapticsOpject::actualRender()
{
	hlBeginFrame();

/*
	// clear color and depth buffers for new frame
	// haptic rendering requires a clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// combined graphic and haptic rendering pass
	hlBeginShape(HL_SHAPE_DEPTH_BUFFER, myShapeId);
	drawMyShape();
	hlEndShape();
	// swap buffers to show graphics results on screen
	glutSwapBuffers();
	// flush haptics changes
	hlEndFrame();
*/
	mesh.usingTexture;

	//mesh.actualRender();
}

