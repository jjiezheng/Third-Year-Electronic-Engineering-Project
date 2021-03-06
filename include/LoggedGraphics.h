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
				

                bool add( renderObject *_Object );
				bool add( renderObject *_Object, int _type );

                //! Add a Coloured Poly with a de::enums::fboTypes for rendering this frame.
                bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority, const int &_type );

                void fullscreen( bool _fullscreen );
                void resize( int _screenWidth, int _screenHeight );

                const de::graphics::VideoInfo& getVideoSettings();
				void setVideoSettings( const de::graphics::VideoInfo &_info );

                Uint32 loadTexture( const std::string& _fileName, int &_width, int &_height );
                void unloadTexture( Uint32 &_texture );

                bool loadShader( lua_State* L, Shader &_shader, const std::string &_name );
                bool unloadShader( Shader &_shader );

				void pushSettings( const std::string &_serialisedText );
            private:
                IGraphics* wrapped;
        };

    }
}
#endif // LOGGEDGRAPHICS_H
