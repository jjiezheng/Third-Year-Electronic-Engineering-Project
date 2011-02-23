#include "Bulletgenerator.h"
#include "BulletFactory.h"
#include <iostream>

using namespace de::classes;
using namespace de::misc;
using namespace de::enums;

using namespace Attrition;



namespace
{
    Vector getShipPositionFor( int _shipMovementTime, const Vector &_offSet, std::vector<Vector> &_Points)
    {
        std::vector<de::classes::Vector>::iterator iterPoints = _Points.begin();
        Vector pos;
        float t(0);

        while(_shipMovementTime > 1000)
        {
            _shipMovementTime -= 1000;
            ++iterPoints;
        }
        t = (float)_shipMovementTime/1000.f;

        pointOnCurve( pos, t, (*iterPoints), (*(iterPoints + 1)), (*(iterPoints + 2)), (*(iterPoints + 3)) );
        return pos + _offSet;
    }
}




void Shape::getCircle( const int& amount,
                       const Vector& position,
                       const Vector& direction,
                       const float& arcSize,
                       const float& rotation,
                       std::vector<Vector> &Velocities,
                       std::vector<Vector> &Positions )
{
    float a(0);
    for( float i = -amount/2.0f; i < amount/2.0f; ++i )
    {
        a = ( arcSize * pi() * (i / (float) amount) ) + rotation + direction.y*pi();
        Velocities.push_back( Vector( cos(a)*direction.x, sin(a)*direction.x ) );
        Positions.push_back( position );
    }
}

void Shape::getLine( const int &amount,
                     const Vector &position,
                     const Vector &direction,
                     const Vector &gap,
                     std::vector<Vector> &Velocities,
                     std::vector<Vector> &Positions )
{
    if( amount%2 == 1 )
    {
        int j(1);
        for( int i = 0; i < amount; ++i )
        {
            if( i%2 == 0 )
            {
                Velocities.push_back( direction );
                Positions.push_back( position + gap*(j-1) );
            }
            else if( i%2 == 1 )
            {
                Velocities.push_back( direction );
                Positions.push_back( position - gap*j );
                ++j;
            }
        }
    }
    else
    {
        int j(1);
        for( int i = 0; i < amount; ++i )
        {
            if( i%2 == 0 )
            {
                Velocities.push_back( direction );
                Positions.push_back( position + gap*j - gap/2 );
            }
            else if( i%2 == 1 )
            {
                Velocities.push_back( direction );
                Positions.push_back( position - gap*j + gap/2 );
                ++j;
            }
        }
    }
}





BulletEmitterFactory::BulletEmitterFactory( const BulletEmitterFactory &_other )
{
    totalTimePassed = _other.totalTimePassed;
    label = _other.label;


    BulletInfo = _other.BulletInfo;
    FiringTimes = _other.FiringTimes;
    Emitters = _other.Emitters;
}

BulletEmitterFactory::BulletEmitterFactory( std::vector<BulletWaveInfo> &_bulletInfo,
                                            std::vector<BulletTimes> &_firingTimes,
                                            const Collision &_label )
{
    BulletInfo = _bulletInfo;
    FiringTimes = _firingTimes;
    label = _label;
    totalTimePassed = 0;

    std::vector<BulletTimes>::iterator iterTime;
    for( iterTime = FiringTimes.begin(); iterTime < FiringTimes.end(); ++iterTime )
    {
        Emitters.push_back( BulletEmitter( iterTime->start, iterTime->duration, BulletInfo[iterTime->weaponUsed], label ) );
    }
}

void BulletEmitterFactory::logic( const Uint32 &deltaTicks,
                                  const Vector &_position,
                                  boost::ptr_vector<Bullet> &_Bullets )
{
    totalTimePassed += deltaTicks;

    std::vector<BulletEmitter>::iterator iterEmitter;
    for( iterEmitter = Emitters.begin(); iterEmitter < Emitters.end(); ++iterEmitter )
    {
        iterEmitter->logic( _position, deltaTicks, _Bullets );
    }
}


