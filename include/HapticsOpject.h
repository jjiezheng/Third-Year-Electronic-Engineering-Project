#ifndef HAPTICSOBJECT_H
#define HAPTICSOBJECT_H
#include "renderobject.h"
#include "modelObject.h"


class hapticsOpject
{
    public:
        /** Default constructor */
        hapticsOpject();
        /** Default destructor */
        virtual ~hapticsOpject();

		template<typename T>
		hapticsOpject& setUniform( const std::string &_name, const T &_value )
		{
			mesh.setUniform( _name, _value );
			return *this;
		}
		hapticsOpject& writeToDepth( bool _writeToDepth );
		hapticsOpject& depth( bool _depth );
		hapticsOpject& alpha( bool _alpha );
		hapticsOpject& blend( bool _blend );

		hapticsOpject& setType( const std::string &_type );
		hapticsOpject& load( const std::string &_mesh, const std::string &_shader );
		hapticsOpject& setTexture( const std::string &_texture, const std::string &_value );

		void reload();
		void render();
		void actualRender();
	private:
			
		modelObject mesh;
		int hapticShape;
        
};

#endif // HAPTICSOBJECT_H
