#include "SpriteGroup.h"

#include <algorithm>

using namespace de::classes;

namespace de
{
    namespace graphics
    {

        SpriteGroup SpriteGroup::operator= ( const SpriteGroup &_other )
        {
            theActiveGroup = _other.theActiveGroup;
            theActiveSprite = _other.theActiveSprite;

            centre = _other.centre;
            last = _other.last;
            rotation = _other.rotation;
            first = _other.first;

            Sprites = _other.Sprites;

            if( !theActiveGroup.empty() )
            {
                currentlySelectedGroup = Sprites.find( theActiveGroup );
            }
            else
            {
                currentlySelectedGroup = Sprites.begin();
            }

            if( !theActiveSprite.empty() )
            {
                currentlySelected = currentlySelectedGroup->second.find( theActiveSprite );
            }
            else
            {
                currentlySelected = currentlySelectedGroup->second.begin();
            }

            groupsIter = Sprites.begin();
            groupsEnd = Sprites.end();

            spritesIter = currentlySelectedGroup->second.begin();
            spritesEnd = currentlySelectedGroup->second.end();

            return *this;
        }





        SpriteGroup::SpriteGroup() :
            last( 0, 1 ), rotation(0), first(true)
        {
            //ctor
        }

        SpriteGroup::~SpriteGroup()
        {
            //dtor
        }


    bool SpriteGroup::parseSprites( const luabind::object &_SpriteTable )
    {
        if( luabind::type(_SpriteTable) != LUA_TTABLE )
        {
            return false;
        }

        for(luabind::iterator i(_SpriteTable), end; i != end; ++i)
        {
            if( luabind::type((*i)) != LUA_TTABLE )
            {
                return false;
            }
            parseSingleSpriteGroup( (*i), luabind::object_cast<std::string>( i.key() ) );
        }

        luabind::iterator i(_SpriteTable);
        activeGroup( luabind::object_cast<std::string>( i.key() ) );
        logic(0);

        return true;
    }

    bool SpriteGroup::parseSingleSpriteGroup( const luabind::object &_SpriteGroup, const std::string &_groupName )
    {
        for(luabind::iterator i(_SpriteGroup), end; i != end; ++i)
        {
            parseSingleSprite( (*i), luabind::object_cast<std::string>( i.key() ), _groupName );
        }
        return true;
    }

    bool SpriteGroup::parseSingleSprite( const luabind::object &_Sprite, const std::string &_spriteName, const std::string &_groupName )
    {
        insert( _spriteName, _groupName );

        try
        {
            current().setGraphics( luabind::object_cast<std::string>( _Sprite["Texture"] ),
                                           luabind::object_cast<std::string>( _Sprite["Shader"] ) );

            current().priority( luabind::object_cast<int>( _Sprite["priority"] ) );


            current().setAnimation( luabind::object_cast<std::string>( _Sprite["AnimationType"] ),
                                            luabind::object_cast<int>( _Sprite["timeBetweenFrames"] ) );

            current().size( luabind::object_cast<int>( _Sprite["w"] ),
                                    luabind::object_cast<int>( _Sprite["h"] ) );

            current().coords().setCentre( Vector( luabind::object_cast<int>( _Sprite["x"] ),
                                                          luabind::object_cast<int>( _Sprite["y"] ) ) );

            std::vector<std::string> frames;
            for(luabind::iterator i(_Sprite["sprites"]), end; i != end; ++i)
            {
                frames.push_back( luabind::object_cast<std::string>( (*i) ) );
            }

            sort( frames.begin(), frames.end() );
            current().addFrames( frames );

        }
        catch( luabind::error &_e )
        {
            std::string error = lua_tostring( _e.state(), -1 );
            de::io::error << _e.what() << "\n";
            de::io::error << error << "\n";
        }
        catch( luabind::cast_failed  &_e )
        {
            std::string error1 = lua_tostring( _e.state(), -1 );
            std::string error2 = lua_tostring( _e.state(), 0 );
            de::io::error << _e.what() << " - " << error1 << "|" << error2 << "\n";
        }
        return true;
    }








        bool SpriteGroup::insert( const std::string &_name, const std::string &_set )
        {
            theActiveGroup = _set;
            theActiveSprite = _name;

            Sprites[ _set ][ _name ] = AnimatedSprite();

            currentlySelectedGroup = Sprites.find( _set );
            currentlySelected = currentlySelectedGroup->second.find( _name );

            return true;
        }