void BulletEmitterFactory::jumpToTime( const int &_currentTime,
                                       const Vector &_offSet,
                                       std::vector<Vector> &_Points,
                                       boost::ptr_vector<Bullet> &_Bullets )
{
    totalTimePassed = _currentTime;

    std::vector<BulletEmitter>::iterator iterEmitter;
    for( iterEmitter = Emitters.begin(); iterEmitter < Emitters.end(); ++iterEmitter )
    {
        iterEmitter->jumpToTime( _currentTime, _offSet, _Points, _Bullets );
    }
}




BulletEmitter::BulletEmitter( int &_timeTillStart,
                              int &_duration,
                              const BulletWaveInfo &_bulletInfo,
                              const de::enums::Collision &_label )
{
    timeTillStart = _timeTillStart;
    duration = _duration;

    bulletInfo = _bulletInfo;
    label = _label;

    if( bulletInfo.bulletReloadTime < 10 )
    {
        reload.countDelayIs( 10 );
    }
    else
    {
        reload.countDelayIs( bulletInfo.bulletReloadTime );
    }
    active = true;
}

void BulletEmitter::logic( const Vector &_position,
                           const Uint32 &deltaTicks,
                           boost::ptr_vector<Bullet> &_Bullets )
{
    if( timeTillStart > 0 )
    {
        timeTillStart -= deltaTicks;
    }
    else
    {
        duration -= deltaTicks;
        if( duration > 0 && active )
        {
            if( reload.count( deltaTicks ) )
            {
                fire( _position, _Bullets, 0 );
            }
        }
        else active = false;
    }
}

void BulletEmitter::jumpToTime( const int &_currentTime,
                                const Vector &_offSet,
                                std::vector<Vector> &_Points,
                                boost::ptr_vector<Bullet> &_Bullets )
{
    int currentTime(_currentTime), totalTime(_currentTime);


    if( timeTillStart > 0 )
    {
        currentTime -= timeTillStart;

        if( currentTime > 0 )
        {
            timeTillStart = 0;
        }
    }

    if( timeTillStart == 0 && duration > 0 )
    {
        int durationThisFrame(0);

        if( currentTime > duration )
        {
            durationThisFrame = duration;
            duration = 0;
            active = false;
        }
        else
        {
            durationThisFrame = currentTime;
            duration -= currentTime;
        }

        int _timeOffset(durationThisFrame);

        for(;durationThisFrame > 0; durationThisFrame = reload.countDown( durationThisFrame ) )
        {
            int timeForBulletMovement(currentTime-(_timeOffset - durationThisFrame));
            int shipMovementTime(totalTime - timeForBulletMovement);

            Vector position = getShipPositionFor( shipMovementTime, _offSet, _Points);
            fire( position, _Bullets, timeForBulletMovement );
        }
    }
}

bool BulletEmitter::finished()
{
    return !active;
}

void BulletEmitter::fire( const Vector &_position, boost::ptr_vector<Bullet> &_Bullets, int _timeOffset )
{
    Velocities.clear();
    Positions.clear();

    position = _position;

    if( bulletInfo.bulletType == SIMPLE_TARGETING ||
        bulletInfo.bulletType == TARGETING ||
        bulletInfo.bulletType == TRACKING )
    {
        getTargets();
    }



    //Rotation if it takes place
    if( bulletInfo.firingPattern == CIRCLE || bulletInfo.firingPattern == CIRCLES2 )
        rotate();


    getPositions();
    createBullets( _Bullets, _timeOffset );
}

void BulletEmitter::getTargets()
{
    if( label == ENEMY )
    {
        Ships = ShipTracker::playerShips;
    }
    else if( label == FRIENDLY )
        Ships = ShipTracker::Ships;


    distanceSort( Ships, position );
    iter = Ships.begin();
}

//Not writen yet
void BulletEmitter::distanceSort( std::vector<shipPos> &_Ships, const Vector &_position )
{
    if( !_Ships.empty() )
    {


    }
}

