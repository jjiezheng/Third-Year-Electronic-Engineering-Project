#include "Graphics.h"

#include "Monitor.h"
#include "Input.h"
#include "CoreGraphics.h"

#include "openGL.h"
#include <corona.h>



using namespace de::classes;
using namespace de::internal;
using namespace de::enums;
using namespace de::filesystem;

namespace fs = boost::filesystem;

namespace de
{
    namespace graphics
    {
        Graphics::Graphics( const VideoInfo &_videoSettings )
            :GameBuffer( glVersion::GL_1_1 ),
             UIBuffer( glVersion::GL_1_1 ),
             OtherBuffer( glVersion::GL_1_1 ),
             backgroundColour( 1.0, 1.0, 1.0, 0.0 )
        {
            videoInfo = SDL_GetVideoInfo();
            videoSettings = _videoSettings;

            setGraphicsContext( videoSettings );
        }

        Graphics::Graphics()
            :GameBuffer( glVersion::GL_1_1 ),
             UIBuffer( glVersion::GL_1_1 ),
             OtherBuffer( glVersion::GL_1_1 ),
             backgroundColour( 1.0, 1.0, 1.0, 0.0 )
        {
            luaState = lua_open();
            luaL_openlibs(luaState);
            luabind::open(luaState);

            videoInfo = SDL_GetVideoInfo();
            videoSettings = getSettings();

            setGraphicsContext( videoSettings );
        }

        Graphics::~Graphics()
        {
            saveSettings();

            lua_close(luaState);

            freeAll();
        }

        VideoInfo Graphics::getSettings()
        {
            fs::path dir( Roots->get( root::SETTINGS ) + "Graphics.lua" );

            luaL_dofile( luaState, dir.file_string().c_str() );

            luabind::object global = luabind::globals(luaState);
            luabind::object GraphicsTable = global["Graphics"];
            luabind::object WindowedTable = GraphicsTable["Windowed"];
            luabind::object FullScreenTable = GraphicsTable["FullScreen"];

            if( luabind::type(GraphicsTable) != LUA_TTABLE ||
                luabind::type(WindowedTable) != LUA_TTABLE ||
                luabind::type(FullScreenTable) != LUA_TTABLE  )
            {
                de::io::error << "In Graphics config file. Problem with tables!\n";
            }

            VideoInfo _videoInfo;
            _videoInfo.postFX = luabind::object_cast<bool>( GraphicsTable["postFX"] );
            _videoInfo.fullScreen = luabind::object_cast<bool>( GraphicsTable["fullScreen"] );
            _videoInfo.colorDepth = luabind::object_cast<int>( GraphicsTable["colorDepth"] );
            _videoInfo.frameRate = luabind::object_cast<int>( GraphicsTable["frameRate"] );

            _videoInfo.screenWidth = luabind::object_cast<int>( WindowedTable["screenWidth"] );
            _videoInfo.screenHeight = luabind::object_cast<int>( WindowedTable["screenHeight"] );
            _videoInfo.AA = luabind::object_cast<int>( WindowedTable["antiAliasing"] );

            _videoInfo.fullscreenWidth = luabind::object_cast<int>( FullScreenTable["screenWidth"] );
            _videoInfo.fullscreenHeight = luabind::object_cast<int>( FullScreenTable["screenHeight"] );
            _videoInfo.AAFullscreen = luabind::object_cast<int>( FullScreenTable["antiAliasing"] );

            return _videoInfo;
        }

