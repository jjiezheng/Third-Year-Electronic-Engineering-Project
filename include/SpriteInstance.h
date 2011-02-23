#ifndef SPRITEINSTANCE_H
#define SPRITEINSTANCE_H

#include "renderObject.h"
#include "CoreEnumsAndClasses.h"

namespace de
{
    namespace graphics
    {
        class Shader;
    }
}

class OpaqueSprite;
class SpriteObject;

class SpriteInstance : public de::graphics::renderObject
{
    public:
        /** Default constructor */
        SpriteInstance();
        /** Default destructor */
        virtual ~SpriteInstance();
        void projection( const glm::vec3 &_offset );

        void seed( const std::string &_texture, const std::string &_shaderName );
        void add( OpaqueSprite* _sprite, int _id );
        int getId( const std::string &_spriteName, const std::string &_shaderName );

        void reload();
        void render();
        void localRender();

    protected:
    private:
        int current;
        glm::mat4 ProjectionView, ProjectionViewReset;

        std::map<int, SpriteObject> FBOS;
        std::map<std::string, int> IDs;
        std::map<int, std::vector<OpaqueSprite*> > Sprites;
};


class OpaqueSprite
{
    public:
        enum SpriteOptions
        {
            NONE = 0,
            SPRITE = 1,
            SHADER = 1 << 1
        };

        OpaqueSprite();
        OpaqueSprite( SpriteInstance *_root, const std::string &_spriteName, const std::string &_shaderName );
        virtual ~OpaqueSprite();

        int id();
        void change( const std::string &_name, int _option );
        void disconnect();
        void addThisFrame();

        void setRotation( const float &_rotation );
        void setPosition( const glm::vec3 &_position );

        float rotation;
        glm::vec3 position;

    private:
        bool alive;
        int spriteId;
        std::string spriteName, shaderName;

        SpriteInstance *root;
};


#endif // SPRITEINSTANCE_H
