#include "RenderStressTest2D.h"

using namespace de::enums;


namespace de
{
	namespace state
			{
		RenderStressTest2D::RenderStressTest2D()
		{
			Engine::Resources().load( "Attrition" );

			srand(10);

			shipNames.push_back( "ships.Enemy1" );
			shipNames.push_back( "ships.Enemy2" );
			shipNames.push_back( "ships.Enemy3" );
			shipNames.push_back( "ships.Enemy4" );
			shipNames.push_back( "ships.Dasein1" );
			shipNames.push_back( "ships.Dasein2" );
			shipNames.push_back( "ships.Dasein3" );
			shipNames.push_back( "ships.Dasein4" );
			shipNames.push_back( "ships.Yukiko" );
			shipNames.push_back( "ships.YukikoOld" );

			shaderNames.push_back( "SpriteInstance" );
			shaderNames.push_back( "SpriteInstanceColour" );


			instance.seed( "ships", "SpriteInstance" );
			instance.seed( "ships", "SpriteInstanceColour" );

			for( int i(0); i != SHIP_AMOUNT; ++i )
			{
				ships[i] = OpaqueSprite( &instance, "ships.Dasein2", "SpriteInstance" );
			}

			glm::mat4 ProjectionView;
			ProjectionView = glm::perspective( 45.0, 16.0/10.0, 200.0, 10000.0 );
			ProjectionView *= glm::lookAt( glm::vec3( 0.0,0.0, 0.0 ),
										   glm::vec3( 0.0,0.0,-1.0 ),
										   glm::vec3( 0.0,1.0, 0.0 ) );

			aSprite =  SpriteObject( "ships", "ships.Enemy1", "SpriteInstance" );

			aSprite.setUniform( "ProjectionView", ProjectionView );
			aSprite.setUniform( "offset", glm::vec3( 0.0 ) );
			aSprite.setUniform( "angle", static_cast<float>(0.0) );
		}

		RenderStressTest2D::~RenderStressTest2D()
		{
		}

		bool RenderStressTest2D::handleEvents( const SDL_Event &_event )
		{
			if( _event.type == SDL_USEREVENT )
			{
				if( _event.user.code == de::enums::events::OPENGL_RELOAD )
				{
					reLoadTextures();
				}
			}

			return true;
		}

		bool RenderStressTest2D::logic( const Uint32 &_deltaTicks, State* &_nextState, options &_options )
		{
			for( int i(0); i != SHIP_AMOUNT; ++i )
			{
				ships[i].change( shipNames[(rand()%10)], OpaqueSprite::SPRITE );
				ships[i].change( shaderNames[(rand()%2)], OpaqueSprite::SHADER );

				ships[i].rotation = static_cast<float>(rand()%2000)/1000.0f;
				ships[i].position = glm::vec3( ( static_cast<float>(rand()%100) - 50.0f )/1.0f,
											   ( static_cast<float>(rand()%100) - 50.0f )/1.5f ,
												 static_cast<float>(rand()%100)/4.0f - 1000.0f );
				ships[i].addThisFrame();
			}
			return false;
		}

		void RenderStressTest2D::reLoadTextures()
		{
			instance.reload();
		}

		void RenderStressTest2D::render()
		{
			//instance.render();
			aSprite.render();
		}
	}
}