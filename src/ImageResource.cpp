#include "ImageResource.h"
#include "GameServices.h"

#include "CoreEnumsAndClasses.h"
#include <luabind/luabind.hpp>
#include <corona.h>


namespace de
{
    namespace resources
    {
        ImageResource::ImageResource( const std::string &_name, const boost::filesystem::path &_path ) : width(0), height(0), Texture(0)
        {
            name = _name;
            path = _path;

            luaPath = path.parent_path().file_string() + "/" + filesystem::stripFileEnding( path.filename() ) + ".lua";
        }

        const Uint32& ImageResource::getTexture( lua_State* L )
        {
            if( Texture == 0 )
            {
                load( L );
            }
            return Texture;
        }

        void ImageResource::load( lua_State* L )
        {
            if( Texture == 0 )
            {
                Texture = Engine::Graphics().loadTexture( path.file_string(), width, height );

                if( L != NULL && boost::filesystem::is_regular( luaPath ) )
                {
                    luaL_dofile( L, luaPath.file_string().c_str() );

                    for( luabind::iterator iter(luabind::globals(L)[name]), end; iter != end; ++iter )
                    {
                        std::string name( luabind::object_cast<std::string>(iter.key()) );
                        classes::Frect coords, texcoords;

                        coords.x = luabind::object_cast<float>((*iter)["X"]);
                        coords.y = luabind::object_cast<float>((*iter)["Y"]);
                        coords.w = luabind::object_cast<float>((*iter)["W"]);
                        coords.h = luabind::object_cast<float>((*iter)["H"]);

                        texcoords.x = luabind::object_cast<float>((*iter)["normX"]);
                        texcoords.y = luabind::object_cast<float>((*iter)["normY"]);
                        texcoords.w = luabind::object_cast<float>((*iter)["normW"]);
                        texcoords.h = luabind::object_cast<float>((*iter)["normH"]);

                        spriteRects[name] = coords;
                        spriteTexs[name] = texcoords;

                        spriteNames.push_back( name );
                    }
                }
            }
        }

        de::classes::Frect ImageResource::getSpriteCoords( const std::string _name, bool tex )
        {
            std::map<std::string, classes::Frect> *current;
            if(tex)
                current = &spriteTexs;
            else
                current = &spriteRects;

            frectIter = current->find(_name);

            if( frectIter == current->end() )
            {
                de::io::error << "Failure to get sprite called " << _name << "\n";
                return classes::Frect();
            }
            return frectIter->second;
        }

        std::map<std::string, de::classes::Frect> ImageResource::getAllSprites( bool tex )
        {
            if( tex )
            {
                return spriteTexs;
            }
            return spriteRects;
        }


        std::vector<std::string> ImageResource::getSpriteNames()
        {
            return spriteNames;
        }

        void ImageResource::unload()
        {
            Engine::Graphics().unloadTexture( Texture );
        }
	}
}