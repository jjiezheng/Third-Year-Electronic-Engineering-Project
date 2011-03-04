#ifndef LEVELBUILDER_H
#define LEVELBUILDER_H

#include "Coords.h"
#include "CoreEngine.h"
#include "Entities.h"

#include "UIWaveInfo.h"
#include "UIGunInfo.h"
#include "UILevelInfo.h"

#include "UIspline.h"
#include "UISlider.h"
#include "Shipspawn.h"

namespace Attrition
{
    namespace builder
    {
        class LevelBuilder
        {
            public:
                LevelBuilder();
                ~LevelBuilder();

                void handleEvent( const SDL_Event &_event );
                void logic( const Uint32 &deltaTicks,
                            boost::ptr_vector<Attrition::Ship> &_Ships,
                            boost::ptr_vector<Bullet> &_Bullets );
                void reLoadTextures();
                void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 );

            private:
                bool refreshUI();

                void addingWave();
                void deletingWave();

                void loadLevelSettings();
                void saveLevelSettings();

                int levelNumber, waveNumber;
                bool addWave, deleteWave, save, load, waveBack, waveForward, levelBackward, levelForward;

                std::vector<de::classes::WaveInfo>::iterator iterWave;
                std::vector<de::classes::LevelInfo>::iterator iterLevel;
                std::vector<de::classes::LevelInfo> levelSettings;


                UILevelInfo levelInfo;
                UIWaveInfo shipInfo;
                UIGunInfo gunInfo;

                UIspline shipPath;
                UISlider slider;
                ShipSpawn ShipSpawner;
                int currentTime;
        };
    }
}




#endif // LEVELBUILDER_H
