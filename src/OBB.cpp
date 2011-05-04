#include "OBB.h"
#include "Functions.h"

namespace
{
    float Abs( const float &_n )
    {
        if(_n>0) return _n;
        return -_n;
    }
}
namespace de
{
    namespace collision
    {
        std::ostream& operator<< ( std::ostream &_output, const glm::vec2 &_vec2 )
        {
            _output << "X:" << _vec2.x << " Y:" << _vec2.y;
            return _output;  // for multiple << operators.
        }

        std::ostream& operator<< ( std::ostream &_output, const glm::vec3 &_vec3 )
        {
            _output << "X:" << _vec3.x << " Y:" << _vec3.y << " Z:" << _vec3.z;
            return _output;  // for multiple << operators.
        }

        std::ostream& operator<< ( std::ostream &_output, const glm::vec4 &_vec4 )
        {
            _output << "X:" << _vec4.x << " Y:" << _vec4.y << "Z:" << _vec4.z << " W:" << _vec4.w;
            return _output;  // for multiple << operators.
        }

        std::ostream& operator<< ( std::ostream &_output, const OBB &_obb )
        {
            _output << "centre:(" << _obb.centre.x << "," << _obb.centre.y << ")  Width:" << _obb.half.x*2.0f << "  Height:" << _obb.half.y*2.0f << " Verts at "

                    << _obb.centre+( _obb.orientation*glm::vec2( -_obb.half.x, -_obb.half.y ) ) << "  "
                    << _obb.centre+( _obb.orientation*glm::vec2(  _obb.half.x, -_obb.half.y ) ) << "  "
                    << _obb.centre+( _obb.orientation*glm::vec2(  _obb.half.x,  _obb.half.y ) ) << "  "
                    << _obb.centre+( _obb.orientation*glm::vec2( -_obb.half.x,  _obb.half.y ) ) << "  " << "\n";

            return _output;  // for multiple << operators.
        }

        OBB::OBB() : active(false)
        {
        }

        OBB::OBB( const float &_width, const float &_height ) : active(true)
        {
            half.x = _width/2.0f;
            half.y = _height/2.0f;
            centre = glm::vec2(0.0f);
			z = 0;
        }

		void OBB::setZ( const float &_z )
		{
			z = _z;
		}
        void OBB::transform( const glm::vec2 &_position )
        {
            centre += _position;
        }

        void OBB::teleport( const glm::vec2& _position )
        {
            centre = _position;
        }

        void OBB::rotate( const float &_rotation )
        {
            orientation[0][0] =  cos( _rotation * misc::piDegrees() );
            orientation[0][1] = -sin( _rotation * misc::piDegrees() );
            orientation[1][0] =  sin( _rotation * misc::piDegrees() );
            orientation[1][1] =  cos( _rotation * misc::piDegrees() );
        }

        void OBB::rotate( const float &_cos, const float &_sin )
        {
            orientation[0][0] =  _cos;
            orientation[0][1] = -_sin;
            orientation[1][0] =  _sin;
            orientation[1][1] =  _cos;
        }

        bool OBB::containsPoint( const glm::vec3 &_point )
        {
			glm::vec2 pointLocalSpace(_point);
			pointLocalSpace -= centre;
			pointLocalSpace = orientation*pointLocalSpace;

            if( pointLocalSpace.x < -half.x || pointLocalSpace.x > half.x )
                return false;

            if( pointLocalSpace.y < -half.y || pointLocalSpace.y > half.y )
                return false;

            return true;
        }

        float GetProjectedRadius( const OBB &_o, const glm::vec2 &_d )
        {
            return _o.half[0]*glm::abs( glm::dot( _d,_o.orientation[0] ) ) + _o.half[1]*glm::abs( glm::dot( _d,_o.orientation[1] ) );
        }

        bool SeparatedOnAxis( const OBB &_a, const OBB &_b, const glm::vec2 &_d )
        {
            // Get projected distance between OBB centers
            float r = glm::abs( glm::dot( _a.centre - _b.centre, _d ) );

            return GetProjectedRadius( _a, _d ) + GetProjectedRadius( _b, _d ) < r;
        }

        bool TestOBBOBB( const OBB &_a, const OBB &_b )
        {
            // Exit early if not overlapping
            if (SeparatedOnAxis( _a, _b, _a.orientation[0] ) )
                return false;
            if (SeparatedOnAxis( _a, _b, _a.orientation[1] ) )
                return false;
            if (SeparatedOnAxis( _a, _b, _b.orientation[0] ) )
                return false;
            if (SeparatedOnAxis( _a, _b, _b.orientation[1] ) )
                return false;
            // Must be overlapping
            return true;
        }

		// Test if segment specified by points p0 and p1 intersects AABB b
		bool TestOOBRay(const OBB &_obb, const glm::vec3 &_p0, const glm::vec3 &_p1) 
		{
			float EPSILON = 0.0000001f;

			glm::vec3 c(_obb.centre.x, _obb.centre.y, _obb.z); // Box center-point

			glm::vec3 e(_obb.half.x,_obb.half.y, 0.1 ); // Box halflength extents
			glm::vec3 m = (_p0 + _p1) * 0.5f; // Segment midpoint
			glm::vec3 d = _p1 - m; // Segment halflength vector
			m = m - c; // Translate box and segment to origin

			// Try world coordinate axes as separating axes
			float adx = glm::abs(d.x);
			if (Abs(m.x) > e.x + adx) 
				return false;

			float ady = glm::abs(d.y);
			if (Abs(m.y) > e.y + ady) 
				return false;

			float adz = glm::abs(d.z);
			if (Abs(m.z) > e.z + adz) 
				return false;

			// Add in an epsilon term to counteract arithmetic errors when segment is
			// (near) parallel to a coordinate axis (see text for detail)
			adx += EPSILON; 
			ady += EPSILON; 
			adz += EPSILON;
			// Try cross products of segment direction vector with coordinate axes
			if (glm::abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) return 0;
			if (glm::abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) return 0;
			if (glm::abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return 0;
			// No separating axis found; segment must be overlapping AABB
			return true;
		}
    }
}
