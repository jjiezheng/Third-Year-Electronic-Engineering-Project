#ifndef PARTICLE_H
#define PARTICLE_H

#include "CoreEngine.h"
#include "Coords.h"

#include <luabind/luabind.hpp>
#include <lua.hpp>

namespace Attrition
{
    class Particle
    {
        public:
            Particle();
            ~Particle();


            Particle( const std::string &_shipType );
            Particle( const std::string &_shipType, const de::classes::Vector &_position, const de::classes::Vector &_velocity = de::classes::Vector(0,0) );
            Particle( const Particle &_other, const de::classes::Vector &_position, const de::classes::Vector &_velocity = de::classes::Vector(0,0) );

            void setPosition( const de::classes::Vector &_position, const de::classes::Vector &_velocity = de::classes::Vector(0,0) );
            void logic( const Uint32& deltaTicks );
            void render( de::enums::fboTypes _layer );
            void reLoad();

            bool isAlive();

            Particle operator= ( const Particle &_other );

        private:
            void loadSettings();

        private:
            bool alive;

            de::classes::Vector velocity;
            de::time::Counter lifeTime;

            std::string particleType;
            de::graphics::SpriteGroup sprites;

            lua_State *luaState;
    };
}


#endif //PARTICLE_H
