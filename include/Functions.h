#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Vector.h"
#include "Base.h"


namespace de
{
    namespace enums
    {
        enum Collision
        {
            FRIENDLY,
            ENEMY
        };

        enum fboTypes
        {
            FBO_UI,
            FBO_GAME,
            FBO_AFTER
        };
        namespace events
        {
            enum SDL_EVENT_CODES
            {
                NONE,
                OPENGL_RELOAD,

                LEVEL_BACK,
                LEVEL_FORWARD,

                WAVE_BACK,
                WAVE_FORWARD,

                WEAPON_BACK,
                WEAPON_FORWARD,

                FIRING_BACK,
                FIRING_FORWARD
            };
        }
    }

    namespace misc
    {
        //! Catmull-Rom splines!!!!!!
        void pointOnCurve( classes::Vector& _pos, const float &t, const classes::Vector &p0, const classes::Vector &p1, const classes::Vector &p2, const classes::Vector &p3 );


        //! Returns pi in radians
        inline float pi()
        {
            static const float Pi = atan(1.0f) * 4.0f;
            return Pi;
        }
        //! Returns pi in degrees
        inline float piDegrees()
        {
            static const float Pi = ( atan(1.0f) * 4.0f )/180.f;
            return Pi;
        }
    }
	namespace sys
	{
        void titleBar( const std::string &_title );
	}

    namespace filesystem
    {
        namespace filetypes
        {
            enum FileTypes
            {
                NONE = 0,
                PNG = 1,
                JPG = 1 << 1,
                BMP = 1 << 2,
                TGA = 1 << 3,
                TXT = 1 << 4,
                LUA = 1 << 5,
                WAV = 1 << 6,
                OGG = 1 << 7,

                IMAGES = PNG | JPG | BMP | TGA,
                MUSIC = WAV | OGG,
                SOUNDEFFECTS = WAV | OGG
            };
        }

        const std::string stripFileEnding( const std::string &_fileName );
        filetypes::FileTypes getFileTypefrom( const std::string &_fileName );
        void getFilesFrom( const boost::filesystem::path &_dir, std::vector<boost::filesystem::path> &_foundFiles, int _choice );

    }


    namespace time
    {
        std::string getTimeString();
    }

}

#endif // FUNCTIONS_H
