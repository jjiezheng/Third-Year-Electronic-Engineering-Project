#ifndef LOGGEDGRAPHICS_H
#define LOGGEDGRAPHICS_H

#include "IGraphics.h"

namespace de
{
    namespace graphics
    {
        class LoggedGraphics : public IGraphics
        {
            public:
                /** Default constructor */
                LoggedGraphics(IGraphics *_wrapped);
                /** Default destructor */
                virtual ~LoggedGraphics();

                //! Renders everything that has been passed into the class since the last time this function was call and then swade the buffers
                frameDetails render();

                //! Add a Sprite with a de::enums::fboTypes for rendering this frame.
                bool add( const de::classes::Sprite &_Sprite, const int &_type );
                //! Add Sprites with a de::enums::fboTypes for rendering this frame.
                bool add( std::vector<de::classes::Sprite> &_Sprites, const int &_type );
                //! Add a Line with a de::enums::fboTypes for rendering this frame.
                bool add( const de::classes::Line &_Line, const int &_type );
                bool add( renderObject *_Object );
                //! Add a Coloured Poly with a de::enums::fboTypes for rendering this frame.
                bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority, const int &_type );

                void toggleFullscreen();
                void resize( int _screenWidth, int _screenHeight );

                const de::graphics::VideoInfo& getVideoSettings();

                Uint32 loadTexture( const std::string& _fileName, int &_width, int &_height );
                void unloadTexture( Uint32 &_texture );

                bool loadShader( lua_State* L, Shader &_shader, const std::string &_name );
                bool unloadShader( Shader &_shader );

            private:
                IGraphics* wrapped;
        };

    }
}
#endif // LOGGEDGRAPHICS_H
