#include "Settings.h"


using namespace de::classes;
using namespace de::misc;
using namespace de::enums;
/*
namespace de
{
    namespace core
    {
        CFG_File Settings::config;
        bool Settings::unloaded;
        std::vector<LevelInfo> Settings::levelSettings;




        void Settings::Init()
        {
            unloaded = true;
            loadData();
        }



        const std::vector<LevelInfo>& Settings::getAllLevelSettings()
        {
            return levelSettings;
        }

        void Settings::saveLevelSettings( const std::vector<de::classes::LevelInfo> &_levelSettings )
        {
            levelSettings = _levelSettings;
        }



        LevelInfo Settings::getLevelSettings( const std::string &name )
        {
            std::vector<LevelInfo>::iterator iter;
            for( iter = levelSettings.begin(); iter < levelSettings.end(); ++iter )
            {
                if( iter->name == name )
                    return (*iter);
            }
            return (*iter);
        }



        bool Settings::loadData()
        {
            de::io::log << "Loading settings now...  ";
            if( unloaded )
            {
                if( loadLevelData() )
                {
                    de::io::log << "Done\n";
                    unloaded = false;
                    return true;
                }
                else
                {
                    de::io::log << "Failed\n";
                }
            }
            return false;
        }

        bool Settings::reloadData()
        {
            if( !saveData() )
                return false;

            if( !unloadData() )
                return false;

            if( !loadData() )
                return false;

            return true;
        }

        bool Settings::unloadData()
        {
            levelSettings.clear();

            unloaded = true;

            return true;
        }

        bool Settings::saveData()
        {
            saveLevelData();

            return true;
        }

        bool Settings::saveLevelData()
        {
            int result = CFG_OpenFile( ( BasicSettings::rootPrefix + "config/levels.cfg" ).c_str(), &config );
            if ( CFG_OK != result )
            {
                de::io::error << "Unrecoverable Failure to load the levels config file\n";
                return false;
            }

            std::vector<LevelInfo>::iterator iterLevel;
            for( iterLevel = levelSettings.begin(); iterLevel < levelSettings.end(); ++iterLevel )
            {
                if ( CFG_OK == CFG_SelectGroup( (iterLevel->groupName).c_str(), CFG_False ) )
                {
                    CFG_WriteText( "name", (iterLevel->name).c_str() );
                    CFG_WriteText( "subText", (iterLevel->subText).c_str() );
                    CFG_WriteText( "music", (iterLevel->music).c_str() );
                    CFG_WriteInt( "time", iterLevel->time );
                    CFG_WriteInt( "number", iterLevel->number );

                    CFG_WriteText( "wave", (iterLevel->actualWavePath).c_str() );
                    CFG_WriteText( "background", (iterLevel->actualBackPath).c_str() );
                }
            }

            CFG_SaveFile( ( BasicSettings::rootPrefix + "config/levels.cfg" ).c_str(), CFG_SORT_TYPE_CONTENT, CFG_SPACE_ENTRIES );
            CFG_CloseFile( &config );

            for( iterLevel = levelSettings.begin(); iterLevel < levelSettings.end(); ++iterLevel )
            {
                saveWaveData( iterLevel, iterLevel->actualWavePath );
            }

            return true;
        }

        bool Settings::saveWaveData( std::vector<LevelInfo>::iterator &_iterLevel, const std::string &_levelPath )
        {
            int result = CFG_OpenFile( ( BasicSettings::rootPrefix + _levelPath ).c_str(), &config );
            CFG_ClearFile();
            if ( CFG_OK != result )
            {
                de::io::error << "Unrecoverable Failure to load the Wave config file\n";
                de::io::error << "File path was " << ( BasicSettings::rootPrefix + _levelPath ).c_str() << "\n";
            }

            std::vector<WaveInfo>::iterator iterWave;
            for( iterWave = _iterLevel->Waves.begin(); iterWave < _iterLevel->Waves.end(); ++iterWave )
            {
                if ( CFG_SelectGroup( (iterWave->name).c_str(), CFG_True ) )
                {
                    CFG_WriteText( "ship", (iterWave->shipType).c_str() );
                    CFG_WriteInt( "number", iterWave->number );
                    CFG_WriteInt( "time", iterWave->time );

                    CFG_WriteText( "formation", (getFormationTypeText(iterWave->formation)).c_str() );
                    CFG_WriteBool( "rotate", iterWave->rotate );
                    CFG_WriteBool( "aligned", iterWave->aligned );
                    CFG_WriteFloat( "gap", iterWave->gap );

                    int i(0);
                    std::vector<Vector>::iterator iterVector;
                    for( iterVector = iterWave->Points.begin(), i = 0; iterVector < iterWave->Points.end(); ++iterVector, ++i )
                    {
                        std::string pointName = "point_" + toString( i );

                        if( !CFG_IsEntryMultiValue( pointName.c_str() ) )
                        {
                            CFG_WriteInt( pointName.c_str(), 0 );
                            CFG_AddMultiValueToInt( pointName.c_str(), 0 );
                        }

                        CFG_StartMultiValueEntryIteration( pointName.c_str() );
                        CFG_WriteInt( CFG_MULTI_VALUE_ENTRY_ITERATION, iterVector->x );

                        CFG_SelectNextMultiValueEntry();
                        CFG_WriteInt( CFG_MULTI_VALUE_ENTRY_ITERATION, iterVector->y );
                    }

                    std::vector<BulletTimes>::iterator iterBulletTimes;
                    for( iterBulletTimes = iterWave->bulletTimes.begin(), i = 0; iterBulletTimes < iterWave->bulletTimes.end(); ++iterBulletTimes, ++i )
                    {
                        std::string bulletName = "bullet_" + toString( i );

                        if( !CFG_IsEntryMultiValue( bulletName.c_str() ) )
                        {
                            CFG_WriteInt( bulletName.c_str(), 0 );
                            CFG_AddMultiValueToInt( bulletName.c_str(), 0 );
                            CFG_AddMultiValueToInt( bulletName.c_str(), 0 );
                        }

                        CFG_StartMultiValueEntryIteration( bulletName.c_str() );
                        CFG_WriteInt( CFG_MULTI_VALUE_ENTRY_ITERATION, iterBulletTimes->start );

                        CFG_SelectNextMultiValueEntry();
                        CFG_WriteInt( CFG_MULTI_VALUE_ENTRY_ITERATION, iterBulletTimes->duration );

                        CFG_SelectNextMultiValueEntry();
                        CFG_WriteInt( CFG_MULTI_VALUE_ENTRY_ITERATION, iterBulletTimes->weaponUsed );

                    }


                    std::vector<BulletWaveInfo>::iterator iterBulletWave;
                    for( iterBulletWave = iterWave->bulletWave.begin(), i = 0; iterBulletWave < iterWave->bulletWave.end(); ++iterBulletWave, ++i )
                    {
                        CFG_WriteText( ("bullet_type_" + toString( i )).c_str(), (getBulletTypeText( iterBulletWave->bulletType )).c_str() );
                        CFG_WriteText( ("firing_pattern_" + toString( i )).c_str(), (getFiringPatternText( iterBulletWave->firingPattern )).c_str() );
                        CFG_WriteText( ("bullet_name_" + toString( i )).c_str(), (iterBulletWave->bulletName).c_str() );

                        CFG_WriteInt( ("bullet_number_" + toString( i )).c_str(), iterBulletWave->bulletNumber );
                        CFG_WriteInt( ("bullet_reload_" + toString( i )).c_str(), iterBulletWave->bulletReloadTime );
                        CFG_WriteFloat( ("bullet_rotation_" + toString( i )).c_str(), iterBulletWave->bulletRotation );
                        CFG_WriteFloat( ("bullet_circle_arc_" + toString( i )).c_str(), iterBulletWave->arcSize );


                        if( !CFG_IsEntryMultiValue( ( "bullet_gap_" + toString( i ) ).c_str() ) )
                        {
                            CFG_WriteInt( ( "bullet_gap_" + toString( i ) ).c_str(), 0 );
                            CFG_AddMultiValueToInt( ( "bullet_gap_" + toString( i ) ).c_str(), 0 );
                        }

                        CFG_StartMultiValueEntryIteration( ( "bullet_gap_" + toString( i ) ).c_str() );
                        CFG_WriteFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, iterBulletWave->bulletGap.x );

                        CFG_SelectNextMultiValueEntry();
                        CFG_WriteFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, iterBulletWave->bulletGap.y );



                        if( !CFG_IsEntryMultiValue( ( "bullet_speed_" + toString( i ) ).c_str() ) )
                        {
                            CFG_WriteInt( ( "bullet_speed_" + toString( i ) ).c_str(), 0 );
                            CFG_AddMultiValueToInt( ( "bullet_speed_" + toString( i ) ).c_str(), 0 );
                        }

                        CFG_StartMultiValueEntryIteration( ( "bullet_speed_" + toString( i ) ).c_str() );
                        CFG_WriteFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, iterBulletWave->bulletSpeed.x );

                        CFG_SelectNextMultiValueEntry();
                        CFG_WriteFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, iterBulletWave->bulletSpeed.y );



                    }
                }
            }

            CFG_SaveFile( ( BasicSettings::rootPrefix + _levelPath ).c_str(), CFG_SORT_TYPE_CONTENT, CFG_SPACE_ENTRIES );
            CFG_CloseFile( &config );
            return true;
        }




        inline std::vector<WaveInfo> Settings::loadWaveData( const std::string &_levelPath )
        {
            int result = CFG_OpenFile( ( BasicSettings::rootPrefix + _levelPath ).c_str(), &config );
            if ( CFG_OK != result )
            {
                de::io::error << "Unrecoverable Failure to load the Wave config file" << "\n";
                de::io::error << "File path was " << ( BasicSettings::rootPrefix + _levelPath ).c_str() << "\n";
            }

            std::vector<WaveInfo> waveSettings;

            for ( CFG_StartGroupIteration(); !CFG_IsLastGroup(); CFG_SelectNextGroup() )
            {
                WaveInfo buffer;
                BulletWaveInfo bufferBullet;
                BulletTimes timeBuffer;
                bool finished;
                int bulletNumber, bulletFireNumber, pointNumber;


                buffer.name = CFG_GetSelectedGroupName();


                buffer.shipType = CFG_ReadText( "ship", "Dasein" );
                buffer.number = CFG_ReadInt( "number", 1 );
                buffer.time = CFG_ReadInt( "time", 0 );

                buffer.formation = getFormationType();
                buffer.rotate = CFG_ReadBool( "rotate", "true" );
                buffer.aligned = CFG_ReadBool( "aligned", "true" );
                buffer.gap = CFG_ReadFloat( "gap", 0 );


                finished = false;
                bulletNumber = 0;
                std::string bulletName;
                while( !finished )
                {
                    bulletName = "bullet_type_" + toString( bulletNumber );

                    if ( CFG_OK == CFG_TextEntryExists( bulletName.c_str() ) )
                    {
                        bufferBullet.bulletType = getBulletType( bulletNumber );
                        bufferBullet.firingPattern = getFiringPattern( bulletNumber );
                        bufferBullet.bulletName = CFG_ReadText( ( "bullet_name_" + toString( bulletNumber ) ).c_str(), "simple" );

                        bufferBullet.bulletNumber = CFG_ReadInt( ( "bullet_number_" + toString( bulletNumber ) ).c_str(), 0 );
                        bufferBullet.bulletReloadTime = CFG_ReadInt( ( "bullet_reload_" + toString( bulletNumber ) ).c_str(), 0 );
                        bufferBullet.bulletRotation = CFG_ReadFloat( ( "bullet_rotation_" + toString( bulletNumber ) ).c_str(), 0.1 );
                        bufferBullet.arcSize = CFG_ReadFloat( ( "bullet_circle_arc_" + toString( bulletNumber ) ).c_str(), 2 );


                        if ( CFG_OK == CFG_FloatEntryExists( ( "bullet_gap_" + toString( bulletNumber ) ).c_str() ) )
                        {
                            CFG_StartMultiValueEntryIteration( ( "bullet_gap_" + toString( bulletNumber ) ).c_str() ) ;
                            bufferBullet.bulletGap.x = CFG_ReadFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, 0.1 );

                            CFG_SelectNextMultiValueEntry();
                            bufferBullet.bulletGap.y = CFG_ReadFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, 0.1 );
                        }
                        else if ( CFG_OK == CFG_IntEntryExists( ( "bullet_gap_" + toString( bulletNumber ) ).c_str() ) )
                        {
                            CFG_StartMultiValueEntryIteration( ( "bullet_gap_" + toString( bulletNumber ) ).c_str() ) ;
                            bufferBullet.bulletGap.x = CFG_ReadFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, 0.1 );

                            CFG_SelectNextMultiValueEntry();
                            bufferBullet.bulletGap.y = CFG_ReadFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, 0.1 );
                        }


                        if ( CFG_OK == CFG_FloatEntryExists( ( "bullet_speed_" + toString( bulletNumber ) ).c_str() ) )
                        {
                            CFG_StartMultiValueEntryIteration( ( "bullet_speed_" + toString( bulletNumber ) ).c_str() ) ;
                            bufferBullet.bulletSpeed.x = CFG_ReadFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, 0.1 );

                            CFG_SelectNextMultiValueEntry();
                            bufferBullet.bulletSpeed.y = CFG_ReadFloat( CFG_MULTI_VALUE_ENTRY_ITERATION, 0.1 );
                        }

                        buffer.bulletWave.push_back( bufferBullet );

                    }
                    else
                        finished = true;

                    ++bulletNumber;
                }

                finished = false;
                bulletFireNumber = 0;
                while( !finished )
                {
                    bulletName = "bullet_" + toString( bulletFireNumber );
                    if ( CFG_OK == CFG_IntEntryExists( bulletName.c_str() ) )
                    {
                        CFG_StartMultiValueEntryIteration( bulletName.c_str() ) ;
                        timeBuffer.start = CFG_ReadInt( CFG_MULTI_VALUE_ENTRY_ITERATION, 0 );

                        CFG_SelectNextMultiValueEntry();
                        timeBuffer.duration = CFG_ReadInt( CFG_MULTI_VALUE_ENTRY_ITERATION, 0 );

                        CFG_SelectNextMultiValueEntry();
                        timeBuffer.weaponUsed = CFG_ReadInt( CFG_MULTI_VALUE_ENTRY_ITERATION, 0 );

                        buffer.bulletTimes.push_back( timeBuffer );
                    }
                    else
                        finished = true;

                    ++bulletFireNumber;
                }

                pointNumber = 0;
                finished = false;
                std::string pointName;
                while( !finished )
                {
                    pointName = "point_" + toString( pointNumber );

                    if ( CFG_OK == CFG_IntEntryExists( pointName.c_str() ) )
                    {
                        Vector pointBuffer;

                        CFG_StartMultiValueEntryIteration( pointName.c_str() ) ;
                        pointBuffer.x = CFG_ReadInt( CFG_MULTI_VALUE_ENTRY_ITERATION, 0 );

                        CFG_SelectNextMultiValueEntry();
                        pointBuffer.y = CFG_ReadInt( CFG_MULTI_VALUE_ENTRY_ITERATION, 0 );

                        buffer.Points.push_back( pointBuffer );

                    }
                    else
                        finished = true;

                    ++pointNumber;
                }


                waveSettings.push_back( buffer );
            }

            int CFG_CloseFile( CFG_File *config = CFG_SELECTED );

            return waveSettings;
        }

        inline bool Settings::loadLevelData()
        {
            int result = CFG_OpenFile( ( BasicSettings::rootPrefix + "config/levels.cfg" ).c_str(), &config );
            if ( CFG_OK != result )
            {
                de::io::error << "Unrecoverable Failure to load the levels config file\n";
                return false;
            }

            LevelInfo buffer;

            for ( CFG_StartGroupIteration(); !CFG_IsLastGroup(); CFG_SelectNextGroup() )
            {
                buffer.groupName = CFG_GetSelectedGroupName();


                buffer.name = CFG_ReadText( "name", "NONE" );
                buffer.subText = CFG_ReadText( "subText", "NONE" );

                buffer.wavePath = CFG_ReadText( "wave", "NONE" );
                buffer.actualWavePath = CFG_ReadText( "wave", "NONE" );

                buffer.backPath = BasicSettings::rootPrefix + CFG_ReadText( "background", "NONE" );
                buffer.actualBackPath = CFG_ReadText( "background", "NONE" );

                buffer.music = CFG_ReadText( "music", "NONE" );
                buffer.time = CFG_ReadInt( "time", 0 );


                levelSettings.push_back( buffer );
            }

            std::vector<LevelInfo>::iterator iter;

            unsigned int currentNumber = 1;
            for( iter = levelSettings.begin(); iter < levelSettings.end(); ++iter )
            {
                iter->number = currentNumber;
                iter->Waves = loadWaveData( iter->wavePath );
                ++currentNumber;
            }

            return true;
        }



        int Settings::getFormationType()
        {
            std::string type = CFG_ReadText( "formation", "basic" );

            if( type == "Horizontal Line" )
                return HORIZONAL_LINE;
            if( type == "Vertical Line" )
                return VERTICAL_LINE;
            if( type == "Arrow" )
                return ARROW;

            else return HORIZONAL_LINE;
        }

        std::string Settings::getFormationTypeText( int _formation )
        {
            if( _formation == HORIZONAL_LINE )
                return "Horizontal Line";
            if( _formation == VERTICAL_LINE )
                return "Vertical Line";
            if( _formation == ARROW )
                return "Arrow";
            else return "Horizontal Line";
        }



        int Settings::getBulletType( const int &_bulletNumber )
        {
            std::string type = CFG_ReadText( ( "bullet_type_" + toString( _bulletNumber ) ).c_str(), "simple" );

            if( type == "simple" )
                return SIMPLE;
            if( type == "simple_targeting" )
                return SIMPLE_TARGETING;
            if( type == "targeting" )
                return TARGETING;
            if( type == "tracking" )
                return TRACKING;
            if( type == "laser" )
                return LASER;

            else return SIMPLE;
        }

        std::string Settings::getBulletTypeText( int _BulletType )
        {
            if( _BulletType == SIMPLE )
                return "simple";
            if( _BulletType == SIMPLE_TARGETING )
                return "simple_targeting";
            if( _BulletType == TARGETING )
                return "targeting";
            if( _BulletType == TRACKING )
                return "tracking";
            if( _BulletType == LASER )
                return "laser";

            else return "simple";

        }

        int Settings::getFiringPattern( const int &_bulletNumber )
        {
            std::string type = CFG_ReadText( ( "firing_pattern_" + toString( _bulletNumber ) ).c_str(), "simple" );

            if( type == "line" )
                return LINE;
            if( type == "circle" )
                return CIRCLE;
            if( type == "circle2" )
                return CIRCLES2;
            if( type == "tracking" )
                return TRACKING;
            if( type == "laser" )
                return LASER;

            else return LINE;
        }

        std::string Settings::getFiringPatternText( int _firingPattern )
        {

            if( _firingPattern == LINE )
                return "line";
            if( _firingPattern == CIRCLE )
                return "circle";
            if( _firingPattern == CIRCLES2 )
                return "circle2";
            if( _firingPattern == TRACKING )
                return "tracking";
            if( _firingPattern == LASER )
                return "laser";


            else return "line";

        }
    }
}
*/
