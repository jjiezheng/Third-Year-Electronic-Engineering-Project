#ifndef UISLIDER_H
#define UISLIDER_H


#include "CoreEngine.h"
#include "Coords.h"


class UISlider
{
    public:
        UISlider();
        virtual ~UISlider();

        void set( const std::string &_name,
                  int *_value,
                  int *_max,
                  const int &_size,
                  const de::classes::Vector &_position,
                  const int &_priority );

        void handleEvent( const SDL_Event &_event );
        void logic( Uint32 _deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 );
    protected:
    private:
        de::classes::Vector mousePosition;
        de::classes::Poly slider;
        de::classes::Line line;
        de::classes::SpriteString name, start, end, here;

        bool pointerNotSet, hoverEvent, draggingEvent;
        int *value, *maxValue;
        int localValue;
        float startPoint, endPoint, length;
};

#endif // UISLIDER_H
