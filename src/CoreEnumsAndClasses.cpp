#include "CoreEnumsAndClasses.h"
#include "GameServices.h"
#include "Material.h"
#include <luabind/luabind.hpp>

namespace de
{
    namespace filesystem
    {
        Root* Roots = NULL;

            Root::Root()
        {
            luaState = lua_open();
            luabind::open(luaState);
            #ifdef __APPLE__
                luaL_dofile(luaState, "roots.lua");
                std::string table("Mac");
            #else
                luaL_dofile(luaState, "roots.lua");
                std::string table("Windows");
            #endif

            luabind::object global = luabind::globals(luaState);
            luabind::object rootTable = global["Roots"];
            luabind::object localTable = rootTable[table];

            if( luabind::type(localTable) != LUA_TTABLE )
            {
                de::io::error << "In Roots config file." << table << " is not a table!\n";
            }

            resources    = luabind::object_cast<std::string>( localTable["resources"] );
            textures     = luabind::object_cast<std::string>( localTable["textures"] );
            shaders      = luabind::object_cast<std::string>( localTable["shaders"] );
            meshes       = luabind::object_cast<std::string>( localTable["meshes"] );
            fonts        = luabind::object_cast<std::string>( localTable["fonts"] );
            music        = luabind::object_cast<std::string>( localTable["music"] );
            soundeffects = luabind::object_cast<std::string>( localTable["soundeffects"] );
            settings     = luabind::object_cast<std::string>( localTable["settings"] );
            ships        = luabind::object_cast<std::string>( localTable["ships"] );
            bullets      = luabind::object_cast<std::string>( localTable["bullets"] );
            particles    = luabind::object_cast<std::string>( localTable["particles"] );
            lua          = luabind::object_cast<std::string>( localTable["lua"] );

        }


        Root::~Root()
        {
            lua_close(luaState);
        }


        const std::string Root::get( root::roots _choice )
        {
            switch(_choice)
            {
                case( root::RESOURCES )    : return resources;
                case( root::TEXTURES )     : return textures;
                case( root::SHADERS )      : return shaders;
                case( root::MESHES )       : return meshes;
                case( root::FONTS )        : return fonts;
                case( root::MUSIC )        : return music;
                case( root::SOUNDEFFECTS ) : return soundeffects;
                case( root::SETTINGS )     : return settings;
                case( root::SHIPS )        : return ships;
                case( root::BULLETS )      : return bullets;
                case( root::PARTICLES )    : return particles;
                case( root::LUA )          : return lua;
            }
            de::io::error << "No root with that name. :(\n";
			assert(!"In Root::get()  No root with that name. :(");
            return "";
        }

    }

    namespace classes
    {
        float BasicSettings::globalScreenRatio(1);
        Vector BasicSettings::mouseOffset(0,0);
        bool BasicSettings::borderHorizontal(false);

        #ifdef __APPLE__
            std::string BasicSettings::rootPrefix( "Test.app/Contents/Resources/" );
        #else
            std::string BasicSettings::rootPrefix( "../" );
        #endif





        std::ostream& operator << ( std::ostream& output, const Frect& _rect )
        {
            output << "X: " << _rect.x << "  Y: " << _rect.y << "  W: " << _rect.w << "  H: " << _rect.h;
            return output;  // for multiple << operators.
        }
    }
}
