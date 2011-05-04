#include "Input.h"


namespace de
{
    namespace events
    {
        classes::Vector mouse( 640, 480 );

        void toggleMouse()
        {
            if( SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE )
                SDL_ShowCursor(SDL_DISABLE);
            else
                SDL_ShowCursor(SDL_ENABLE);
        }

        void toggleMouseGrab()
        {
            if( SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON )
                SDL_WM_GrabInput(SDL_GRAB_OFF);
            else
                SDL_WM_GrabInput(SDL_GRAB_ON);
        }

        glm::vec3 MousePosition( const SDL_Event &_event )
        {
            return glm::vec3( _event.motion.x, _event.motion.y, 0.0f );
        }

        classes::Vector getRelativeMouseMovement( const SDL_Event &_event )
        {
            if( _event.type == SDL_MOUSEMOTION )
                return classes::Vector( _event.motion.xrel, _event.motion.yrel );

            return classes::Vector(0.0f,0.0f);
        }

        glm::vec3 getRelativeMouseMovementVec3( const SDL_Event &_event )
        {
            if( _event.type == SDL_MOUSEMOTION )
                return glm::vec3( _event.motion.xrel, _event.motion.yrel, 0.0f );

            return glm::vec3(0.0f);
        }

        float relativeMouseAngle( const SDL_Event &_event, int _axis )
        {
            if( _event.type == SDL_MOUSEMOTION )
            {
                if( mouse.x == _event.motion.x && mouse.y == _event.motion.y )
                    return 0;

                if( _axis )
                {
                    return _event.motion.yrel;
                }
                return _event.motion.xrel;
            }
            return 0;
        }

        classes::Vector getMousePosition( const SDL_Event &_event, bool _motion, de::enums::fboTypes _fbo )
        {
            if( _motion )
            {
                if( classes::BasicSettings::borderHorizontal )
                    return getMousePositionH( _event.motion.x, _event.motion.y, _fbo );
                else
                    return getMousePositionV( _event.motion.x, _event.motion.y, _fbo );

            }
            else
            {
                if( classes::BasicSettings::borderHorizontal )
                    return getMousePositionH( _event.button.x, _event.button.y, _fbo );
                else
                    return getMousePositionV( _event.button.x, _event.button.y, _fbo );

            }

            return classes::Vector(0,0);
        }

        classes::Vector getMousePosition( const SDL_Event &_event, bool _motion )
        {
            return getMousePosition( _event, _motion, enums::FBO_UI );
        }

        void resetMouse()
        {
            SDL_WarpMouse( 640, 480 );
            mouse.x = 640;
            mouse.y = 480;
        }


        classes::Vector getMousePositionV( const int &_x, const int &_y, enums::fboTypes _fbo )
        {
            float ratio = 1 / classes::BasicSettings::globalScreenRatio;
            classes::Vector mouseOffset = classes::BasicSettings::mouseOffset;

            if( _fbo == de::enums::FBO_UI || _fbo == de::enums::FBO_AFTER )
            {
                return classes::Vector(_x*ratio - mouseOffset.x, _y*ratio - mouseOffset.y );
            }
            if( _fbo == de::enums::FBO_GAME )
            {
                return classes::Vector( _x*ratio - 400 - mouseOffset.x, _y*ratio - mouseOffset.y );
            }
            return classes::Vector(0,0);
        }


        classes::Vector getMousePositionH( const int &_x, const int &_y, enums::fboTypes _fbo )
        {
            float ratio = classes::BasicSettings::globalScreenRatio;
            //ratio = 1.25;
            //de::io::log << "ratio = " << ratio << "\n";
            classes::Vector mouseOffset = classes::BasicSettings::mouseOffset;

            //mouseOffset = -mouseOffset;

            if( _fbo == de::enums::FBO_UI || _fbo == de::enums::FBO_AFTER )
            {
                return classes::Vector( ( _x*ratio - mouseOffset.x ), ( _y*ratio - mouseOffset.y ) );
            }
            if( _fbo == de::enums::FBO_GAME )
            {
                return classes::Vector( ( _x*ratio  - mouseOffset.x - 400 ), ( _y*ratio - mouseOffset.y ) );
            }
            return classes::Vector(0,0);
        }


