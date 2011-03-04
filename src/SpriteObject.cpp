#include "SpriteObject.h"

#include "GameServices.h"
#include "Material.h"
#include "openGL.h"

namespace
{
    static Uint32 make_buffer( GLenum target, const void *buffer_data, GLsizei buffer_size )
    {
        Uint32 buffer;
        CHECKGL( glGenBuffers( 1, &buffer ) );
        CHECKGL( glBindBuffer( target, buffer ) );
        CHECKGL( glBufferData( target, buffer_size, buffer_data, GL_STATIC_DRAW ) );
        return buffer;
    }
}

SpriteObject::SpriteObject() : refreshVBO(false)
{
    ElementBuffer.push_back( 0 );
    ElementBuffer.push_back( 1 );
    ElementBuffer.push_back( 2 );
    ElementBuffer.push_back( 3 );

    Sprites[0].usingTex = false;
    Sprites[1].usingTex = false;
    Sprites[2].usingTex = false;
}

SpriteObject::SpriteObject( const std::string &_textureName, const std::string &_spriteName, const std::string &_shaderName ) : refreshVBO(false)
{
    ElementBuffer.push_back( 0 );
    ElementBuffer.push_back( 1 );
    ElementBuffer.push_back( 2 );
    ElementBuffer.push_back( 3 );

    Sprites[0].usingTex = false;
    Sprites[1].usingTex = false;
    Sprites[2].usingTex = false;

    set( _textureName, _spriteName, _shaderName );
}

SpriteObject::~SpriteObject()
{
    //dtor
}

SpriteObject& SpriteObject::addSprite( const std::string &_textureName, const std::string &_spriteName, int _number )
{
    Sprites[_number].textureName = _textureName;
    Sprites[_number].spriteName = _spriteName;
    Sprites[_number].texture = de::Engine::Resources().getTexture( Sprites[_number].textureName );


    shader.setUniform( "Texture"+de::misc::toString(_number), Sprites[_number].texture );

    setFBO( de::Engine::Resources().getSpriteCoords( Sprites[_number].spriteName, true ),
            de::Engine::Resources().getSpriteCoords( Sprites[_number].spriteName, false ) );

    refreshVBO = true;
    Sprites[_number].usingTex = true;
    return *this;
}

SpriteObject& SpriteObject::set( const std::string &_textureName, const std::string &_spriteName, const std::string &_shaderName )
{
    shaderName = _shaderName;
    Sprites[0].textureName = _textureName;
    Sprites[0].spriteName = _spriteName;

    shader = de::Engine::Resources().getShader( shaderName );
    Sprites[0].texture = de::Engine::Resources().getTexture( Sprites[0].textureName );

    if( !shader.isOK() )
    {
        de::io::log << "Shader not ok. :(\n";
    }


    shader.setUniform( "Texture0", Sprites[0].texture );

    setFBO( de::Engine::Resources().getSpriteCoords( Sprites[0].spriteName, true ),
            de::Engine::Resources().getSpriteCoords( Sprites[0].spriteName, false ) );

    refreshVBO = true;
    return *this;
}

SpriteObject& SpriteObject::set( const std::string &_spriteName, const std::string &_shaderName )
{
    std::vector<std::string> strs;
    std::vector<std::string>::iterator iter, end;

    boost::split(strs, _spriteName, boost::is_any_of("."));
    iter = strs.begin();
    end = strs.end();

    std::string _textureName = (*iter);
    refreshVBO = true;

    return set( _textureName, _spriteName, _shaderName );
}

void SpriteObject::setFBO( de::classes::Frect _tex, de::classes::Frect _coords )
{
    using namespace de::graphics;

    _coords.w = _coords.w;
    _coords.h = _coords.h;

    VertexBuffer.push_back( vertex( _coords.w/2,-_coords.h/2, -10, 1 ) );
    VertexBuffer.push_back( vertex( _coords.w/2, _coords.h/2, -10, 1 ) );
    VertexBuffer.push_back( vertex( -_coords.w/2,-_coords.h/2, -10, 1 ) );
    VertexBuffer.push_back( vertex( -_coords.w/2, _coords.h/2, -10, 1 ) );



    VertexBuffer.push_back( vertex( _tex.x + (_tex.w/2),  _tex.y + (-_tex.h/2), 0, 0 ) );
    VertexBuffer.push_back( vertex( _tex.x + (_tex.w/2),  _tex.y + (_tex.h/2),  0, 0 ) );
    VertexBuffer.push_back( vertex( _tex.x + (-_tex.w/2), _tex.y + (-_tex.h/2), 0, 0 ) );
    VertexBuffer.push_back( vertex( _tex.x + (-_tex.w/2), _tex.y + (_tex.h/2),  0, 0 ) );
}

