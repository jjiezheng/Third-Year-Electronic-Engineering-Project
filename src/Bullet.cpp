#include "Bullet.h"
#include "SpatialHash.h"
#include <iostream>


using namespace de::classes;
using namespace de::graphics;
using namespace de::misc;
using namespace de::filesystem;
using namespace de::enums;

namespace fs = boost::filesystem;



std::vector<shipPos> ShipTracker::Ships, ShipTracker::playerShips;
ShipTracker::ShipTracker() {}


void ShipTracker::add( Vector _pos, int _id )
{
    Ships.push_back( shipPos( _pos, _id)  );

}

void ShipTracker::addPlayer( Vector _pos, int _id )
{
    playerShips.push_back( shipPos( _pos, _id)  );
}

void ShipTracker::clear()
{
    Ships.clear();
    playerShips.clear();
}


Vector ShipTracker::getPos( const int& _id )
{
    static std::vector<shipPos>::iterator iter;
    for( iter = Ships.begin(); iter < Ships.end(); ++iter )
    {
        if( _id == iter->id )
            return iter->pos;
    }

    for( iter = playerShips.begin(); iter < playerShips.end(); ++iter )
    {
        if( _id == iter->id )
        {
            return iter->pos;
        }
    }

    Vector empty(0,0);
    return empty;
}

Vector ShipTracker::getPlayerPos( const int& _id )
{
    static std::vector<shipPos>::iterator iter;
    for( iter = playerShips.begin(); iter < playerShips.end(); ++iter )
    {
        if( _id == iter->id )
            return iter->pos;
    }
    Vector empty(0,0);
    return empty;
}


int ShipTracker::count()
{
    return Ships.size();
}


/*

    de::enums::Collision label;

    bool alive, lockon;
    int maxVelocity, damage, idLock;

    float turningCircle;
    std::string bulletName, shader, texture;


    de::classes::Vector Velocity;
    de::classes::Vector* target;

    de::graphics::SpriteGroup sprites;

    de::classes::Poly hitbox;
    de::classes::LinedBox visualHitbox;

*/


namespace Attrition
{

    Bullet::Bullet()
        : active(false), lockOn(false), initialMode(true), activeMode(false), maxVelocity(1), damage(1), idLock(-1), turningCircle(0)
    {
    }

    Bullet::Bullet( const Bullet &_other )
        : active(false), lockOn(false), initialMode(true), activeMode(false), maxVelocity(1), damage(1), idLock(-1), turningCircle(0)
    {
        alive = _other.alive;

        damage = _other.damage;
        maxVelocity = _other.maxVelocity;
        idLock = _other.idLock;

        delay = _other.delay;
        activeFor = _other.activeFor;

        turningCircle = _other.turningCircle;
        Velocity = _other.Velocity;
        bulletType = _other.bulletType;
        type = _other.type;

        hitbox = _other.hitbox;
        sprites = _other.sprites;
    }

    Bullet::Bullet( const std::string &_bulletType )
        : active(true), lockOn(false), initialMode(true), activeMode(false), maxVelocity(1), damage(1), idLock(-1), turningCircle(0)
    {
        bulletType = _bulletType;
        loadSettings();
    }

    Bullet::Bullet( const std::string &_bulletType,
                    const de::enums::Collision &_label,
                    const de::classes::Vector &_position,
                    const de::classes::Vector &_direction,
                    const int& _id )

        : active(true), lockOn(false), initialMode(true), activeMode(false), maxVelocity(1), damage(1), idLock(-1), turningCircle(0)
    {
        bulletType = _bulletType;
        loadSettings();

        idLock = _id;
        label = _label;
        Velocity = _direction*static_cast<float>(maxVelocity);
        sprites.moveBy( _position );
        hitbox.transform( _position );
    }

    Bullet::Bullet( const Bullet &_other,
                    const de::enums::Collision &_label,
                    const de::classes::Vector &_position,
                    const de::classes::Vector &_direction,
                    const int& _id )

        : active(true), lockOn(false), initialMode(true), activeMode(false), maxVelocity(1), damage(1), idLock(-1), turningCircle(0)
    {
        bulletType = _other.bulletType;
        type = _other.type;
        label = _label;
        alive = _other.alive;

        damage = _other.damage;
        maxVelocity = _other.maxVelocity;
        turningCircle = _other.turningCircle;
        Velocity = _other.Velocity;


        hitbox = _other.hitbox;
        sprites = _other.sprites;

        delay = _other.delay;
        activeFor = _other.activeFor;

        idLock = _id;
        label = _label;
        Velocity = _direction*static_cast<float>(maxVelocity);
        sprites.moveBy( _position );
        hitbox.transform( _position );
    }

