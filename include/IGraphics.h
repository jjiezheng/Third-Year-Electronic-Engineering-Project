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
                //!Frees all Textures, Shaders and FBO's
                virtual void freeAll() = 0;
                //! Renders everything that has been passed into the class since the last time this function was call and then swade the buffers
                virtual frameDetails render() = 0;

                //! Add a Sprite with a de::enums::fboTypes for rendering this frame.
                virtual bool add( const de::classes::Sprite &_Sprite, const int &_type ) = 0;
                //! Add Sprites with a de::enums::fboTypes for rendering this frame.
                virtual bool add( std::vector<de::classes::Sprite> &_Sprites, const int &_type ) = 0;
                //! Add a Line with a de::enums::fboTypes for rendering this frame.
                virtual bool add( const de::classes::Line &_Line, const int &_type ) = 0;
                virtual bool add( renderObject *_Object ) = 0;
                //! Add a Coloured Poly with a de::enums::fboTypes for rendering this frame.
                virtual bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority, const int &_type ) = 0;

                virtual void toggleFullscreen() = 0;
                virtual void resize( int _screenWidth, int _screenHeight ) = 0;

                virtual bool videoModeOk( const int &_width, const int &_height ) = 0;

                virtual const de::graphics::VideoInfo& getVideoSettings() = 0;

                virtual Uint32 loadTexture( const std::string &_fileName, int &_width, int &_height ) = 0;
                virtual void unloadTexture( Uint32 &_texture ) = 0;

                virtual bool loadShader( lua_State* L, Shader &_shader, const std::string &_name ) = 0;
                virtual bool unloadShader( Shader &_shader ) = 0;
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

                //!Frees all Textures, Shaders and FBO's
                void freeAll() {}
                //! Renders everything that has been passed into the class since the last time this function was call and then swade the buffers
                frameDetails render();

                //! Add a Sprite with a de::enums::fboTypes for rendering this frame.
                bool add( const de::classes::Sprite &_Sprite, const int &_type ) { return true; }
                //! Add Sprites with a de::enums::fboTypes for rendering this frame.
                bool add( std::vector<de::classes::Sprite> &_Sprites, const int &_type ) { return true; }
                //! Add a Line with a de::enums::fboTypes for rendering this frame.
                bool add( const de::classes::Line &_Line, const int &_type ) { return true; }
                bool add( renderObject *_Object ) { return true; }
                //! Add a Coloured Poly with a de::enums::fboTypes for rendering this frame.
                bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority, const int &_type ) { return true; }

                void toggleFullscreen() {}
                void resize( int _screenWidth, int _screenHeight ) {}

                bool videoModeOk( const int &_width, const int &_height ) { return true; }

                const de::graphics::VideoInfo& getVideoSettings()
                {
                    static VideoInfo videoInfo;
                    return videoInfo;
                }

                Uint32 loadTexture( const std::string& _fileName, int &_width, int &_height ) { return 0; }
                void unloadTexture( Uint32 &_texture ) {}

                bool loadShader( lua_State* L, Shader &_shader, const std::string &_name ) { return true; }
                bool unloadShader( Shader &_shader ) { return true; }
            protected:
            private:
                SDL_Surface* Display;
        };

    }
}

#endif // IGRAPHICS_H
