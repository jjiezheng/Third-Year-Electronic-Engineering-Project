#ifndef PARTICLEFACTORY_H
#define PARTICLEFACTORY_H

#include "CoreEngine.h"
#include "Coords.h"
#include "Particle.h"

#include <luabind/luabind.hpp>
#include <lua.hpp>

namespace Attrition
{
    class ParticleFactory
    {
        public:
            ParticleFactory();
            virtual ~ParticleFactory();

            void parseParticles();
            const Particle& get( const std::string &_particleName );
            const std::vector<std::string>& particleNames();


        private:
            std::map<std::string, Particle> Particles;
            std::map<std::string, Particle>::iterator particlesIter;
            typedef std::pair<std::string, Particle> particleType;

            std::vector<std::string> ParticleNames;

            bool parsingDone;
    };

    extern ParticleFactory particleManager;
}

#endif // PARTICLEFACTORY_H
