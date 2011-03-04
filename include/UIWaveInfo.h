#ifndef UIWAVEINFO_H
#define UIWAVEINFO_H

#include "CoreEngine.h"
#include "Coords.h"
#include "UIbaseData.h"

class UIWaveInfo
{
    public:
        UIWaveInfo();
        virtual ~UIWaveInfo();
        void set( de::classes::WaveInfo *_wave,
                  const std::string &_wavesName,
                  bool &_addWave,
                  bool &_deleteWave,
                  const de::classes::Vector &_position,
                  const de::classes::Vector &_offset );

        void handleEvent( const SDL_Event &_event );
        bool logic( Uint32 _deltaTicks, bool &_resize );
        void reLoadTextures();
        void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 );
    protected:
    private:
        void setVectorPoints();

        std::vector<std::string> formationNames;
        std::vector<int> formationNumbers;
        std::vector<std::string> ShipOptions;

        de::classes::WaveInfo *wavePtr;
        de::classes::SpriteString title;
        de::classes::Vector offset, position;

        UIContainer wayPoints, waveUI;
};

#endif // UIWAVEINFO_H