    Bullet::~Bullet()
    {

    }




    bool Bullet::loadSettings()
    {
        label = ENEMY;

        luaState = lua_open();
        luaL_openlibs(luaState);
        luabind::open(luaState);


        fs::path dir( Roots->get( root::BULLETS ) + bulletType + ".lua" );

        if( !fs::exists( dir ) )
        {
            assert(!"Bullet type doesn't exist");
        }

        try
        {
            luaL_dofile( luaState, dir.file_string().c_str() );

            luabind::object MainTable = luabind::globals(luaState)[bulletType];
            luabind::object SpriteTable = MainTable["Sprites"];

            if( luabind::type(MainTable) != LUA_TTABLE ||
                luabind::type(SpriteTable) != LUA_TTABLE )
            {
                de::io::error << "In Bullets config file. Problem with tables!\n";
                return false;
            }

            parseSettings( MainTable );
            sprites.parseSprites( SpriteTable );
        }
        catch( luabind::error &_e )
        {
            std::string error = lua_tostring( _e.state(), -1 );
            de::io::error << _e.what() << "\n";
            de::io::error << error << "\n";
        }
        catch( luabind::cast_failed  &_e )
        {
            std::string error1 = lua_tostring( _e.state(), -1 );
            std::string error2 = lua_tostring( _e.state(), 0 );
            de::io::error << _e.what() << " - " << error1 << "|" << error2 << "\n";
        }

        lua_close(luaState);

        return true;
    }

    bool Bullet::parseSettings( const luabind::object &_MainTable )
    {
        try
        {
            damage      = luabind::object_cast<int>( _MainTable["Damage"] );
            maxVelocity = luabind::object_cast<int>( _MainTable["maxVelocity"] );

/*
            hitboxOffset.x      = luabind::object_cast<int>( _MainTable["hitboxX"] );
            hitboxOffset.y      = luabind::object_cast<int>( _MainTable["hitboxY"] );*/
            int hitBoxWidth     = luabind::object_cast<int>( _MainTable["hitboxW"] );
            int hitBoxHeight    = luabind::object_cast<int>( _MainTable["hitboxH"] );
            hitbox.Setup( static_cast<float>(hitBoxWidth), static_cast<float>(hitBoxHeight), true );


            luabind::object DelayTable = _MainTable["Delay"];
            luabind::object ParticleTable = _MainTable["ParticleTrail"];

            if( luabind::type(DelayTable) == LUA_TTABLE )
            {
                delay.countDelayIs( luabind::object_cast<int>( DelayTable["WaitFor"] ) );
                activeFor.countDelayIs( luabind::object_cast<int>( DelayTable["activeFor"] ) );
            }
            else
            {
                delay.countDelayIs( 0 );
                activeFor.countDelayIs( 1000000 );
            }

            if( luabind::type(ParticleTable) == LUA_TTABLE )
            {
                luabind::object_cast<std::string>( ParticleTable["particleType"] );
                luabind::object_cast<int>( ParticleTable["timeBetweenParticles"] );
            }

            std::string typeString = luabind::object_cast<std::string>( _MainTable["Type"] );
            if( typeString ==  "Simple" )
            {
                type = bulletTypes::SIMPLE;
            }
            else if( typeString == "Homing" )
            {
                type = bulletTypes::HOMING;
            }
            else
            {
                type = bulletTypes::SIMPLE;
            }

        }
        catch( luabind::error &_e )
        {
            std::string error = lua_tostring( _e.state(), -1 );
            de::io::error << _e.what() << "\n";
            de::io::error << error << "\n";
        }
        catch( luabind::cast_failed  &_e )
        {
            std::string error1 = lua_tostring( _e.state(), -1 );
            std::string error2 = lua_tostring( _e.state(), 0 );
            de::io::error << _e.what() << " - " << error1 << "|" << error2 << "\n";
        }


        return true;
    }




    void Bullet::logic( const float &_oneOverTicks, Uint32 _deltaTicks )
    {
        if( checkActive() ) return;

        sprites.logic( _deltaTicks );
        move( _oneOverTicks, _deltaTicks );
    }

    void Bullet::render( de::enums::fboTypes _layer, bool _showHitbox )
    {
        if( checkActive() ) return;

        sprites.render( _layer );
        if( _showHitbox )
        {
            Vector min, max;
            hitbox.getMinMax( min, max );

            Poly axisAligned( max.x - min.x, max.y - min.y, true );
            axisAligned.transform( hitbox.centre );


            visualHitbox.update( hitbox );
            visualHitbox.render( label, _layer );
            /*de::Engine::Graphics().add( axisAligned, Colour(1,1,1,0.3 ), 10, _layer );*/
        }
    }

