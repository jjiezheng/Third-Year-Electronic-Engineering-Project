#include "CoreEngine.h"
#include "Coords.h"
#include "Entities.h"
#include "pixelstorm.h"

#include "Player.h"
#include "Menu.h"
#include "VerticalShooter.h"
#include "SpatialHash.h"

#include "MainMenu.h"

using namespace de::classes;
using namespace de::core;
using namespace de::enums;

using namespace Attrition;


//Constructor
VerticalShooter::VerticalShooter():
    pause(false), showHitboxes(false), collisions(true),
    explosion(0), particleAmount(0), shipAmount(0), bulletAmount(0),
    oneOverTicks(0), Player_1( "Player1" )
{
    SpatialHash::init();

    explosion = de::Engine::Resources().getSoundEffect( "Explosion" );

    Player_1_UI = new UI( Player_1.getStatsPointer() );

    CollisionText.set( "Collisions ", "GeoSansLight", Vector(50, 550) );
}

//Destructor
VerticalShooter::~VerticalShooter()
{
    if( Player_1_UI )    { delete Player_1_UI;}
}


//Handles all user input
bool VerticalShooter::handleEvents( const SDL_Event &_event )
{
    if( _event.type == SDL_USEREVENT )
    {
        if( _event.user.code == events::OPENGL_RELOAD )
        {
            reLoadTextures();
        }
    }


    Player_1.handleEvent ( _event );
    Player_1_UI->handleEvent ( _event, pause );
    LevelControl.handleEvent ( _event );

    if ( _event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        if ( _event.key.keysym.sym )
        {
            if ( _event.key.keysym.sym == SDLK_p )
                de::Engine::Audio().pauseMusic();
            if ( _event.key.keysym.sym == SDLK_LCTRL )
                showHitboxes = !showHitboxes;
            if ( _event.key.keysym.sym == SDLK_LSHIFT )
                collisions = !collisions;
        }
    }

    return true;
}


//Logic
bool VerticalShooter::logic( const Uint32 &_deltaTicks, de::state::State* &_nextState, de::state::options &_options )
{
    if( Player_1_UI->logic( LevelControl.getTime(), pause ) )
    {
        _nextState = new MainMenu();
    }

    if( pause )
    {
        //Mix_PauseMusic();
    }
    else
    {
        oneOverTicks = _deltaTicks/1000.f;

        //Mix_ResumeMusic();

        LevelControl.logic( _deltaTicks, Ships );

        //Move the Backround and player ship
        backround.logic( _deltaTicks );

        ShipTracker::clear();

        Player_1.logic( _deltaTicks, playerBullets, Particles );
        ShipTracker::addPlayer( Player_1.getPosition(), Player_1.getId() );

        PCounter.start();
        if( collisions )
            checkCollisions();
        SpatialHash::finishedFrame();
        PCounter.stop();
        //unsigned int timeCollisions = PCounter.get_microseconds();


        //Move all other Ships, bullets and particles
        PCounter.start();
        logicShips( _deltaTicks );
        PCounter.stop();
        //unsigned int timeShips = PCounter.get_microseconds();

        PCounter.start();
        logicBullets( _deltaTicks );
        PCounter.stop();
        //unsigned int timeBullets = PCounter.get_microseconds();

        PCounter.start();
        logicParticles( _deltaTicks );
        PCounter.stop();
        //unsigned int timeParticles = PCounter.get_microseconds();

    }
    return false;
}


//Renders to screen
void VerticalShooter::render()
{
    //Rendering Bullets----------------------------------------------------------------------
    for ( iterBullets = playerBullets.begin(); iterBullets < playerBullets.end(); ++iterBullets )
        iterBullets->render( FBO_GAME, showHitboxes );

    for ( iterBullets = enemyBullets.begin(); iterBullets < enemyBullets.end(); ++iterBullets )
        iterBullets->render( FBO_GAME, showHitboxes );
    //Rendering Bullets----------------------------------------------------------------------

    //Rendering Ships-------------------------------------
    for ( iterShips = Ships.begin(); iterShips < Ships.end(); ++iterShips )
        iterShips->render( FBO_GAME, showHitboxes );
    //Rendering Ships-------------------------------------

    //Player 1 hitbox and sprite here---------------------
    Player_1.render( showHitboxes );
    //-----------------------------------------------------

    //Rendering Particles----------------------------------
    for ( iterParticles = Particles.begin(); iterParticles < Particles.end(); ++iterParticles )
        iterParticles->render( FBO_GAME );
    //Rendering Particles----------------------------------

    //Extra------------------------------------------------
    Player_1_UI->render();
    backround.render();
    LevelControl.render();
    //Extra------------------------------------------------
}


