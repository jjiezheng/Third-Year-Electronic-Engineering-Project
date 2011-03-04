#include "AttritionTestState.h"
#include "GameServices.h"

namespace Attrition
{
   AttritionTestState::AttritionTestState()
    {
        de::Engine::Resources().load( "Attrition" );

        proView = glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 1000.0f );
        proView *= glm::translate( glm::mat4(1.0f), glm::vec3(0.0,0.0f,-200.0f) );

        hitbox = de::collision::OBB( 100, 100 );
        hitbox.transform( glm::vec2( 0, 150 ) );
        showHitbox = de::collision::VisualHitbox( hitbox );
        showHitbox.setUniform( "Colour", glm::vec3(0.16863,0.1451,0.1647) );
        showHitbox.setUniform( "ProjectionView", proView );

        hitbox2 = de::collision::OBB( 100, 100 );
        hitbox2.transform( glm::vec2( 150, 0 ) );
        showHitbox2 = de::collision::VisualHitbox( hitbox2 );
        showHitbox2.setUniform( "Colour", glm::vec3(0.16863,0.1451,0.1647) );
        showHitbox2.setUniform( "ProjectionView", proView );
    }

    AttritionTestState::~AttritionTestState()
    {
        //dtor
    }

    bool AttritionTestState::handleEvents( const SDL_Event &_event )
    {

        if( _event.type == SDL_MOUSEMOTION )
        {
            position = de::events::MousePosition( _event );
            position.x -= 640;
            position.y -= 400;

            de::io::tests << "position:" << position.x << " " << position.y << "\n";

/*
unProject	(	detail::tvec3< T > const & 	win,
            detail::tmat4x4< T > const & 	model,
            detail::tmat4x4< T > const & 	proj,
            detail::tvec4< U > const & 	viewport	*/

            position = glm::unProject( position, glm::mat4(1.0f), proView, glm::vec4(1.0f) );

            de::io::tests << "position:" << position.x << " " << position.y << "\n\n";
        }

        if( _event.type == SDL_USEREVENT )
        {
            if( _event.user.code == de::enums::events::OPENGL_RELOAD )
            {
                reLoadTextures();
            }
        }
        return true;
    }

    bool AttritionTestState::logic( const Uint32 &_deltaTicks, State* &_nextState, de::state::options &_options )
    {
        static float rotation = 0;
        hitbox.transform( glm::vec2(0,-0.3));
        hitbox2.transform( glm::vec2(-0.3,0));
        rotation += (float)_deltaTicks/20.0f;

        hitbox.rotate( rotation );
        hitbox2.rotate( -rotation/2.0f );
        showHitbox.update( hitbox );
        showHitbox2.update( hitbox2 );

        if( de::collision::TestOBBOBB( hitbox2, hitbox ) )
        {
            showHitbox.setUniform( "Colour", glm::vec3(1.0,0.1451,0.1647) );
        }
        else
        {
            showHitbox.setUniform( "Colour", glm::vec3(0.16863,0.1451,0.1647) );
        }

        if( hitbox2.containsPoint( position ) )
        {
            showHitbox2.setUniform( "Colour", glm::vec3(1.0,1.0,0.1647) );
        }
        else
        {
            showHitbox2.setUniform( "Colour", glm::vec3(0.16863,0.1451,0.1647) );
        }

        return false;
    }

    void AttritionTestState::reLoadTextures()
    {
        showHitbox.reload();
        showHitbox2.reload();
    }

    void AttritionTestState::render()
    {
        showHitbox.render();
        showHitbox2.render();
    }
}

