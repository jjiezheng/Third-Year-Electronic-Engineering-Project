#include "AABB.h"

namespace de
{
    namespace collision
    {
        float Abs( const float &_n )
        {
            if(_n>0) return _n;
            return -_n;
        }
        int TestAABBAABB( const AABB &_a, const AABB &_b )
        {
            if (Abs(_a.c[0] - _b.c[0]) > (_a.r[0] + _b.r[0])) return 0;
            if (Abs(_a.c[1] - _b.c[1]) > (_a.r[1] + _b.r[1])) return 0;
            return 1;
        }
    }
}
