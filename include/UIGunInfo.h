#ifndef UIGUNINFO_H
#define UIGUNINFO_H

#include "CoreEngine.h"
#include "Coords.h"
#include "UIbaseData.h"


class UIGunInfo
{
    public:
        UIGunInfo();
        virtual ~UIGunInfo();
        void set( de::classes::WaveInfo *_wave, const de::classes::Vector &_position, const de::classes::Vector &_offset );

        void handleEvent( const SDL_Event &_event );
        bool logic( Uint32 _deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 );
    protected:
    private:
        void localHandleEvent( const SDL_Event &_event );
        bool localLogic( Uint32 _deltaTicks );
        void refreshUI();


        bool addFiring, deleteFiring, addWeapon, deleteWeapon, playPattern;
        int weaponNumber, firingNumber;

        std::vector<de::classes::BulletWaveInfo>::iterator iterBulletWave;
        std::vector<de::classes::BulletTimes>::iterator iterBulletTimes;

        de::classes::WaveInfo *wavePtr;
        de::classes::SpriteString bulletTitle, firingTitle;
        de::classes::Vector position, offset;

        UIContainer gunUI;

        std::vector<std::string> typeNames, shapeNames;
        std::vector<int> typeNumbers, shapeNumbers;
};

#endif // UIGUNINFO_H