    void Bullet::reLoad()
    {
        sprites.reload();
    }

    void Bullet::jumpToTime( const int &_currentTime )
    {
        float oneOverTicks(0);
        if(_currentTime != 0)
            oneOverTicks = (float)_currentTime/1000.f;

        move( oneOverTicks, _currentTime );
    }


    void Bullet::move( const float &_oneOverTicks, Uint32 _deltaTicks )
    {
        if( type == bulletTypes::HOMING )
        {

            homingMove( _oneOverTicks, _deltaTicks );
        }



        static Vector displacement;
        displacement = Velocity*_oneOverTicks;

        sprites.moveRotateBy( displacement );
        hitbox.transformRotate( displacement );

        alive = checkBounds( hitbox.centre );
    }

    void Bullet::homingMove( const float &_oneOverTicks, Uint32 _deltaTicks )
    {
/*

        if( initialMode )
        {
            de::io::log << " in initialMode\n";
            initialMode = !delay.count( _deltaTicks );
            if( !initialMode )
            {
                activeMode = true;
                idLock = ShipTracker::playerShips.begin()->id;
                de::io::log << "idLock = " << idLock << "\n";
                homingMove( _oneOverTicks, _deltaTicks - delay.getOverFlow() );
            }
            return;
        }
        else if( activeMode )
        {
            de::io::log << " in activeMode _deltaTicks = " << (int)_deltaTicks << "\n";
            de::io::log << " in activeMode countDelay = " << (int)activeFor.countDelay << "\n";
            bool lol = activeFor.count( _deltaTicks );
            de::io::log << " lol = " << lol << "overflow = " << activeFor.getOverFlow() << "\n";
            activeMode = !lol;

            if( !activeMode )
            {
                de::io::log << " And I'm gone\n";
                return;
            }


            if( !lockOn )
            {
                Vector currentDirection = Velocity, enemyPos = ShipTracker::getPos( idLock );
                if( enemyPos.x != 0 && enemyPos.y != 0 )
                {
                    static float squareRootOf2 = sqrtf( 2.0 );

                    enemyPos = ( enemyPos - sprites.getCentre() ).normalise() * squareRootOf2;

                    Velocity.x = Velocity.x * enemyPos.x;
                    Velocity.y = Velocity.y * enemyPos.y;
                }
                lockOn = true;
            }
            else
            {
                Vector currentDirection = Velocity, enemyPos = ShipTracker::getPos( idLock );

                if( enemyPos.x != 0 && enemyPos.y != 0 )
                {
                    float turningCircle(0.2);
                    enemyPos = ( enemyPos - sprites.getCentre() ).normalise();
                    currentDirection.normalise();

                    if( enemyPos.x > currentDirection.x )
                        currentDirection.x += turningCircle;

                    if( enemyPos.x < currentDirection.x )
                        currentDirection.x -= turningCircle;

                    if( enemyPos.y > currentDirection.y )
                        currentDirection.y += turningCircle;

                    if( enemyPos.y < currentDirection.y )
                        currentDirection.y -= turningCircle;

                    currentDirection.normalise();

                    Velocity = currentDirection * maxVelocity;
                }
            }

        }


        if( !lockOn )
        {
            lockOn = true;
            Vector currentDirection = Velocity, enemyPos = ShipTracker::getPos( idLock );

            if( enemyPos.x != 0 && enemyPos.y != 0 )
            {
                static float squareRootOf2 = sqrtf( 2.0 );

                enemyPos = ( enemyPos - sprites.getCentre() ).normalise() * squareRootOf2;

                Velocity.x = Velocity.x * enemyPos.x;
                Velocity.y = Velocity.y * enemyPos.y;
            }
        }
*/

    }





    bool Bullet::checkActive()
    {
        if( !active )
        {
            de::io::error << "Ship not active\n";
        }
        return !active;
    }

    bool Bullet::isAlive()
    {
        return alive;
    }

    int Bullet::getDamage()
    {
        return damage;
    }

    de::classes::Poly& Bullet::getHitbox()
    {
        return hitbox;
    }

    void Bullet::isHit( const bool &_isHit )
    {
        if( _isHit )
        {
            alive = false;
            return;
        }
        alive = true;
    }

