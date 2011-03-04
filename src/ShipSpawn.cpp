#include "Shipspawn.h"

using namespace de::classes;
using namespace de::core;
using namespace de::theme;
using namespace de::misc;
using namespace de::enums;

using namespace Attrition;


ShipSpawn::ShipSpawn() :
    localTime(0),
    lastTime(0),
    timeStep(0)
{
    //ctor
}

ShipSpawn::~ShipSpawn()
{
    //dtor
}


void ShipSpawn::set( int *_time )
{
    currentTime = _time;
}

void ShipSpawn::handleEvent( const SDL_Event &_event )
{

}

void ShipSpawn::logic( bool _wasThereChange,
                       const Uint32 &_deltaTicks,
                       std::vector<LevelInfo>::iterator &_iterLevel,
                       boost::ptr_vector<Attrition::Ship> &_Ships,
                       boost::ptr_vector<Bullet> &_Bullets )
{
    if( localTime != (*currentTime) || _wasThereChange )
    {
        lastTime = localTime;
        localTime = (*currentTime);
        timeStep = localTime - lastTime;

        _Ships.clear();
        _Bullets.clear();
        runShipWaves( _iterLevel, _Ships, _Bullets);
    }
}

void ShipSpawn::logic( bool _wasThereChange,
                       const Uint32 &_deltaTicks,
                       std::vector<WaveInfo>::iterator &_iterWave,
                       boost::ptr_vector<Attrition::Ship> &_Ships,
                       boost::ptr_vector<Bullet> &_Bullets )
{
    if( localTime != (*currentTime) || _wasThereChange )
    {
        lastTime = localTime;
        localTime = (*currentTime);
        timeStep = localTime - lastTime;

        _Ships.clear();
        _Bullets.clear();

        runShipWave( _iterWave, _Ships, _Bullets );
    }
}

void ShipSpawn::reLoadTextures()
{

}

void ShipSpawn::render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 )
{

}





void ShipSpawn::runShipWaves( std::vector<LevelInfo>::iterator &_iterLevel,
                              boost::ptr_vector<Attrition::Ship> &_Ships,
                              boost::ptr_vector<Bullet> &_Bullets )
{
    std::vector<WaveInfo>::iterator iterWave;
    for( iterWave = _iterLevel->Waves.begin(); iterWave < _iterLevel->Waves.end(); ++iterWave )
    {
        runShipWave( iterWave, _Ships, _Bullets );
    }
}


void ShipSpawn::runShipWave( std::vector<WaveInfo>::iterator &_iterWave,
                             boost::ptr_vector<Attrition::Ship> &_Ships,
                             boost::ptr_vector<Bullet> &_Bullets )
{
    if( localTime > _iterWave->time )
    {
        int waveDuration = _iterWave->Points.size()*1000 - 3000;
        int shipCurrentTime = localTime - _iterWave->time;

        if( waveDuration > shipCurrentTime )
        {
            generateWave( shipCurrentTime, _iterWave, _Ships, _Bullets );
        }
    }
}


void ShipSpawn::generateWave( int _time,
                              std::vector<WaveInfo>::iterator &_iterWave,
                              boost::ptr_vector<Attrition::Ship> &_Ships,
                              boost::ptr_vector<Bullet> &_Bullets )
{
    if( _iterWave->number == 0)
        return;


    float distanceToEdge = ( _iterWave->gap * ( _iterWave->number - 1 ) ) / 2;

    std::vector<Vector>::iterator iterPoints = _iterWave->Points.begin();
    boost::ptr_vector<Attrition::Ship>::iterator iterShips;


    float i;
    for( i = 0; i < _iterWave->number; ++i )
    {
        float gap(i*_iterWave->gap);
        Vector offset;

        //_Ships.push_back( new Attrition::Ship( iterWave ) );
        _Ships.push_back( new Attrition::Ship( Attrition::shipManager.get( _iterWave->shipType ) ) );
        iterShips = _Ships.end() - 1;

        iterShips->loadSettings( _iterWave );

        if( _iterWave->formation == ARROW )
        {
            if( i <  _iterWave->number / 2  )
            {
                offset = Vector( gap - distanceToEdge, gap - distanceToEdge);
            }
            else
            {
                offset = Vector( gap - distanceToEdge, distanceToEdge - gap );
            }
        }
        else if( _iterWave->formation == VERTICAL_LINE )
        {
            offset = Vector( 0, gap - distanceToEdge );
        }
        else if( _iterWave->formation == HORIZONAL_LINE )
        {
            offset = Vector( gap -  distanceToEdge, 0 );
        }

        iterShips->setOffset( offset );
        iterShips->jumpToTime( _time, _Bullets );
    }
}
