#ifndef BULLETGENERATOR_H
#define BULLETGENERATOR_H

#include "CoreEngine.h"
#include "Coords.h"
#include "CoreEngine.h"
#include "Bullet.h"



class Shape
{
    public:
        static void getCircle(
                const int& amount,
                const de::classes::Vector& position,
                const de::classes::Vector& direction,
                const float& arcSize,
                const float& rotation,
                std::vector<de::classes::Vector> &Velocities,
                std::vector<de::classes::Vector> &Positions );

        static void getLine(
                const int &amount,
                const de::classes::Vector &position,
                const de::classes::Vector &direction,
                const de::classes::Vector &gap,
                std::vector<de::classes::Vector> &Velocities,
                std::vector<de::classes::Vector> &Positions );
};


class BulletEmitter
{
    public:
        BulletEmitter( int &_timeTillStart,
                       int &_duration,
                       const de::classes::BulletWaveInfo &_bulletInfo,
                       const de::enums::Collision &_label );

        void logic( const de::classes::Vector &_position,
                    const Uint32 &deltaTicks,
                    boost::ptr_vector<Attrition::Bullet> &_Bullets );

        void jumpToTime( const int &_currentTime,
                         const de::classes::Vector &_offSet,
                         std::vector<de::classes::Vector> &_Points,
                         boost::ptr_vector<Attrition::Bullet> &_Bullets );
        bool finished();

    protected:
        float rotation, otherRotation;
        int timeTillStart, duration;
        bool active;

        de::enums::Collision label;
        de::classes::BulletWaveInfo bulletInfo;
        de::classes::Vector direction, directionAiming, position, gap;
        de::time::Counter reload;

        std::vector<de::classes::Vector> Velocities, Positions, gade;
        std::vector<shipPos> Ships;
        std::vector<shipPos>::iterator iter;



        void fire( const de::classes::Vector &, boost::ptr_vector<Attrition::Bullet> &_Bullets, int _timeOffset );
        void getPositions();
        void getTargets();
        void rotate();

        void distanceSort( std::vector<shipPos> &_Ships, const de::classes::Vector &_position );
        void circularIncrement( std::vector<shipPos> &_Ships, std::vector<shipPos>::iterator &_iter );
        void createBullets( boost::ptr_vector<Attrition::Bullet> &_Bullets, const int &_timeOffset );
};

class BulletEmitterFactory
{
    public:
        BulletEmitterFactory( std::vector<de::classes::BulletWaveInfo> &_bulletInfo,
                              std::vector<de::classes::BulletTimes> &_firingTimes,
                              const de::enums::Collision &_label );

        BulletEmitterFactory( const BulletEmitterFactory &_other );

        void logic( const Uint32 &deltaTicks,
                    const de::classes::Vector &_position,
                    boost::ptr_vector<Attrition::Bullet> &_Bullets );

        void jumpToTime( const int &_currentTime,
                         const de::classes::Vector &_offSet,
                         std::vector<de::classes::Vector> &_Points,
                         boost::ptr_vector<Attrition::Bullet> &_Bullets );

        //BulletEmitterFactory operator= ( const BulletEmitterFactory &_other );

    protected:
        int totalTimePassed;
        de::enums::Collision label;

        std::vector<de::classes::BulletWaveInfo> BulletInfo;
        std::vector<de::classes::BulletTimes> FiringTimes;
        std::vector<BulletEmitter> Emitters;
};



#endif //BULLETGENERATOR_H
