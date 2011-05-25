#include "Haptics.h"
#include "HLU/hlu.h"
#include "HDU/hdu.h"
#include <glm/gtc/type_ptr.hpp>

namespace de
{
	namespace haptics
	{
		static std::map<int, std::string> activeEffects;
		bool init()
		{
			deviceHandle = hdInitDevice(HD_DEFAULT_DEVICE);
			if (HD_DEVICE_ERROR(hdGetError()))
			{
				de::io::error << "No haptics Device found\n";
				//assert(!"No haptics Device found, Continue?");
				return false;
			}

			hapticsContext = hlCreateContext(deviceHandle);
			hlMakeCurrent(hapticsContext);

			de::io::tests << "Starting Haptics\n";
			return true;
		}

		void workspace( const glm::mat4 &_projection, const glm::mat4 &_modelView )
		{

			hlMatrixMode(HL_TOUCHWORKSPACE);
			hlLoadIdentity();
			//hlWorkspace (-80, -80, -70, 80, 80, 20);
			hluFitWorkspace(glm::value_ptr(glm::dmat4(_projection)));

			hlEnable(HL_HAPTIC_CAMERA_VIEW);
			hlTouchableFace(HL_FRONT);


			/*
			float _modelview[16] = glm::value_ptr(_modelView);
			f
			double modelview[16];
			double projection[16];

			for( int i = 0; i != 16; ++i )
			{
				modelview[i] = _modelview[i];
				projection[i] = _projection[i];
			}
			*/
			/*GLint viewport[4];
    
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			glGetDoublev(GL_PROJECTION_MATRIX, projection);
			glGetIntegerv(GL_VIEWPORT, viewport);
    */
			/*
			hlMatrixMode(HL_TOUCHWORKSPACE);
			hlLoadIdentity();
    */
			// Fit haptic workspace to view volume.
			//hluFitWorkspace(projection);
			
			de::io::tests << "Reseting workspace\n";
		}

		void runFrame()
		{
			hlBeginFrame();
			hlEndFrame();
		}

		int startEffect( const std::string &_type, const glm::vec3 &_position, const glm::vec3 &_direction, const double &_gain, const double &_magnitude )
		{
			HLenum Enumtype = HL_EFFECT_VISCOUS;

			if( _type == "viscous" ) Enumtype = HL_EFFECT_VISCOUS;
			else if( _type == "constant" ) Enumtype = HL_EFFECT_CONSTANT;
			else if( _type == "spring" ) Enumtype = HL_EFFECT_SPRING;
			else if( _type == "friction" ) Enumtype = HL_EFFECT_FRICTION;

			HLdouble pos[3], dir[3];
			pos[0] = _position.x;
			pos[1] = _position.y;
			pos[2] = _position.z;

			dir[0] = _direction.x;
			dir[1] = _direction.y;
			dir[2] = _direction.z;



			HLuint effect = hlGenEffects(1);
			hlBeginFrame();
			hlEffectdv(HL_EFFECT_PROPERTY_POSITION, pos );
			hlEffectdv(HL_EFFECT_PROPERTY_DIRECTION, dir );/*
			hlEffectdv(HL_EFFECT_PROPERTY_POSITION, glm::value_ptr( glm::dvec3(_position ) ) );
			hlEffectdv(HL_EFFECT_PROPERTY_DIRECTION, glm::value_ptr( glm::dvec3(_direction ) ) );*/
			hlEffectd(HL_EFFECT_PROPERTY_GAIN, _gain);
			hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, _magnitude);
			hlStartEffect(Enumtype, effect);
			hlEndFrame();

			//de::io::tests << "Starting Effect: " << _type << "\n";
			activeEffects[effect] = _type;
			return effect;
		}

		void stopEffect( const int &_effect )
		{
			hlBeginFrame();
			hlStopEffect(_effect);
			hlEndFrame();
			hlDeleteEffects(_effect, 1);

			//de::io::tests << "Stopping Effect\n";
			activeEffects.erase(_effect);
		}

		void stopAllEffects()
		{
			std::map<int,std::string>::iterator iter;
			for ( iter=activeEffects.begin(); iter!=activeEffects.end(); iter++ )
			{
				hlBeginFrame();
				hlStopEffect(iter->first);
				hlEndFrame();
				hlDeleteEffects(iter->first, 1);
			}
			activeEffects.clear();

			de::io::tests << "Stopping all Effects\n";
		}

		glm::vec3 getProxy()
		{
			double proxyPosition[3];
			hlGetDoublev(HL_PROXY_POSITION, proxyPosition);

			return glm::vec3(proxyPosition[0], proxyPosition[1], proxyPosition[2]);
		}

		glm::mat4 getProxyTransform()
		{
			double proxyTransform[16];
			hlGetDoublev(HL_PROXY_TRANSFORM, proxyTransform);

			return glm::mat4(glm::dmat4( proxyTransform[0], proxyTransform[1], proxyTransform[2], proxyTransform[3],
										 proxyTransform[4], proxyTransform[5], proxyTransform[6], proxyTransform[7], 
										 proxyTransform[8], proxyTransform[9], proxyTransform[10], proxyTransform[11],
										 proxyTransform[12], proxyTransform[13], proxyTransform[14], proxyTransform[15] ) );
		}				 
	}
}