        void Graphics::saveSettings()
        {
            de::io::log << "Saving Graphics settings\n";

            fs::path serialize( Roots->get( root::LUA ) + "serialize.lua" );
            fs::path savePath( Roots->get( root::SETTINGS ) + "Graphics.lua" );

            luaL_dofile( luaState, serialize.file_string().c_str() );

            luabind::object global = luabind::globals(luaState);
            luabind::object GraphicsTable = global["Graphics"];
            luabind::object WindowedTable = GraphicsTable["Windowed"];
            luabind::object FullScreenTable = GraphicsTable["FullScreen"];
            luabind::object serializeFunction = global["serialize"];


            GraphicsTable["postFX"] = videoSettings.postFX;
            GraphicsTable["fullScreen"] = videoSettings.fullScreen;
            GraphicsTable["colorDepth"] = videoSettings.colorDepth;
            GraphicsTable["frameRate"] = videoSettings.frameRate;

            WindowedTable["screenWidth"] = videoSettings.screenWidth;
            WindowedTable["screenHeight"] = videoSettings.screenHeight;
            WindowedTable["antiAliasing"] = videoSettings.AA;

            FullScreenTable["screenWidth"] = videoSettings.fullscreenWidth;
            FullScreenTable["screenHeight"] = videoSettings.fullscreenHeight;
            FullScreenTable["antiAliasing"] = videoSettings.AAFullscreen;

            try
            {
                std::string serialisedTable = luabind::call_function<std::string>( global["serialize"], GraphicsTable );

                std::ofstream file;
                file.open( savePath.file_string().c_str() );
                file << "Graphics = " << serialisedTable << "\n";
                file.close();
            }
            catch( luabind::error &e)
            {
                std::string error = lua_tostring( e.state(), -1 );
                de::io::error << e.what() << "\n";
                de::io::error << error << "\n";
            }
        }

        void Graphics::setupFbos()
        {
            fboGame.setup( ( ( screenHeight/4 )*3 ), screenHeight );
            fboUI.setup( screenWidth, screenHeight );
        }

        void Graphics::freeAll()
        {
            if( videoSettings.postFX )
            {
                fboGame.freeAll();
                fboUI.freeAll();
                fbosSetup = false;
            }

            GameBuffer.clear();
            UIBuffer.clear();
            OtherBuffer.clear();
        }

        const VideoInfo& Graphics::getVideoSettings()
        {
            return videoSettings;
        }

        void Graphics::toggleFullscreen()
        {
            videoSettings.fullScreen = !videoSettings.fullScreen;

            setGraphicsContext( videoSettings );

            fboGame.needToLoadTextures();
            fboUI.needToLoadTextures();

            events::pushEvent( de::enums::events::OPENGL_RELOAD );
        }

        void Graphics::resize( int _screenWidth, int _screenHeight )
        {
            if( videoSettings.fullScreen )
            {
                videoSettings.fullscreenWidth = _screenWidth;
                videoSettings.fullscreenHeight = _screenHeight;
            }
            else
            {
                videoSettings.screenWidth = _screenWidth;
                videoSettings.screenHeight = _screenHeight;
            }
            setGraphicsContext( videoSettings );

            events::pushEvent( de::enums::events::OPENGL_RELOAD );
        }

        frameDetails Graphics::getFrameInfo()
        {
            int number( spriteNumber );
            spriteNumber = 0;
            return frameDetails( number, (int)( GameBuffer.size() + UIBuffer.size() + OtherBuffer.size() ), screenWidth, screenHeight );
        }




        bool Graphics::add( std::vector<Sprite> &_Sprites, const int &_type )
        {
            static std::vector<Sprite>::iterator iterSprite;
            for( iterSprite = _Sprites.begin(); iterSprite < _Sprites.end(); ++iterSprite )
            {
                add( (*iterSprite), _type );
            }
            return true;
        }

        bool Graphics::add( const Sprite &_Sprite, const int &_type )
        {
            ++spriteNumber;

            if( _type == FBO_GAME )
                GameBuffer.add( _Sprite );

            else if( _type == FBO_UI )
                UIBuffer.add( _Sprite );

            else if( _type == FBO_AFTER )
                OtherBuffer.add( _Sprite );

            else
                OtherBuffer.add( _Sprite );

            return true;
        }

        bool Graphics::add( const Line &_Line, const int &_type )
        {
            if( _type == FBO_GAME )
                GameBuffer.add( _Line );

            else if( _type == FBO_UI )
                UIBuffer.add( _Line );

            else if( _type == FBO_AFTER )
                OtherBuffer.add( _Line );

            else
                OtherBuffer.add( _Line );

            return true;
        }

        bool Graphics::add( renderObject *_Object )
        {
            Objects.push_back( _Object );
            return true;
        }

