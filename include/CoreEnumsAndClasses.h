#ifndef BASIC_ENUMS_AND_CLASSES_H
#define BASIC_ENUMS_AND_CLASSES_H

#include "Material.h"
#include "Functions.h"
#include "Vector.h"
#include "Log.h"

#include "Base.h"


namespace de
{/*
    namespace graphics
    {
        class Shader;
    }*/

    namespace filesystem
    {
        namespace root
        {
            enum roots
            {
                RESOURCES,
                TEXTURES,
                SHADERS,
                MESHES,
                FONTS,
                MUSIC,
                SOUNDEFFECTS,
                SETTINGS,
                SHIPS,
                BULLETS,
                PARTICLES,
                LUA
            };
        }

        class Root
        {
            public:
                Root();
                ~Root();

                const std::string get( root::roots _choice );

            private:
                std::string resources, textures, shaders, meshes, fonts, music, soundeffects, settings, ships, bullets, particles, lua;
                lua_State *luaState;
        };

        extern Root* Roots;

    }

    namespace classes
    {
        //! A box struct.
        class Frect
        {
            friend std::ostream& operator<< ( std::ostream& output, const Frect& _rect );
            public:
                //! Basic Constructor
                Frect() : x(0), y(0), w(0), h(0) {}
                Frect( const float &_x, const float &_y, const float &_w, const float &_h ) :
                    x(_x), y(_y), w(_w), h(_h) {}

                //! Position Horizontally.
                float x;
                //! Position Vertically.
                float y;
                //! Width.
                float w;
                //! Height.
                float h;
        };


        class BasicSettings
        {
            public:
                static float globalScreenRatio;
                static Vector  mouseOffset;
                static std::string rootPrefix;

                static bool borderHorizontal;
        };
    }
}


#endif //BASIC_ENUMS_AND_CLASSES_H

