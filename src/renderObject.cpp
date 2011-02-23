#include "renderObject.h"
#include "Math.h"



namespace de
{
    namespace graphics
    {
        vertex normalise( const vertex &_aVertex )
        {
            float length = sqrt( (_aVertex.r*_aVertex.r) + (_aVertex.g*_aVertex.g) + (_aVertex.b*_aVertex.b) + (_aVertex.a*_aVertex.a) );

            return vertex( _aVertex.r/length, _aVertex.g/length, _aVertex.b/length, _aVertex.a/length );
        }

        renderObject::renderObject()
        {
            //ctor
        }

        renderObject::~renderObject()
        {
            //dtor
        }
    }
}
