#ifndef UISPLINE_H
#define UISPLINE_H


#include "CoreEngine.h"
#include "Coords.h"


class UIspline
{
    public:
        UIspline();
        UIspline( std::vector<de::classes::Vector> *_VectorPointsPtr );
        ~UIspline();

        void setPoints( std::vector<de::classes::Vector> *_VectorPointsPtr );

        void handleEvent( const SDL_Event &_event );
        bool logic( Uint32 _deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _fbo );

    private:
        bool pointerNotSet, draggingEvent, addEvent, deleteEvent, hoverEvent;

        de::classes::Vector mousePosition;
        de::classes::Poly boxLocation;
        de::classes::Line line;

        std::vector<de::classes::LinedBox> BoxPoints;
        std::vector<de::classes::LinedBox>::iterator iterLinedBox, activeBox;

        std::vector<de::classes::Vector> *VectorPointsPtr;
        std::vector<de::classes::Vector>::iterator iterVector, activePoint;



    private:
        void getSplinePath( de::classes::Line &_path, std::vector<de::classes::Vector> &_Points );

};

#endif // UISPLINE_H
