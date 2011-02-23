#ifndef UILEVELINFO_H
#define UILEVELINFO_H

#include "CoreEngine.h"
#include "Coords.h"
#include "UIbaseData.h"


class UILevelInfo
{
    public:
        UILevelInfo();
        ~UILevelInfo();
        void set( de::classes::LevelInfo *_level,
                  const std::string &_levelsName,
                  bool &_save,
                  bool &_load,
                  const de::classes::Vector &_position,
                  const de::classes::Vector &_offset );

        void handleEvent( const SDL_Event &_event );
        bool logic( Uint32 _deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 );

    private:
        de::classes::LevelInfo *levelPtr;
        de::classes::SpriteString levelText;
        UIContainer levelUI;
};

#endif // UILEVELINFO_H