        bool Graphics::add( const Poly &_poly, const Colour &_colour, const int &_priority, const int &_type )
        {
            ++spriteNumber;

            if( _type == FBO_GAME )
                GameBuffer.add( _poly, _colour, _priority );

            else if( _type == FBO_UI )
                UIBuffer.add( _poly, _colour, _priority );

            else if( _type == FBO_AFTER )
                OtherBuffer.add( _poly, _colour, _priority );

            else
                OtherBuffer.add( _poly, _colour, _priority );


            return true;
        }


        frameDetails Graphics::render()
        {
            if( !fbosSetup && videoSettings.postFX )
            {
                setupFbos();
                fbosSetup = true;
            }

            if( multiSample ) CHECKGL( glEnable(GL_MULTISAMPLE) );

                interalRender();
                CHECKGL( SDL_GL_SwapBuffers() );

            if( multiSample ) CHECKGL( glDisable(GL_MULTISAMPLE) );

            return getFrameInfo();
        }

        void Graphics::interalRender()
        {
            getCentreVector( screenWidth, screenHeight );

            if( videoSettings.postFX )
            {
                fboGame.activate( videoSettings );

                    CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
                    GameBuffer.render();

                CHECKGL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );


                fboUI.activate( videoSettings );

                    CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
                    UIBuffer.add( fboGame.render( videoSettings ) );
                    UIBuffer.render();

                    CHECKGL( glAlphaFunc( GL_GREATER, 0.1 ) );
                    CHECKGL( glEnable( GL_ALPHA_TEST ) );
                    CHECKGL( glEnable( GL_DEPTH_TEST ) );
                        std::vector<renderObject*>::iterator objectIter;
                        for( objectIter = Objects.begin(); objectIter != Objects.end(); ++objectIter )
                        {
                            (*objectIter)->localRender();
                        }
                    CHECKGL( glDisable( GL_DEPTH_TEST ) );
                    CHECKGL( glDisable( GL_ALPHA_TEST ) );

                CHECKGL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );


                CHECKGL( glClearColor( backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a  ) );
                CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
                fboUI.render( videoSettings );
                OtherBuffer.render();
            }
            else
            {
                CHECKGL( glDisable( GL_DEPTH_TEST ) );
                CHECKGL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
                CHECKGL( glClearColor( backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a  ) );
                CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

                CHECKGL( glPushMatrix() );

                    CHECKGL( glTranslatef( centre.x , centre.y, 0.0 ) );
                    CHECKGL( glPushMatrix() );

                        CHECKGL( glTranslatef( 400 , 0.0, 0.0 ) );
                        GameBuffer.render();

                    CHECKGL( glPopMatrix() );

                    UIBuffer.render();

                    if( multiSample ) CHECKGL( glEnable(GL_MULTISAMPLE) );
                    /*CHECKGL( glAlphaFunc( GL_GREATER, 0.5 ) );
                    CHECKGL( glEnable( GL_ALPHA_TEST ) );
                    CHECKGL( glEnable( GL_DEPTH_TEST ) );*/
                    //CHECKGL( glEnable( GL_BLEND ) );

                        std::vector<renderObject*>::iterator objectIter;
                        for( objectIter = Objects.begin(); objectIter != Objects.end(); ++objectIter )
                        {
                            (*objectIter)->localRender();
                        }
						/*
                    CHECKGL( glDisable( GL_DEPTH_TEST ) );
                    CHECKGL( glDisable( GL_ALPHA_TEST ) );*/
                    //CHECKGL( glDisable( GL_BLEND ) );
                    if( multiSample ) CHECKGL( glDisable(GL_MULTISAMPLE) );

                    OtherBuffer.render();

                CHECKGL( glPopMatrix() );
            }

