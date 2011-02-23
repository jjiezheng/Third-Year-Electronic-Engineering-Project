#ifndef DASEIN_H
#define DASEIN_H

#include "CoreEngine.h"
#include "Coords.h"


#include "SpriteGroup.h"
#include "Bullet.h"
#include "Particle.h"





class BulletEmitterFactory;






namespace Attrition
{
    class Ship
    {
        public:
            Ship();
            ~Ship();

            Ship( const Ship &_other );
            Ship( const std::string &_shipType );
            void loadSettings( std::vector<de::classes::WaveInfo>::iterator &_iterWave );


            void logic( Uint32 deltaTicks, boost::ptr_vector<Bullet> & Bullets, boost::ptr_vector<Particle> &_Particles );
            void render( de::enums::fboTypes _layer, bool _showHitbox );
            void reLoad();
/*
            bool checkCollisions( boost::ptr_vector<Bullet> &, boost::ptr_vector<Particle> & );
            bool checkCollisions( boost::ptr_vector<Ship> &, boost::ptr_vector<Particle> & );


            void isHit( int, bool isHit = false );
            void explosion( boost::ptr_vector<Particle> &_Particles );
*/
            bool isAlive( boost::ptr_vector<Particle> &_Particles );
            void playerControled( bool _yes );


            void setVelocity( de::classes::Vector & );
            void rotate( const int &_rotateAngle );
            void setPosition( const de::classes::Vector & );
            void setOffset( const de::classes::Vector & );
            void jumpToTime( int _currentTime, boost::ptr_vector<Bullet> &_Bullets );


            de::classes::Poly& getHitbox();
            const de::classes::Vector& getPosition();
            de::enums::Collision getType();
            int getId();

            Ship operator= ( const Ship &_other );

        private:
            bool loadSettings();
            bool parseSettings( const luabind::object &_MainTable );
            void setShipAttributes( std::vector<de::classes::WaveInfo>::iterator iterWave );
            void generateId();


            bool checkActive();
            void move( Uint32 deltaTicks );
            void splineMove( Uint32 deltaTicks );


            void getSplinePath( const de::classes::Line &_path, std::vector<de::classes::Vector> &_Points );
            void updateVisualSpline( const float &_location, const bool _nextPoint );
            bool deleteVisualSplinePoint( int _howManyToDelete );


        private:
            de::graphics::SpriteGroup sprites;
            //de::coords::Hitbox hitbox;

            lua_State *luaState;

            bool active, bounded, alive, usingSplines, firstTime;
            int maxVelocity, maxHealth, currentHealth;
            int id;
            float t, last_t;
            std::string shipType;
            de::enums::Collision label;

            de::classes::Vector offSet, Velocity, hitboxOffset;

            std::vector<de::classes::Vector> Points;
            std::vector<de::classes::Vector>::iterator iterPoints;

            BulletEmitterFactory *weapon;
            de::classes::Poly hitbox;

            de::classes::LinedBox visualHitbox;
            de::classes::Line pathToFollow;
    };
}

#endif //DASEIN_H

