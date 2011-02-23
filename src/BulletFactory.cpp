#include "BulletFactory.h"

using namespace de::filesystem;

namespace fs = boost::filesystem;


namespace Attrition
{
    BulletFactory bulletManager;



    BulletFactory::BulletFactory(): parsingDone(false)
    {
        //ctor
    }

    BulletFactory::~BulletFactory()
    {
        //dtor
    }


    const Bullet& BulletFactory::get( const std::string &_bulletName )
    {
        bulletsIter = Bullets.find( _bulletName );

        if( bulletsIter != Bullets.end() )
        {
            return bulletsIter->second;
        }
        else
        {
            de::io::error << "The bullet " << _bulletName << "deosn't exist\n";
        }
        static Bullet blank;

        return blank;
    }

    const std::vector<std::string>& BulletFactory::bulletNames()
    {
        return BulletNames;
    }


    void BulletFactory::parseBullets()
    {
        if( !parsingDone )
        {
            std::vector<fs::path> foundFiles;
            std::vector<fs::path>::iterator iter, end;

            fs::path particleFilePath( Roots->get( root::BULLETS ) );
            getFilesFrom( particleFilePath, foundFiles, filetypes::LUA );

            for( iter = foundFiles.begin(); iter < foundFiles.end(); ++iter )
            {
                std::string bulletName( stripFileEnding( iter->filename() ) );

                Bullets.insert(  bulletType( bulletName, Bullet( bulletName ) )  );
                BulletNames.push_back( bulletName );
            }

            parsingDone = true;
        }
    }
}
