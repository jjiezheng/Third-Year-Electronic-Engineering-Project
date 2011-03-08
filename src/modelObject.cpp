#include "modelObject.h"
#include "GameServices.h"
#include "openGL.h"

modelObject::modelObject() : usingTexture(false), writingToDepth(false), depthTest(false), alphaTest(false), blending(false), active(false)
{

}

modelObject::~modelObject()
{

}


modelObject& modelObject::writeToDepth( bool _writeToDepth )
{
	writingToDepth = _writeToDepth;
	return *this;
}

modelObject& modelObject::depth( bool _depth )
{
	depthTest = _depth;
	return *this;
}

modelObject& modelObject::alpha( bool _alpha )
{
	alphaTest = _alpha;
	return *this;
}

modelObject& modelObject::blend( bool _blend )
{
	blending = _blend;
	return *this;
}


modelObject& modelObject::setType( const std::string &_type )
{
	return *this;
}

modelObject& modelObject::load( const std::string &_mesh, const std::string &_shader )
{
	meshName = _mesh;
	shaderName = _shader;

	vbo = de::Engine::Resources().getMesh( meshName );
    shader = de::Engine::Resources().getShader( shaderName );
    AttributeNames = shader.getAttributeNames();
	iterNames = AttributeNames.begin();


	active = true;
	return *this;
}


void modelObject::reload()
{

}

void modelObject::render()
{
	de::Engine::Graphics().add( this );
}

void modelObject::localRender()
{
	if( active )
	{

		if( usingTexture )
		{
			CHECKGL( glEnable( GL_TEXTURE_2D ) );
			CHECKGL( glActiveTexture( GL_TEXTURE0 ) );
			CHECKGL( glBindTexture( GL_TEXTURE_2D, texture ) );
		}
		else
			CHECKGL( glDisable( GL_TEXTURE_2D ) );

		if( writingToDepth )
		{
			CHECKGL( glDepthMask( GL_TRUE ) );
		}
		else glDepthMask( GL_FALSE );

		if( depthTest ) 
		{
			CHECKGL( glEnable( GL_DEPTH_TEST ) );
		}
		else CHECKGL( glDisable( GL_DEPTH_TEST ) );

		if( alphaTest ) 
		{
			CHECKGL( glEnable( GL_ALPHA_TEST ) );
		}
		else CHECKGL( glDisable( GL_ALPHA_TEST ) );

		if( blending ) 
		{
			CHECKGL( glEnable( GL_BLEND ) );
		}
		else CHECKGL( glDisable( GL_BLEND ) );


		shader.bindShader();
		CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, vbo.vertexBuffer ) );
		
		for( iterNames = AttributeNames.begin(); iterNames != AttributeNames.end(); ++iterNames )
		{
			if( vbo.Attribs.find( (*iterNames) ) != vbo.Attribs.end() )
			{
				de::io::tests << "Shader input: " << (*iterNames) << " found\n";
				de::io::tests << "size:" << 4 << " stride:" << vbo.Attribs[(*iterNames)].stride << " offset:" << vbo.Attribs[(*iterNames)].offset << "\n";

				CHECKGL( glVertexAttribPointer(
					shader.getAttribute( (*iterNames) ),			// attribute
					4,												// size
					GL_FLOAT,										// type
					GL_FALSE,										// normalized?
					vbo.Attribs[(*iterNames)].stride,				// stride
					(void*)(vbo.Attribs[(*iterNames)].offset)		// array buffer offset
				) );
				CHECKGL( glEnableVertexAttribArray( shader.getAttribute( (*iterNames) ) ) );
			}
			else
			{
				de::io::error << "Attribute missing:\n";
			}
		}
		
		CHECKGL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo.elementBuffer ) );
			CHECKGL( glDrawElements(
				GL_TRIANGLES,				// mode
				vbo.triangles,                  // count
				GL_UNSIGNED_INT,    // type
				(void*)0            // element array buffer offset
			) );
			
		for( iterNames = AttributeNames.begin(); iterNames != AttributeNames.end(); ++iterNames )
		{
			if( vbo.Attribs.find( *iterNames ) != vbo.Attribs.end() )
			{
				CHECKGL( glDisableVertexAttribArray( shader.getAttribute( (*iterNames) ) ) );
			}
		}

		CHECKGL( glUseProgram( 0 ) );
	}
}

void modelObject::clear()
{

}
