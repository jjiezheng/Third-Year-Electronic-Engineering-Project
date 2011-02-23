#include "VisualHitbox.h"


namespace de
{
    namespace collision
    {
        VisualHitbox::VisualHitbox()
        {
            hitbox.set( "Hitbox" );
            hitbox.setType( "Triangles" );
        }

        VisualHitbox::VisualHitbox( const OBB &_obb )
        {
            using namespace de::graphics;
            std::vector<vertex> verts;
            verts.push_back( vertex( -_obb.half.x, -_obb.half.y, -0.0f, 1.0f ) );
            verts.push_back( vertex(  _obb.half.x, -_obb.half.y, -0.0f, 1.0f ) );
            verts.push_back( vertex(  _obb.half.x,  _obb.half.y, -0.0f, 1.0f ) );
            verts.push_back( vertex( -_obb.half.x,  _obb.half.y, -0.0f, 1.0f ) );

            std::vector<int> elems;
            elems.push_back( 0 );
            elems.push_back( 1 );
            elems.push_back( 2 );
            elems.push_back( 2 );
            elems.push_back( 3 );
            elems.push_back( 0 );


            hitbox.set( "Hitbox" );
            hitbox.setType( "Triangles" );
            hitbox.add( "Position", verts, elems );
        }

        VisualHitbox::~VisualHitbox()
        {
            //dtor
        }

        void VisualHitbox::update( const OBB &_obb )
        {
            hitbox.setUniform( "Model", glm::mat4(_obb.orientation) );
            hitbox.setUniform( "Centre", _obb.centre);
        }

        void VisualHitbox::reload()
        {
            hitbox.reload();
        }

        void VisualHitbox::render()
        {
            hitbox.render();
        }
    }
}
