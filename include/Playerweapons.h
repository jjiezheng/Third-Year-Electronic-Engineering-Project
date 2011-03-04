#ifndef PLAYERWEAPON_H
#define PLAYERWEAPON_H

#include "CoreEngine.h"
#include "Coords.h"
#include "CoreEngine.h"
#include "Bullet.h"
#include "Bulletgenerator.h"


namespace de
{
    namespace enums
    {
        enum PlayerGunType
        {
            GATLING,
            SPREAD,
            PLAYERLASER
        };
    }
}


class Weapon
{
private:
    std::vector<de::classes::Vector> Velocities, Positions;
    std::string bulletName;
    int type, bulletAmount;
    bool on;
    de::time::Counter Timer;



    void setupGatling();
    void setudepread();
    void setupLaser();
public:
    Weapon( const std::string &_type );
    ~Weapon(){};

    void fire( const de::classes::Vector &_position, const Uint32 &deltaTicks, boost::ptr_vector<Attrition::Bullet> &_Bullets );
};


class Playerweapon
{
private:
    Weapon Gatling, Spread, Laser;
    bool fire1, fire2;

public:
    Playerweapon();
    ~Playerweapon();

    void handleInput( const bool &, const bool & );
    void logic( const de::classes::Vector &_position, const Uint32 &deltaTicks, boost::ptr_vector<Attrition::Bullet> &_Bullets );
};







#endif //PLAYERWEAPON_H
