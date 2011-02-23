#include "Levelscontroller.h"

#include "ShipFactory.h"

using namespace de::classes;
using namespace de::core;
using namespace de::graphics;
using namespace de::misc;
using namespace de::theme;
using namespace de::enums;




LevelStart::LevelStart()
    :start(true), currentNumber(0), currentTime(0)
{
    level.setColour( DefaultTextColour );
    levelNumber.setColour( DefaultTextColour );

    levelName.setColour( DefaultTextColour );
    levelSubText.setColour( DefaultTextColour );
/*
    level.setAlpha( 0 );
    levelNumber.setAlpha( 0 );
    levelName.setAlpha( 0 );*/
}

bool LevelStart::logic( const Uint32 &deltaTicks, const LevelInfo &_currentLevel )
{
    if( start )
    {
        start = !start;
        level.set( "LEVEL", "UnsteadyLevel", Vector( 140,180 ), 10 );
        levelNumber.set( "1", "UnsteadyNumbers", Vector( 492,1 ), 10 );

        levelName.set( "", "UnsteadySmall", Vector( 148, 315 ), 10 );
        levelSubText.set( "", "UnsteadySmall", Vector( 65, 540 ), 10 );
    }

    if( currentNumber != _currentLevel.number )
    {
        currentNumber = _currentLevel.number;
        levelNumber = toString( currentNumber );

        levelName = _currentLevel.name;
        levelSubText = _currentLevel.subText;
        currentTime = 0;


        int currentMusic = de::Engine::Resources().getMusic( _currentLevel.music );
        de::Engine::Audio().playMusic( currentMusic );
        de::Engine::Audio().setMusicVolume( 1.0 );
    }


    currentTime += deltaTicks;

    if( currentTime < 2000 )
    {
        level.setAlpha( 0 );
        levelNumber.setAlpha( 0 );
        levelName.setAlpha( 0 );
    }

    else if( currentTime < 4000 && currentTime > 2000 )
    {
        float alpha =  ( (currentTime-2000)/2000 );

        level.setAlpha( alpha );
        levelNumber.setAlpha( alpha );
        levelName.setAlpha( alpha );
    }

    else if( currentTime < 7000 && currentTime > 5000 )
    {
        float alpha = ( 1.0 - ( (currentTime-5000)/2000 ) );

        level.setAlpha( alpha );
        levelNumber.setAlpha( alpha );
        levelName.setAlpha( alpha );
    }


    else if( currentTime > 7000 )
    {
        return false;
    }

    return true;
}

void LevelStart::reLoadTextures()
{
    level.reload();
    levelNumber.reload();

    levelName.reload();
    levelSubText.reload();
}

void LevelStart::render()
{
    level.render( FBO_UI );
    levelNumber.render( FBO_UI );

    levelName.render( FBO_UI );
}




LevelEnd::LevelEnd()
    :start(true), currentNumber(0), currentTime(0)
{
    Frect rect;
    rect.x = 1280/2;
    rect.y = 800/2;
    rect.w = 1280;
    rect.h = 800;

    fullScreenFade.setShader( "Untextured" );
    fullScreenFade.coords.Setup( rect, 0 );

    fullScreenFade.colour.r = 0.482353;
    fullScreenFade.colour.g = 0.137255;
    fullScreenFade.colour.b = 0.137255;
    fullScreenFade.colour.a = 0.0;
    fullScreenFade.priority = 11;

    level.setColour( DefaultTextColour );
    score.setColour( DefaultTextColour );
    rank.setColour( DefaultTextColour );
    rankText.setColour( DefaultTextColour );
    pressButton.setColour( DefaultTextColour );

    level.setAlpha( 0.0 );
    score.setAlpha( 0.0 );
    rank.setAlpha( 0.0 );
    rankText.setAlpha( 0.0 );
    pressButton.setAlpha( 0.0 );

    moveToNextLevel = false;
    readyToMoveOn = false;
}



void LevelEnd::handleEvent( const SDL_Event &_event )
{
    if( readyToMoveOn )
    {
        if ( _event.type == SDL_KEYDOWN )
        {
            if ( _event.key.keysym.sym )
            {
                moveToNextLevel = true;
            }
        }
    }
}

bool LevelEnd::logic( const Uint32 &deltaTicks, const LevelInfo &_currentLevel )
{
    if( start )
    {
        start = !start;
        level.set( "", "Unsteady", Vector( 430,300 ), 12 );
        score.set( "", "Unsteady", Vector( 430,1 ), 12 );
        rank.set( "", "UnsteadyLevel", Vector( 780,350 ), 12 );
        rankText.set( "YOUR RANK IS", "Unsteady", Vector( 330,400 ), 12 );
        pressButton.set( "press any key to continue", "UnsteadySmall", Vector( 440,500 ), 12 );
    }
    if( currentNumber != _currentLevel.number )
    {
        currentNumber = _currentLevel.number;
        currentTime = 0;
        moveToNextLevel = false;
        readyToMoveOn = false;

        level = "LEVEL " + toString( currentNumber ) + " FINISHED";
        score = "00001";
        rank = "B++";


        level.setAlpha( 0.0 );
        score.setAlpha( 0.0 );
        rank.setAlpha( 0.0 );
        rankText.setAlpha( 0.0 );
        pressButton.setAlpha( 0.0 );
    }
    currentTime += deltaTicks;


    if( fullScreenFade.colour.a < 1.0 )
        fullScreenFade.colour.a = currentTime/1200;


    if( currentTime > 1200 && currentTime < 2400 )
    {
        float alpha = (currentTime-1200)/1200;

        level.setAlpha( alpha );
        score.setAlpha( alpha );
        rank.setAlpha( alpha );
        rankText.setAlpha( alpha );
        pressButton.setAlpha( alpha );
    }


    if( currentTime > 2500 )
    {
        readyToMoveOn = true;
    }

    return moveToNextLevel;
}

