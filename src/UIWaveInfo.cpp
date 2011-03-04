#include "UIWaveInfo.h"

#include "Entities.h"



using namespace de::classes;
using namespace de::core;
using namespace de::theme;
using namespace de::misc;
using namespace de::enums;

UIWaveInfo::UIWaveInfo()
{
    formationNumbers.push_back(HORIZONAL_LINE);
    formationNumbers.push_back(VERTICAL_LINE);
    formationNumbers.push_back(ARROW);

    formationNames.push_back( "Horizontal Line" );
    formationNames.push_back( "Vertical Line" );
    formationNames.push_back( "Arrow" );
}

UIWaveInfo::~UIWaveInfo()
{
    //dtor
}



void UIWaveInfo::set( WaveInfo *_wave,
                      const std::string &_wavesName,
                      bool &_addWave,
                      bool &_deleteWave,
                      const Vector &_position,
                      const Vector &_offset  )
{
    Vector buttonOffset(50,0);

    wavePtr = _wave;
    position = _position;
    offset = _offset;

    title.set( "Wave Information", "System", _position, 11, ALIGN_CENTRE );
    title.setColour( DefaultTextColour );

    waveUI.clear();


    waveUI.add( new UIDataEventButtonBF( _wavesName, events::WAVE_BACK, events::WAVE_FORWARD, position + offset, 11, ALIGN_CENTRE ) );

    waveUI.add( new UIDataBoolButton( "Add Wave",    &_addWave,    position + offset*2 - buttonOffset, 11, ALIGN_CENTRE ) );
    waveUI.add( new UIDataBoolButton( "Delete Wave", &_deleteWave, position + offset*2 + buttonOffset, 11, ALIGN_CENTRE ) );

    waveUI.add( new UIDataString( "Name:",        &wavePtr->name,                     _position + offset*3,     11, ALIGN_LEFT, false ) );
    waveUI.add( new UIDataString( "Ship Name:",   &wavePtr->shipType,
                                                  Attrition::shipManager.shipNames(),
                                                  Attrition::shipManager.shipNames(), _position + offset*4,   11, ALIGN_LEFT, true ) );

    waveUI.add( new UIDataEnum( "Formation:",   &wavePtr->formation,
                                                formationNumbers,
                                                formationNames,             _position + offset*5,   11, ALIGN_LEFT, true ) );

    waveUI.add( new UIDataInt( "Number:",      &wavePtr->number,           _position + offset*6,   11, ALIGN_LEFT, true ) );
    waveUI.add( new UIDataFloat( "Gap:",       &wavePtr->gap,              _position + offset*7,   11, ALIGN_LEFT, true ) );
    waveUI.add( new UIDataInt( "Time:",        &wavePtr->time,             _position + offset*8,   11, ALIGN_LEFT, true ) );

    waveUI.add( new UIDataBool( "Rotate:",      &wavePtr->rotate,           _position + offset*9,   11, ALIGN_LEFT, true ) );
    waveUI.add( new UIDataBool( "Aligned:",     &wavePtr->aligned,          _position + offset*10,   11, ALIGN_LEFT, true ) );


    setVectorPoints();
}



void UIWaveInfo::setVectorPoints()
{
    wayPoints.clear();
    std::vector<Vector>::iterator iterVector;

    int i(0);
    for( iterVector = wavePtr->Points.begin(), i = 0;
         iterVector < wavePtr->Points.end();
         ++iterVector, ++i )
    {
        wayPoints.add( new UIDataVector( "Point:" + toString(i), &(*iterVector), position + offset*(12+i) - Vector(50,0), 11, ALIGN_LEFT ) );
    }
}



void UIWaveInfo::handleEvent( const SDL_Event &_event )
{
    waveUI.handleEvent( _event );
    wayPoints.handleEvent( _event );
}


bool UIWaveInfo::logic( Uint32 _deltaTicks, bool &_resize )
{
    bool wasThereChange(false);

    wasThereChange = waveUI.logic( _deltaTicks );


    _resize = false;
    if( wayPoints.size() != wavePtr->Points.size() )
    {
        setVectorPoints();
        _resize = true;
    }

    wasThereChange = wasThereChange | wayPoints.logic( _deltaTicks );

    return wasThereChange;
}


void UIWaveInfo::reLoadTextures()
{
    title.reload();
    waveUI.reLoadTextures();
    wayPoints.reLoadTextures();
}


void UIWaveInfo::render( fboTypes _fbo, fboTypes _fbo2 )
{
    title.render( _fbo2 );

    waveUI.render( _fbo, _fbo2 );
    wayPoints.render( _fbo, _fbo2 );
}