//Reloads all textures when going in and out of fullscreen because openGL losese its context
 void VerticalShooter::reLoadTextures()
{
    for ( iterBullets = playerBullets.begin(); iterBullets < playerBullets.end(); ++iterBullets )
    {
        iterBullets->reLoad();
    }
    for ( iterBullets = enemyBullets.begin(); iterBullets < enemyBullets.end(); ++iterBullets )
    {
        iterBullets->reLoad();
    }

    for ( iterShips = Ships.begin(); iterShips < Ships.end(); ++iterShips )
    {
        iterShips->reLoad();
    }

    for ( iterParticles = Particles.begin(); iterParticles < Particles.end(); ++iterParticles )
    {
        iterParticles->reLoad();
    }

    backround.reLoadTextures();
    Player_1.reLoadTextures();
    Player_1_UI->reLoadTextures();
    LevelControl.reLoadTextures();

}




//Checks for collisions
inline void VerticalShooter::checkCollisions()
{/*
     //Checking for bullet collisions against Player_1
    Player_1->checkCollisions( enemyBullets, Particles );

    //Checking for ship collisions against Player_1
    Player_1->checkCollisions( Ships, Particles );

    //Checking for bullet collisions against Enemy Ships
    for ( iterShips = Ships.begin(); iterShips < Ships.end(); ++iterShips )
    {
        iterShips->checkCollisions( playerBullets, Particles );
    }*/
}


//Logic for Ships
//Moves Ships that are alive and delete the ones that aren't
inline void VerticalShooter::logicShips( const Uint32 &_deltaTicks )
{
    shipAmount = 0;
    iterShips = Ships.begin();
    while ( iterShips != Ships.end() )
    {

        if( iterShips->isAlive( Particles ) )
        {
            iterShips->logic( _deltaTicks, enemyBullets, Particles );
            ShipTracker::add( iterShips->getPosition(), iterShips->getId() );
            ++shipAmount;
            ++iterShips;
        }
        else
        {
            iterShips = Ships.erase( iterShips );
        }
    }
}

//Logic for Bullets
//Moves Bullets that are alive and delete the ones that aren't
inline void VerticalShooter::logicBullets( const Uint32 &_deltaTicks )
{
    float oneOverTicks = (float)_deltaTicks/1000.f;

    bulletAmount = 0;
    iterBullets = playerBullets.begin();
    while ( iterBullets != playerBullets.end() )
    {

        if( iterBullets->isAlive() )
        {
            iterBullets->logic( oneOverTicks, _deltaTicks );
            ++bulletAmount;
            ++iterBullets;
        }
        else
        {
            iterBullets = playerBullets.erase( iterBullets );
        }
    }

    iterBullets = enemyBullets.begin();
    while ( iterBullets != enemyBullets.end() )
    {

        if( iterBullets->isAlive() )
        {
            iterBullets->logic( oneOverTicks, _deltaTicks );
            ++bulletAmount;
            ++iterBullets;
        }
        else
        {
            iterBullets = enemyBullets.erase( iterBullets );
        }
    }
}


//Logic for Particles
//Moves Particles that are alive and delete the ones that aren't
inline void VerticalShooter::logicParticles( const Uint32 &_deltaTicks )
{
    particleAmount = 0;
    iterParticles = Particles.begin();
    while ( iterParticles != Particles.end() )
    {
        iterParticles->logic( _deltaTicks );

        if( iterParticles->isAlive() )
        {
            ++particleAmount;
            ++iterParticles;
        }
        else
            iterParticles = Particles.erase( iterParticles );
    }
}
