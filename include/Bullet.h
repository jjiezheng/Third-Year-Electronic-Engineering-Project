#ifndef BULLET_H
#define BULLET_H

#include "CoreEngine.h"
#include "Coords.h"


#include <SDL/SDL.h>
#include <vector>




struct shipPos
{
    de::classes::Vector pos;
    int id;

    shipPos( de::classes::Vector _pos, int _id )
    {
        pos = _pos;
        id = _id;
    }
};

class ShipTracker
{
    public:
        ShipTracker();

        static void add( de::classes::Vector _pos, int _id );
        static de::classes::Vector getPos( const int& _id );

        static void addPlayer( de::classes::Vector _pos, int _id );
        static de::classes::Vector getPlayerPos( const int& _id );

        static void clear();
        static int count();

    public:
        static std::vector<shipPos> Ships;
        static std::vector<shipPos> playerShips;
};


namespace Attrition
{
    namespace bulletTypes
    {
        enum BulletTypes
        {
            SIMPLE,
            HOMING
        };
    }

    class Bullet
    {
        public:
            Bullet();
            Bullet( const Bullet &_other );
            Bullet( const std::string &_bulletType );

            Bullet( const std::string &_bulletType,
                    const de::enums::Collision &_label,
                    const de::classes::Vector &_position,
                    const de::classes::Vector &_direction,
                    const int& _id = -1 );

            Bullet( const Bullet &_other,
                    const de::enums::Collision &_label,
                    const de::classes::Vector &_position,
                    const de::classes::Vector &_direction,
                    const int& _id = -1 );

            ~Bullet();

            void logic( const float &_oneOverTicks, Uint32 _deltaTicks );
            void render( de::enums::fboTypes _layer, bool _showHitbox );

            void jumpToTime( const int &_currentTime );
            void reLoad();


            bool checkActive();
            bool isAlive();
            int getDamage();
            de::classes::Poly& getHitbox();
            void isHit( const bool &_isHit );


        public:
            de::enums::Collision label;

        private:
            bool loadSettings();
            bool parseSettings( const luabind::object &_MainTable );

            void move( const float &_oneOverTicks, Uint32 _deltaTicks );
            void homingMove( const float &_oneOverTicks, Uint32 _deltaTicks );

            bool checkBounds( const de::classes::Vector &_position  );
            void addToSpatialHash();

        private:
            bool alive, active, lockOn, initialMode, activeMode;
            int maxVelocity, damage, idLock;

            float turningCircle, scalarSpeed;
            std::string bulletType;
            bulletTypes::BulletTypes type;

            de::time::Counter delay, activeFor, particleDelay;
            de::classes::Vector Velocity, direction;
            de::graphics::SpriteGroup sprites;

            de::classes::Poly hitbox;
            de::classes::LinedBox visualHitbox;

            lua_State *luaState;
    };
}


/*
class Bullet
{

protected:
    bool alive;
    int maxVelocity, damage;

    de::classes::Vector Velocity;
    de::classes::Animation animation;

    std::string bulletName, shader, texture;

    //SpriteHandle Sprite;
    de::classes::Sprite sprite;
    de::classes::Poly hitbox;
    de::classes::LinedBox visualHitbox;


    void setBulletAttributes( const de::classes::bulletInfo &, const de::classes::Vector & );
    bool checkBounds( const de::classes::Vector&  );
    void addToSpatialHash();

public:
    de::enums::Collision label;

    const bool& isAlive();
    const int& getDamage();
    de::classes::Poly& getHitbox();

    void render( bool );
    void reLoadTextures();


    virtual void jumpToTime( const int &_currentTime ) = 0;

    virtual void logic( const float& oneOverTicks, const Uint32& deltaTicks ) = 0;
    virtual void isHit( const bool &_isHit ) = 0;
    virtual ~Bullet(){};
};


class Simple : public Bullet
{
    private:
        void move( const float &oneOverTicks );

    public:
        Simple( const de::classes::Vector&, const de::classes::Vector &, const de::enums::Collision &_label, const std::string & );

        void isHit( const bool &_isHit );
        void logic( const float& oneOverTicks, const Uint32& deltaTicks );
        void jumpToTime( const int &_currentTime );
};


class Laser : public Bullet
{
    private:
        void move( const float &oneOverTicks );

    public:
        Laser( const de::classes::Vector&, const de::classes::Vector &, const de::enums::Collision &_label, const std::string & );

        void isHit( const bool &_isHit );
        void logic( const float& oneOverTicks, const Uint32& deltaTicks );
        void jumpToTime( const int &_currentTime );
};


class SimpleTargeting : public Bullet
{
    private:
        bool lockon;
        int idLock;
        de::classes::Vector* target;

        void move( const float &oneOverTicks );

    public:
        SimpleTargeting( const de::classes::Vector&, const de::classes::Vector&, const int &, const de::enums::Collision &_label, const std::string & );

        void isHit( const bool &_isHit );
        void logic( const float& oneOverTicks, const Uint32& deltaTicks );
        void jumpToTime( const int &_currentTime );
};


class Targeting : public Bullet
{
    private:
        bool lockon;
        int idLock;
        float turningCircle;
        de::classes::Vector* target;

        void move( const float &oneOverTicks );

        de::classes::Vector* findClosest();
        void lockOn();
    public:
        Targeting( const de::classes::Vector&, const de::classes::Vector &, const int &, const de::enums::Collision &_label, const std::string & );

        void isHit( const bool &_isHit );
        void logic( const float& oneOverTicks, const Uint32& deltaTicks );
        void jumpToTime( const int &_currentTime );
};
*/



#endif //BULLET_H

