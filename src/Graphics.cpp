#include "Graphics.h"

#include "Monitor.h"
#include "Input.h"
#include "CoreGraphics.h"

#include "openGL.h"
#include <corona.h>
#include "Haptics.h"

using namespace de::filesystem;
namespace fs = boost::filesystem;

namespace de
{
    namespace graphics
    {
        Graphics::Graphics( const VideoInfo &_videoSettings )
        {
            videoInfo = SDL_GetVideoInfo();
            videoSettings = _videoSettings;

             context.create( videoSettings );
        }
        Graphics::Graphics()
        {
            luaState = lua_open();
            luaL_openlibs(luaState);
            luabind::open(luaState);

            fs::path dir( Roots->get( root::SETTINGS ) + "Graphics.lua" );
            luaL_dofile( luaState, dir.file_string().c_str() );

            videoSettings = getSettings();
            context.create( videoSettings );
        }
        Graphics::~Graphics()
        {
            saveSettings();
            lua_close(luaState);
        }

        VideoInfo Graphics::getSettings()
        {
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

        const VideoInfo& Graphics::getVideoSettings()
        {
            return videoSettings;
        }

		void Graphics::setVideoSettings( const de::graphics::VideoInfo &_info )
		{
			videoSettings = _info;
			context.create( videoSettings );
            events::pushEvent( de::enums::events::OPENGL_RELOAD );
		}

        frameDetails Graphics::getFrameInfo()
        {
            return frameDetails( context.width(), context.height() );
        }

		void Graphics::fullscreen( bool _fullscreen )
        {
            videoSettings.fullScreen = _fullscreen;

            context.create( videoSettings );
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
            context.create( videoSettings );
            events::pushEvent( de::enums::events::OPENGL_RELOAD );
        }


        bool Graphics::add( renderObject *_Object )
        {
            Objects.push_back( _Object );
            return true;
        }

		bool Graphics::add( renderObject *_Object, int _type )
        {
            HapticsObjects.push_back( _Object );
            return true;
        }



        frameDetails Graphics::render()
        {
            if( context.AA() ) CHECKGL( glEnable(GL_MULTISAMPLE) );

                interalRender();
                CHECKGL( SDL_GL_SwapBuffers() );

            if( context.AA() ) CHECKGL( glDisable(GL_MULTISAMPLE) );

            return getFrameInfo();
        }
        void Graphics::interalRender()
        {
			CHECKGL( glClearColor( 0.0f,0.0f,0.0f,1.0f ) );
			CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
				hlBeginFrame();
				std::vector<renderObject*>::iterator objectIter;
                for( objectIter = HapticsObjects.begin(); objectIter != HapticsObjects.end(); ++objectIter )
                {
                    (*objectIter)->actualRender();
                }
				hlEndFrame();
            HapticsObjects.clear();

            CHECKGL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

                for( objectIter = Objects.begin(); objectIter != Objects.end(); ++objectIter )
                {
                    (*objectIter)->actualRender();
                }

            Objects.clear();
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


		void Graphics::pushSettings( const std::string &_serialisedText )
		{
			luaL_dostring( luaState, _serialisedText.c_str() );
            videoSettings = getSettings();
            context.create( videoSettings );
			events::pushEvent( de::enums::events::OPENGL_RELOAD );
		}
    }
}
