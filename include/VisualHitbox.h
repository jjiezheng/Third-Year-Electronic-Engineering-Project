#ifndef VISUALHITBOX_H
#define VISUALHITBOX_H

#include "OBB.h"
#include "Object3D.h"

namespace de
{
    namespace collision
    {
        class VisualHitbox
        {
            public:
                /** Default constructor */
                VisualHitbox();
                VisualHitbox( const OBB &_obb );
                /** Default destructor */
                virtual ~VisualHitbox();

                template<typename T>
                void setUniform( const std::string &_name, const T &_value )
                {
                    hitbox.setUniform( _name, _value );
                }
				VisualHitbox& writeToDepth( bool _depth );
				VisualHitbox& depth( bool _depth );
				VisualHitbox& alpha( bool _alpha );
				VisualHitbox& blend( bool _blend );

				void flip( const bool &_flipIt );
                void update( const OBB &_obb );
                void reload();
                void render();
            protected:
            private:
                Object3D hitbox;
				bool flipIt;
        };
    }
}

#endif // VISUALHITBOX_H
