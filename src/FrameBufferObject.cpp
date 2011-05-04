#include "FrameBufferObject.h"
#include "GameServices.h"
#include "corona.h"
#include "openGL.h"

using namespace de::enums;
using namespace de::classes;

namespace de
{
    namespace graphics
    {
        static Uint32 make_buffer( GLenum target, const void *buffer_data, GLsizei buffer_size )
        {
            Uint32 buffer;
            CHECKGL( glGenBuffers( 1, &buffer ) );
            CHECKGL( glBindBuffer( target, buffer ) );
            CHECKGL( glBufferData( target, buffer_size, buffer_data, GL_DYNAMIC_DRAW ) );
            return buffer;
        }


        FBO::FBO() :
            image(0), depthTex(0), fboHandle(0), depthBuffer(0)
        {
        }

        FBO::~FBO()
        {
            CHECKGL( glDeleteFramebuffers( 1, &fboHandle ) );
            CHECKGL( glDeleteRenderbuffers( 1, &depthBuffer ) );
            CHECKGL( glDeleteTextures( 1,&image ) );
        }

        void FBO::setup( const int &_width, const int &_height )
        {
            CHECKGL( glGenFramebuffers( 1, &fboHandle ) );
            CHECKGL( glBindFramebuffer( GL_FRAMEBUFFER, fboHandle ) );


            CHECKGL( glGenTextures(1, &depthTex) );
            CHECKGL( glBindTexture(GL_TEXTURE_2D, depthTex) );
            CHECKGL( glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY) );
            CHECKGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE) );
            CHECKGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL) );
            CHECKGL( glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, _width, _height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL) );
            CHECKGL( glGenerateMipmap( GL_TEXTURE_2D ) );
            CHECKGL( glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0 ) );

