#ifndef VERTICAL_SHOOTER_STATE_H
#define VERTICAL_SHOOTER_STATE_H

#include "CoreEngine.h"
#include "Coords.h"
#include "Entities.h"
#include "UI.h"

#include "backround.h"

#include "Menu.h"
#include "Levelscontroller.h"
#include "State.h"



class Player;


class VerticalShooter : public de::state::State
{
    public:
        VerticalShooter();
        ~VerticalShooter();

        bool handleEvents( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks, de::state::State* &_nextState, de::state::options &_options );
        void render();

    private:
        void reLoadTextures();
        void checkCollisions();
        void logicShips( const Uint32 &_deltaTicks );
        void logicBullets( const Uint32 &_deltaTicks );
        void logicParticles( const Uint32 &_deltaTicks );


    private:
        bool pause, showHitboxes, collisions;
        int explosion;
        int particleAmount, shipAmount, bulletAmount;
        float oneOverTicks, deltaTicks;
        platformstl::performance_counter    PCounter;

        de::classes::SpriteString CollisionText, Presents;

        Attrition::Player Player_1;
        UI* Player_1_UI;
        Backround backround;
        LevelsController LevelControl;

    private:
        //Vectors
        //ptr_vectors are used for polymorphism
        boost::ptr_vector<Attrition::Ship> Ships;
        boost::ptr_vector<Attrition::Ship>::iterator iterShips;
        /*
        boost::ptr_vector<Entity> Ships;
        boost::ptr_vector<Entity>::iterator iterShips;
*/
        boost::ptr_vector<Attrition::Bullet> enemyBullets, playerBullets;
        boost::ptr_vector<Attrition::Bullet>::iterator iterBullets;

        boost::ptr_vector<Attrition::Particle> Particles;
        boost::ptr_vector<Attrition::Particle>::iterator iterParticles;
};


#endif //VERTICAL_SHOOTER_STATE_H
