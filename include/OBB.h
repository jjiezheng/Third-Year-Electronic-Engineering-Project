#ifndef OOB_H
#define OOB_H

#include "base.h"

namespace de
{
    namespace collision
    {
        class OBB
        {
            friend std::ostream& operator<< ( std::ostream &_output, const OBB &_obb );
            public:
                //! Basic Constructor.
                OBB();
                OBB( const float &_width, const float &_height );

                void transform( const glm::vec2 &_position );
                void teleport( const glm::vec2& _position );

                void rotate( const float &_rotation );
                void rotate( const float &_cos, const float &_sin );

                //! Checks to see if the point "_point" is inside the Poly. Returns true if yes. Uses Barrycentric coordinates.
                bool containsPoint( const glm::vec3 &_point, const glm::mat4 &_mat );

            public:
                glm::mat2 orientation;
                glm::vec2 half,centre;
                float radiusSqrd;
                bool active;

            private:
                bool isPointInTriangle( const glm::vec2& _A, const glm::vec2& _B, const glm::vec2& _C, const glm::vec2& _P ) const;
        };

        std::ostream& operator<< ( std::ostream &_output, const glm::vec2 &_vec2 );
        std::ostream& operator<< ( std::ostream &_output, const glm::vec3 &_vec3 );
        std::ostream& operator<< ( std::ostream &_output, const glm::vec4 &_vec4 );

        float GetProjectedRadius( const OBB &_o, const glm::vec2 &_d );
        bool SeparatedOnAxis( const OBB &_a, const OBB &_b, const glm::vec2 &_d );
        bool TestOBBOBB( const OBB &_a, const OBB &_b );
    }
}
#endif // OOB_H

