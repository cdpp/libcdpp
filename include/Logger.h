#ifndef LOGGER_H
#define LOGGER_H
/********************************************//**
 * \file Logger.h
 * \brief Simple logger class
 ***********************************************/
#include "cdpp.h"
#include <type_traits>
#include <stdexcept>
#include <utility>
#include <stdint.h>

#ifndef LOGGING_LEVEL
  #ifdef NDEBUG
	#define LOGGING_LEVEL LOGGING_LEVEL_WARN
  #else
	#define LOGGING_LEVEL LOGGING_LEVEL_DEBUG
  #endif // NDEBUG
#endif // LOGGING_LEVEL

namespace cdpp {
	/********************************************//**
	* \def CONSOLE_LOGGER
	* Defines to log to the console only
	***********************************************/
    #define CONSOLE_LOGGER 0x01
    /********************************************//**
	* \def FILE_LOGGER
	* Defines to log to a file only
	***********************************************/
    #define FILE_LOGGER 0x10
    /********************************************//**
	* \def HYBRID_LOGGER
	* Defines to log to the console and to a file.
	***********************************************/
    #define HYBRID_LOGGER 0x11

	#define LOGGING_LEVEL_DEFAULT 0x00
	#define LOGGING_LEVEL_DEBUG 0x01
	#define LOGGING_LEVEL_INFO 0x02
	#define LOGGING_LEVEL_WARN 0x03
	#define LOGGING_LEVEL_ERROR 0x04
	#define LOGGING_LEVEL_FATAL 0x05

    #define LOGGING_COLOR_DEBUG "\033[1;36m"
    #define LOGGING_COLOR_INFO "\033[1;32m"
    #define LOGGING_COLOR_WARN "\033[1;33m"
    #define LOGGING_COLOR_ERROR "\033[1;31m"
    #define LOGGING_COLOR_FATAL "\033[1;5;31;47m"
    #define LOGGING_COLOR_DEFAULT "\033[0m"

    /********************************************//**
     * \class Logger
	 * \brief Simple logger
    ***********************************************/
    class Logger
    {
        public:
            /********************************************//**
			 * \brief Get Logger instance
             * \return Static logger instance (Singleton)
             ***********************************************/
            static Logger& getLogger();
            /********************************************//**
			 * \brief Setup type of logger and if needed filename where to log
             * \param type Type of logger
             * \param level Logging level
             * \param filename If Hybrid- or filelogger this is the file where to log
             ***********************************************/
            void setupLogger(uint8_t type, const uint8_t level = LOGGING_LEVEL_WARN, const std::string& filename = "");
            /********************************************//**
			 * \brief Setting logging level
             * \param level Logging level
             * \sa LOGGING_LEVEL_DEBUG LOGGING_LEVEL_INFO LOGGING_LEVEL_WARN LOGGING_LEVEL_ERROR LOGGING_LEVEL_FATAL
             ***********************************************/
            void setLoggingLevel(const uint8_t level);
            void debug(const std::string& message, const std::string& what = "");
            void debug(const std::string& message, std::exception& throwable);
            void info(const std::string& message);
            void warn(const std::string& message);
            void error(const std::string& message, const std::string& what = "");
            void error(const std::string& message, std::exception& throwable);
            void fatal(const std::string& message, const std::string& what = "");
            void fatal(const std::string& message, std::exception& throwable);
        private:
            Logger(){}; //!< Singleton
            inline std::string getLevelStr(const uint8_t level);
            void write(const uint8_t level, const std::string& message, const std::string& what = "");
            template<typename Base, typename T>
			inline bool instanceof(const T*) {
				return std::is_base_of<Base, T>::value;
			}
            /********************************************//**
			 * \brief Formats message to fit the given pattern
             * \param message Message to log
             * \param level Loglevel
             * \param forFile Defines if this format is for file logging (no color)
             * \return Formated message
             ***********************************************/
            std::string formatMessage(uint8_t level, const std::string& message, const std::string& what, bool forFile = false);

            uint8_t type_ = CONSOLE_LOGGER;
            uint8_t level_ = LOGGING_LEVEL;
            std::string filename_;
            const std::string pattern_ = "%highlight{%level - %date} - %msg%ex{\n\t}";
            const std::string color_[6] = {	LOGGING_COLOR_DEFAULT,	LOGGING_COLOR_DEBUG,
											LOGGING_COLOR_INFO,		LOGGING_COLOR_WARN,
											LOGGING_COLOR_ERROR,	LOGGING_COLOR_FATAL};
    };
}
#endif // LOGGER_H
