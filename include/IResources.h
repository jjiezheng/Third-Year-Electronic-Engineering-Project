#ifndef IRESOURCES_H
#define IRESOURCES_H

#include "Base.h"

namespace de
{
    namespace classes
    {
        class Sprite;
        class Line;
        class Poly;
        class Colour;
        class Frect;
    }
    namespace graphics
    {
        class Shader;
		class VBO;
    }
    namespace resources
    {
        class IResources
        {
            public:
                IResources() {}
                virtual ~IResources() {}

                virtual void free( int _choice ) = 0;
                virtual void load( int _choice ) = 0;

                virtual void free( const std::string &_choice ) = 0;
                virtual void load( const std::string &_choice ) = 0;

                virtual de::classes::Poly getSprite( const std::string &_name ) = 0;
                virtual const std::vector<de::classes::Poly>& getSprites( const std::vector<std::string> &_names ) = 0;

                virtual de::classes::Frect getSpriteCoords( const std::string &_name, bool tex = true ) = 0;
                virtual std::map<std::string, de::classes::Frect>& getSpritesCoords( const std::vector<std::string> &_names, bool tex = true ) = 0;
                virtual std::map<std::string, de::classes::Frect>& getSpritesCoords( const std::string &_texture, bool tex = true ) = 0;
                virtual std::vector<std::string>& getSpritesName( const std::string &_texture ) = 0;

                virtual bool getTextureSize( const std::string &_texture, float &_width, float &_height ) = 0;

                virtual const unsigned int& getTexture( const std::string &_texture ) = 0;
                virtual const de::graphics::Shader& getShader( const std::string &_shaderName ) = 0;
				virtual const de::graphics::VBO& getMesh( const std::string &_mesh ) = 0;
                virtual const int& getSoundEffect( const std::string &_soundEffect ) = 0;
                virtual const int& getMusic( const std::string &_music ) = 0;

            protected:
            private:
        };

        class NullResources : public IResources
        {
            public:
                NullResources() {}
                virtual ~NullResources() {}

                void free( int _choice ) {}
                void load( int _choice ) {}

                void free( const std::string &_choice ) {}
                void load( const std::string &_choice ) {}

                de::classes::Poly getSprite( const std::string &_name );
                const std::vector<de::classes::Poly>& getSprites( const std::vector<std::string> &_names );

                de::classes::Frect getSpriteCoords( const std::string &_name, bool tex = true );
                std::map<std::string, de::classes::Frect>& getSpritesCoords( const std::vector<std::string> &_names, bool tex = true );
                std::map<std::string, de::classes::Frect>& getSpritesCoords( const std::string &_texture, bool tex = true );
                std::vector<std::string>& getSpritesName( const std::string &_texture );

                bool getTextureSize( const std::string &_texture, float &_width, float &_height ) { return true; }

                const unsigned int& getTexture( const std::string &_texture );
                const de::graphics::Shader& getShader( const std::string &_shaderName );
				const de::graphics::VBO& getMesh( const std::string &_mesh );
                const int& getSoundEffect( const std::string &_soundEffect );
                const int& getMusic( const std::string &_music );

            protected:
            private:
        };
    }
}

#endif // IRESOURCES_H
