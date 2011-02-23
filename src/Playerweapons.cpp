#include "Playerweapons.h"

using namespace de::classes;
using namespace de::enums;

using namespace Attrition;

Weapon::Weapon( const std::string &_type )
    :on(false)
{
    if( _type == "Gatling" )
        setupGatling();

    else if( _type == "Spread" )
        setudepread();

    else if( _type == "Laser" )
        setupLaser();

    else setupGatling();
}


void Weapon::setupGatling()
{
    Timer.countDelayIs( 20 );
    type = GATLING;
    bulletAmount = 3;
    bulletName = "Gatling";

    Shape::getLine( bulletAmount, Vector(0,-0), Vector(0.0,-1.0), Vector(20,0), Velocities, Positions );
}

void Weapon::setudepread()
{
    Timer.countDelayIs( 350 );
    type = SPREAD;
    bulletAmount = 2;
    bulletName = "Spread";

    Velocities.push_back( Vector(-0.5,-1.0) );
    Positions.push_back( Vector(-20,0) );

    Velocities.push_back( Vector(0.5,-1.0) );
    Positions.push_back( Vector(20,0) );

/*
    for( int i(0); i < bulletAmount; ++i )
    {
        Velocities.push_back( Vector(0,-1) );
        Positions.push_back( Vector(0,0) );
    }*/
}

void Weapon::setupLaser()
{
    Timer.countDelayIs( 100 );
    type = PLAYERLASER;
    bulletAmount = 1;
    bulletName = "Gatling";

    for( int i(0); i < bulletAmount; ++i )
    {
        Velocities.push_back( Vector(0,-1) );
        Positions.push_back( Vector(0,0) );
    }
}


void Weapon::fire( const Vector &_position, const Uint32 &deltaTicks, boost::ptr_vector<Bullet> &_Bullets )
{
    if( Timer.count( deltaTicks ) )
    {
        for( unsigned int i(0); i < Velocities.size(); ++i )
        {/*
            if( type == GATLING )
            {
                _Bullets.push_back( new Simple( Positions[i] + _position, Velocities[i], FRIENDLY, bulletName ) );
            }

            if( type == SPREAD )
            {
                _Bullets.push_back( new Simple( Positions[i] + _position, Velocities[i], FRIENDLY, bulletName ) );
            }

            else if( type == PLAYERLASER )
            {

                _Bullets.push_back( new Simple( Positions[i] + _position, Velocities[i], FRIENDLY, bulletName ) );
            }*/
        }
    }
}




Playerweapon::Playerweapon()
    :Gatling( "Gatling" ),
    Spread( "Spread" ),
    Laser( "Laser" ),
    fire1(false),
    fire2(false)
{

}

Playerweapon::~Playerweapon()
{
}



void Playerweapon::handleInput( const bool &_input1, const bool &_input2 )
{
    fire1 = _input1;
    fire2 = _input2;
}


void Playerweapon::logic( const Vector &_position, const Uint32 &deltaTicks, boost::ptr_vector<Bullet> &_Bullets )
{
    if( fire1 && !fire2 )
        Gatling.fire( _position, deltaTicks, _Bullets );


    else if( !fire1 && fire2 )
        Spread.fire( _position, deltaTicks, _Bullets );


    else if( fire1 && fire2 )
    {
        Laser.fire( _position, deltaTicks, _Bullets );
    }
}
