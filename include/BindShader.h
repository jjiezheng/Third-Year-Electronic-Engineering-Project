#ifndef BINDSHADER_H
#define BINDSHADER_H

#include <boost/variant.hpp>
#include <glm/gtc/type_ptr.hpp>


class BindShader : public boost::static_visitor<>
{
    public:
        void operator()( int &_uniformAddress, float &_value ) const;
        void operator()( int &_uniformAddress, int &_value ) const;
        void operator()( int &_uniformAddress, glm::vec2 &_value ) const;
        void operator()( int &_uniformAddress, glm::vec3 &_value ) const;
        void operator()( int &_uniformAddress, glm::vec4 &_value ) const;
        void operator()( int &_uniformAddress, glm::mat3 &_value ) const;
        void operator()( int &_uniformAddress, glm::mat4 &_value ) const;
};

class AreSameType : public boost::static_visitor<bool>
{
    public:
        template <typename T, typename U>
        bool operator()( const T &, const U & ) const
        {
            return false;
        }

        template <typename T>
        bool operator()( const T & lhs, const T & rhs ) const
        {
            return true;
        }
};

class ChangeButKeepType : public boost::static_visitor<bool>
{
    public:
        template <typename T, typename U>
        bool operator()( T &, U & ) const
        {
            return false;
        }

        template <typename T>
        bool operator()( T & lhs, T & rhs ) const
        {
            lhs = rhs;
            return true;
        }
};

#endif // BINDSHADER_H
