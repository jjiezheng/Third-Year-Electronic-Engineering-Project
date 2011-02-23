#include "PlayerControl.h"
#include "ComponentShip.h"

namespace fs = boost::filesystem;
using namespace de::filesystem;

namespace Attrition
{

    PlayerControl::PlayerControl( int _playerNumber )
    {
        luaState = lua_open();
        luaL_openlibs(luaState);
        luabind::open(luaState);
        fs::path dir( Roots->get( root::SETTINGS ) + "Player.lua" );
        luaL_dofile( luaState, dir.file_string().c_str() );


        try
        {
            luabind::object PlayersTable = luabind::globals(luaState)["Players"];
            luabind::object currentPlayer = PlayersTable["Player"+de::misc::toString(_playerNumber)];

            if( luabind::type(PlayersTable) != LUA_TTABLE ||
                luabind::type(currentPlayer) != LUA_TTABLE )
            {
                de::io::error << "In Players config file. Problem with tables!\n";
            }

            controls.up      = luabind::object_cast<SDLKey>( currentPlayer["up"] );
            controls.down    = luabind::object_cast<SDLKey>( currentPlayer["down"] );
            controls.left    = luabind::object_cast<SDLKey>( currentPlayer["left"] );
            controls.right   = luabind::object_cast<SDLKey>( currentPlayer["right"] );
            controls.fire    = luabind::object_cast<SDLKey>( currentPlayer["fire1"] );
            controls.fire_2  = luabind::object_cast<SDLKey>( currentPlayer["fire2"] );
            controls.special = luabind::object_cast<SDLKey>( currentPlayer["special"] );

            de::io::tests << "Got Players settings.\n";
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
    }

    PlayerControl::~PlayerControl()
    {
        lua_close(luaState);
    }


    void PlayerControl::handleEvents( ComponentShip &_ship, const SDL_Event &_event )
    {
        _ship.velocity = glm::vec3(0.0f);

        Uint8 *keystate = SDL_GetKeyState(NULL);


        if( keystate[controls.up] )
        {
            _ship.velocity.y = 1;
        }

        if( keystate[controls.down] )
        {
            _ship.velocity.y = -1;
        }

        if( keystate[controls.left] )
        {
            _ship.velocity.x = -1;
        }

        if( keystate[controls.right] )
        {
            _ship.velocity.x = 1;
        }

        if( keystate[controls.up] && keystate[controls.left] )
        {
            _ship.velocity.y = 0.7;
            _ship.velocity.x = -0.7;
        }

        if( keystate[controls.up] && keystate[controls.right] )
        {
            _ship.velocity.y = 0.7;
            _ship.velocity.x = 0.7;
        }

        if ( keystate[controls.down] && keystate[controls.left] )
        {
            _ship.velocity.y = -0.7;
            _ship.velocity.x = -0.7;
        }

        if ( keystate[controls.down] && keystate[controls.right] )
        {
            _ship.velocity.y = -0.7;
            _ship.velocity.x = 0.7;
        }


        if( !(keystate[controls.left] || keystate[controls.right]) )
        {
            _ship.velocity.x = 0;
        }

        if( !( keystate[controls.up] || keystate[controls.down] ) )
        {
            _ship.velocity.y = 0;
        }

        //weapon.handleInput( keystate[controls.fire], keystate[controls.fire_2] );
    }

    void PlayerControl::logic( ComponentShip &_ship, const Uint32 &_delta )
    {
        _ship.position += _ship.velocity*0.5f*(float)_delta;


        /////////////////////////////////////////////////////////////////////////////////
        //
        //Temp Code Delete when collision detection is done
        //
        /////////////////////////////////////////////////////////////////////////////////
        if( _ship.position.x > 200 )
        {
            _ship.position.x = 199;
        }
        else if( _ship.position.x < -200 )
        {
            _ship.position.x = -199;
        }
        if( _ship.position.y > 400 )
        {
            _ship.position.y = 399;
        }
        else if( _ship.position.y < -400 )
        {
            _ship.position.y = -399;
        }
    }
}
