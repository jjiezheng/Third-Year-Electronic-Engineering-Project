#include "UILevelInfo.h"

using namespace de::classes;
using namespace de::core;
using namespace de::enums;
using namespace de::theme;

UILevelInfo::UILevelInfo()
{
    levelText.set( "Level Information", "System", Vector( 200, 50 ), 11, ALIGN_CENTRE );
    levelText.setColour( DefaultTextColour );
}

UILevelInfo::~UILevelInfo()
{
    //dtor
}


void UILevelInfo::set( LevelInfo *_level,
                       const std::string &_levelsName,
                       bool &_save,
                       bool &_load,
                       const Vector &_position,
                       const Vector &_offset  )
{
    Vector buttonOffset(50,0);
    levelPtr = _level;

    levelUI.clear();

    levelUI.add( new UIDataEventButtonBF( _levelsName, events::LEVEL_BACK, events::LEVEL_FORWARD, _position + _offset*1, 11, ALIGN_CENTRE ) );
    levelUI.add( new UIDataInt( "Time:",     &levelPtr->time,   _position + _offset*2, 11, ALIGN_LEFT ) );
    levelUI.add( new UIDataString( "Name:",     &levelPtr->name,   _position + _offset*3, 11, ALIGN_LEFT ) );
    levelUI.add( new UIDataString( "Music:",   &levelPtr->music,  _position + _offset*4, 11, ALIGN_LEFT ) );

    levelUI.add( new UIDataBoolButton( "Save", &_save, _position + _offset*5 - buttonOffset, 11, ALIGN_CENTRE ) );
    levelUI.add( new UIDataBoolButton( "Load", &_load, _position + _offset*5 + buttonOffset, 11, ALIGN_CENTRE ) );
}


void UILevelInfo::handleEvent( const SDL_Event &_event )
{
    levelUI.handleEvent( _event );
}


bool UILevelInfo::logic( Uint32 _deltaTicks )
{
    return levelUI.logic( _deltaTicks );
}


void UILevelInfo::reLoadTextures()
{
    levelUI.reLoadTextures();
    levelText.reload();
}


void UILevelInfo::render( fboTypes _fbo, fboTypes _fbo2 )
{
    levelText.render( _fbo2 );
    levelUI.render( _fbo, _fbo2 );
}

