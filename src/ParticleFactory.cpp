#include "ParticleFactory.h"

using namespace de::filesystem;

namespace fs = boost::filesystem;


namespace Attrition
{
    ParticleFactory particleManager;



    ParticleFactory::ParticleFactory(): parsingDone(false)
    {
        //ctor
    }

    ParticleFactory::~ParticleFactory()
    {
        //dtor
    }


    const Particle& ParticleFactory::get( const std::string &_particleName )
    {
        particlesIter = Particles.find( _particleName );

        if( particlesIter != Particles.end() )
        {
            return particlesIter->second;
        }
        else
        {
            de::io::error << "The particle " << _particleName << "deosn't exist\n";
        }
        static Particle blank;

        return blank;
    }

    const std::vector<std::string>& ParticleFactory::particleNames()
    {
        return ParticleNames;
    }


    void ParticleFactory::parseParticles()
    {
        if( !parsingDone )
        {
            std::vector<fs::path> foundFiles;
            std::vector<fs::path>::iterator iter, end;

            fs::path particleFilePath( Roots->get( root::PARTICLES ) );
            getFilesFrom( particleFilePath, foundFiles, filetypes::LUA );

            for( iter = foundFiles.begin(); iter < foundFiles.end(); ++iter )
            {
                std::string particleName( stripFileEnding( iter->filename() ) );

                Particles.insert(  particleType( particleName, Particle( particleName ) )  );
                ParticleNames.push_back( particleName );
            }

            parsingDone = true;
        }
    }
}
