#ifndef VBO_H
#define VBO_H

#include "Base.h"

namespace de
{
	namespace graphics
	{
		//Move these from here.
		struct AttInfo
		{
			int size, offset, stride;
		};

		class VBO
		{
			public:
				VBO();
				~VBO();
				bool setup();
				void render();
			private:
				int vboVerts, vboElems;
				std::map< std::string, AttInfo >  Attribs;
		};
	}
}

#endif // VBO_H