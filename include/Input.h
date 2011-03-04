#ifndef INPUT_H
#define INPUT_H

#include "Base.h"

#include "Vector.h"
#include "CoreEnumsAndClasses.h"

namespace de
{
    namespace events
    {
        //! Get the position of the mouse
        classes::Vector getMousePosition( const SDL_Event &_event, bool _motion, enums::fboTypes _fbo  );
        classes::Vector getMousePosition( const SDL_Event &_event, bool _motion );
        classes::Vector getRelativeMouseMovement( const SDL_Event &_event );
        glm::vec3 getRelativeMouseMovementVec3( const SDL_Event &_event );
        glm::vec3 MousePosition( const SDL_Event &_event );

        float relativeMouseAngle( const SDL_Event &_event, int _axis );
        void resetMouse();


        classes::Vector getMousePositionV( const int &_x, const int &_y, enums::fboTypes _fbo = enums::FBO_UI );
        classes::Vector getMousePositionH( const int &_x, const int &_y, enums::fboTypes _fbo = enums::FBO_UI );


        void showMouse( bool _show );
        void toggleMouse();
        void toggleMouseGrab();



        void pushEvent( enums::events::SDL_EVENT_CODES _code );

        bool handleBoolInput( const SDL_Event &_event, bool &_bool );
        bool handleIntInput( const SDL_Event &_event, int &_int );
        bool handleFloatInput( const SDL_Event &_event, float &_float, bool &_decimalPoint, int &_decimalPosition );
        bool handleStringInput( const SDL_Event &_event, std::string &_string );

        //! Returns a number if "_key" stands for one Returns 0 other wise
        int UnicodeToInt( const Uint16 &_key );

        Uint16 toUnicode( const std::string &_character );
        bool wasKeyPressedUnicode( const SDL_Event &_event, const std::string &_character );
        bool wasKeyPressed( const SDL_Event &_event, const int &_key );

        bool isEvent( const SDL_Event &_event, const int &_key );
        bool isKeyDown( const SDL_Event &_event, const int &_key );
        bool isMouseButtonDown( const SDL_Event &_event, const int &_key );
        bool isMouseButtonUp( const SDL_Event &_event, const int &_key );

        std::string getKeyPressed( const SDL_Event &_event );


        class dummyClassSDLKey
        {
            public:
                dummyClassSDLKey(){}
                ~dummyClassSDLKey(){}
        };
    }
}
#endif // INPUT_H
