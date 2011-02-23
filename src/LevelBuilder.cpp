#include "LevelBuilder.h"

#include <sstream>


#include <string>
#include <iostream>
#include <fstream>

using namespace de::classes;
using namespace de::enums;
using namespace de::theme;
using namespace de::misc;
using namespace de::core;

namespace Attrition
{
    namespace builder
    {

        LevelBuilder::LevelBuilder() :
            levelNumber(0),
            waveNumber(0),
            addWave(false),
            deleteWave(false),
            save(false),
            load(false),
            waveBack(false),
            waveForward(false),
            levelBackward(false),
            levelForward(false),
            currentTime(0)
        {

            loadLevelSettings();
        }

        LevelBuilder::~LevelBuilder()
        {

        }

        bool LevelBuilder::refreshUI()
        {
            Vector offset(0,18);

            int number( levelNumber + 1 );
            int size( levelSettings.size() );
            std::string levelsName( toString( number ) + " of " + toString( size ) );

            number = waveNumber + 1;
            size = iterLevel->Waves.size();
            std::string wavesName( toString( number ) + " of " + toString( size ) );


            levelInfo.set( &(*iterLevel), levelsName, save, load, Vector( 200, 50 ), offset );
            shipInfo.set( &(*iterWave), wavesName, addWave, deleteWave, Vector( 1130, 50 ), offset );
            gunInfo.set( &(*iterWave), Vector( 200, 200 ), offset );

            shipPath.setPoints( &(iterWave->Points) );
            slider.set( "Time:", &currentTime, &(*iterLevel).time, 500, Vector(730,15), 11 );
            ShipSpawner.set( &currentTime);
            return false;
        }



        void LevelBuilder::loadLevelSettings()
        {/*
            levelSettings = Settings::getAllLevelSettings();

            iterLevel = levelSettings.begin();
            levelNumber = 0;

            iterWave = iterLevel->Waves.begin();
            waveNumber = 0;

*/
            refreshUI();

        }

        void LevelBuilder::saveLevelSettings()
        {
            //Settings::saveLevelSettings( levelSettings );
        }

        void LevelBuilder::addingWave()
        {
            iterLevel->Waves.push_back( WaveInfo( iterLevel->Waves.size(), true ) );

            iterLevel = levelSettings.begin();
            for( int i = 0; i < levelNumber; ++i )
                ++iterLevel;

            iterWave = iterLevel->Waves.end()  - 1;
            waveNumber = iterLevel->Waves.size() - 1;

            refreshUI();
        }

        void LevelBuilder::deletingWave()
        {
            if( iterLevel->Waves.size() > 1 )
            {
                iterWave = iterLevel->Waves.erase( iterWave );

                if( iterWave == iterLevel->Waves.end() )
                {
                    --iterWave;
                    --waveNumber;
                }
                else
                {
                    int i(0);
                    std::vector<de::classes::WaveInfo>::iterator iter;
                    for( iter = iterLevel->Waves.begin(), i = 0; iter < iterLevel->Waves.end(); ++iter, ++i )
                    {
                        iter->name = "Wave_" + toString( i );
                    }
                }

                refreshUI();
            }
        }




        void LevelBuilder::handleEvent( const SDL_Event &_event )
        {
            shipPath.handleEvent( _event );

            shipInfo.handleEvent( _event );
            gunInfo.handleEvent( _event );
            levelInfo.handleEvent( _event );

            slider.handleEvent( _event );
            ShipSpawner.handleEvent( _event );

            waveBack = waveForward = levelBackward = levelForward = false;

            if( _event.type == SDL_USEREVENT )
            {
                if( _event.user.code == events::WAVE_BACK )
                {
                    waveBack = true;
                }
                else if( _event.user.code == events::WAVE_FORWARD )
                {
                    waveForward = true;
                }
                else if( _event.user.code == events::LEVEL_BACK )
                {
                    levelBackward = true;
                }
                else if( _event.user.code == events::LEVEL_FORWARD )
                {
                    levelForward = true;
                }
            }

            if( _event.type == SDL_KEYDOWN )
            {
                if( _event.key.keysym.sym )
                {
                    if( _event.key.keysym.sym == SDLK_PAGEUP )
                    {
                        waveForward = true;
                    }
                    else if( _event.key.keysym.sym == SDLK_PAGEDOWN )
                    {
                        waveBack = true;
                    }
                    else if( _event.key.keysym.sym == SDLK_HOME )
                    {
                       levelForward = true;
                    }
                    else if( _event.key.keysym.sym == SDLK_END )
                    {
                        levelBackward = true;
                    }
                }
            }
        }


        void LevelBuilder::logic( const Uint32 &deltaTicks,
                                  boost::ptr_vector<Attrition::Ship> &_Ships,
                                  boost::ptr_vector<Bullet> &_Bullets )
        {
            bool resetPoints(false), wasThereChange(false);

            wasThereChange =
                gunInfo.logic( deltaTicks ) ||
                levelInfo.logic( deltaTicks ) ||

                shipInfo.logic( deltaTicks, resetPoints ) ||
                shipPath.logic( deltaTicks );

            if( resetPoints )
            {
                shipPath.setPoints( &(iterWave->Points) );
            }


            slider.logic( deltaTicks );
            ShipSpawner.logic( wasThereChange, deltaTicks, iterWave, _Ships, _Bullets );
            //ShipSpawner.logic( wasThereChange, deltaTicks, iterLevel, _Ships, _Bullets );

            if( addWave )
            {
                addWave = false;
                addingWave();
            }

            if( deleteWave )
            {
                deleteWave = false;
                deletingWave();
            }

            if( save )
            {
                save = false;
                saveLevelSettings();
            }

            if( load )
            {
                load = false;
                loadLevelSettings();
            }

            if( waveBack )
            {
                waveBack = false;
                if( iterWave != iterLevel->Waves.begin() )
                {
                    --iterWave;
                    --waveNumber;

                    refreshUI();
                }
            }
            if( waveForward )
            {
                waveForward = false;
                if( iterWave != ( iterLevel->Waves.end() - 1 ) )
                {
                    ++iterWave;
                    ++waveNumber;

                    refreshUI();
                }
            }
            if( levelBackward )
            {
                levelBackward = false;
                if( iterLevel != levelSettings.begin() )
                {
                    --iterLevel;
                    iterWave = iterLevel->Waves.begin();

                    --levelNumber;
                    waveNumber = 0;

                    refreshUI();
                }
            }
            if( levelForward )
            {
                levelForward = false;
                if( iterLevel != ( levelSettings.end() - 1 ) )
                {
                    ++iterLevel;
                    iterWave = iterLevel->Waves.begin();

                    ++levelNumber;
                    waveNumber = 0;

                    refreshUI();
                }
            }
        }

        void LevelBuilder::reLoadTextures()
        {
            ShipSpawner.reLoadTextures();
            slider.reLoadTextures();

            shipInfo.reLoadTextures();
            gunInfo.reLoadTextures();
            shipPath.reLoadTextures();
            levelInfo.reLoadTextures();
        }

        void LevelBuilder::render( fboTypes _fbo, fboTypes _fbo2 )
        {
            slider.render( _fbo, _fbo2 );
            ShipSpawner.render( _fbo, _fbo2 );

            shipInfo.render( _fbo, _fbo2 );
            gunInfo.render( _fbo, _fbo2  );
            shipPath.render( FBO_GAME );
            levelInfo.render( _fbo, _fbo2 );
        }

    }
}
