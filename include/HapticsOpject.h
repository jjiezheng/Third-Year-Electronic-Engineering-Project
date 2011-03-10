#ifndef HAPTICSOBJECT_H
#define HAPTICSOBJECT_H
#include "renderobject.h"

namespace de
{
    class HapticsOpject
    {
        public:
            /** Default constructor */
            HapticsOpject();
            /** Default destructor */
            virtual ~HapticsOpject();

            void reload();
            void render();
            void actualRender();
        protected:
        private:
			
			//HLuint hapticsShape;
    };
}

#endif // HAPTICSOBJECT_H
