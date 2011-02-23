#ifndef LEVEL_EDITOR_STATE_H
#define LEVEL_EDITOR_STATE_H

#include "CoreEngine.h"
#include "Coords.h"
#include "Entities.h"
#include "UI.h"

#include "backround.h"


#include "Menu.h"
#include "Levelscontroller.h"
#include "State.h"
#include "LevelBuilder.h"
#include "ShipBuilder.h"



namespace de
{
    namespace enums
    {
        namespace editorMenu
        {
            enum EditorMenu
            {
                LEVEL = 0,
                SHIP = 1,
                BULLET = 2,
                BACKGROUND = 3
            };
        }
    }
}


class LevelEditor : public de::state::State
{
    public:
        LevelEditor();
        ~LevelEditor();

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
        float oneOverTicks;


        platformstl::performance_counter    PCounter;
        de::classes::SpriteString CollisionText, Presents;

        Attrition::Player Player_1;
        UI* Player_1_UI;
        //Backround background;
        LevelsController LevelControl;
        Attrition::builder::LevelBuilder levelBuilder;
        Attrition::builder::ShipBuilder shipBuilder;

        de::enums::editorMenu::EditorMenu currentMenu;
        bool levelBuilderButtonPress, shipBuilderButtonPress, bulletBuilderButtonPress, backgroundBuilderButtonPress;
        UIContainer builderMenu;

    private:
        boost::ptr_vector<Attrition::Ship> Ships;
        boost::ptr_vector<Attrition::Ship>::iterator iterShips;

        boost::ptr_vector<Attrition::Bullet> enemyBullets, playerBullets;
        boost::ptr_vector<Attrition::Bullet>::iterator iterBullets;

        boost::ptr_vector<Attrition::Particle> Particles;
        boost::ptr_vector<Attrition::Particle>::iterator iterParticles;
};



#endif //LEVEL_EDITOR_STATE_H

