#include "SpriteInstance.h"

#include "GameServices.h"
#include "Material.h"
#include "SpriteObject.h"

#include "openGL.h"

OpaqueSprite::OpaqueSprite()
{

}

OpaqueSprite::OpaqueSprite( SpriteInstance *_root, const std::string &_spriteName, const std::string &_shaderName )
{
    spriteName = _spriteName;
    shaderName = _shaderName;

    root = _root;

    spriteId = root->getId( spriteName, shaderName );
    //_root->add( this );
}

OpaqueSprite::~OpaqueSprite()
{

}

int OpaqueSprite::id()
{
    return spriteId;
}

void OpaqueSprite::change( const std::string &_name, int _option  )
{
    if ( (_option & SPRITE) == SPRITE )
    {
        spriteName = _name;
    }
    if ( (_option & SHADER) == SHADER )
    {
        shaderName = _name;
    }

    spriteId = root->getId( spriteName, shaderName );
}

void OpaqueSprite::setRotation( const float &_rotation )
{
    rotation = _rotation;
}
void OpaqueSprite::setPosition( const glm::vec3 &_position )
{
    position = _position;
}

void OpaqueSprite::disconnect()
{
    root = NULL;
    alive = false;
}

void OpaqueSprite::addThisFrame()
{
   root->add( this, spriteId );
}



SpriteInstance::SpriteInstance() : current(0)
{

}

SpriteInstance::~SpriteInstance()
{
    //dtor
}

void SpriteInstance::projection( const glm::vec3 &_offset )
{
    ProjectionView = glm::translate(ProjectionViewReset, _offset );

    std::map<int, SpriteObject>::iterator iter = FBOS.begin();
    std::map<int, SpriteObject>::iterator end = FBOS.end();
    for(;iter!=end;++iter)
    {
        iter->second.setUniform( "ProjectionView", ProjectionView );
    }
}

void SpriteInstance::seed( const std::string &_texture, const std::string &_shaderName )
{
    std::vector<std::string> spriteNames = de::Engine::Resources().getSpritesName( _texture );
    std::vector<std::string>::iterator iter = spriteNames.begin();
    std::vector<std::string>::iterator end = spriteNames.end();

    for(; iter !=end; ++iter )
    {
        std::string spriteName( _texture+"."+(*iter) );
        FBOS[current] = SpriteObject( _texture, spriteName , _shaderName );

        ProjectionView = glm::perspective( 45.0, 16.0/10.0, 200.0, 10000.0 );
        ProjectionView *= glm::lookAt( glm::vec3( 0.0,0.0, 0.0 ),
                                       glm::vec3( 0.0,0.0,-1.0 ),
                                       glm::vec3( 0.0,1.0, 0.0 ) );

        ProjectionViewReset = ProjectionView;

        FBOS[current].setUniform( "ProjectionView", ProjectionView );
        IDs[spriteName+_shaderName] = current;
        ++current;
    }
}

void SpriteInstance::add( OpaqueSprite* _sprite, int _id )
{
    Sprites[_id].push_back( _sprite );
}

int SpriteInstance::getId( const std::string &_spriteName, const std::string &_shaderName )
{
    std::string fboName(_spriteName+_shaderName);
    return IDs[fboName];
}

void SpriteInstance::reload()
{
    std::map<int, SpriteObject>::iterator iter = FBOS.begin();
    std::map<int, SpriteObject>::iterator end = FBOS.end();

    for(;iter!=end;++iter)
    {
        iter->second.reload();
        iter->second.setUniform( "ProjectionView", ProjectionView );
    }
}


void SpriteInstance::render()
{
    de::Engine::Graphics().add( this );
}

void SpriteInstance::actualRender()
{
    std::map<int, std::vector<OpaqueSprite*> >::iterator fboIter = Sprites.begin();
    std::map<int, std::vector<OpaqueSprite*> >::iterator fboEnd = Sprites.end();
    std::vector<OpaqueSprite*>::iterator iter, end;

    for(; fboIter !=fboEnd; ++fboIter )
    {
        iter = fboIter->second.begin();
        end = fboIter->second.end();

        int id( fboIter->first );

        FBOS[id].start();
        for(; iter !=end; ++iter )
        {
            FBOS[id].setUniform( "offset", (*iter)->position );
            FBOS[id].setUniform( "angle", (*iter)->rotation );
            FBOS[id].groupRender();
        }
        FBOS[id].end();
    }

    Sprites.clear();
}
