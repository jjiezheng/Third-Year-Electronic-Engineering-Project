#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include "ControlComponent.h"


namespace Attrition
{
    class PlayerControl : public ControlComponent
    {
        public:
            /** Default constructor */
            PlayerControl( int _playerNumber );
            /** Default destructor */
            virtual ~PlayerControl();

            void handleEvents( ComponentShip &_ship, const SDL_Event &_event );
            void logic( ComponentShip &_ship, const Uint32 &_delta );
        protected:
        private:
            lua_State *luaState;

            struct
            {
                //! If the player is using a joystick or pad.
                bool usingJoystick;
                //! The number SDL has given the joystick or pad.
                int joystickNumber;

                //! The key for moving up.
                SDLKey up;
                //! The key for moving down.
                SDLKey down;
                //! The key for moving left.
                SDLKey left;
                //! The key for moving right.
                SDLKey right;
                //! The key for fire.
                SDLKey fire;
                //! The key for fire_2.
                SDLKey fire_2;
                //! The key for special.
                SDLKey special;
                //! A spare.
                SDLKey unused;

                //! Number of the button for fire.
                Uint8 joyFire;
                //! Number of the button for fire_2.
                Uint8 joyFire_2;
                //! Number of the button for special.
                Uint8 joySpecial;
            } controls;
    };
}

#endif // PLAYERCONTROL_H
