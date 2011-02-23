#include "SpriteSheet.h"

#include <luabind/luabind.hpp>
#include "corona.h"
#include <map>
#include "openGL.h"


using namespace de::classes;
using namespace de::core;
using namespace de::enums;
using namespace de::misc;
using namespace de::filesystem;
using namespace de::graphics;


namespace fs = boost::filesystem;

namespace de
{
    namespace graphics
    {
        SpriteSheet::SpriteSheet( const std::string &_dir,
                                  const std::string &_spriteName,
                                  int _width,
                                  int _height )
            :spriteBuffer( Engine::Graphics().getVideoSettings().OpenGL )
        {
            stringDir = _dir;
            spriteName = _spriteName;
            width = _width;
            height = _height;
        }

        void SpriteSheet::refresh()
        {
            std::vector<resources::ImageResource>::iterator iterImage, endImage = Images.end();
            for( iterImage = Images.begin(); iterImage < endImage; ++iterImage )
            {
                iterImage->unload();
            }
            Images.clear();
            foundFiles.clear();
            rects.clear();
            packs.clear();

            try
            {
                dir = fs::path( Roots->get( root::RESOURCES ) + stringDir );
                fileName = fs::path( Roots->get( root::RESOURCES ) + spriteName );

                if( !( fs::exists( fileName.parent_path() ) && fs::is_directory( fileName.parent_path() ) ) )
                {
                    fs::create_directories( fileName.parent_path() );
                }



                renderTexture.setup( width, height );
            }
            catch( boost::filesystem::filesystem_error e)
            {
                de::io::error << e.what();
            }

            packer = BinPacker();
        }

        SpriteSheet::~SpriteSheet()
        {
            std::vector<resources::ImageResource>::iterator iterImage, endImage = Images.end();
            for( iterImage = Images.begin(); iterImage < endImage; ++iterImage )
            {
                iterImage->unload();
            }
            lua_close(luaState);
        }

        bool SpriteSheet::generateSpriteSheet()
        {
            refresh();

            try
            {
                getFiles();
                getSprites();
                packTexture();
                renderToTexture();
                generateCoordsFile();
            }
            catch( boost::filesystem::filesystem_error e)
            {
                de::io::error << e.what();
                return false;
            }
            return true;
        }

        void SpriteSheet::getFiles()
        {
             getFilesFrom( dir, foundFiles, filetypes::IMAGES );
             end = foundFiles.end();
        }

        void SpriteSheet::getSprites()
        {
            Sprites.clear();


            for( iter = foundFiles.begin(); iter < end; ++iter )
            {
                Images.push_back( resources::ImageResource( iter->filename(), iter->file_string() ) );
                Sprites.push_back( Sprite() );
            }

            endImage = Images.end();
            endSprite = Sprites.end();

            for( iterImage = Images.begin(), iterSprite = Sprites.begin(); iterImage < endImage; ++iterImage, ++iterSprite )
            {
                iterImage->load();

                iterSprite->setTexture( iterImage->Texture );
                iterSprite->coords.Setup( iterImage->width, iterImage->height, false );
                rects.push_back(iterImage->width);
                rects.push_back(iterImage->height);
            }
        }

        void SpriteSheet::packTexture()
        {
            packer.Pack( rects, packs, width, height, false );

            std::map<int, Vector> vectorMap;
            typedef std::pair<const int, Vector> VecMapType;

            endIter = packs[0].end();
            for ( iterInt = packs[0].begin(); iterInt < endIter; iterInt +=4 )
            {
                vectorMap.insert( VecMapType( (*iterInt), Vector( (*(iterInt + 1)), (*(iterInt + 2)) ) ) );
            }

            int i(0);
            endSprite = Sprites.end();
            for( iterSprite = Sprites.begin(); iterSprite < endSprite; ++iterSprite, ++i )
            {
                iterSprite->coords.transform( vectorMap[i] );
            }
        }

        void SpriteSheet::renderToTexture()
        {
            renderTexture.activate( Engine::Graphics().getVideoSettings() );

                spriteBuffer.add( Sprites );
                spriteBuffer.render();

                std::string imageName( fileName.file_string() + ".png");

                GLvoid *imageData = malloc(width*height*(8+8+8+8));
                CHECKGL( glReadPixels( 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData ) );

                corona::Image *myImage = corona::CreateImage(width, height, corona::PF_R8G8B8A8, imageData);
                corona::FlipImage( myImage, corona::CA_X);
                corona::SaveImage( imageName.c_str(), corona::FF_PNG, myImage);

                delete myImage;
                free(imageData);

            CHECKGL( glBindFramebuffer(GL_FRAMEBUFFER, 0 ) );
        }

        void SpriteSheet::generateCoordsFile()
        {
            luaState = lua_open();
            luaL_openlibs(luaState);
            luabind::open(luaState);

            fs::path serialize( Roots->get( root::LUA ) + "serialize.lua" );
            fs::path savePath( fileName.file_string() + ".lua" );

            luaL_dofile( luaState, serialize.file_string().c_str() );
            luaL_dostring( luaState, "Sprites = {}\n" );


            luabind::object global = luabind::globals(luaState);
            luabind::object SpritesTable = global["Sprites"];


            for( iter = foundFiles.begin(), iterSprite = Sprites.begin(); iter < end; ++iter, ++iterSprite )
            {
                std::string name( /*fileName.filename() + "." +*/ stripFileEnding( iter->filename() ) );

                std::string tableConstructor( "Sprites[\"" + name + "\"] = {}\n" );
                luaL_dostring( luaState, tableConstructor.c_str() );
                luabind::object currentTable = SpritesTable[name];

                if( luabind::type(currentTable) != LUA_TTABLE )
                {
                    de::io::error << "luabind::type(currentTable) != LUA_TTABLE.\n";
                    de::io::error << "Sprites[\"" + name + "\"] = {}\n";
                }


                currentTable["X"] = iterSprite->coords.centre.x;
                currentTable["Y"] = iterSprite->coords.centre.y;
                currentTable["W"] = (int)iterSprite->coords.width();
                currentTable["H"] = (int)iterSprite->coords.height();

                currentTable["normX"] = iterSprite->coords.centre.x/width;
                currentTable["normY"] = iterSprite->coords.centre.y/height;
                currentTable["normW"] = iterSprite->coords.width() /width;
                currentTable["normH"] = iterSprite->coords.height()/height;
            }




            try
            {
                std::string ret = luabind::call_function<std::string>( global["serialize"], SpritesTable );

                std::ofstream file;
                file.open( savePath.file_string().c_str() );
                file << fileName.filename() << " = " << ret << "\n";
                file.close();
            }
            catch( luabind::error &e )
            {
                std::string error = lua_tostring( e.state(), -1 );
                de::io::error << e.what() << "\n";
                de::io::error << error << "\n";
            }
        }

    }
}
