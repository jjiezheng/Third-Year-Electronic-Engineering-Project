#include "Player.h"
#include "Ships.h"
#include <luabind/luabind.hpp>

using namespace de::classes;
using namespace de::core;
using namespace de::filesystem;
using namespace de::enums;

namespace fs = boost::filesystem;

namespace Attrition
{
    Player::Player( const std::string &_playerType )
        :playerType( _playerType )
    {

        luaState = lua_open();
        luaL_openlibs(luaState);
        luabind::open(luaState);
        fs::path dir( Roots->get( root::SETTINGS ) + "Player.lua" );
        luaL_dofile( luaState, dir.file_string().c_str() );

        setShipAttributes();
    }

    Player::~Player()
    {
        lua_close(luaState);
    }


    void Player::setShipAttributes()
    {
        try
        {
            luabind::object PlayersTable = luabind::globals(luaState)["Players"];
            luabind::object currentPlayer = PlayersTable[playerType];

            if( luabind::type(PlayersTable) != LUA_TTABLE ||
                luabind::type(currentPlayer) != LUA_TTABLE )
            {
                de::io::error << "In Graphics config file. Problem with tables!\n";
            }

            playerType = luabind::object_cast<std::string>( currentPlayer["shipType"] );
            playerShip = shipManager.get( playerType );

            flickerRate.countDelayIs( luabind::object_cast<int>( currentPlayer["respawnFlicker"] ) );
            flickerTime.countDelayIs( luabind::object_cast<int>( currentPlayer["flickerTime"] ) );
            respawnTime.countDelayIs( luabind::object_cast<int>( currentPlayer["respawnTime"] ) );

            controls.up      = luabind::object_cast<SDLKey>( currentPlayer["up"] );
            controls.down    = luabind::object_cast<SDLKey>( currentPlayer["down"] );
            controls.left    = luabind::object_cast<SDLKey>( currentPlayer["left"] );
            controls.right   = luabind::object_cast<SDLKey>( currentPlayer["right"] );
            controls.fire    = luabind::object_cast<SDLKey>( currentPlayer["fire1"] );
            controls.fire_2  = luabind::object_cast<SDLKey>( currentPlayer["fire2"] );
            controls.special = luabind::object_cast<SDLKey>( currentPlayer["special"] );
        }
        catch( luabind::error &_e )
        {
            std::string error = lua_tostring( _e.state(), -1 );
            de::io::error << _e.what() << "\n";
            de::io::error << error << "\n";
        }
        catch( luabind::cast_failed  &_e )
        {
            std::string error1 = lua_tostring( _e.state(), -1 );
            std::string error2 = lua_tostring( _e.state(), 0 );
            de::io::error << _e.what() << " - " << error1 << "|" << error2 << "\n";
        }

        playerShip.setPosition( Vector ( 275, 600 ) );
        playerShip.playerControled( true );

        doRespawn = doFlicker = isInvincible = false;

        stats.credits = 1;
        stats.lives = 4;
        stats.score = 0;

        controls.usingJoystick = true;
    }



    void Player::checkCollisions( boost::ptr_vector<Bullet> &_Bullets, boost::ptr_vector<Particle> &_Particles )
    {/*
        if( !isInvincible )
        {
            if( controlingShip->checkCollisions( _Bullets, _Particles ) )
                isHit( true );
        }*/
    }

    void Player::checkCollisions( boost::ptr_vector<Attrition::Ship> &_Ships, boost::ptr_vector<Particle> &_Particles )
    {/*
        if( controlingShip->checkCollisions( _Ships, _Particles ) )
            isHit( true );*/
    }


    void Player::handleEvent( const SDL_Event &event )
    {
        if( !doRespawn )
        {
            handleKeyboard( event );
            handleJoystick( event );
        }
    }

    void Player::logic( Uint32 deltaTicks, boost::ptr_vector<Bullet> &_Bullets, boost::ptr_vector<Particle> &_Particles )
    {
        if( doRespawn )
        {
            if( respawnTime.count( deltaTicks ) )
            {
                Vector pos( 0, 0 );
                playerShip.setVelocity( pos );
                //controlingShip->bounded = true;
                doRespawn = false;
            }

        }

        if( doFlicker )
        {
            flicker( deltaTicks );
        }

        playerShip.logic( deltaTicks, _Bullets, _Particles );
        weapon.logic( playerShip.getPosition(), deltaTicks, _Bullets );

        if( stats.lives < 0 )
        {
            //continue();?
        }
    }

    void Player::reLoadTextures()
    {
        playerShip.reLoad();
    }

    void Player::render( bool _showHitbox )
    {
        playerShip.render( FBO_GAME, _showHitbox );
    }









    void Player::isHit( bool Hit )
    {
        if( Hit )
        {
            stats.lives--;
            doRespawn = true;
            doFlicker = true;
            isInvincible = true;
            //controlingShip->bounded = false;
            playerShip.setPosition( Vector( 275, 900 ) );

            Vector pos( 0, -1 );
            playerShip.setVelocity( pos );
        }

    }

    bool Player::getInvincible()
    {
        return isInvincible;
    }

    Poly& Player::getHitbox()
    {
        return playerShip.getHitbox();
    }

    Stats* Player::getStatsPointer()
    {
        return &stats;
    }

    const Vector& Player::getPosition()
    {
        return playerShip.getPosition();
    }

    int Player::getId()
    {
        return playerShip.getId();
    }









