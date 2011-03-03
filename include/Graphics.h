#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Base.h"
#include "frameDetails.h"
#include "IGraphics.h"
#include "Material.h"
#include "renderObject.h"


#include "ShaderModule.h"
#include "TextureModule.h"
#include "ContextModule.h"


namespace de
{
    namespace graphics
    {

        //! Handles all rendering to the screen.
        /*!This class handles all rendering to the screen. To render a Sprite you must first pass the Sprite in with add() and then call render() at the end of the frame.
        */
        class Graphics : public IGraphics
        {
            public:
                //! Initalises the class
                Graphics( const de::graphics::VideoInfo &_videoSettings );
                Graphics();

                virtual ~Graphics();

                //!Frees all Textures, Shaders and FBO's
                void freeAll();
                //! Renders everything that has been passed into the class since the last time this function was called and then swaps the framebuffer.
                frameDetails render();

                //! Add a 3D mesh for rendering this frame.
                bool add( renderObject *_Object );

                //!Toggles Fullscreen mode
                void toggleFullscreen();
                //!Resizes screen size.
                void resize( int _screenWidth, int _screenHeight );

                bool videoModeOk( const int &_width, const int &_height );
                const de::graphics::VideoInfo& getVideoSettings();


                //!Load the texture with the name "_fileName" in the texture folder.
                Uint32 loadTexture( const std::string& _fileName, int &_width, int &_height );
                //!Unload texture "_texture"
                void unloadTexture( Uint32 &_texture );
                //!Load shader "_name"
                bool loadShader( lua_State* L, Shader &_shader, const std::string &_name );
                //!Unload the shader "_shader"
                bool unloadShader( Shader &_shader );

            private:
                Uint32 loadTexture( const std::string& _fileName, int &_width, int &_height, int _textureDimensions );

                void interalRender();
                frameDetails getFrameInfo();
                de::graphics::VideoInfo getSettings();
                void saveSettings();
                bool setGraphicsContext( const de::graphics::VideoInfo &_videoSettings );
                void setOpenGLContext( int _width, int _height, int _aa, int _colourDepth, int _flags );


            private:
                bool multiSample;
                int screenWidth, screenHeight;

                const SDL_VideoInfo* videoInfo;
                de::graphics::VideoInfo videoSettings;
                lua_State *luaState;

                std::vector< renderObject* > Objects;

                ShaderModule shaderModule;
                TextureModule textureModule;
                ContextModule context;
        };
    }
}

#endif //GRAPHICS_H_