inline void BulletEmitter::rotate()
{
    rotation += bulletInfo.bulletRotation;

    if( rotation > pi()* 2 )
        rotation = 0;

    otherRotation = pi()*2 - rotation;
}


inline void BulletEmitter::circularIncrement( std::vector<shipPos> &_Ships, std::vector<shipPos>::iterator &_iter )
{
    if( !_Ships.empty() )
    {
        if( _iter < _Ships.end() )
            ++_iter;
        if( _iter >= _Ships.end() )
            _iter = _Ships.begin();
    }
}


void BulletEmitter::getPositions()
{
    if( bulletInfo.firingPattern == LINE )
    {
        Shape::getLine( bulletInfo.bulletNumber, position, bulletInfo.bulletSpeed, bulletInfo.bulletGap, Velocities, Positions );
    }
    else if( bulletInfo.firingPattern == CIRCLE )
    {
        Shape::getCircle( bulletInfo.bulletNumber, position, bulletInfo.bulletSpeed, bulletInfo.arcSize, rotation, Velocities, Positions );
    }
    else if( bulletInfo.firingPattern == CIRCLES2 )
    {
        Shape::getCircle( bulletInfo.bulletNumber, position, bulletInfo.bulletSpeed, bulletInfo.arcSize, rotation, Velocities, Positions );
        Shape::getCircle( bulletInfo.bulletNumber, position, bulletInfo.bulletSpeed, bulletInfo.arcSize, otherRotation, Velocities, Positions );
    }
    else
    {
        Shape::getLine( bulletInfo.bulletNumber, position, bulletInfo.bulletSpeed, bulletInfo.bulletGap, Velocities, Positions );
    }

/*
    if( bulletInfo.bulletType == SIMPLE_TARGETING ||
        bulletInfo.bulletType == TARGETING ||
        bulletInfo.bulletType == TRACKING )
    {
        for( unsigned int i(0); i < Velocities.size(); ++i )
        {
            if( !Ships.empty() )
            {
                directionAiming = ( iter->pos - Positions[i] ).normalise();


                Vector dir = directionAiming;
                Vector speed = bulletInfo.bulletSpeed;

                float scalar = sqrt( dir.x*dir.x + dir.y*dir.y );

                Vector ans = bulletInfo.bulletSpeed;
                ans.x *= directionAiming.x * scalar;
                ans.y *= directionAiming.y * scalar;

                Velocities[i] = ans;
                circularIncrement( Ships, iter );
            }
            else
            {
                Velocities[i] = directionAiming;
            }
        }
    }*/
}

void BulletEmitter::createBullets( boost::ptr_vector<Bullet> &_Bullets, const int &_timeOffset )
{
    boost::ptr_vector<Bullet>::iterator iterBullets;
    for( unsigned int i = 0; i < Positions.size(); ++i )
    {
        _Bullets.push_back( new Bullet( bulletManager.get( bulletInfo.bulletName ), label, Positions[i], Velocities[i] ) );
        /*
        if( bulletInfo.bulletType == SIMPLE )
        {
            _Bullets.push_back( new Bullet( bulletManager.get( bulletInfo.bulletName ), label, Positions[i], Velocities[i] ) );
        }
        else if( bulletInfo.bulletType == TARGETING || bulletInfo.bulletType == SIMPLE_TARGETING )
        {
            _Bullets.push_back( new Bullet( bulletManager.get( bulletInfo.bulletName ), label, Positions[i], Velocities[i], iter->id ) );
        }
        else if( bulletInfo.bulletType == TRACKING )
        {
            _Bullets.push_back( new Bullet( bulletManager.get( bulletInfo.bulletName ), label, Positions[i], Velocities[i], iter->id ) );
        }
        else
        {
            _Bullets.push_back( new Bullet( bulletManager.get( bulletInfo.bulletName ), label, Positions[i], Velocities[i] ) );
        }*/

        iterBullets = _Bullets.end() - 1;
        iterBullets->jumpToTime( _timeOffset );
    }
}

