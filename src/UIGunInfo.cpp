#include "UIGunInfo.h"

#include "BulletFactory.h"


using namespace de::classes;
using namespace de::core;
using namespace de::theme;
using namespace de::misc;
using namespace de::enums;

UIGunInfo::UIGunInfo() :
    addFiring(false), deleteFiring(false), addWeapon(false), deleteWeapon(false), playPattern(false)
{
    typeNumbers.push_back(SIMPLE);
    typeNumbers.push_back(SIMPLE_TARGETING);
    typeNumbers.push_back(TARGETING);
    typeNumbers.push_back(TRACKING);
    //typeNumbers.push_back(LASER);

    typeNames.push_back( "Simple" );
    typeNames.push_back( "Simple Targeting" );
    typeNames.push_back( "Targeting" );
    typeNames.push_back( "Tracking" );
    //typeNames.push_back( "Laser" );


    shapeNumbers.push_back(LINE);
    shapeNumbers.push_back(CIRCLE);
    shapeNumbers.push_back(CIRCLES2);

    shapeNames.push_back( "Line" );
    shapeNames.push_back( "Circle" );
    shapeNames.push_back( "Circles" );
}

UIGunInfo::~UIGunInfo()
{
    //dtor
}


void UIGunInfo::set( de::classes::WaveInfo *_wave, const de::classes::Vector &_position, const de::classes::Vector &_offset )
{
    wavePtr = _wave;

    position = _position;
    offset = _offset;

    if( wavePtr->bulletWave.empty() )
        wavePtr->bulletWave.push_back(BulletWaveInfo());

    if( wavePtr->bulletTimes.empty() )
        wavePtr->bulletTimes.push_back(BulletTimes());


    iterBulletWave = wavePtr->bulletWave.begin();
    iterBulletTimes = wavePtr->bulletTimes.begin();
    weaponNumber = firingNumber = 1;


    refreshUI();
}

void UIGunInfo::refreshUI()
{
    Vector buttonOffset(50,0);

    iterBulletWave = wavePtr->bulletWave.begin();
    iterBulletTimes = wavePtr->bulletTimes.begin();

    for( int i(0); i < weaponNumber-1; ++i )
        ++iterBulletWave;

    for( int i(0); i < firingNumber-1; ++i )
        ++iterBulletTimes;

    bulletTitle.set( "Bullet Information", "System", position, 11, ALIGN_CENTRE );
    bulletTitle.setColour( DefaultTextColour );

    int size( wavePtr->bulletWave.size() );
    std::string weaponsName( toString( weaponNumber ) + " of " + toString( size ) );

    gunUI.clear();

    gunUI.add( new UIDataEventButtonBF( weaponsName, events::WEAPON_BACK, events::WEAPON_FORWARD, position + offset, 11, ALIGN_CENTRE ) );
    gunUI.add( new UIDataBoolButton( "Add Weapon", &addWeapon, position - buttonOffset + offset*2, 11, ALIGN_CENTRE ) );
    gunUI.add( new UIDataBoolButton( "Delete Weapon",&deleteWeapon, position + buttonOffset + offset*2,11, ALIGN_CENTRE ) );

    gunUI.add( new UIDataString( "Bullet Name:", &iterBulletWave->bulletName,
                                                 Attrition::bulletManager.bulletNames(),
                                                 Attrition::bulletManager.bulletNames(),     position + offset*3,  11, ALIGN_LEFT, true ) );

    gunUI.add( new UIDataEnum( "Bullet Type:", &iterBulletWave->bulletType,
                                               typeNumbers,
                                               typeNames,                      position + offset*4,  11, ALIGN_LEFT, true ) );

    gunUI.add( new UIDataEnum( "Firing Pattern:", &iterBulletWave->firingPattern,
                                                  shapeNumbers,
                                                  shapeNames,                      position + offset*5,  11, ALIGN_LEFT, true ) );

    gunUI.add( new UIDataInt( "Number of Bullets:", &iterBulletWave->bulletNumber, position + offset*7, 11, ALIGN_LEFT, true ) );
    gunUI.add( new UIDataInt( "Reload Time:", &iterBulletWave->bulletReloadTime, position + offset*8, 11, ALIGN_LEFT, true ) );

    gunUI.add( new UIDataVector( "Speed:", &iterBulletWave->bulletSpeed, position + offset*9,  11, ALIGN_LEFT, true ) );
    gunUI.add( new UIDataVector( "Gap:", &iterBulletWave->bulletGap, position + offset*10, 11, ALIGN_LEFT, true ) );
    gunUI.add( new UIDataFloat( "Rotation:", &iterBulletWave->bulletRotation, position + offset*11, 11, ALIGN_LEFT, true ) );
    gunUI.add( new UIDataFloat( "Arc Size:", &iterBulletWave->arcSize, position + offset*12, 11, ALIGN_LEFT, true ) );


    firingTitle.set( "Firing Information", "System", position + offset*14, 11, ALIGN_CENTRE );
    firingTitle.setColour( DefaultTextColour );

    size = wavePtr->bulletTimes.size();
    std::string firingName( toString( firingNumber ) + " of " + toString( size ) );


    gunUI.add( new UIDataEventButtonBF( firingName, events::FIRING_BACK, events::FIRING_FORWARD, position + offset*15, 11, ALIGN_CENTRE ) );
    gunUI.add( new UIDataBoolButton( "Add Firing", &addFiring, position + offset*16 - buttonOffset, 11, ALIGN_CENTRE ) );
    gunUI.add( new UIDataBoolButton( "Delete Firing",&deleteFiring, position + offset*16 + buttonOffset, 11, ALIGN_CENTRE ) );

    gunUI.add( new UIDataInt( "Start:", &iterBulletTimes->start, position + offset*17, 11, ALIGN_LEFT, true ) );
    gunUI.add( new UIDataInt( "Duration:", &iterBulletTimes->duration, position + offset*18, 11, ALIGN_LEFT, true ) );
    gunUI.add( new UIDataInt( "Weapon Used:",  &iterBulletTimes->weaponUsed, position + offset*19, 11, ALIGN_LEFT, true ) );
}



