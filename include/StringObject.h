#ifndef STRINGOBJECT_H
#define STRINGOBJECT_H

#include "renderObject.h"
#include "Object3D.h"

typedef boost::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4> ShaderVariant;


class StringObject : public de::graphics::renderObject
{
	public:
		StringObject(void);
		virtual ~StringObject(void);

		template<typename T>
        StringObject& setUniform( const std::string &_name, const T &_value )
        {
            renderableText.setUniform( _name, _value );

			boost::variant<T> localVariant( _value );
			Uniforms[_name] = localVariant;

            return *this;
        }

		StringObject& font( const std::string &_font );
		StringObject& text( const std::string &_text );
		StringObject& shader( const std::string &_shader );

		StringObject& writeToDepth( bool _depth );
		StringObject& depth( bool _depth );
		StringObject& alpha( bool _alpha );
		StringObject& blend( bool _blend );

		
		glm::vec3& align();
		glm::vec2 getSize();

		void reload();
        void render();
		void actualRender();
	private:
		
		void rebuild();

		std::map<std::string, ShaderVariant> Uniforms;
		Object3D renderableText;
		std::string textString, fontType, shaderType;
		glm::vec3 halfLength;
		glm::vec2 size;
		bool refresh;
};


#endif //STRINGOBJECT_H