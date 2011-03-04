#ifndef SPRITEOBJECT_H
#define SPRITEOBJECT_H

#include "renderObject.h"
#include "CoreEnumsAndClasses.h"

namespace de
{
    namespace graphics
    {
        class Shader;
    }
}

class SpriteObject : public de::graphics::renderObject
{
    public:
        /** Default constructor */
        SpriteObject();
        SpriteObject( const std::string &_textureName, const std::string &_spriteName, const std::string &_shaderName );
        /** Default destructor */
        virtual ~SpriteObject();

        SpriteObject& set( const std::string &_textureName, const std::string &_spriteName, const std::string &_shaderName );
        SpriteObject& set( const std::string &_spriteName, const std::string &_shaderName );
        SpriteObject& addSprite( const std::string &_textureName, const std::string &_spriteName, int _number );

        template<typename T>
        SpriteObject& setUniform( const std::string &_name, const T &_value )
        {
            shader.setUniform( _name, _value );
            return *this;
        }


        void reload();
        void render();
        void localRender();
        void start();
        void end();
        void groupRender();

    protected:
    private:
        void setFBO( de::classes::Frect _tex, de::classes::Frect _coords );
        void makeBuffers();


        bool refreshVBO;

        struct
        {
            bool usingTex;
            std::string textureName, spriteName;
            Uint32 texture;
        } Sprites[3];

        de::graphics::Shader shader;
        std::string shaderName;
        Uint32 vertexBuffer, elementBuffer;

        std::vector<de::graphics::vertex> VertexBuffer;
        std::vector< std::vector<de::graphics::vertex> >::iterator iter;

        std::vector<Uint16> ElementBuffer;
};



#endif // SPRITEINSTANCE_H
