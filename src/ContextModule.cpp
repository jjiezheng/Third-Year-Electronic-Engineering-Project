#include "ContextModule.h"
#include "openGL.h"
#include "base.h"

namespace de
{
	namespace graphics
	{
		ContextModule::ContextModule() : screenWidth(0), screenHeight(0), multiSample(false)
		{
			//ctor
		}

		ContextModule::~ContextModule()
		{
			//dtor
		}

		bool ContextModule::AA()
		{
			return multiSample;
		}
		int ContextModule::width()
		{
			return screenWidth;
		}
		int ContextModule::height()
		{
			return screenHeight;
		}

		bool ContextModule::create( VideoInfo &_videoSettings )
		{
			de::io::log << "Setting Graphics Mode to | ";

            if( _videoSettings.fullScreen )
            {
                de::io::log << "Fullscreen | ";
                setOpenGLContext(
                    _videoSettings.fullscreenWidth,
                    _videoSettings.fullscreenHeight,
                    _videoSettings.AAFullscreen,
                    _videoSettings.colorDepth,
                    SDL_OPENGL | SDL_FULLSCREEN
                );
            }
            else
            {
                de::io::log << "Windowed | ";
                setOpenGLContext(
                    _videoSettings.screenWidth,
                    _videoSettings.screenHeight,
                    _videoSettings.AA,
                    _videoSettings.colorDepth,
                    SDL_OPENGL | SDL_RESIZABLE
                );
            }

			
			getVersion( _videoSettings );

			return true;
		}

		void ContextModule::setOpenGLContext( int _width, int _height, int _aa, int _colourDepth, int _flags )
        {
            SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 32 );
            SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

            if( GL_ARB_multisample && _aa )
            {
                SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
                SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, _aa );
				CHECKGL( glEnable(GL_MULTISAMPLE) );

                de::io::log << "AA " << _aa  << "X |";
                multiSample = true;
            }
            else
            {
                SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 0 );
                SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 0 );
				CHECKGL( glDisable(GL_MULTISAMPLE) );

                multiSample = false;
            }

            de::io::log << _width << " X " << _height << "\n";
            if( SDL_SetVideoMode( _width, _height, _colourDepth, _flags ) == NULL )
            {
                de::io::error << "Error setting graphics context, defaulting to lowest settings.\n";
                if( SDL_SetVideoMode( 600, 800, 32, SDL_OPENGL | SDL_RESIZABLE ) == NULL )
                {
                    de::io::error << "Lowest settings failed, exiting...\n";
                    exit(50);
                }
            }

            screenWidth = _width;
            screenHeight = _height;


        }
		void ContextModule::getVersion( VideoInfo &_videoSettings )
		{
			std::string vendor = (char*)glGetString(GL_VENDOR);
            std::string renderer = (char*)glGetString(GL_RENDERER);
            std::string version = (char*)glGetString(GL_VERSION);
            CHECKGL_MSG( "glGetString" );

            CHECKGL( glClearDepth( 1 ) );
			CHECKGL( glClearColor( 1.0, 1.0, 1.0, 0.0 ) );


            CHECKGL( glEnable( GL_TEXTURE_2D ) );
            CHECKGL( glEnable( GL_BLEND ) );
            CHECKGL( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
            CHECKGL( glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA) );
            CHECKGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR ) );
            CHECKGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );


            glVersion::glVersions OpenGL = getOpenGLVersion( version );
            glslVersion::glslVersions GLSL = getGLSLVersion( version );

            _videoSettings.OpenGL = getOpenGLVersion( version );
            _videoSettings.GLSL = getGLSLVersion( version );

            if( _videoSettings.OpenGL > glVersion::GL_3_0 )
            {
                _videoSettings.OpenGL = glVersion::GL_3_0;
                _videoSettings.GLSL = glslVersion::GLSL_130;
            }
/*
            videoSettings.OpenGL = glVersion::GL_2_1;
            videoSettings.GLSL = glslVersion::GLSL_120;
*/
/*
            videoSettings.OpenGL = glVersion::GL_3_0;
            videoSettings.GLSL = glslVersion::GLSL_130;
*/
            de::io::log << "Available | OpenGL | " << glToString( OpenGL ) << " | GLSL | " << glToString( GLSL ) << "\n";
            de::io::log << "Using | OpenGL | " << glToString( _videoSettings.OpenGL ) << " | GLSL | " << glToString( _videoSettings.GLSL ) << "\n";
            de::io::log << "Graphics card | " << renderer << " | " << vendor << "\n";

		}
	}
}