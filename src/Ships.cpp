#include "Ships.h"
#include "Bullet.h"
#include "Bulletgenerator.h"
#include "SpatialHash.h"

#include <set>

using namespace de::classes;
using namespace de::graphics;
using namespace de::core;
using namespace de::misc;
using namespace de::filesystem;
using namespace de::enums;

namespace fs = boost::filesystem;


namespace Attrition
{

    Ship Ship::operator= ( const Ship &_other )
    {
        sprites = _other.sprites;

        active = _other.active;
        bounded = _other.bounded;
        alive = _other.alive;
        usingSplines = _other.usingSplines;
        firstTime = _other.firstTime;

        maxVelocity = _other.maxVelocity;
        maxHealth = _other.maxHealth;
        currentHealth = _other.currentHealth;

        t = _other.t;
        last_t = _other.last_t;
        shipType = _other.shipType;
        label = _other.label;


        offSet = _other.offSet;
        Velocity = _other.Velocity;
        hitboxOffset = _other.hitboxOffset;


        Points = _other.Points;
        for( std::vector<de::classes::Vector>::const_iterator i = _other.Points.begin(); i != _other.iterPoints; ++iterPoints );

        hitbox = _other.hitbox;
        visualHitbox = _other.visualHitbox;
        pathToFollow = _other.pathToFollow;

        if( _other.weapon != NULL )
        {
            delete weapon;
            weapon = new BulletEmitterFactory( (*_other.weapon) );
        }
        else weapon = NULL;


        generateId();

        return *this;
    }

    Ship::Ship()
        :active(false),bounded(false), alive(true), usingSplines(false), firstTime(true),
         maxVelocity(0), maxHealth(1), currentHealth(1),
         t(0.0), last_t(0.0)
    {}

    Ship::Ship( const Ship &_other )
        :active(true),bounded(false), alive(true), usingSplines(false), firstTime(true),
         maxVelocity(0), maxHealth(1), currentHealth(1),
         t(0.0), last_t(0.0)
    {
        sprites = _other.sprites;

        active = _other.active;
        bounded = _other.bounded;
        alive = _other.alive;
        usingSplines = _other.usingSplines;
        firstTime = _other.firstTime;

        maxVelocity = _other.maxVelocity;
        maxHealth = _other.maxHealth;
        currentHealth = _other.currentHealth;

        t = _other.t;
        last_t = _other.last_t;
        shipType = _other.shipType;
        label = _other.label;


        offSet = _other.offSet;
        Velocity = _other.Velocity;
        hitboxOffset = _other.hitboxOffset;


        Points = _other.Points;

        iterPoints = Points.begin();
        for( std::vector<de::classes::Vector>::const_iterator i = _other.Points.begin(); i != _other.iterPoints; ++iterPoints );

        hitbox = _other.hitbox;
        visualHitbox = _other.visualHitbox;
        pathToFollow = _other.pathToFollow;

        if( _other.weapon != NULL )
        {
            delete weapon;
            weapon = new BulletEmitterFactory( (*_other.weapon) );
        }
        else weapon = NULL;


        generateId();
    }

    Ship::Ship( const std::string &_shipType )
        :active(true), bounded(false), alive(true), usingSplines(false), firstTime(true),
         maxVelocity(0), maxHealth(1), currentHealth(1),
         t(0.0), last_t(0.0)
    {
        shipType = _shipType;

        loadSettings();
        generateId();

        usingSplines = false;
    }

    Ship::~Ship( )
    {
        if( weapon )
            delete weapon;
    }



    void Ship::generateId()
    {
        static int i(0);
        ++i;
        id = i;
    }


