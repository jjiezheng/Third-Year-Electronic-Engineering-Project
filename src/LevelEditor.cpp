#include "CoreEngine.h"
#include "Coords.h"
#include "Entities.h"
#include "pixelstorm.h"

#include "Player.h"
#include "Menu.h"
#include "LevelEditor.h"
#include "SpatialHash.h"

#include "MainMenu.h"


using namespace de::classes;
using namespace de::core;
using namespace de::enums;
using namespace de::theme;
using namespace de::time;


using namespace Attrition::builder;
using namespace Attrition;

//Constructor
LevelEditor::LevelEditor():
    pause(false), showHitboxes(false), collisions(true),
    explosion(0), particleAmount(0), shipAmount(0), bulletAmount(0), oneOverTicks(0),
    Player_1( "Player1" ), currentMenu(editorMenu::LEVEL),
    levelBuilderButtonPress(false), shipBuilderButtonPress(false), bulletBuilderButtonPress(false), backgroundBuilderButtonPress(false)
{
    SpatialHash::init();
    CollisionText.set( "Collisions ", "GeoSansLight", Vector(50, 550) );

    Player_1_UI = new UI( Player_1.getStatsPointer() );

    Vector position(50, 10), buttonOffset(100, 0);
    builderMenu.add( new UIDataBoolButton( "Level",      &levelBuilderButtonPress,      position + buttonOffset*0, 11, ALIGN_CENTRE, true ) );
    builderMenu.add( new UIDataBoolButton( "Ship",       &shipBuilderButtonPress,       position + buttonOffset*1, 11, ALIGN_CENTRE, true ) );
    builderMenu.add( new UIDataBoolButton( "Bullet",     &bulletBuilderButtonPress,     position + buttonOffset*2, 11, ALIGN_CENTRE, true ) );
    builderMenu.add( new UIDataBoolButton( "Background", &backgroundBuilderButtonPress, position + buttonOffset*3, 11, ALIGN_CENTRE, true ) );
}

//Destructor
LevelEditor::~LevelEditor()
{
    if( Player_1_UI )    { delete Player_1_UI;}
}


//Handles all user input
bool LevelEditor::handleEvents( const SDL_Event &_event )
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


    //levelBuilderButton.handleEvent ( _event );
    builderMenu.handleEvent( _event );


    if( currentMenu == editorMenu::LEVEL )
    {
        levelBuilder.handleEvent ( _event );
    }


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
bool LevelEditor::logic( const Uint32 &_deltaTicks, de::state::State* &_nextState, de::state::options &_options )
{/*
    static bool lolol(true);

    if(lolol)
    {
        Particles.push_back( new Particle( particleManager.get( "Explosion" ), Vector( 300,300 ) ) );
        Particles.push_back( new Particle( particleManager.get( "Explosion" ), Vector( 200,300 ), Vector( 200,300 ) ) );
        lolol = false;
    }*/

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
        //levelBuilderButton.logic( _deltaTicks );
        builderMenu.logic( _deltaTicks );
        logicParticles( _deltaTicks );

        if(levelBuilderButtonPress)
        {
            levelBuilderButtonPress = false;
            currentMenu = editorMenu::LEVEL;
            de::io::log << "currentMenu = editorMenu::LEVEL\n";
        }
        if(shipBuilderButtonPress)
        {
            Ships.clear();
            enemyBullets.clear();
            shipBuilderButtonPress = false;
            currentMenu = editorMenu::SHIP;
        }
        if(bulletBuilderButtonPress)
        {
            Ships.clear();
            enemyBullets.clear();
            bulletBuilderButtonPress = false;
            currentMenu = editorMenu::BULLET;
        }
        if(backgroundBuilderButtonPress)
        {
            Ships.clear();
            enemyBullets.clear();
            backgroundBuilderButtonPress = false;
            currentMenu = editorMenu::BACKGROUND;
        }


        if( currentMenu == editorMenu::LEVEL )
        {
            levelBuilder.logic( _deltaTicks, Ships, enemyBullets );


            //background.logic( _deltaTicks );
            ShipTracker::clear();
            Player_1.logic( _deltaTicks, playerBullets, Particles );
            ShipTracker::addPlayer( Player_1.getPosition(), Player_1.getId() );

/*
            if( collisions )
                checkCollisions();

            SpatialHash::finishedFrame();
            logicShips( _deltaTicks );
            logicBullets( _deltaTicks );
            logicParticles( _deltaTicks );*/
        }
        else if( currentMenu == editorMenu::SHIP )
        {

        }
    }

    return false;
}


//Renders to screen
void LevelEditor::render()
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

    //Rendering Particles----------------------------------
    for ( iterParticles = Particles.begin(); iterParticles < Particles.end(); ++iterParticles )
        iterParticles->render( FBO_GAME );
    //Rendering Particles----------------------------------


    //LevelControl.render();
    builderMenu.render( FBO_UI, FBO_AFTER );

    if( currentMenu == editorMenu::LEVEL )
    {
        levelBuilder.render( FBO_UI, FBO_AFTER );

        Player_1.render( showHitboxes );
        Player_1_UI->render();
        //background.render();
    }
    //Extra------------------------------------------------
}


//Reloads all textures when going in and out of fullscreen because openGL losese its context
 void LevelEditor::reLoadTextures()
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

    //background.reLoadTextures();
    Player_1.reLoadTextures();
    Player_1_UI->reLoadTextures();
    LevelControl.reLoadTextures();
    levelBuilder.reLoadTextures();

    builderMenu.reLoadTextures();
}



//Checks for collisions
inline void LevelEditor::checkCollisions()
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
inline void LevelEditor::logicShips( const Uint32 &_deltaTicks )
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
inline void LevelEditor::logicBullets( const Uint32 &_deltaTicks )
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
    //__gnu_parallel::for_each( playerBullets.begin(), playerBullets.end(), iterBullets->logic( oneOverTicks, deltaTicks ) );

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
inline void LevelEditor::logicParticles( const Uint32 &_deltaTicks )
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


