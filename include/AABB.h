#ifndef AABB_H
#define AABB_H

#include "base.h"

namespace de
{
    namespace collision
    {
        struct AABB
        {
            glm::vec3 c;
            float r[2];
        };

        float Abs( const float &_n );
        int TestAABBAABB( const AABB &_a, const AABB &_b );
    }
}
#endif // AABB_H