/*
            CHECKGL( glGenRenderbuffers(1, &depthBuffer) );
            CHECKGL( glBindRenderbuffer( GL_RENDERBUFFER, depthBuffer ) );
            CHECKGL( glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height) );
            CHECKGL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer) );
*/

            CHECKGL( glGenTextures( 1, &image) );
            CHECKGL( glBindTexture( GL_TEXTURE_2D, image ) );
            CHECKGL( glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE) );
            CHECKGL( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL ) );
            CHECKGL( glGenerateMipmap( GL_TEXTURE_2D ) );
            CHECKGL( glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, image, 0 ) );


            GLenum status;
            CHECKGL( status = glCheckFramebufferStatus(GL_FRAMEBUFFER) );


            if( status == GL_FRAMEBUFFER_UNSUPPORTED )
                de::io::error << "GL_FRAMEBUFFER_UNSUPPORTED\n";
        }

        void FBO::freeAll()
        {
            if( fboHandle )
            {
                CHECKGL( glDeleteFramebuffers( 1, &fboHandle ) );
                fboHandle = 0;
            }
            if( depthBuffer )
            {
                CHECKGL( glDeleteRenderbuffers( 1, &depthBuffer ) );
                depthBuffer = 0;
            }
            if( image )
            {
                CHECKGL( glDeleteTextures( 1, &image ) );
                image = 0;
            }
            if( depthTex )
            {
                CHECKGL( glDeleteTextures( 1, &depthTex ) );
                depthTex = 0;
            }
        }




        FboBase::FboBase(): vertexBuffer(0), textureBuffer(0), elementBuffer(0), ready(false), reload(false), width(0), height(0) {}

        FboBase::~FboBase()
        {
        }

        void FboBase::setupBuffers( const int &_width, const int &_height )
        {
            fbo.setup( _width, _height );
        }

        void FboBase::needToLoadTextures()
        {
            reload = true;
        }

        void FboBase::freeAll()
        {
            reload = true;

            fbo.freeAll();

            if( vertexBuffer )
            {
                glDeleteBuffers( 1, &vertexBuffer );
                vertexBuffer = 0;
            }

            if( textureBuffer )
            {
                glDeleteBuffers( 1, &textureBuffer );
                textureBuffer = 0;
            }

            if( elementBuffer )
            {
                glDeleteBuffers( 1, &elementBuffer );
                elementBuffer = 0;
            }

        }



        void FrameBufferObject::activate( const VideoInfo &_videoSettings )
        {
            CHECKGL( glMatrixMode( GL_PROJECTION ) );
            CHECKGL( glLoadIdentity() );

            CHECKGL( glOrtho( 0, width, height, 0, -1, 1 ) );
            CHECKGL( glViewport( static_cast<GLint>(0), static_cast<GLint>(0), static_cast<GLsizei>(width), static_cast<GLsizei>(height) ) );

            if( reload )
            {
                reload = false;
                setup( width, height );
            }

            CHECKGL( glBindFramebuffer( GL_FRAMEBUFFER, fbo.fboHandle ) );

            CHECKGL( glClearColor( 1.0, 1.0, 1.0, 0.0  ) );
            CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
        }

        void FrameBufferObject::setup( const int &_width, const int &_height )
        {
            width = _width;
            height = _height;

            de::io::error << "rofl";
            setupBuffers( _width, _height );
			/*
            sprite.priority = 2;
            sprite.setTexture( fbo.image );
            sprite.coords.Setup( _width, _height, false );
            sprite.texCoords.Setup( 1, 1 );
            sprite.texCoords.flip( true );*/
        }

        void FrameBufferObject::handleEvent( const SDL_Event &_event )
        {
        }
		/*
        Sprite FrameBufferObject::render( const VideoInfo &_videoSettings )
        {
            return sprite;
        }
		*/


        void FboGame::activate( const VideoInfo &_videoSettings )
        {
            if( _videoSettings.fullScreen )
            {
                width = ( ( _videoSettings.fullscreenHeight/4 )*3 );
                height = _videoSettings.fullscreenHeight;
            }
            else
            {
                width = ( ( _videoSettings.screenHeight/4 )*3 );
                height = _videoSettings.screenHeight;
            }

            CHECKGL( glMatrixMode( GL_PROJECTION ) );
            CHECKGL( glLoadIdentity() );

            CHECKGL( glOrtho( 0, 600, 800, 0, -1, 1 ) );
            CHECKGL( glViewport( 0, 0, width, height ) );

            if( reload )
            {
                reload = false;
                setup( width, height );
            }

            CHECKGL( glBindFramebuffer( GL_FRAMEBUFFER, fbo.fboHandle ) );

            CHECKGL( glClearColor( 1.0, 1.0, 1.0, 0.0  ) );
            CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
        }

        void FboGame::setup( const int &_width, const int &_height )
        {
            width = _width;
            height = _height;

            de::io::error << "lol";
            setupBuffers( _width, _height );


            Frect rect;
            rect.x = 700;
            rect.y = 400;
            rect.w = 600;
            rect.h = 800;
			/*
            sprite.priority = 3;
            sprite.setTexture( fbo.image );
            sprite.coords.Setup( rect, 0 );
            sprite.texCoords.Setup( 1, 1 );
            sprite.texCoords.flip( true );*/
        }

        void FboGame::handleEvent( const SDL_Event &_event )
        {
        }
		/*
        Sprite FboGame::render( const VideoInfo &_videoSettings )
        {
            return sprite;
        }*/



        void FboUI::activate( const VideoInfo &_videoSettings )
        {
            if( _videoSettings.fullScreen )
            {
                width = _videoSettings.fullscreenWidth;
                height = _videoSettings.fullscreenHeight;
            }
            else
            {
                width = _videoSettings.screenWidth;
                height = _videoSettings.screenHeight;
            }


            CHECKGL( glMatrixMode( GL_PROJECTION ) );
            CHECKGL( glLoadIdentity() );

            CHECKGL( glOrtho( 0, 1280.0, 800.0 * BasicSettings::globalScreenRatio, 0, -1, 1 ) );
            CHECKGL( glViewport( 0, 0, width, height ) );


            if( reload )
            {
                reload = false;
                setup( width, height );
            }

            CHECKGL( glBindFramebuffer( GL_FRAMEBUFFER, fbo.fboHandle ) );

            CHECKGL( glClearColor( 0.1686274509803922,0.1450980392156863,0.1647058823529412, 1.0  ) );
            CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
        }

        void FboUI::setup( const int &_width, const int &_height )
        {
            width = _width;
            height = _height;

            fbo.setup( _width, _height );
            fbo2.setup( _width, _height );

            static const GLfloat vertexBufferData[] =
            {
                -1.0f, -1.0f,
                 1.0f, -1.0f,
                -1.0f,  1.0f,
                 1.0f,  1.0f
            };
            static const GLuint elementBufferData[] = { 0, 1, 2, 3 };

            vertexBuffer = make_buffer( GL_ARRAY_BUFFER, vertexBufferData, sizeof(vertexBufferData) );
            elementBuffer = make_buffer( GL_ELEMENT_ARRAY_BUFFER, elementBufferData, sizeof(elementBufferData) );

            fbo.shader = Engine::Resources().getShader( "Dof" );
            fbo2.shader = Engine::Resources().getShader( "Fullscreen" );
            /*fbo.shader = Engine::Resources().getShader( "Dof" );
            fbo2.shader = Engine::Resources().getShader( "Dof" );*/

        }

        void FboUI::Shader( const std::string &_shader )
        {
            fbo.shader = Engine::Resources().getShader( _shader );
        }

        void FboUI::handleEvent( const SDL_Event &_event )
        {
        }
		/*
        Sprite FboUI::render( const VideoInfo &_videoSettings )
        {
            CHECKGL( glBindFramebuffer( GL_FRAMEBUFFER, fbo2.fboHandle ) );
            localRender( fbo, _videoSettings );

            CHECKGL( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
            localRender( fbo2, _videoSettings );

            CHECKGL( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
            localRender( fbo, _videoSettings );

            CHECKGL( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
            localRender( fbo2, _videoSettings );

            return sprite;
        }*/
		/*
        void FboUI::localRender( FBO &_fbo, const VideoInfo &_videoSettings )
        {
            _fbo.shader.setUniform( "width", width );
            _fbo.shader.setUniform( "height", height );
            _fbo.shader.setUniform( "Texture", (int)0 );
            _fbo.shader.setUniform( "DepthTexture", (int)1 );

            _fbo.shader.bindShader();

            CHECKGL( glEnable( GL_TEXTURE_2D ) );
            CHECKGL( glActiveTexture( GL_TEXTURE0 ) );
            CHECKGL( glBindTexture( GL_TEXTURE_2D, _fbo.image ) );

            CHECKGL( glActiveTexture( GL_TEXTURE1 ) );
            CHECKGL( glBindTexture( GL_TEXTURE_2D, _fbo.depthTex ) );


            CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer ) );

            CHECKGL( glVertexAttribPointer(
                _fbo.shader.getAttribute( "position" ),    /* attribute 
                2,                      // size
                GL_FLOAT,               // type 
                GL_FALSE,               // normalized? 
                sizeof(GLfloat)*2,      // stride 
                (void*)0                // array buffer offset 
            ) );
            CHECKGL( glEnableVertexAttribArray( _fbo.shader.getAttribute( "position" ) ) );

            CHECKGL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementBuffer ) );
            CHECKGL( glDrawElements(
                GL_TRIANGLE_STRIP,  // mode 
                4,                  // count 
                GL_UNSIGNED_INT,    // type 
                (void*)0            // element array buffer offset 
            ) );

            CHECKGL( glDisableVertexAttribArray( _fbo.shader.getAttribute( "position" ) ) );

            CHECKGL( glUseProgram( 0 ) );
            CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
            CHECKGL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );


            CHECKGL( glDisableClientState( GL_COLOR_ARRAY ) );
            CHECKGL( glDisableClientState( GL_VERTEX_ARRAY ) );
            CHECKGL( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
        }
		*/
    }
}