void SpriteObject::makeBuffers()
{
    vertexBuffer = make_buffer( GL_ARRAY_BUFFER, &(VertexBuffer[0]), VertexBuffer.size()*sizeof(de::graphics::vertex) );
    elementBuffer = make_buffer( GL_ELEMENT_ARRAY_BUFFER, &(ElementBuffer[0]), ElementBuffer.size()*sizeof(int) );

    refreshVBO = false;
}

void SpriteObject::reload()
{
    refreshVBO = true;
    shader = de::Engine::Resources().getShader( shaderName );
    Sprites[0].texture = de::Engine::Resources().getTexture( Sprites[0].textureName );
}

void SpriteObject::render()
{
    de::Engine::Graphics().add( this );
}

void SpriteObject::localRender()
{
    start();
    groupRender();
    end();
}

void SpriteObject::start()
{
    if( refreshVBO )
    {
        makeBuffers();
    }

    CHECKGL( glEnable( GL_TEXTURE_2D ) );
    CHECKGL( glActiveTexture( GL_TEXTURE0 ) );
    CHECKGL( glBindTexture( GL_TEXTURE_2D, Sprites[0].texture ) );

    shader.bindShader();
    CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer ) );
    CHECKGL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementBuffer ) );


    de::time::PerfTimer timer;
    timer.start();
    CHECKGL( glVertexAttribPointer(
        shader.getAttribute( "Position" ),        // attribute
        4,                                         // size
        GL_FLOAT,                                     // type
        GL_FALSE,                                     // normalized?
        sizeof(de::graphics::vertex),                 // stride
        (void*)(0) // array buffer offset
    ) );


    CHECKGL( glVertexAttribPointer(
        shader.getAttribute( "Tex" ),        // attribute
        4,                                         // size
        GL_FLOAT,                                     // type
        GL_FALSE,                                     // normalized?
        sizeof(de::graphics::vertex),                               // stride
        (void*)(4*sizeof(de::graphics::vertex)) // array buffer offset
    ) );

    if( Sprites[1].usingTex )
    {
        CHECKGL( glActiveTexture( GL_TEXTURE1 ) );
        CHECKGL( glBindTexture( GL_TEXTURE_2D, Sprites[1].texture ) );

        CHECKGL( glVertexAttribPointer(
                shader.getAttribute( "Tex1" ),        // attribute
                4,                                         // size
                GL_FLOAT,                                     // type
                GL_FALSE,                                     // normalized?
                sizeof(de::graphics::vertex),                               // stride
                (void*)(12*sizeof(de::graphics::vertex)) // array buffer offset
            ) );

        CHECKGL( glEnableVertexAttribArray( shader.getAttribute( "Tex1" ) ) );

        if( Sprites[2].usingTex )
        {
            CHECKGL( glActiveTexture( GL_TEXTURE2 ) );
            CHECKGL( glBindTexture( GL_TEXTURE_2D, Sprites[2].texture ) );

            CHECKGL( glVertexAttribPointer(
                    shader.getAttribute( "Tex2" ),        // attribute
                    4,                                         // size
                    GL_FLOAT,                                     // type
                    GL_FALSE,                                     // normalized?
                    sizeof(de::graphics::vertex),                               // stride
                    (void*)(20*sizeof(de::graphics::vertex)) // array buffer offset
                ) );

            CHECKGL( glEnableVertexAttribArray( shader.getAttribute( "Tex2" ) ) );
        }
    }

    CHECKGL( glEnableVertexAttribArray( shader.getAttribute( "Position" ) ) );
    CHECKGL( glEnableVertexAttribArray( shader.getAttribute( "Tex" ) ) );
}

void SpriteObject::groupRender()
{
    shader.bindUniforms();

    glDrawElementsInstancedEXT( GL_TRIANGLE_STRIP,  // mode
                                ElementBuffer.size(),                  // count
                                GL_UNSIGNED_SHORT,    // type
                                (void*)0,            // element array buffer offset )
                                150
                                );
/*
    CHECKGL( glDrawRangeElements(
        GL_TRIANGLE_STRIP,  // mode
        0,
        4,
        ElementBuffer.size(),                  // count
        GL_UNSIGNED_SHORT,    // type
        (void*)0            // element array buffer offset
    ) );*/
}

void SpriteObject::end()
{
    CHECKGL( glDisableVertexAttribArray( shader.getAttribute( "Position" ) ) );
    CHECKGL( glDisableVertexAttribArray( shader.getAttribute( "Tex" ) ) );
    if( Sprites[1].usingTex )
    {
        CHECKGL( glDisableVertexAttribArray( shader.getAttribute( "Tex1" ) ) );
        if( Sprites[2].usingTex )
        {
             CHECKGL( glDisableVertexAttribArray( shader.getAttribute( "Tex2" ) ) );
        }
    }


    CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
    CHECKGL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
    CHECKGL( glUseProgram( 0 ) );
    CHECKGL( glDisable( GL_TEXTURE_2D ) );
}