        void showMouse( bool _show )
        {
            if( _show ) SDL_ShowCursor(SDL_ENABLE);
            else        SDL_ShowCursor(SDL_DISABLE);
        }



        void pushEvent( enums::events::SDL_EVENT_CODES _code )
        {
            SDL_Event event;
            event.type = SDL_USEREVENT;
            event.user.code = _code;
            SDL_PushEvent(&event);
        }



        bool handleBoolInput( const SDL_Event &_event, bool &_bool )
        {
            bool change(false);

            if( _event.key.keysym.unicode == (Uint16)'0' ||
                _event.key.keysym.unicode == (Uint16)'1' ||
                _event.key.keysym.unicode == (Uint16)'n' ||
                _event.key.keysym.unicode == (Uint16)'y' ||
                _event.key.keysym.unicode == (Uint16)'N' ||
                _event.key.keysym.unicode == (Uint16)'Y' )
            {
                change = true;
                if( _event.key.keysym.unicode == '0' || _event.key.keysym.unicode == 'n' || _event.key.keysym.unicode == 'N' )
                {
                    _bool = false;
                }
                else if( _event.key.keysym.unicode == '1' || _event.key.keysym.unicode == 'y' || _event.key.keysym.unicode == 'Y' )
                {
                    _bool = true;
                }
            }
            else if( _event.key.keysym.unicode == (Uint16)' ' )
            {
                change = true;
                _bool = !_bool;
            }
            return change;
        }

        bool handleFloatInput( const SDL_Event &_event, float &_float, bool &_decimalPoint, int &_decimalPosition )
        {
            bool change(false);

            if( ( _event.key.keysym.unicode >= (Uint16)'0' ) && ( _event.key.keysym.unicode <= (Uint16)'9' ) )
            {
                change = true;
                if( !_decimalPoint )
                {
                    _float = _float * 10;
                    _float = _float + UnicodeToInt( _event.key.keysym.unicode );
                }
                else //if( decimalPoint )
                {
                    ++_decimalPosition;
                    _float = _float + ( (float)UnicodeToInt( _event.key.keysym.unicode ) * 1.0f/powf(10.0f, _decimalPosition ) );
                }
            }

            else if( _event.key.keysym.unicode == (Uint16)'.' )
            {
                _decimalPoint = true;
            }
            else if( _event.key.keysym.unicode == (Uint16)'-' )
            {
                change = true;
                _float = -_float;
            }

            else if( ( _event.key.keysym.sym == SDLK_BACKSPACE ) )
            {
                change = true;
                if( !_decimalPoint )
                {
                    _float = (float)(_float/10);
                }
                // TODO (daniel#1#): Fix deleting numbers past the floating point
                else //if( decimalPoint )
                {
                    --_decimalPosition;
                    if( _decimalPosition <= 0 )
                    {
                        _decimalPoint = false;
                        _decimalPosition = 0;
                    }
                }
            }

            return change;
        }

        bool handleIntInput( const SDL_Event &_event, int &_int )
        {
            bool change(false);
            if( ( _event.key.keysym.unicode >= (Uint16)'0' ) && ( _event.key.keysym.unicode <= (Uint16)'9' ) )
            {
                if( _int < 9999999 )
                {
                    _int = _int * 10;
                    _int = _int + UnicodeToInt( _event.key.keysym.unicode );
                    change = true;
                }
            }
            else if( _event.key.keysym.unicode == (Uint16)'-' )
            {
                _int = -_int;
                change = true;
            }
            else if( ( _event.key.keysym.sym == SDLK_BACKSPACE ) )
            {
                 _int = _int / 10;
                 change = true;
            }
            return change;
        }

