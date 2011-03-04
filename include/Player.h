#ifndef PLAYER_H
#define PLAYER_H

#include "Bullet.h"
#include "Particle.h"
#include "Playerweapons.h"

#include "Coords.h"
#include "CoreEngine.h"
#include "Entities.h"









struct Stats
{
    int credits, lives, score;
};

namespace Attrition
{
    //! A struct used by Player and Some Menus. Contains controls for Game.
    struct Controls
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
    };

    //! A struct used by Player and Some Menus. Contains controls for Game.
    struct UnicodeControls
    {
        //! If the player is using a joystick or pad.
        bool usingJoystick;
        //! The number SDL has given the joystick or pad.
        int joystickNumber;

        //! The key for moving up.
        Uint16 up;
        //! The key for moving down.
        Uint16 down;
        //! The key for moving left.
        Uint16 left;
        //! The key for moving right.
        Uint16 right;
        //! The key for fire.
        Uint16 fire;
        //! The key for fire_2.
        Uint16 fire_2;
        //! The key for special.
        Uint16 special;
        //! A spare.
        Uint16 unused;

        //! Number of the button for fire.
        Uint8 joyFire;
        //! Number of the button for fire_2.
        Uint8 joyFire_2;
        //! Number of the button for special.
        Uint8 joySpecial;
    };

    class Player
    {
        public:
            Player( const std::string &_playerType );
            ~Player();

            bool getInvincible();
            int getId();
            void isHit( bool );
            void respawn( Uint32 deltaTicks );

            const de::classes::Vector& getPosition();
            de::classes::Poly& getHitbox();
            Stats* getStatsPointer();


            void checkCollisions( boost::ptr_vector<Bullet> &, boost::ptr_vector<Particle> & );
            void checkCollisions( boost::ptr_vector<Attrition::Ship> &, boost::ptr_vector<Particle> & );


            void handleEvent( const SDL_Event &event );
            void logic( Uint32 deltaTicks, boost::ptr_vector<Attrition::Bullet> &_Bullets, boost::ptr_vector<Particle> &_Particles );
            void reLoadTextures();
            void render( bool );

        private:
            bool doRespawn, doFlicker, isInvincible;
            de::time::Counter respawnTime, flickerTime, flickerRate;
            std::string playerType;


            Controls controls;
            UnicodeControls unicodeControls;
            Stats stats;

            Ship playerShip;
            Playerweapon weapon;
            lua_State *luaState;


            void setShipAttributes();
            void handleJoystick( const SDL_Event &event );
            void handleKeyboard( const SDL_Event &event );
            void flicker( Uint32 deltaTicks );
    };
}


#endif //PLAYER_H
