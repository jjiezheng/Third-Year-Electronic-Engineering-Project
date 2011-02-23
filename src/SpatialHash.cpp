#include "SpatialHash.h"
#include "CoreEngine.h"

using namespace de::classes;
using namespace de::enums;

using namespace Attrition;


namespace de
{
    namespace core
    {


        unsigned int SpatialHash::bulletId(0), SpatialHash::maxX(0), SpatialHash::maxY(0), SpatialHash::limitX(0), SpatialHash::limitY(0);
        std::vector< std::vector< std::vector<BulletPtr> > > SpatialHash::playerBulletHash, SpatialHash::enemyBulletHash;
        int SpatialHash::leftBorder(0), SpatialHash::topBorder(0), SpatialHash::gridRes(20);



        void SpatialHash::init()
        {
            maxX = ( 600/gridRes) + 1;
            maxY = ( 800/gridRes) + 1;

            limitX = 600;
            limitY = 800;

            for( unsigned int i = 0; i < maxX; ++i )
            {
                playerBulletHash.push_back( std::vector< std::vector<BulletPtr> >() );
                enemyBulletHash.push_back( std::vector< std::vector<BulletPtr> >() );

                for( unsigned int j = 0; j < maxY; ++j )
                {
                    playerBulletHash[i].push_back( std::vector<BulletPtr>() );
                    enemyBulletHash[i].push_back( std::vector<BulletPtr>() );
                }
            }
        }


        std::vector< std::vector<BulletPtr> > SpatialHash::getBulletsFor( Vector &_min, Vector &_max, const int &_label )
        {
            //First work out what part of the grid is needed

            _min.x -= leftBorder;
            _min.y -= topBorder;

            _max.x -= leftBorder;
            _max.y -= topBorder;

            _min /= gridRes;
            _max /= gridRes;



            std::vector< std::vector<BulletPtr> > BulletVector;
            for( unsigned int i = _min.x; i < _max.x; ++i )
            {
                for( unsigned int j = _min.y; j < _max.y; ++j )
                {
                    if( i > 0 && i < maxX && j > 0 && j < maxY )
                    {
                        if( _label == ENEMY && !playerBulletHash[i][j].empty() )
                            BulletVector.push_back( playerBulletHash[i][j] );

                        else if( _label == FRIENDLY && !enemyBulletHash[i][j].empty() )
                            BulletVector.push_back( enemyBulletHash[i][j] );
                    }
                }
            }
            return BulletVector;
        }


        void SpatialHash::addBullet( Vector &_min, Vector &_max, const int &_label, Bullet* _currentBullet )
        {
            _min.x -= leftBorder;
            _min.y -= topBorder;

            _max.x -= leftBorder;
            _max.y -= topBorder;


            if( _min.x < 0 )
                _min.x = 0;
            if( _min.x > limitX )
                _min.x = limitX;

             if( _max.x < 0 )
                _max.x = 0;
            if( _max.x > limitX )
                _max.x = limitX;

            if( _min.y < 0 )
                _min.y = 0;
            if( _min.y > limitY )
                _min.y = limitY;

            if( _max.y < 0 )
                _max.y = 0;
            if( _max.y > limitY )
                _max.y = limitY;



            _min /= gridRes;
            _max /= gridRes;


            for( int i = _min.x; i < _max.x; ++i )
            {
                for( int j = _min.y; j < _max.y; ++j )
                {
                    if( i > 0 && j > 0 )
                    {
                        if( _label == ENEMY )
                            enemyBulletHash[i][j].push_back( BulletPtr(bulletId, _currentBullet) );
                        else if( _label == FRIENDLY )
                            playerBulletHash[i][j].push_back( BulletPtr(bulletId, _currentBullet) );
                    }
                }
            }

            ++bulletId;
        }


        void SpatialHash::finishedFrame()
        {
            bulletId = 0;
            for( unsigned int i = 0; i < maxX; ++i )
            {
                for( unsigned int j = 0; j < maxY; ++j )
                {
                    playerBulletHash[i][j].clear();
                    enemyBulletHash[i][j].clear();
                }
            }
        }

    }
}