        bool SpriteGroup::selected( const std::string &_name, const std::string &_set )
        {
            theActiveGroup = _set;
            theActiveSprite = _name;

            groupsIter = Sprites.find( _set );
            if( groupsIter == Sprites.end() )
            {
                de::io::error << "Failure to find SpriteGroup with the name " << _set << "\n";
                return false;
            }

            spritesIter = groupsIter->second.find( _name );
            if( spritesIter == groupsIter->second.end() )
            {
                de::io::error << "Failure to find Sprite with the name " << _name << "\n";
                return false;
            }

            currentlySelectedGroup = groupsIter;
            currentlySelected = spritesIter;
            return true;
        }

        bool SpriteGroup::activeGroup( const std::string &_set )
        {
            theActiveGroup = _set;

            Vector thecentre( centre );
            float theRotation( rotation );


            moveBy( -thecentre );
            rotateBy( -theRotation );

            groupsIter = Sprites.find( _set );
            if( groupsIter == Sprites.end() )
            {
                de::io::error << "Failure to find SpriteGroup with the name " << _set << "\n";
                return false;
            }

            currentlySelectedGroup = groupsIter;
            currentlySelected = currentlySelectedGroup->second.begin();

            moveBy( thecentre );
            rotateBy( theRotation );



            return true;
        }

        AnimatedSprite& SpriteGroup::current()
        {
            if( Sprites.empty() )
            {
                de::io::error << "Sprites is empty in AnimatedSprite\n";
                static AnimatedSprite blank;
                return blank;
            }

            return currentlySelected->second;
        }


        void SpriteGroup::moveBy( const Vector &_movement, bool _moveFromOrign )
        {
            groupsEnd = Sprites.end();
            spritesEnd = currentlySelectedGroup->second.end();

            Vector totalMovement(0,0);

            if( _moveFromOrign )
            {
                totalMovement = _movement - centre;
                centre = _movement;
            }
            else
            {
                totalMovement = _movement;
                centre += _movement;
            }

            for( spritesIter = currentlySelectedGroup->second.begin(); spritesIter != spritesEnd; ++spritesIter )
            {
                spritesIter->second.moveBy( totalMovement );
            }
        }

        void SpriteGroup::rotateBy( const float &_rotation )
        {
            rotation += _rotation;
            for( spritesIter = currentlySelectedGroup->second.begin(); spritesIter != spritesEnd; ++spritesIter )
            {
                spritesIter->second.coords().rotateFrom( _rotation, centre );
            }
        }

        void SpriteGroup::moveRotateBy( const Vector &_movement, bool _moveFromOrign )
        {
            if( _movement.x != 0 || _movement.y != 0 )
            {
                moveBy( _movement, _moveFromOrign );

                Vector normalised = _movement/_movement.length();

                float cos = last*normalised;
                float sin = last^normalised;

                float rotationAmount = acos( cos )/misc::piDegrees();
                rotation = rotationAmount;

                last = normalised;


                for( spritesIter = currentlySelectedGroup->second.begin(); spritesIter != spritesEnd; ++spritesIter )
                {
                    spritesIter->second.coords().rotateFrom( cos, sin, centre );
                }
            }
        }


        void SpriteGroup::logic( const Uint32 &_deltaticks )
        {
            groupsEnd = Sprites.end();
            spritesEnd = currentlySelectedGroup->second.end();

            for( spritesIter = currentlySelectedGroup->second.begin(); spritesIter != spritesEnd; ++spritesIter )
            {
                spritesIter->second.logic( _deltaticks );
            }
        }

        void SpriteGroup::render( de::enums::fboTypes _layer )
        {
            groupsEnd = Sprites.end();
            spritesEnd = currentlySelectedGroup->second.end();

            for( spritesIter = currentlySelectedGroup->second.begin(); spritesIter != spritesEnd; ++spritesIter )
            {
                spritesIter->second.render( _layer );
            }
        }

        void SpriteGroup::reload()
        {
            groupsEnd = Sprites.end();
            for( groupsIter = Sprites.begin(); groupsIter != groupsEnd; ++groupsIter )
            {
                spritesEnd = groupsIter->second.end();
                for( spritesIter = groupsIter->second.begin(); spritesIter != spritesEnd; ++spritesIter )
                {
                    spritesIter->second.reload();
                }
            }
        }


        const classes::Vector& SpriteGroup::getCentre()
        {
            return centre;
        }
    }
}
