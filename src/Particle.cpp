#include "Particle.h"

#include <algorithm>



using namespace de::classes;
using namespace de::core;
using namespace de::enums;
using namespace de::filesystem;
using namespace de::misc;

namespace fs = boost::filesystem;

namespace Attrition
{

    Particle Particle::operator= ( const Particle &_other )
    {
        alive = _other.alive;
        velocity = _other.velocity;
        lifeTime = _other.lifeTime;
        particleType = _other.particleType;
        sprites = _other.sprites;

        return *this;
    }

    Particle::Particle() : alive(false)
    {
    }

    Particle::Particle( const Particle &_other, const Vector &_position, const Vector &_velocity )
    {
        lifeTime = _other.lifeTime;
        particleType = _other.particleType;
        sprites = _other.sprites;

        alive = true;
        velocity = _velocity;
        sprites.moveBy( _position, true );
    }

    Particle::Particle( const std::string &_shipType )
    {
        particleType = _shipType;
        loadSettings();
    }

    Particle::Particle( const std::string &_shipType, const Vector &_position, const Vector &_velocity )
    {
        particleType = _shipType;
        loadSettings();

        setPosition( _position, _velocity );
    }

    Particle::~Particle()
    {

    }

    void Particle::loadSettings()
    {
        luaState = lua_open();
        luaL_openlibs(luaState);
        luabind::open(luaState);


        fs::path dir( Roots->get( root::PARTICLES ) + particleType + ".lua" );

        if( !fs::exists( dir ) )
        {
            assert(!"particles type doesn't exist");
        }

        try
        {
            luaL_dofile( luaState, dir.file_string().c_str() );

            luabind::object MainTable = luabind::globals(luaState)[particleType];
            luabind::object SpriteTable = MainTable["Sprites"];

            if( luabind::type(MainTable) != LUA_TTABLE ||
                luabind::type(SpriteTable) != LUA_TTABLE )
            {
                de::io::error << "In particle config file. Problem with tables!\n";
            }
            int time = luabind::object_cast<int>( MainTable["time"] );
            lifeTime.setUp( 1, time );

            sprites.parseSprites( SpriteTable );
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

        lua_close(luaState);
    }


    void Particle::setPosition( const Vector &_position, const Vector &_velocity )
    {
        velocity = _velocity;
        sprites.moveBy( _position, true );
    }

    void Particle::logic( const Uint32& _deltaTicks )
    {
        if( lifeTime.count( _deltaTicks ) )
        {
            alive = false;
        }
        else
        {
            static Vector displacement;
            displacement = velocity*( _deltaTicks / 1000.f );



            sprites.logic( _deltaTicks );
            sprites.moveBy( displacement );
        }
    }

    void Particle::reLoad()
    {
        if( alive )
        {
            sprites.reload();
        }
    }


    void Particle::render( de::enums::fboTypes _layer )
    {
        if( alive )
        {
            sprites.render( _layer );
        }
    }


    bool Particle::isAlive()
    {
        return alive;
    }

}