void LevelEnd::reLoadTextures()
{
    fullScreenFade.setShader( "Untextured" );;

    pressButton.reload();
    level.reload();
    score.reload();
    rank.reload();
    rankText.reload();
}

void LevelEnd::render()
{
    /*de::Engine::Graphics().add( fullScreenFade, FBO_UI );*/

    pressButton.render( FBO_UI );
    level.render( FBO_UI );
    rankText.render( FBO_UI );
    rank.render( FBO_UI );
}






LevelsController::LevelsController()
{

    //levelSettings = Settings::getAllLevelSettings();
    currentLevel = levelSettings.begin();

    currentState.set( "", "Unsteady", Vector( 65, 640 ), 10 );
    currentState.setColour( DefaultTextColour );


    float screenWidth = 1280;
    float screenHeight = 800;

    Frect rect;
    rect.x = screenWidth / 2;
    rect.y = screenHeight / 2;
    rect.w = screenWidth;
    rect.h = screenHeight;

    fadeScreen.coords.Setup( rect, 0 );
    fadeScreen.priority = 11;

    fadeScreen.setShader( "Untextured" );


    levelEnded = false;
    levelStarting = true;
    levelIsRunning = true;
    currentMusic = 0;
}


void LevelsController::handleEvent( const SDL_Event &_event )
{
    if( levelEnded )
    {
        levelEnd.handleEvent( _event );
    }
}


void LevelsController::runShipWaves( boost::ptr_vector<Attrition::Ship> &_Ships )
{
    for ( iterWave = currentLevel->Waves.begin(); iterWave < currentLevel->Waves.end(); ++iterWave )
    {
        if( time.getMilliSeconds() > (unsigned) iterWave->time )
        {
            generateWave( _Ships );
            iterWave = currentLevel->Waves.erase( iterWave );
        }
    }
}


const std::string& LevelsController::getTime()
{
    return time.getTimeString();
}

void LevelsController::logic( const Uint32 &deltaTicks, boost::ptr_vector<Attrition::Ship> &_Ships )
{
    if( levelIsRunning )
    {
        //If the level is starting
        if( levelStarting )
        {
            levelStarting = levelStart.logic( deltaTicks, (*currentLevel) );
            currentState = "LEVEL " + toString( currentLevel->number ) + " STARTING";
        }



        //If the level is running
        else if( time.getMilliSeconds() < (unsigned)currentLevel->time )
        {
            time.addMilliSeconds( deltaTicks );
            runShipWaves( _Ships );

            currentState = "LEVEL " + toString( currentLevel->number ) + " RUNNING";
        }



        //If the level is finished
        else
        {
            levelEnded = true;
            bool moveToNextLevel = levelEnd.logic( deltaTicks, (*currentLevel) );

            currentState = "LEVEL " + toString( currentLevel->number ) + " ENDED";

            if( moveToNextLevel )
            {
                ++currentLevel;
                if( currentLevel == levelSettings.end() )
                {
                    levelStarting = false;
                    levelIsRunning = false;

                    currentState = "GAME ENDED";
                }
                else
                {
                    time.reset();
                    levelStarting = true;
                    levelEnded = false;

                    //currentState = "LEVEL " + intToString( currentLevel->number ) + " ENDED";
                }
            }
        }
    }
}


void LevelsController::reLoadTextures()
{
    fadeScreen.setShader( "Untextured" );

    Score.reload();
    currentState.reload();

    levelStart.reLoadTextures();
    levelEnd.reLoadTextures();
}


void LevelsController::render()
{
    currentState.render( FBO_UI );
    if( levelEnded )
    {
        levelEnd.render();
        Score.render( FBO_UI );
    }
    if( levelStarting )
    {
        levelStart.render();
    }
}


void LevelsController::generateWave( boost::ptr_vector<Attrition::Ship> &_Ships )
{
    float distanceToEdge = ( iterWave->gap * ( iterWave->number - 1 ) ) / 2;

    std::vector<Vector>::iterator iterPoints = iterWave->Points.begin();
    boost::ptr_vector<Attrition::Ship>::iterator iterShips;

    float i;
    for( i = 0; i < iterWave->number; ++i )
    {
        float gap(i*iterWave->gap);

        //_Ships.push_back( new Attrition::Ship( iterWave ) );
        _Ships.push_back( new Attrition::Ship( Attrition::shipManager.get( iterWave->shipType ) ) );
        iterShips = _Ships.end() - 1;

        iterShips->loadSettings( iterWave );

        if( iterWave->formation == ARROW )
        {
            if( i <  iterWave->number / 2  )
            {
                Vector offset( gap - distanceToEdge, gap - distanceToEdge);
                iterShips->setOffset( offset ) ;
            }
            else
            {
                Vector offset( gap - distanceToEdge, distanceToEdge - gap );
                iterShips->setOffset( offset );
            }
        }
        else if( iterWave->formation == VERTICAL_LINE )
        {
            Vector offset( 0, gap - distanceToEdge );
            iterShips->setOffset( offset );
        }
        else if( iterWave->formation == HORIZONAL_LINE )
        {
            Vector offset( gap -  distanceToEdge, 0 );
            iterShips->setOffset( offset );
        }
    }
}

