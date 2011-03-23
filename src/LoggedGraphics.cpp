#include "LoggedGraphics.h"
#include "CoreEnumsAndClasses.h"
#include "renderObject.h"
#include "Log.h"

namespace de
{
    namespace graphics
    {
        LoggedGraphics::LoggedGraphics(IGraphics *_wrapped) : wrapped(_wrapped)
        {

        }

        LoggedGraphics::~LoggedGraphics()
        {
            delete wrapped;
        }

        frameDetails LoggedGraphics::render()
        {
            de::io::log << "render was called.\n";
            return wrapped->render();
        }

        bool LoggedGraphics::add( renderObject *_Object )
        {
            //de::io::log << "add was called with a Line.\n";
            return wrapped->add( _Object );
        }

        void LoggedGraphics::fullscreen( bool _fullscreen )
        {
            de::io::log << "fullscreen was called.\n";
            wrapped->fullscreen( _fullscreen );
        }

        void LoggedGraphics::resize( int _screenWidth, int _screenHeight )
        {
            de::io::log << "resize was called with _screenWidth = " << _screenWidth << " and _screenHeight = " << _screenHeight << "\n";
            wrapped->resize( _screenWidth, _screenHeight );
        }

        const de::graphics::VideoInfo& LoggedGraphics::getVideoSettings()
        {
            de::io::log << "getVideoSettings was called.\n";
            return wrapped->getVideoSettings();
        }

		void LoggedGraphics::setVideoSettings( const de::graphics::VideoInfo &_info )
		{
			de::io::log << "setVideoSettings was called.\n";
			wrapped->setVideoSettings( _info );
		}
        Uint32 LoggedGraphics::loadTexture( const std::string& _fileName, int &_width, int &_height )
        {
            de::io::log << "loadTexture was called with _fileName " << _fileName << ".\n";
            return wrapped->loadTexture( _fileName, _width, _height );
        }

        void LoggedGraphics::unloadTexture( Uint32 &_texture )
        {
            de::io::log << "unloadTexture was called with _texture " << (int)_texture << ".\n";
            wrapped->unloadTexture( _texture );
        }


        bool LoggedGraphics::loadShader( lua_State* L, Shader &_shader, const std::string &_name )
        {
            de::io::log << "loadShader was called with _shader = " << (int)_shader.get( Shader::PROGRAM ) << " and _name = " << _name << "\n";
            return wrapped->loadShader( L, _shader, _name );
        }

        bool LoggedGraphics::unloadShader( Shader &_shader )
        {
            de::io::log << "unloadShader was called with _shader " << (int)_shader.get( Shader::PROGRAM ) << ".\n";
            return wrapped->unloadShader( _shader );
        }

    }
}
