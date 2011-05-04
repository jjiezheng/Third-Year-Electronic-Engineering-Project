#ifndef FRAMEBUFFEROBJECTS_H_
#define FRAMEBUFFEROBJECTS_H_

#include "CoreEnumsAndClasses.h"
#include "Coords.h"
#include "frameDetails.h"


namespace de
{
    namespace graphics
    {
        //! Class that holds a handle to a Frame buffer object along with depthbuffer, Texture and Shader.
        class FBO
        {
            public:
                FBO();
                ~FBO();

                void setup( const int &_width, const int &_height );
                void freeAll();

                //! Testing work-around for ATI cards.
                //de::classes::Sprite sprite;
                //! The Texture that will be the render target.
                Uint32 image, depthTex;
                //! Handle to the actual Frame buffer object.
                Uint32 fboHandle;
                //! Handle to the depth buffer used when rendering to "image".
                Uint32 depthBuffer;
                //! The Shader used when rendering "image".
                de::graphics::Shader shader;

        };

        //! Virtual Base class for FrameBufferObject Classes
        /*!
        */
        class FboBase
        {
            public:
                //! Basic Constructor
                FboBase();
                //! Virtual Destructor
                virtual ~FboBase();

                void *pixels;

                //! Reloads "image", "fboHandle" and "Shader".
                void needToLoadTextures();

                //! Frees up "image", "fboHandle" and "Shader".
                void freeAll();

                //! Virtual function to be derived by inheriting classes
                virtual void activate( const VideoInfo &_videoSettings ) = 0;
                //! Virtual function to be derived by inheriting classes
                //virtual de::classes::Sprite render( const VideoInfo &_videoSettings ) = 0;
                //! Handles input
                virtual void handleEvent( const SDL_Event &_event ) = 0;
                //! Virtual function to be derived by inheriting classes
                virtual void setup( const int &_width, const int &_height ) = 0;


            protected:
                //! Testing work-around for ATI cards.
                //de::classes::Sprite sprite;
                //! Buffers used for rendering "image"
                Uint32 vertexBuffer, colourBuffer, textureBuffer, elementBuffer;
                //! Class that contains the handle to the Frame buffer object
                FBO fbo;
                //! If the class is fully intialised.
                bool ready;
                //! If the class needs to reload.
                bool reload;
                //! The width of "image"
                float width;
                //! The height of "image"
                float height;

                //! Sets up "image" and "fboHandle" along with everything else.
                void setupBuffers( const int &_width, const int &_height );
        };


        //! Derived class of FboBase.
        /*! For rendering to texture
            */
        class FrameBufferObject : public FboBase
        {
            public:
                //! Basic Constructor
                FrameBufferObject(){};
                //! Basic Destructor
                ~FrameBufferObject() {};

                //! Makes the Frame buffer object within FboGame the render target.
                void activate( const VideoInfo &_videoSettings );
                //! Set ude the class. Acts like a constructor.
                void setup( const int &_width, const int &_height );
                //! Handles input
                void handleEvent( const SDL_Event &_event );
                //! Renders the Texture "image" to the current render target. Can't render to itself.
                //de::classes::Sprite render( const VideoInfo &_videoSettings );

            private:

        };

        //! Derived class of FboBase.
        /*! Used when rendering to the game area.
            */
        class FboGame : public FboBase
        {
            public:
                //! Basic Constructor
                FboGame(){};
                //! Basic Destructor
                ~FboGame() {};

                //! Makes the Frame buffer object within FboGame the render target.
                void activate( const VideoInfo &_videoSettings );
                //! Set ude the class. Acts like a constructor.
                void setup( const int &_width, const int &_height );
                //! Handles input
                void handleEvent( const SDL_Event &_event );
                //! Renders the Texture "image" to the current render target. Can't render to itself.
                //de::classes::Sprite render( const VideoInfo &_videoSettings );

            private:

        };

        //! Derived class of FboBase.
        /*! Used when rendering to the UI area.
            */
        class FboUI : public FboBase
        {
            public:

                //! Basic Constructor
                FboUI(){};
                //! Basic Destructor
                ~FboUI() {};

                //! Makes the Frame buffer object within FboGame the render target.
                void activate( const VideoInfo &_videoSettings );
                //! Setude the class. Acts like a constructor.
                void setup( const int &_width, const int &_height );

                void Shader( const std::string &_shader );
                //! Handles input
                void handleEvent( const SDL_Event &_event );
                //! Renders the Texture "image" to the current render target. Can't render to itself.
                //de::classes::Sprite render( const VideoInfo &_videoSettings );



            private:
                void localRender( FBO &_fbo, const VideoInfo &_videoSettings );

                FBO fbo2;
        };


    }
}

#endif //FRAMEBUFFEROBJECTS_H_
