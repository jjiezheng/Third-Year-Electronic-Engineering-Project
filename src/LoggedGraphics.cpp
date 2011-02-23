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

        void LoggedGraphics::freeAll()
        {
            de::io::log << "freeAll was called.\n";
            wrapped->freeAll();
        }

        frameDetails LoggedGraphics::render()
        {
            de::io::log << "render was called.\n";
            return wrapped->render();
        }

        bool LoggedGraphics::add( const de::classes::Sprite &_Sprite, const int &_type )
        {
            //de::io::log << "add was called with a Sprite.\n";
            return wrapped->add( _Sprite, _type );
        }

        bool LoggedGraphics::add( std::vector<de::classes::Sprite> &_Sprites, const int &_type )
        {
            //de::io::log << "add was called with Sprites.\n";
            return wrapped->add( _Sprites, _type );
        }

        bool LoggedGraphics::add( const de::classes::Line &_Line, const int &_type )
        {
            //de::io::log << "add was called with a Line.\n";
            return wrapped->add( _Line, _type );
        }

        bool LoggedGraphics::add( renderObject *_Object )
        {
            //de::io::log << "add was called with a Line.\n";
            return wrapped->add( _Object );
        }

        bool LoggedGraphics::add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority, const int &_type )
        {
            //de::io::log << "add was called with a Poly.\n";
            return wrapped->add( _poly, _colour, _priority, _type );
        }

        void LoggedGraphics::toggleFullscreen()
        {
            de::io::log << "toggleFullscreen was called.\n";
            wrapped->toggleFullscreen();
        }

        void LoggedGraphics::resize( int _screenWidth, int _screenHeight )
        {
            de::io::log << "resize was called with _screenWidth = " << _screenWidth << " and _screenHeight = " << _screenHeight << "\n";
            wrapped->resize( _screenWidth, _screenHeight );
        }


        bool LoggedGraphics::videoModeOk( const int &_width, const int &_height )
        {
            de::io::log << "checkGraphicsContext was called with _width = " << _width << " and _height = " << _height << "\n";
            return wrapped->videoModeOk( _width, _height );
        }

        const de::graphics::VideoInfo& LoggedGraphics::getVideoSettings()
        {
            de::io::log << "getVideoSettings was called.\n";
            return wrapped->getVideoSettings();
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
