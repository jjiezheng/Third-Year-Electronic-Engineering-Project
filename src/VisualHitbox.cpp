#include "VisualHitbox.h"


namespace de
{
    namespace collision
    {
        VisualHitbox::VisualHitbox() :flipIt(false)
        {
            hitbox.set( "Hitbox" );
            hitbox.setType( "Triangles" );
        }

        VisualHitbox::VisualHitbox( const OBB &_obb ) :flipIt(false)
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

		void VisualHitbox::flip( const bool &_flipIt )
		{
			flipIt = _flipIt;
		}

		VisualHitbox& VisualHitbox::writeToDepth( bool _depth )
		{
			hitbox.writeToDepth( _depth );
			return *this;
		}
		VisualHitbox& VisualHitbox::depth( bool _depth )
		{
			hitbox.depth( _depth );
			return *this;
		}
		VisualHitbox& VisualHitbox::alpha( bool _alpha )
		{
			hitbox.alpha( _alpha );
			return *this;
		}
		VisualHitbox& VisualHitbox::blend( bool _blend )
		{
			hitbox.blend( _blend );
			return *this;
		}


        void VisualHitbox::update( const OBB &_obb )
        {
            hitbox.setUniform( "Model", glm::mat4(_obb.orientation) );
			if(flipIt)
				hitbox.setUniform( "Centre", glm::vec2(_obb.centre.x,-_obb.centre.y) );
			else
				hitbox.setUniform( "Centre", _obb.centre);
			hitbox.setUniform( "Z", _obb.z);
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
