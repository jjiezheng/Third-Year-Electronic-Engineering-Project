#ifndef RENDERSTRESSTEST2D_H
#define RENDERSTRESSTEST2D_H

#include "State.h"
#include "Object3D.h"
#include "SpriteInstance.h"
#include "SpriteObject.h"


#define SHIP_AMOUNT 400

namespace de
{
	namespace state
	{
		class RenderStressTest2D : public State
		{
			public:
				/** Default constructor */
				RenderStressTest2D();
				/** Default destructor */
				virtual ~RenderStressTest2D();

				bool handleEvents( const SDL_Event &_event );
				bool logic( const Uint32 &_deltaTicks, State* &_nextState, options &_options );

				void render();

			private:
				void reLoadTextures();

				SpriteInstance instance;
				OpaqueSprite ships[SHIP_AMOUNT];
				SpriteObject aSprite;

				std::vector<std::string> shipNames;
				std::vector<std::string> shaderNames;
		};
	}
}

#endif // RENDERSTRESSTEST2D_H
