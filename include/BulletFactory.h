#ifndef BULLETFACTORY_H
#define BULLETFACTORY_H

#include "CoreEngine.h"
#include "Coords.h"
#include "Bullet.h"

#include <luabind/luabind.hpp>
#include <lua.hpp>

namespace Attrition
{
    class BulletFactory
    {
        public:
            BulletFactory();
            virtual ~BulletFactory();

            void parseBullets();
            const Bullet& get( const std::string &_bulletName );
            const std::vector<std::string>& bulletNames();


        private:
            std::map<std::string, Bullet> Bullets;
            std::map<std::string, Bullet>::iterator bulletsIter;
            typedef std::pair<std::string, Bullet> bulletType;

            std::vector<std::string> BulletNames;

            bool parsingDone;
    };

    extern BulletFactory bulletManager;
}

#endif // BULLETFACTORY_H
