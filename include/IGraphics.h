#ifndef IGRAPHICS_H
#define IGRAPHICS_H

#include "frameDetails.h"
#include "renderObject.h"
#include "Material.h"
#include "Lua.hpp"

struct SDL_Surface;
class Object3D;

namespace de
{
    namespace classes
    {
        class Sprite;
        class Line;
        class Poly;
        class Colour;
    }
    namespace graphics
    {
        class IGraphics
        {
            public:

                IGraphics() {}
                virtual ~IGraphics() {}
                //! Renders everything that has been passed into the class since the last time this function was call and then swade the buffers
                virtual frameDetails render() = 0;

                virtual bool add( renderObject *_Object ) = 0;

                virtual void fullscreen( bool _fullscreen ) = 0;
                virtual void resize( int _screenWidth, int _screenHeight ) = 0;

                virtual const de::graphics::VideoInfo& getVideoSettings() = 0;
				virtual void setVideoSettings( const de::graphics::VideoInfo &_info ) = 0;

                virtual Uint32 loadTexture( const std::string &_fileName, int &_width, int &_height ) = 0;
                virtual void unloadTexture( Uint32 &_texture ) = 0;

                virtual bool loadShader( lua_State* L, Shader &_shader, const std::string &_name ) = 0;
                virtual bool unloadShader( Shader &_shader ) = 0;

				virtual void pushSettings( const std::string &_serialisedText ) = 0;
            protected:
            private:
        };


        class NullGraphics : public IGraphics
        {
            public:
                /** Default constructor */
                NullGraphics();
                /** Default destructor */
                virtual ~NullGraphics() {}

                //! Renders everything that has been passed into the class since the last time this function was call and then swade the buffers
                frameDetails render();

                bool add( renderObject *_Object ) { return true; }

                void fullscreen( bool _fullscreen ) {}
                void resize( int _screenWidth, int _screenHeight ) {}

                const de::graphics::VideoInfo& getVideoSettings()
                {
                    static VideoInfo videoInfo;
                    return videoInfo;
                }

				void setVideoSettings( const de::graphics::VideoInfo &_info ) {}

                Uint32 loadTexture( const std::string& _fileName, int &_width, int &_height ) { return 0; }
                void unloadTexture( Uint32 &_texture ) {}

                bool loadShader( lua_State* L, Shader &_shader, const std::string &_name ) { return true; }
                bool unloadShader( Shader &_shader ) { return true; }

				void pushSettings( const std::string &_serialisedText ) {};
            protected:
            private:
                SDL_Surface* Display;
        };

    }
}

#endif // IGRAPHICS_H