    bool Ship::loadSettings()
    {
        weapon = NULL;
        label = ENEMY;
        currentHealth = maxHealth;
        pathToFollow.priority = 6;
        pathToFollow.shader = de::Engine::Resources().getShader( "Untextured" );




        luaState = lua_open();
        luaL_openlibs(luaState);
        luabind::open(luaState);


        fs::path dir( Roots->get( root::SHIPS ) + shipType + ".lua" );

        if( !fs::exists( dir ) )
        {
            assert(!"Ship type doesn't exist");
        }

        try
        {
            luaL_dofile( luaState, dir.file_string().c_str() );

            luabind::object MainTable = luabind::globals(luaState)[shipType];
            luabind::object SpriteTable = MainTable["Sprites"];

            if( luabind::type(MainTable) != LUA_TTABLE ||
                luabind::type(SpriteTable) != LUA_TTABLE )
            {
                de::io::error << "In Graphics config file. Problem with tables!\n";
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

    bool Ship::parseSettings( const luabind::object &_MainTable )
    {
        try
        {
            maxHealth   = luabind::object_cast<int>( _MainTable["Hitpoints"] );
            maxVelocity = luabind::object_cast<int>( _MainTable["maxVelocity"] );


            hitboxOffset.x      = luabind::object_cast<int>( _MainTable["hitboxX"] );
            hitboxOffset.y      = luabind::object_cast<int>( _MainTable["hitboxY"] );
            int hitBoxWidth     = luabind::object_cast<int>( _MainTable["hitboxW"] );
            int hitBoxHeight    = luabind::object_cast<int>( _MainTable["hitboxH"] );

            hitbox.Setup( hitBoxWidth, hitBoxHeight, true );
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

    inline void Ship::setShipAttributes( std::vector<WaveInfo>::iterator iterWave )
    {
        weapon = new BulletEmitterFactory( iterWave->bulletWave, iterWave->bulletTimes, label );

        //aligned = iterWave->aligned;

        Points = iterWave->Points;
        iterPoints = Points.begin();
    }

    void Ship::loadSettings( std::vector<WaveInfo>::iterator &_iterWave )
    {
        shipType = _iterWave->shipType;
        setShipAttributes( _iterWave );

        usingSplines = true;
    }



    inline void Ship::move( Uint32 deltaTicks )
    {
        static int leftEdge(0), rightEdge(600);

        static Vector displacement;
        displacement = Velocity * ( deltaTicks / 1000.f );

        sprites.moveBy( displacement );
        hitbox.transform( displacement );


        if( bounded )
        {
            //X components
            if( hitbox.centre.x + hitbox.width()/2 < leftEdge ||
                hitbox.centre.x - hitbox.width()/2 > rightEdge )
            {
                sprites.moveBy( Vector( -displacement.x, 0 ) );
                hitbox.transform( Vector( -displacement.x, 0 ) );
            }

            //Y components
            if( hitbox.centre.y + hitbox.height()/2 < 0 ||
                hitbox.centre.y - hitbox.height()/2 > 800 )
            {
                sprites.moveBy( Vector( 0, -displacement.y ) );
                hitbox.transform( Vector( 0, -displacement.y ) );
            }
        }
    }

    inline void Ship::splineMove( Uint32 _deltaTicks )
    {
        Vector old, displacement(0,0), pos;
        old = sprites.getCentre();

        if( iterPoints + 3  < Points.end() )
        {
            t += ( (float)_deltaTicks / 1000 );

            if( t > 1.0 )
            {
                while( t > 1.0 && iterPoints + 3  < Points.end() )
                {
                    --t;
                    ++iterPoints;
                }
                t = 0;
                updateVisualSpline( 0.0, true );
            }

            updateVisualSpline( t, false );

            if(firstTime)
            {
                firstTime = false;

                float firstT = t - 0.0166667;

                if( firstT < 0 && iterPoints - 1  > Points.begin() )
                {
                    firstT = 1.0 - firstT;
                    --iterPoints;
                    pointOnCurve( pos, firstT, (*iterPoints), (*(iterPoints + 1)), (*(iterPoints + 2)), (*(iterPoints + 3)) );
                    ++iterPoints;
                }
                else
                {
                    pointOnCurve( pos, firstT, (*iterPoints), (*(iterPoints + 1)), (*(iterPoints + 2)), (*(iterPoints + 3)) );
                }
                displacement = pos + offSet - old;

                sprites.moveRotateBy( displacement );
                hitbox.transformRotate( displacement );

                old = sprites.getCentre();
            }

            pointOnCurve( pos, t, (*iterPoints), (*(iterPoints + 1)), (*(iterPoints + 2)), (*(iterPoints + 3)) );

            displacement = pos + offSet - old;

            sprites.moveRotateBy( displacement );
            hitbox.transformRotate( displacement );
        }
        else
            alive = false;



    }

    void Ship::jumpToTime( int _currentTime, boost::ptr_vector<Bullet> &_Bullets )
    {
        int currentTime = _currentTime;
        iterPoints = Points.begin();


        while( currentTime > 1000 && iterPoints + 3  < Points.end() )
        {
            currentTime -= 1000;
            ++iterPoints;
        }
        splineMove( currentTime );


        weapon->jumpToTime( _currentTime, offSet, Points, _Bullets );
    }

    void Ship::logic( Uint32 deltaTicks, boost::ptr_vector<Bullet> &_Bullets, boost::ptr_vector<Particle> &_Particles )
    {
        if( checkActive() ) return;

        if( usingSplines )
        {
            splineMove( deltaTicks );
        }

        else
        {
            move( deltaTicks );
        }

        sprites.logic( deltaTicks );


        if( weapon != NULL )
            weapon->logic( deltaTicks, sprites.getCentre(), _Bullets );
    }

    void Ship::render( de::enums::fboTypes _layer, bool _showHitbox )
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
            de::Engine::Graphics().add( axisAligned, Colour(1,1,1,0.3 ), 10, _layer );
            de::Engine::Graphics().add( pathToFollow, _layer );
        }
    }

    void Ship::reLoad()
    {
        if( checkActive() ) return;

        sprites.reload();
        pathToFollow.shader = de::Engine::Resources().getShader( "Basic");
    }




    bool Ship::isAlive( boost::ptr_vector<Particle> &_Particles )
    {
        return alive;
    }

    bool Ship::checkActive()
    {
        if( !active )
        {
            de::io::error << "Ship not active\n";
        }
        return !active;
    }


    /*
    bool Ship::checkCollisions( boost::ptr_vector<Bullet> &_Bullets, boost::ptr_vector<Particle> &_Particles )
    {
        Vector min, max;
        hitbox.getMinMax( min, max );

        std::vector< std::vector<BulletPtr> > BulletVector( SpatialHash::getBulletsFor( min, max, label ) );

        std::vector< std::vector<BulletPtr> >::iterator iterBulletVector;
        std::vector<BulletPtr>::iterator iterBullet;


        bool result(false);
        std::set<int> CheckedBullets;


        for ( iterBulletVector = BulletVector.begin(); iterBulletVector < BulletVector.end(); ++iterBulletVector )
        {
            for ( iterBullet = iterBulletVector->begin(); iterBullet < iterBulletVector->end(); ++iterBullet )
            {
                if( CheckedBullets.find( iterBullet->bulletId ) == CheckedBullets.end() )
                {
                    if( ( iterBullet->bullet->isAlive() ) )
                    {
                        CheckedBullets.insert( iterBullet->bulletId );
                        if( hitbox.collide( iterBullet->bullet->getHitbox() ) )
                        {
                            currentHealth -= iterBullet->bullet->getDamage();
                            if( currentHealth < 0 )
                                alive = false;

                            iterBullet->bullet->isHit( true );
                            result = true;

                            //Sound::playSoundEffect( explosion );
                            _Particles.push_back( new SmallExplosion( getPosition() ) );
                        }
                    }
                }
            }
        }
        return result;
    }

    bool Ship::checkCollisions( boost::ptr_vector<Ship> &_Ships, boost::ptr_vector<Particle> &_Particles )
    {
        boost::ptr_vector<Ship>::iterator iterShips;
        bool result(false);

        for ( iterShips = _Ships.begin(); iterShips < _Ships.end(); ++iterShips )
        {
            if( hitbox.collide( iterShips->getHitbox() ) )
            {
                currentHealth -= 1;
                if( currentHealth < 0 )
                    alive = false;

                result = true;
            }
        }
        return result;
    }

    void Ship::isHit( int damage, bool isHit )
    {
        if( isHit )
        {
            currentHealth -= damage;
        }
        if( currentHealth < 0 )
        {
            alive = false;
        }
    }

    bool Ship::isAlive()
    {
        return alive;
    }
    */




    void Ship::playerControled( bool _yes )
    {
        if( _yes )
        {
            bounded = true;
            label = FRIENDLY;
            rotate( 180 );
        }
        else
        {
            bounded = false;
            label = ENEMY;
        }
    }

    de::enums::Collision Ship::getType()
    {
        return label;
    }

    int Ship::getId()
    {
        return id;
    }



    void Ship::setVelocity( Vector &_Velocity )
    {
        if( _Velocity.x > 1 || _Velocity.x < -1 )
            _Velocity.x = 0;

        if( _Velocity.y > 1 || _Velocity.y < -1 )
            _Velocity.y = 0;

        Velocity = _Velocity * maxVelocity;
    }

    void Ship::setOffset( const Vector &_offSet )
    {
        offSet = _offSet;
        getSplinePath( pathToFollow, Points );
    }

    void Ship::setPosition( const Vector &_position )
    {
        sprites.moveBy( _position, true );
        hitbox.transform( -hitbox.centre + hitboxOffset + _position );
    }

    void Ship::rotate( const int &_rotateAngle )
    {
        sprites.rotateBy( _rotateAngle );
        hitbox.rotateCentre( _rotateAngle );
    }



    Poly& Ship::getHitbox()
    {
        return hitbox;
    }

    const Vector& Ship::getPosition()
    {
        return hitbox.centre;
    }





    inline void Ship::getSplinePath( const Line &_path, std::vector<Vector> &_Points )
    {
        std::vector<Vector>::iterator iterVector;
        Vector Position;
        float location = 0.0;

        iterVector = _Points.begin();
        while( iterVector + 3  < _Points.end() )
        {
            location += 0.01;
            if( location > 1 )
            {
                location = 0;
                ++iterVector;
            }

            pointOnCurve( Position, location, (*iterVector), (*(iterVector + 1)), (*(iterVector + 2)), (*(iterVector + 3)) );
            Position = Position + offSet;

            pathToFollow.add( Position, Colour( 1*(1/location),1*location,0,1) );
        }
    }

    void Ship::updateVisualSpline( const float &_location, const bool _nextPoint )
    {
        if( _nextPoint )
        {
            int howManyToDelete = ( 1.0 - last_t ) + _location;
            last_t = _location;

            pathToFollow.remove1stPoint();
            deleteVisualSplinePoint( howManyToDelete );
        }
        else
        {
            int howManyToDelete = (_location - last_t)*(1.0/0.1);

            if( deleteVisualSplinePoint( howManyToDelete ) )
                last_t = _location;
        }
    }

    bool Ship::deleteVisualSplinePoint( int _howManyToDelete )
    {
        if( _howManyToDelete >= 1 )
        {
            while( _howManyToDelete >= 1 )
            {
                pathToFollow.remove1stPoint();
                --_howManyToDelete;
            }
            return true;
        }
        return false;
    }

}



