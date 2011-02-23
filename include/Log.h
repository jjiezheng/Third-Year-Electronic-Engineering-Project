#ifndef LOG_H
#define LOG_H

#include <sstream>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>


namespace de
{
    namespace misc
    {
        //!  returns the passed value as a string as long as "<<" for "T" is overloaded correctly
        template <class T>
        std::string toString( T &_value )
        {
            std::stringstream out;
            out << _value;

            return out.str();
        }
    }

    namespace io
    {
        class Logger
        {
            public:

                Logger( const std::string &_filePath, const std::string &_firstLine );
                ~Logger();

                template<typename T>
                Logger& operator<< ( const T &_message )
                {
                    this->print( de::misc::toString(_message) );
                    return *this;
                }


            private:
                void print( const std::string &_message );

                bool needTimeStamp;

                boost::escaped_list_separator<char> els;
                boost::tokenizer<boost::escaped_list_separator<char> >::iterator beg, check;
                std::ofstream logFile;
        };

        void logMessage( const std::string &_message );

        extern Logger log, tests, error;
    }
}

#endif // LOG_H