    inline void Player::flicker( Uint32 deltaTicks )
    {
    /*
        if( flickerRate.count( deltaTicks ) )
        {
            if( controlingShip->getPriority() > 0 )
            {
                //controlingShip->setPriority( -1 );
                return;
            }

            if( controlingShip->getPriority() < 0 )
            {
                //controlingShip->setPriority( 8 );
                return;
            }

        }

        if( flickerTime.count( deltaTicks ) )
        {
            doFlicker = false;
            isInvincible = false;
            //controlingShip->setPriority( 8 );

        }*/
    }

    inline void Player::handleKeyboard( const SDL_Event &event )
    {
        Vector pos;
        Uint8 *keystate = SDL_GetKeyState(NULL);


        if( keystate[controls.up] )
            pos.y = -1;

        if( keystate[controls.down] )
            pos.y = 1;

        if( keystate[controls.left] )
            pos.x = -1;

        if( keystate[controls.right] )
            pos.x = 1;

        if( keystate[controls.up] && keystate[controls.left] )
        {
            pos.y = -0.7;
            pos.x = -0.7;
        }

        if( keystate[controls.up] && keystate[controls.right] )
        {
            pos.y = -0.7;
            pos.x = 0.7;
        }

        if ( keystate[controls.down] && keystate[controls.left] )
        {
            pos.y = 0.7;
            pos.x = -0.7;
        }

        if ( keystate[controls.down] && keystate[controls.right] )
        {
            pos.y = 0.7;
            pos.x = 0.7;
        }


        if( !(keystate[controls.left] || keystate[controls.right]) )
            pos.x = 0;

        if( !( keystate[controls.up] || keystate[controls.down] ) )
            pos.y = 0;

        playerShip.setVelocity( pos );
        weapon.handleInput( keystate[controls.fire], keystate[controls.fire_2] );

    }

    inline void Player::handleJoystick( const SDL_Event &event )
    {
        Vector pos;
        bool fire1(false), fire2(false);

        //Joystick button event handling
        if ( controls.usingJoystick == true )
        {
            if ( event.jbutton.which == controls.joystickNumber )
            {
                if ( ( event.type == SDL_JOYBUTTONDOWN ) )
                {
                    if( event.jbutton.state == SDL_PRESSED && event.jbutton.button == controls.joyFire )
                    {
                        fire1 = true;
                    }

                    if( event.jbutton.state == SDL_PRESSED && event.jbutton.button == controls.joyFire_2 )
                    {
                        fire2 = true;
                    }
                }

                if ( ( event.type == SDL_JOYBUTTONUP ) )
                {
                    if( event.jbutton.state == SDL_RELEASED && event.jbutton.button == controls.joyFire )
                    {

                    }
                }
                //weapon.handleInput( fire1, fire2 );
            }


            //Joystick hi-hat event handling
            if ( ( event.type == SDL_JOYHATMOTION ) && ( event.jhat.which == controls.joystickNumber )  )
            {
                if ( event.jhat.value == SDL_HAT_UP )
                {
                    pos.x = 0;
                    pos.y = -1;
                }

                if ( event.jhat.value == SDL_HAT_DOWN )
                {
                    pos.x = 0;
                    pos.y = 1;
                }

                if ( event.jhat.value == SDL_HAT_LEFT )
                {
                    pos.x = -1;
                    pos.y = 0;
                }

                if ( event.jhat.value == SDL_HAT_RIGHT )
                {
                    pos.x = 1;
                    pos.y = 0;
                }

                if ( event.jhat.value == SDL_HAT_RIGHTUP )
                {
                    pos.x = 0.70;
                    pos.y = -0.70;
                }

                if ( event.jhat.value == SDL_HAT_RIGHTDOWN )
                {
                    pos.x = 0.70;
                    pos.y = 0.70;
                }

                if ( event.jhat.value == SDL_HAT_LEFTUP )
                {
                    pos.x = -0.70;
                    pos.y = -0.70;
                }

                if ( event.jhat.value == SDL_HAT_LEFTDOWN )
                {
                    pos.x = -0.70;
                    pos.y = 0.70;
                }

                if ( event.jhat.value == SDL_HAT_CENTERED )
                {
                    pos.x = 0;
                    pos.y = 0;
                }
                playerShip.setVelocity( pos );
            }

            //Joystick axis event handling
            if ( event.type == SDL_JOYAXISMOTION )
            {
                //If joystick 0 has moved
                if ( event.jaxis.which == controls.joystickNumber )
                {
                    //If the X axis changed
                    if ( event.jaxis.axis == 0 )
                    {
                        //If the X axis is neutral
                        if ( ( event.jaxis.value > -8000 ) && ( event.jaxis.value < 8000 ) )
                        {
                            pos.x = 0;
                        }
                        //If not
                        else
                        {
                            //Adjust the velocity
                            if ( event.jaxis.value < 0 )
                            {
                                pos.x = -1;
                            }
                            else
                            {
                                pos.x = 1;
                            }
                        }
                    }
                    //If the Y axis changed
                    else if ( event.jaxis.axis == 1 )
                    {
                        //If the Y axis is neutral
                        if ( ( event.jaxis.value > -8000 ) && ( event.jaxis.value < 8000 ) )
                        {
                            pos.y = 0;
                        }
                        //If not
                        else
                        {
                            //Adjust the velocity
                            if ( event.jaxis.value < 0 )
                            {
                                pos.y = -1;
                            }
                            else
                            {
                                pos.y = 1;
                            }
                        }
                    }
                }

                playerShip.setVelocity( pos );
            }
        }
    }

}
