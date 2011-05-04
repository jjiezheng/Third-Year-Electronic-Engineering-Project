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
                OBB();
                OBB( const float &_width, const float &_height );
				void setZ( const float &_z );
                void transform( const glm::vec2 &_position );
                void teleport( const glm::vec2& _position );

                void rotate( const float &_rotation );
                void rotate( const float &_cos, const float &_sin );

                bool containsPoint( const glm::vec3 &_point );

            public:
                glm::mat2 orientation;
                glm::vec2 half,centre;
                float radiusSqrd, z;
                bool active;
        };

        std::ostream& operator<< ( std::ostream &_output, const glm::vec2 &_vec2 );
        std::ostream& operator<< ( std::ostream &_output, const glm::vec3 &_vec3 );
        std::ostream& operator<< ( std::ostream &_output, const glm::vec4 &_vec4 );

        float GetProjectedRadius( const OBB &_o, const glm::vec2 &_d );
        bool SeparatedOnAxis( const OBB &_a, const OBB &_b, const glm::vec2 &_d );
        bool TestOBBOBB( const OBB &_a, const OBB &_b );
		bool TestOOBRay(const OBB &_obb, const glm::vec3 &_p0, const glm::vec3 &_p1);
    }
}
#endif // OOB_H