    /*
    void Bullet::setBulletAttributes( const de::classes::bulletInfo &, const de::classes::Vector & )
    {

    }*/

    void Bullet::addToSpatialHash()
    {
        Vector min, max;
        hitbox.getMinMax( min, max );

        //SpatialHash::addBullet( min, max, label, this );
    }

    bool Bullet::checkBounds( const Vector& centre )
    {
        static int leftBorder = 0;
        static int rightBorder = 600;
        static int bottomBorder = 800;


        if( centre.x < leftBorder || centre.y < 0 )
            return false;
        if ( centre.x > rightBorder || centre.y > bottomBorder )
            return false;

        return true;
    }

}

/*
Simple::Simple( const Vector& _pos, const Vector &_dir, const de::enums::Collision &_label, const std::string &_bulletName )
{
    label = _label;
    //bulletName = "Simple";

    bulletInfo buffer = Settings::getBulletSettings( _bulletName );
    setBulletAttributes( buffer, _dir );
    sprite.coords.transform( _pos );
    hitbox.transform( _pos );
}

void Simple::logic( const float& oneOverTicks, const Uint32& deltaTicks )
{
    move( oneOverTicks );
    addToSpatialHash();

    if( sprite.useAnimation )
        sprite.texCoords = animation.loop( deltaTicks );
}

void Simple::move( const float& oneOverTicks )
{
    static Vector displacement;
    displacement = Velocity*oneOverTicks;

    sprite.coords.transformRotate( displacement );
    hitbox.transformRotate( displacement );

    alive = checkBounds( hitbox.centre );
}

void Simple::jumpToTime( const int &_currentTime )
{
    float oneOverTicks(0);
    if(_currentTime != 0)
        oneOverTicks = (float)_currentTime/1000.f;

    move(oneOverTicks);
    //de::io::log << "Hello World. _currentTime is " + toString(_currentTime) + "\n";
    //de::io::log << "Hello World. oneOverTicks is " + toString(oneOverTicks) + "\n\n";
}

void Simple::isHit( const bool& _isHit )
{
    if( _isHit )
    {
        alive = false;
        return;
    }

    alive = true;
}




Laser::Laser( const Vector& _pos, const Vector& _dir, const de::enums::Collision& _label, const std::string &_bulletName )
{
    label = _label;
    //bulletName = "BulletType_1";

    bulletInfo buffer = Settings::getBulletSettings( _bulletName );
    setBulletAttributes( buffer, _dir );

    sprite.coords.transform( _pos );
    hitbox.transform( _pos );

}

void Laser::logic( const float& oneOverTicks, const Uint32& deltaTicks )
{
    move( oneOverTicks );
    addToSpatialHash();

    if( sprite.useAnimation )
        sprite.texCoords = animation.loop( deltaTicks );
}

void Laser::move( const float& oneOverTicks )
{
    static Vector displacement;

    displacement = Velocity*oneOverTicks;

    sprite.coords.transformRotate( displacement );
    hitbox.transformRotate( displacement );

    alive = checkBounds( hitbox.centre );
}

void Laser::jumpToTime( const int &_currentTime )
{
    //float oneOverTicks(1/_currentTime);
    //move(oneOverTicks);
}

void Laser::isHit( const bool& _isHit )
{
    if( _isHit )
    {
        alive = false;
        return;
    }

    alive = true;
}




SimpleTargeting::SimpleTargeting( const Vector& _pos, const Vector& _dir, const int& _id, const de::enums::Collision& _label, const std::string &_bulletName )
{
    label = _label;
    idLock = _id;

    bulletInfo buffer = Settings::getBulletSettings( _bulletName );
    setBulletAttributes( buffer, _dir );

    sprite.coords.transform( _pos, 0 );
    hitbox.transform( _pos );
}

void SimpleTargeting::logic( const float& oneOverTicks, const Uint32& deltaTicks )
{
    move( oneOverTicks );
    addToSpatialHash();

    if( sprite.useAnimation )
        sprite.texCoords = animation.loop( deltaTicks );
}

void SimpleTargeting::move( const float& oneOverTicks )
{
    Vector enemyPos = ShipTracker::getPos( idLock );

    if( enemyPos.x != 0 && enemyPos.y != 0 )
    {
        enemyPos = ( enemyPos - sprite.coords.centre ).normalise();
        Velocity = enemyPos * maxVelocity;
    }


    static Vector displacement;
    displacement = Velocity*oneOverTicks;

    sprite.coords.transformRotate( displacement );
    hitbox.transformRotate( displacement );

    alive = checkBounds( hitbox.centre );
}

void SimpleTargeting::jumpToTime( const int &_currentTime )
{
    //float oneOverTicks(1/_currentTime);
    //move(oneOverTicks);
}

void SimpleTargeting::isHit( const bool& _isHit )
{
    if( _isHit )
    {
        alive = false;
        return;
    }

    alive = true;

}




Targeting::Targeting( const Vector& _pos, const Vector& _dir, const int& _id, const de::enums::Collision& _label, const std::string &_bulletName )
{
    label = _label;
    idLock = _id;
    //bulletName = "Targeting";

    bulletInfo buffer = Settings::getBulletSettings( _bulletName );
    setBulletAttributes( buffer, _dir );
    turningCircle = buffer.turningCircle;


    sprite.coords.transform( _pos, 0 );
    hitbox.transform( _pos );
}

void Targeting::logic( const float& oneOverTicks, const Uint32& deltaTicks )
{
    move( oneOverTicks );
    addToSpatialHash();

    if( sprite.useAnimation )
        sprite.texCoords = animation.loop( deltaTicks );
}

void Targeting::move( const float& oneOverTicks )
{
    Vector currentDirection = Velocity, enemyPos = ShipTracker::getPos( idLock );

    if( enemyPos.x != 0 && enemyPos.y != 0 )
    {
        enemyPos = ( enemyPos - sprite.coords.centre ).normalise();
        currentDirection.normalise();

        if( enemyPos.x > currentDirection.x )
            currentDirection.x += turningCircle;

        if( enemyPos.x < currentDirection.x )
            currentDirection.x -= turningCircle;

        if( enemyPos.y > currentDirection.y )
            currentDirection.y += turningCircle;

        if( enemyPos.y < currentDirection.y )
            currentDirection.y -= turningCircle;

        Velocity = currentDirection * maxVelocity;
    }


    static Vector displacement;
    displacement = Velocity*oneOverTicks;

    sprite.coords.transformRotate( displacement );
    hitbox.transformRotate( displacement );

    alive = checkBounds( hitbox.centre );
}

void Targeting::jumpToTime( const int &_currentTime )
{
    //float oneOverTicks(1/_currentTime);
    //move(oneOverTicks);
}

void Targeting::isHit( const bool& _isHit )
{
    if( _isHit )
    {
        alive = false;
        return;
    }
    alive = true;
}









void Bullet::addToSpatialHash()
{
    Vector min, max;
    hitbox.getMinMax( min, max );

    SpatialHash::addBullet( min, max, label, this );
}

bool Bullet::checkBounds( const Vector& centre )
{
    static int leftBorder = 0;
    static int rightBorder = 600;
    static int bottomBorder = 800;


    if( centre.x < leftBorder || centre.y < 0 )
        return false;
    if ( centre.x > rightBorder || centre.y > bottomBorder )
        return false;

    return true;
}

const int& Bullet::getDamage()
{
    return damage;
}

const bool& Bullet::isAlive()
{
    return alive ;
}

Poly& Bullet::getHitbox()
{
    return hitbox;
}

void Bullet::render( bool _showHitbox )
{
    Render->addSprite( sprite, FBO_GAME );
    if( _showHitbox )
    {
        visualHitbox.update( hitbox );
        visualHitbox.render( label, FBO_GAME );
        //Graphics::addPoly( hitbox, FBO_GAME );
    }
}

void Bullet::reLoadTextures()
{
    sprite.Texture = resourceManager->getTexture( texture );
    sprite.shader = resourceManager->getShaderObject( shader );
}

inline void Bullet::setBulletAttributes( const bulletInfo &_bulletDetails, const Vector &dir )
{
    maxVelocity = _bulletDetails.velocity;
    damage  = _bulletDetails.damage;
    alive = true;


    Velocity.x = dir.x * maxVelocity;
    Velocity.y = dir.y * maxVelocity;


    texture = _bulletDetails.texture;
    shader = _bulletDetails.shader;
    sprite.priority = _bulletDetails.priority;

    sprite.coords.Setup( _bulletDetails.spriteRect, 0 );
    sprite.texCoords.Setup( _bulletDetails.texRect, 0 );

    sprite.Texture = resourceManager->getTexture( texture );
    sprite.shader = resourceManager->getShaderObject( shader );

    hitbox.Setup( _bulletDetails.hitBox, 0 );

    animation.setup( _bulletDetails.texRect );
    animation.addAnimation( _bulletDetails.animationVector, _bulletDetails.animationDelay, _bulletDetails.framesOfAnimation );
    //---------------------------------------------------------------
}


*/
