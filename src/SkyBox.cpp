#include "SkyBox.h"
#include "GameServices.h"
#include "openGL.h"

SkyBox::SkyBox()
{
    //ctor
}

SkyBox::~SkyBox()
{
    //dtor
}

SkyBox& SkyBox::add( const std::string &_name, int _face )
{
    textureName[_face] = _name;
    texture[_face] = de::Engine::Resources().getTexture(_name);
    textureSet[_face] = true;

    shader.setUniform( "Texture0", de::Engine::Resources().getTexture(_name) );
    return *this;
}

SkyBox& SkyBox::set( const std::string &_shaderName )
{
    shaderName = _shaderName;
    shader = de::Engine::Resources().getShader( shaderName );
    AttributeNames = shader.getAttributeNames();

    std::vector<std::string>::iterator start, end = AttributeNames.end();
    /*for( start = AttributeNames.begin(); start != end; ++start )
    {
        AttributeInfo[(*start)] = bufferInfo();
        AttributeElemenRange[(*start)] = bufferInfo();
    }*/
    return *this;
}

void SkyBox::reload()
{
    shader = de::Engine::Resources().getShader( shaderName );
}

void SkyBox::render()
{
    de::Engine::Graphics().add( this );
}

void SkyBox::localRender()
{

}
