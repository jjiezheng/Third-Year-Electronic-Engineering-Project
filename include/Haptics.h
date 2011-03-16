#ifndef HAPTICS_H
#define HAPTICS_H
#include "CoreEngine.h"
#include "HL\hl.h"
#include "HD\hd.h"


namespace de
{
	namespace haptics
	{
		static HHD deviceHandle;
		static HHLRC hapticsContext;

		bool init();
		void workspace( const glm::mat4 &_projection, const glm::mat4 &_modelView );
		void runFrame();
		int startEffect( const std::string &_type, 
					     const glm::vec3 &_position, 
						 const glm::vec3 &_direction, 
						 const double &_gain, 
						 const double &_magnitude );
		void stopEffect( const int &_effect );
	}
}

#endif // HAPTICS_H
