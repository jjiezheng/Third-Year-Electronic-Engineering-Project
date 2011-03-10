#include "StringObject.h"
#include "GameServices.h"
#include "openGL.h"

StringObject::StringObject(void)
{
}
StringObject::~StringObject(void)
{
}

StringObject& StringObject::font( const std::string &_font )
{
	if( fontType != _font )
	{
		fontType = _font;
		refresh = true;
	}
	return *this;
}
StringObject& StringObject::text( const std::string &_text )
{
	if( textString != _text )
	{
		textString = _text;
		refresh = true;
	}
	return *this;
}
StringObject& StringObject::shader( const std::string &_shader )
{
	if( shaderType != _shader )
	{
		shaderType = _shader;
		refresh = true;
	}
	return *this;
}

StringObject& StringObject::writeToDepth( bool _depth )
{
	renderableText.writeToDepth( _depth );
	return *this;
}
StringObject& StringObject::depth( bool _depth )
{
	renderableText.depth( _depth );
	return *this;
}
StringObject& StringObject::alpha( bool _alpha )
{
	renderableText.alpha( _alpha );
	return *this;
}
StringObject& StringObject::blend( bool _blend )
{
	renderableText.blend( _blend );
	return *this;
}

glm::vec3& StringObject::align()
{
	return halfLength;
}

void StringObject::rebuild()
{
	if( textString.empty() || fontType.empty() || shaderType.empty() ) return;

	//renderableText.clear();
	//renderableText = Object3D();
	std::vector<de::graphics::vertex> tempVerts, tempTexVerts;
	std::vector<int> tempElems;
	de::Engine::Fonts().write( textString, 
							   fontType,
							   tempVerts,
							   tempTexVerts,
							   tempElems ); 
	
	glm::vec3 min, max;
	min.x = tempVerts[0].r;
	min.y = tempVerts[0].g;
	min.z = tempVerts[0].b;

	min.x = tempVerts[0].r;
	min.y = tempVerts[0].g;
	min.z = tempVerts[0].b;

	int i(0);
	for( i;i!=tempVerts.size();++i )
	{
		if( min.x > tempVerts[i].r ) min.x = tempVerts[i].r ;
		if( min.y > tempVerts[i].g ) min.y = tempVerts[i].g ;
		if( min.z > tempVerts[i].b ) min.z = tempVerts[i].b ;

		if( max.x < tempVerts[i].r ) max.x = tempVerts[i].r ;
		if( max.y < tempVerts[i].g ) max.y = tempVerts[i].g ;
		if( max.z < tempVerts[i].b ) max.z = tempVerts[i].b ;
	}

	halfLength = (min-max)/2.0f - min;
	for( i = 0;i!=tempVerts.size();++i )
	{
		tempVerts[i].r = tempVerts[i].r + halfLength.x;
		tempVerts[i].g = tempVerts[i].g + halfLength.y;
		tempVerts[i].b = tempVerts[i].b + halfLength.z;
	}
	
	renderableText.clear();
	renderableText.set( shaderType );
	renderableText.setType( "Triangles" );
	renderableText.add( "Position", tempVerts, tempElems );
	renderableText.add( "Tex", tempTexVerts );
	
	renderableText.setTexture( "Texture0", fontType );

	std::map<std::string, ShaderVariant>::iterator iter;
	for( iter = Uniforms.begin(); iter !=Uniforms.end();++iter )
	{
		renderableText.setUniform( iter->first, iter->second );
	}
	
	refresh = false;
}
void StringObject::reload()
{
	renderableText.reload();
}
void StringObject::render()
{
	de::Engine::Graphics().add( this );
}
void StringObject::actualRender()
{
	if( refresh) rebuild();
	renderableText.actualRender();
}