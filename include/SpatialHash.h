#ifndef SPATIALHASH_H
#define SPATIALHASH_H


#include "Vector.h"
#include "Bullet.h"
#include <vector>
#include <SDL/SDL.h>

namespace de
{
    namespace classes
    {
        //! Pointer to a Bullet along with a unique id. Used by de::core::SpatialHash.
        struct BulletPtr
        {
            //! The Actual Pointer
            Attrition::Bullet* bullet;
            //! The Bullets id.
            unsigned int bulletId;

            //! Constructor.
            BulletPtr( unsigned int _bulletId, Attrition::Bullet* _bullet )
                :bullet(_bullet),
                bulletId(_bulletId) {}
        };
    }
    namespace core
    {
        //! Stores Bullets in a 2D Spatial Hash each frame.
        class SpatialHash
        {
        public:
            //! Acts like a Constructor. Must be called first.
            static void init();

            //! Returns a Vector of BulletPtr for the rectanglur area from "_min" to "_max" with the label "_label".
            static std::vector< std::vector<de::classes::BulletPtr> > getBulletsFor( de::classes::Vector &_min, de::classes::Vector &_max, const int &_label );
            //! Add the Bullet "_currentBullet" which is in an area from "_min" to "_max" with the "_label" to the Spatial Hash.
            static void addBullet( de::classes::Vector &_min, de::classes::Vector &_max, const int &_label, Attrition::Bullet* _currentBullet );

            //! Use this function to Let the class know that the current frame is over. Deletes the contents of the SpatialHash and gets ready for the next frame.
            static void finishedFrame();

        private:
            static unsigned int bulletId, maxX, maxY, limitX, limitY;
            static int leftBorder, topBorder, gridRes;
            static std::vector< std::vector< std::vector<de::classes::BulletPtr> > > playerBulletHash, enemyBulletHash;
        };
    }
}


#endif //SPATIALHASH_H