        bool handleStringInput( const SDL_Event &_event, std::string &_string )
        {
            bool change(false);
            //If the key is a space
            if( _event.key.keysym.unicode == (Uint16)' ' )
            {
                change = true;
                _string += (char)_event.key.keysym.unicode;
            }
            //If the key is a number
            else if( ( _event.key.keysym.unicode >= (Uint16)'0' ) && ( _event.key.keysym.unicode <= (Uint16)'9' ) )
            {
                change = true;
                _string += (char)_event.key.keysym.unicode;
            }
            //If the key is a uppercase letter
            else if( ( _event.key.keysym.unicode >= (Uint16)'A' ) && ( _event.key.keysym.unicode <= (Uint16)'Z' ) )
            {
                change = true;
                _string += (char)_event.key.keysym.unicode;
            }
            //If the key is a lowercase letter
            else if( ( _event.key.keysym.unicode >= (Uint16)'a' ) && ( _event.key.keysym.unicode <= (Uint16)'z' ) )
            {
                change = true;
                _string += (char)_event.key.keysym.unicode;
            }

                //If backspace was pressed and the string isn't blank
            else if( ( _event.key.keysym.sym == SDLK_BACKSPACE ) && ( _string.length() != 0 ) )
            {
                change = true;
                _string.erase( _string.length() - 1 );
            }
            return change;
        }



        int UnicodeToInt( const Uint16 &_key )
        {
            if( _key == (Uint16)'0' )
                return 0;

            else if( _key == (Uint16)'1' )
                return 1;

            else if( _key == (Uint16)'2' )
                return 2;

            else if( _key == (Uint16)'3' )
                return 3;

            else if( _key == (Uint16)'4' )
                return 4;

            else if( _key == (Uint16)'5' )
                return 5;

            else if( _key == (Uint16)'6' )
                return 6;

            else if( _key == (Uint16)'7' )
                return 7;

            else if( _key == (Uint16)'8' )
                return 8;

            else if( _key == (Uint16)'9' )
                return 9;

            else return 0;
        }

        bool wasKeyPressedUnicode( const SDL_Event &_event, const std::string &_character )
        {
            if( _event.type == SDL_KEYDOWN )
            {
                if( _event.key.keysym.unicode == (Uint16)_character[0] )
                {
                    return true;
                }
            }
            return false;
        }

        bool wasKeyPressed( const SDL_Event &_event, const int &_key )
        {
            if( _event.type == SDL_KEYDOWN )
            {
                if( _event.key.keysym.sym == _key )
                {
                    return true;
                }
            }
            return false;
        }

        bool isKeyDown( const SDL_Event &_event, const int &_key )
        {
            Uint8 *keystate = SDL_GetKeyState(NULL);
            if( keystate[_key] )
            {
                return true;
            }

            return false;
        }

        bool isMouseButtonDown( const SDL_Event &_event, const int &_key )
        {
            if( _event.type == SDL_MOUSEBUTTONDOWN )
            {
                if( _event.button.button == _key  )
                    return true;
            }
            return false;
        }

        bool isMouseButtonUp( const SDL_Event &_event, const int &_key )
        {
            if( _event.type == SDL_MOUSEBUTTONUP )
            {
                if( _event.button.button == _key  )
                    return true;
            }
            return false;
        }

        std::string getKeyPressed( const SDL_Event &_event )
        {
            std::string out;
            if( _event.type == SDL_KEYDOWN )
            {
                //If the key is a space
                if( _event.key.keysym.unicode == (Uint16)' ' )
                {
                    out += (char)_event.key.keysym.unicode;
                    return out;
                }
                //If the key is a number
                else if( ( _event.key.keysym.unicode >= (Uint16)'0' ) && ( _event.key.keysym.unicode <= (Uint16)'9' ) )
                {
                    out += (char)_event.key.keysym.unicode;
                    return out;
                }
                //If the key is a uppercase letter
                else if( ( _event.key.keysym.unicode >= (Uint16)'A' ) && ( _event.key.keysym.unicode <= (Uint16)'Z' ) )
                {
                    out += (char)_event.key.keysym.unicode;
                    return out;
                }
                //If the key is a lowercase letter
                else if( ( _event.key.keysym.unicode >= (Uint16)'a' ) && ( _event.key.keysym.unicode <= (Uint16)'z' ) )
                {
                    out += (char)_event.key.keysym.unicode;
                    return out;
                }
            }
            return out;
        }

		float getkey( const SDL_Event &_event )
		{
			if( _event.type == SDL_KEYDOWN )
            {
				return (float)_event.key.keysym.sym;
			}
			return 0;
		}
    }
}
