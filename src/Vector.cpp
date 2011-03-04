#include "Vector.h"




namespace de
{
    namespace classes
    {
        std::ostream& operator << ( std::ostream& output, const Vector& _vector )
        {
            output << "X:" << _vector.x << " Y:" << _vector.y;
            return output;  // for multiple << operators.
        }
    }
}