void UIGunInfo::localHandleEvent( const SDL_Event &_event )
{
    if( _event.type == SDL_USEREVENT )
    {
        if( _event.user.code == events::WEAPON_BACK )
        {
            if( iterBulletWave != ( wavePtr->bulletWave.begin() ) )
            {
                --iterBulletWave;
                --weaponNumber;

                refreshUI();
            }
        }

        else if( _event.user.code == events::WEAPON_FORWARD )
        {
            if( iterBulletWave != ( wavePtr->bulletWave.end() - 1 ) )
            {
                ++iterBulletWave;
                ++weaponNumber;

                refreshUI();
            }
        }

        else if( _event.user.code == events::FIRING_BACK )
        {
            if( iterBulletTimes != ( wavePtr->bulletTimes.begin() ) )
            {
                --iterBulletTimes;
                --firingNumber;

                refreshUI();
            }
        }

        else if( _event.user.code == events::FIRING_FORWARD )
        {
            if( iterBulletTimes != ( wavePtr->bulletTimes.end() - 1 ) )
            {
                ++iterBulletTimes;
                ++firingNumber;

                refreshUI();
            }
        }
    }
}

void UIGunInfo::handleEvent( const SDL_Event &_event )
{
    localHandleEvent( _event );

    gunUI.handleEvent( _event );
}


bool UIGunInfo::localLogic( Uint32 _deltaTicks )
{
    bool wasThereChange(false);
    if( addFiring )
    {
        addFiring = false;
        wasThereChange = true;

        wavePtr->bulletTimes.push_back(BulletTimes());

        iterBulletTimes = wavePtr->bulletTimes.end()  - 1;
        firingNumber = wavePtr->bulletTimes.size();

        refreshUI();
    }

    if( deleteFiring )
    {
        deleteFiring = false;
        wasThereChange = true;

        if( wavePtr->bulletTimes.size() > 1 )
        {
            iterBulletTimes = wavePtr->bulletTimes.erase( iterBulletTimes );
            --firingNumber;

            refreshUI();
        }
    }


    if( addWeapon )
    {
        addWeapon = false;
        wasThereChange = true;

        wavePtr->bulletWave.push_back(BulletWaveInfo());

        iterBulletWave = wavePtr->bulletWave.end();
        weaponNumber = wavePtr->bulletWave.size() - 1;

        refreshUI();
    }

    if( deleteWeapon )
    {
        deleteWeapon = false;
        wasThereChange = true;

        if( wavePtr->bulletWave.size() > 1 )
        {
            iterBulletWave = wavePtr->bulletWave.erase( iterBulletWave );
            --weaponNumber;

            refreshUI();
        }
    }
    return wasThereChange;
}


bool UIGunInfo::logic( Uint32 _deltaTicks )
{
    bool wasThereChange(false);

    wasThereChange =
        localLogic( _deltaTicks ) ||
        gunUI.logic( _deltaTicks );

    return wasThereChange;
}


void UIGunInfo::reLoadTextures()
{
    bulletTitle.reload();
    firingTitle.reload();

    gunUI.reLoadTextures();
}


void UIGunInfo::render( fboTypes _fbo, fboTypes _fbo2 )
{
    bulletTitle.render( _fbo2 );
    firingTitle.render( _fbo2 );


    gunUI.render( _fbo, _fbo2 );
}
