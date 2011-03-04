#include "Log.h"
#include "CoreEnumsAndClasses.h"
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace de::classes;
using namespace de::misc;


namespace
{
    using namespace boost::posix_time;
    using namespace boost::gregorian;

    std::string getTimeString()
    {
        ptime now = second_clock::local_time();
        return to_simple_string(now);
    }
}



namespace de
{
    namespace io
    {
        Logger log("../log.txt",  "Starting general logger\n\n");
        Logger error("../error.txt", "Starting error logger\n\n");
        Logger tests("../tests.txt", "Starting test logger\n\n");

        Logger::Logger( const std::string &_filePath, const std::string &_firstLine )
            :needTimeStamp(true), els("","\n","")
        {
            logFile.open(_filePath.c_str());
            print(_firstLine);
        }

        Logger::~Logger()
        {
            logFile.close();
        }

        void Logger::print( const std::string &_message )
        {
            std::string timeStamp = getTimeString();

            boost::tokenizer<boost::escaped_list_separator<char> > tok(_message, els);
            for( beg = tok.begin(); beg != tok.end(); ++beg )
            {
                check = beg;
                ++check;

                if( needTimeStamp && (*beg) != "" )
                {
                    needTimeStamp = false;
                    logFile << timeStamp << ": ";
                }

                if( check == tok.end() )
                {
                    logFile << (*beg);
                }
                else
                {
                    logFile  << (*beg) << "\n";
                    needTimeStamp = true;
                }
            }
        }

        void logMessage( const std::string &_message )
        {
            log << _message << "\n";
        }
    }
}
