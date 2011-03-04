#ifndef LEVELCONTROLLER_H
#define LEVELCONTROLLER_H

#include "Ships.h"
#include "CoreEngine.h"




class LevelStart
{
    private:
        bool start;
        int currentNumber;
        float currentTime;
        de::classes::SpriteString level, levelNumber, levelName, levelSubText;
    public:
        LevelStart();
        ~LevelStart(){};
    public:
        bool logic( const Uint32 &deltaTicks, const de::classes::LevelInfo &currentLevel );
        void reLoadTextures();
        void render();
};


class LevelEnd
{
    private:
        bool moveToNextLevel, readyToMoveOn, start;
        int currentNumber;
        float currentTime;
        de::classes::SpriteString level, score, rankText, rank, pressButton;
        de::classes::Sprite fullScreenFade;

    public:
        LevelEnd();
        ~LevelEnd(){};

    public:
        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &deltaTicks, const de::classes::LevelInfo &currentLevel );
        void reLoadTextures();
        void render();
};




class LevelsController
{
    public:
        LevelsController();
        ~LevelsController(){};
    public:
        void handleEvent( const SDL_Event &_event );
        void logic( const Uint32 &deltaTicks, boost::ptr_vector<Attrition::Ship> &_Ships );
        void reLoadTextures();
        void render();

        const std::string& getTime();



    private:
        bool levelEnded, levelStarting, levelIsRunning;
        int currentMusic;

        std::vector<de::classes::WaveInfo>::iterator iterWave;
        std::vector<de::classes::LevelInfo>::iterator currentLevel;
        std::vector<de::classes::LevelInfo> levelSettings;

    private:
        de::classes::SpriteString Score, currentState;
        de::classes::Sprite fadeScreen;
        de::time::TimeCounter time;

        LevelStart levelStart;
        LevelEnd levelEnd;

    private:
        void runShipWaves( boost::ptr_vector<Attrition::Ship> &_Ships );
        void generateWave( boost::ptr_vector<Attrition::Ship> &_Ships );

};


#endif //LEVELCONTROLLER_H

