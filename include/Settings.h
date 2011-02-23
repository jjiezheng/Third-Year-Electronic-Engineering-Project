#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Monitor.h"
#include "Coords.h"

#include "CoreEnumsAndClasses.h"
//#include "SDL_config_lib.h"

#include <vector>
#include <sstream>
#include <iostream>

namespace de
{

    namespace classes
    {
        //! Background Settings to be used for the game.
        struct BackroundInfo
        {
            de::classes::Frect spriteRect;
            int priority, yVel;
            bool useRect;
        };

        //! Bullet Timing Settings to be used for the game.
        struct BulletTimes
        {
            int start, duration, weaponUsed;
            BulletTimes(  const int &_number = 0 ) :
                start(0), duration(0), weaponUsed(0)
            {}
        };

        //! Bullet Wave Settings to be used for the game.
        struct BulletWaveInfo
        {
            std::string bulletName;
            int bulletType, firingPattern;

            de::classes::Vector bulletSpeed, bulletGap;
            float bulletRotation, arcSize;
            int bulletNumber, bulletReloadTime;

            BulletWaveInfo( const int &_number = 0 ) :
                bulletType(0), firingPattern(0), bulletRotation(0), arcSize(0), bulletNumber(0), bulletReloadTime(0)
            {}
        };

        //! Wave Settings to be used for the game.
        struct WaveInfo
        {
            std::string name, shipType;

            std::vector<BulletWaveInfo> bulletWave;
            std::vector<BulletTimes> bulletTimes;
            std::vector<de::classes::Vector> Points;
            int number, time, formation;
            float gap;
            bool rotate, aligned;

            WaveInfo( const int &_number = 0, bool _pushBack = false ) :
                number(0), time(0), formation(0), gap(0), rotate(false), aligned(false)
            {
                name = "Wave_" + de::misc::toString( _number );

                if( _pushBack )
                {
                    bulletWave.push_back( BulletWaveInfo() );
                    bulletTimes.push_back( BulletTimes() );
                }
            }
        };

        //! Level Settings to be used for the game.
        struct LevelInfo
        {
            std::string groupName, name, subText, wavePath, backPath, actualWavePath, actualBackPath, music;
            int time;
            int number;

            std::vector<WaveInfo> Waves;
        };

    }

    namespace enums
    {
        //! Ship Formations to be used for the game.
        enum formationTypes
        {
            HORIZONAL_LINE,
            VERTICAL_LINE,
            ARROW
        };

        //! Bullet types to be used for the game.
        enum bulletTypes
        {
            SIMPLE,
            SIMPLE_TARGETING,
            TARGETING,
            TRACKING,
            LASER
        };

        //! Bullet Shape types to be used for the game.
        enum shapeTypes
        {
            LINE,
            CIRCLE,
            CIRCLES2
        };

        //! Settings that can be changed.
        enum enumSettings
        {
            S_NOTHING,
            C_JOYSTICK_NUMBER,
            C_JOYSTICK_FIRE,
            C_JOYSTICK_FIRE2,
            C_JOYSTICK_SPECIAL,

            C_KEYBOARD_UP,
            C_KEYBOARD_DOWN,
            C_KEYBOARD_LEFT,
            C_KEYBOARD_RIGHT,
            C_KEYBOARD_FIRE,
            C_KEYBOARD_FIRE2,
            C_KEYBOARD_SPECIAL
        };

    }
/*
    namespace core
    {
        class Settings
        {
            public:
                static const std::vector<de::classes::LevelInfo>& getAllLevelSettings();
                static void saveLevelSettings( const std::vector<de::classes::LevelInfo> &_levelSettings );

                static de::classes::LevelInfo getLevelSettings( const std::string &name );

                static bool loadData();
                static bool reloadData();
                static bool saveData();
                static void Init();


            private:
                static CFG_File config;
                static bool unloaded;

                static std::vector<de::classes::LevelInfo> levelSettings;

                static std::vector<de::classes::WaveInfo> loadWaveData( const std::string &_levelPath );
                static bool loadLevelData();

                static bool unloadData();
                static bool saveLevelData();
                static bool saveWaveData( std::vector<de::classes::LevelInfo>::iterator &_iterLevel, const std::string &_levelPath );


                static int getFiringPattern( const int &_bulletNumber );
                static std::string getFiringPatternText( int _firingPattern );

                static int getBulletType( const int &_bulletNumber );
                static std::string getBulletTypeText( int _BulletType );

                static int getFormationType();
                static std::string getFormationTypeText( int _formation );
        };
    }*/
}
#endif //SETTINGS_H_
