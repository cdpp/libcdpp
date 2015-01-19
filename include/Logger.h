#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <stdint.h>

namespace cdpp {
    #define CONSOLE_LOGGER 0x01
    #define FILE_LOGGER 0x10
    #define HYBRID_LOGGER 0x11

    #define LOGGING_COLOR_DEBUG "\033[1;36m"
    #define LOGGING_COLOR_WARN "\033[1;33m"
    #define LOGGING_COLOR_ERROR "\033[1;31m"
    #define LOGGING_COLOR_FATAL "\033[5;41m"
    #define LOGGING_COLOR_DEFAULT "\033[0m"

    class Logger
    {
        public:
            static Logger& getLogger();
            void setupLogger(uint8_t type, std::string filename = "");
            void debug(std::string message);
            void warn(std::string message);
            void error(std::string message);
            void fatal(std::string message);
        private:
            Logger(){};
            void write(std::string message);
            std::string formatMessage(std::string message, std::string level, std::string color);
            uint8_t type_ = CONSOLE_LOGGER;
            std::string filename_;
            std::string pattern_ = "[COLOR][LEVEL] - DATE TIME -[/COLOR] MESSAGE";
    };
}
#endif // LOGGER_H