            Objects.clear();
        }

        bool Graphics::videoModeOk( const int &_width, const int &_height )
        {
            int depth = SDL_VideoModeOK( _width, _height, 32, SDL_OPENGL | SDL_FULLSCREEN );

            if( depth != 0 )
            {
                return true;
            }
            return false;
        }

        void Graphics::getCentreVector( int _screenWidth, int _screenHeight )
        {
            CHECKGL( glMatrixMode(GL_MODELVIEW) );
            CHECKGL( glLoadIdentity() );

            CHECKGL( glMatrixMode( GL_PROJECTION ) );
            CHECKGL( glLoadIdentity() );


            CHECKGL( glViewport(0, 0, screenWidth, screenHeight) );



            if( _screenWidth/16*10 > _screenHeight )
            {
                BasicSettings::globalScreenRatio = screenRatio = (1280.0/screenWidth)/(800.0/screenHeight);
                BasicSettings::borderHorizontal = false;

                int difference = ( _screenWidth - (_screenHeight/10*16) )/2;
                centre = Vector( difference,0 );
                BasicSettings::mouseOffset = centre;

                CHECKGL( glOrtho( 0, 1280.0/screenRatio, 800.0, 0, -1, 1 ) );

            }
            else
            {
                screenRatio = (1280.0/screenWidth)/(800.0/screenHeight);
                BasicSettings::globalScreenRatio = (1280.0/screenWidth);
                BasicSettings::borderHorizontal = true;

                int difference = ( _screenHeight - (_screenWidth/16*10)  )/2;

                centre = Vector( 0,difference );
                BasicSettings::mouseOffset = centre;

                CHECKGL( glOrtho( 0, 1280.0, 800.0*screenRatio, 0, -1, 1 ) );
            }
        }


        void Graphics::setOpenGLContext( int _width, int _height, int _aa, int _colourDepth, int _flags )
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

                de::io::log << "AA " << _aa  << "X |";
                multiSample = true;
            }
            else
            {
                SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 0 );
                SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 0 );

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

        bool Graphics::setGraphicsContext( const VideoInfo &_videoSettings )
        {
            freeAll();
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


            std::string vendor = (char*)glGetString(GL_VENDOR);
            std::string renderer = (char*)glGetString(GL_RENDERER);
            std::string version = (char*)glGetString(GL_VERSION);
            CHECKGL_MSG( "glGetString" );


            getCentreVector( screenWidth, screenHeight );

            CHECKGL( glClearColor( backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a  ) );
            CHECKGL( glClearDepth( 1 ) );


            CHECKGL( glEnable( GL_TEXTURE_2D ) );
            CHECKGL( glEnable( GL_BLEND ) );
            CHECKGL( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
            CHECKGL( glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA) );
            CHECKGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR ) );
            CHECKGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );


            glVersion::glVersions OpenGL = getOpenGLVersion( version );
            glslVersion::glslVersions GLSL = getGLSLVersion( version );

            videoSettings.OpenGL = getOpenGLVersion( version );
            videoSettings.GLSL = getGLSLVersion( version );

            if( videoSettings.OpenGL > glVersion::GL_3_0 )
            {
                videoSettings.OpenGL = glVersion::GL_3_0;
                videoSettings.GLSL = glslVersion::GLSL_130;
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
            de::io::log << "Using | OpenGL | " << glToString( videoSettings.OpenGL ) << " | GLSL | " << glToString( videoSettings.GLSL ) << "\n";
            de::io::log << "Graphics card | " << renderer << " | " << vendor << "\n";


            GameBuffer.set( videoSettings.OpenGL );
            UIBuffer.set( videoSettings.OpenGL );
            OtherBuffer.set( videoSettings.OpenGL );

            return true;
        }




        Uint32 Graphics::loadTexture( const std::string& _fileName, int &_width, int &_height )
        {
            return textureModule.load( _fileName, _width, _height, GL_TEXTURE_2D );
        }

        Uint32 Graphics::loadTexture( const std::string& _fileName, int &_width, int &_height, int _textureDimensions  )
        {
            return textureModule.load( _fileName,_width, _height, _textureDimensions );
        }

        void Graphics::unloadTexture( Uint32 &_texture )
        {
            textureModule.unload( _texture );
        }


        bool Graphics::loadShader( lua_State* L, Shader &_shader, const std::string &_name )
        {
            return shaderModule.load( L, _shader, _name, videoSettings.GLSL );
        }

        bool Graphics::unloadShader( Shader &_shader )
        {
            return shaderModule.unload( _shader );
        }
    }
}
