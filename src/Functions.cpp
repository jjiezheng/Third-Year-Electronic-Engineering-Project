#include "Functions.h"

#include <time.h>


namespace de
{
    namespace filesystem
    {
        namespace fs = boost::filesystem;

        filetypes::FileTypes getFileTypefrom( const std::string &_fileName )
        {
            std::vector<std::string> strs;
            std::vector<std::string>::iterator iter, end;

            boost::split(strs, _fileName, boost::is_any_of("."));

            if( !strs.empty() )
            {
                iter = strs.end()-1;

                if( (*iter) == "png" )
                    return filetypes::PNG;
                else if( (*iter) == "jpg" )
                    return filetypes::JPG;
                else if( (*iter) == "bmp" )
                    return filetypes::BMP;
                else if( (*iter) == "tga" )
                    return filetypes::TGA;
                else if( (*iter) == "txt" )
                    return filetypes::TXT;
                else if( (*iter) == "lua" )
                    return filetypes::LUA;
                else if( (*iter) == "wav" )
                    return filetypes::WAV;
                else if( (*iter) == "ogg" )
                    return filetypes::OGG;
                else if( (*iter) == "3ds" )
                    return filetypes::STUDIOMAX;
                else if( (*iter) == "lwo" )
                    return filetypes::LWO;
                else if( (*iter) == "obj" )
                    return filetypes::OBJ;
				else if( (*iter) == "fnt" )
                    return filetypes::FONT;
            }
            return filetypes::NONE;
        }

        void getFilesFrom( const fs::path &_dir, std::vector<fs::path> &_foundFiles, int _choice )
        {
            for( fs::directory_iterator pos(_dir), end; pos != end; ++pos )
            {
                if( is_regular_file( (*pos) ) )
                {
                    int type = getFileTypefrom( pos->path().filename() );

                    if( (_choice & type) == type && type != filetypes::NONE )
                    {
                        _foundFiles.push_back( pos->path() );
                    }
                }
            }
        }

        const std::string stripFileEnding( const std::string &_fileName )
        {
            if( !_fileName.empty() )
            {
                std::string returnString;
                std::vector<std::string> strs;
                std::vector<std::string>::iterator iter, end;

                boost::split(strs, _fileName, boost::is_any_of("."));

                for( iter = strs.begin(), end = strs.end(); iter < end-1; ++iter )
                {
                    returnString += (*iter);
                }
                return returnString;
            }
            else return _fileName;
        }

    }

    namespace misc
    {
        void pointOnCurve( classes::Vector& _pos, const float &t, const classes::Vector &p0, const classes::Vector &p1, const classes::Vector &p2, const classes::Vector &p3 )
        {
            float t2 = t * t;
            float t3 = t2 * t;

            //TrackPoint buffer;
            _pos.x = 0.5f * ( ( 2.0f * p1.x ) + ( -p0.x + p2.x ) * t + ( 2.0f * p0.x - 5.0f * p1.x + 4 * p2.x - p3.x ) * t2 + ( -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x ) * t3 );
            _pos.y = 0.5f * ( ( 2.0f * p1.y ) + ( -p0.y + p2.y ) * t + ( 2.0f * p0.y - 5.0f * p1.y + 4 * p2.y - p3.y ) * t2 + ( -p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y ) * t3 );
        }
    }

	namespace sys
	{
		void titleBar( const std::string &_title )
        {
            SDL_WM_SetCaption( _title.c_str(), NULL );
        }
	}

    namespace time
    {
        using namespace boost::posix_time;
        using namespace boost::gregorian;

        std::string getTimeString()
        {
            ptime now = second_clock::local_time();
            return to_simple_string(now);
        }
    }
}

