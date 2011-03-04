#ifndef SHIPSPAWN_H
#define SHIPSPAWN_H

#include "CoreEngine.h"
#include "Coords.h"
#include "Entities.h"


class ShipSpawn
{
    public:
        ShipSpawn();
        virtual ~ShipSpawn();

        void set( int *_time );
        void handleEvent( const SDL_Event &_event );
        void logic( bool _wasThereChange,
                    const Uint32 &_deltaTicks,
                    std::vector<de::classes::LevelInfo>::iterator &_iterLevel,
                    boost::ptr_vector<Attrition::Ship> &_Ships,
                    boost::ptr_vector<Attrition::Bullet> &_Bullets );

        void logic( bool _wasThereChange,
                    const Uint32 &_deltaTicks,
                    std::vector<de::classes::WaveInfo>::iterator &_iterWave,
                    boost::ptr_vector<Attrition::Ship> &_Ships,
                    boost::ptr_vector<Attrition::Bullet> &_Bullets );

        void reLoadTextures();
        void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 );
    protected:
    private:
        void runShipWaves( std::vector<de::classes::LevelInfo>::iterator &_iterLevel,
                           boost::ptr_vector<Attrition::Ship> &_Ships,
                           boost::ptr_vector<Attrition::Bullet> &_Bullets );

        void runShipWave( std::vector<de::classes::WaveInfo>::iterator &_iterWave,
                                     boost::ptr_vector<Attrition::Ship> &_Ships,
                                     boost::ptr_vector<Attrition::Bullet> &_Bullets );

        void generateWave( int _time,
                           std::vector<de::classes::WaveInfo>::iterator &_iterWave,
                           boost::ptr_vector<Attrition::Ship> &_Ships,
                           boost::ptr_vector<Attrition::Bullet> &_Bullets );

        int *currentTime;
        int localTime, lastTime, timeStep;
};

#endif // SHIPSPAWN_H
