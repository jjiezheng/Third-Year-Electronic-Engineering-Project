#ifndef SPRITEGROUP_H
#define SPRITEGROUP_H

#include <vector>
#include "CoreEnumsAndClasses.h"
#include "AnimatedSprite.h"

#include <luabind/luabind.hpp>
#include <lua.hpp>

namespace de
{
    namespace graphics
    {
        class SpriteGroup
        {
            public:
                SpriteGroup();
                virtual ~SpriteGroup();
                bool parseSprites( const luabind::object &_SpriteTable );

                bool insert( const std::string &_name, const std::string &_set );
                bool selected( const std::string &_name, const std::string &_set );
                bool activeGroup( const std::string &_set );
                AnimatedSprite& current();

                void moveBy( const de::classes::Vector &_movement, bool _moveFromOrign = false );
                void rotateBy( const float &_rotation );
                void moveRotateBy( const de::classes::Vector &_movement, bool _moveFromOrign = false );

                void logic( const Uint32 &_deltaticks );
                void render( de::enums::fboTypes _layer );
                void reload();

                const classes::Vector& getCentre();

                SpriteGroup operator= ( const SpriteGroup &_other );

            private:
                bool parseSingleSpriteGroup( const luabind::object &_SpriteGroup, const std::string &_groupName );
                bool parseSingleSprite( const luabind::object &_Sprite, const std::string &_spriteName, const std::string &_groupName );

            private:
                std::string theActiveGroup, theActiveSprite;
                de::classes::Vector centre, last;
                float rotation;
                bool first;

                std::map< std::string, std::map<std::string, AnimatedSprite> > Sprites;

                std::map< std::string, std::map<std::string, AnimatedSprite> >::iterator currentlySelectedGroup, groupsIter, groupsEnd;
                std::map<std::string, AnimatedSprite>::iterator currentlySelected, spritesIter, spritesEnd;
        };
    }
}

#endif // SPRITEGROUP_H
