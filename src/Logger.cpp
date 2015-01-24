#include "Logger.h"

#include <stdexcept>
#include <regex>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace cdpp;

Logger& Logger::getLogger()
{
    static Logger logger;
    return logger;
}

/********************************************//**
 * \details Set up the logger by using the available logger types.
 *			Filename is only needed if you use Hybrid- or Filelogger.
 * \exception std::invalid_argument If filename is needed but empty.
 * \sa HYBRID_LOGGER
 * \sa FILE_LOGGER
 ***********************************************/

void Logger::setupLogger(uint8_t type, const std::string& filename)
{
    if (type == CONSOLE_LOGGER) {
        type_ = CONSOLE_LOGGER;
    } else if ((type == FILE_LOGGER) || (type == HYBRID_LOGGER)) {
        if(filename.empty())
            throw new std::invalid_argument("filename is empty!");
        type_ = type;
        filename_ = filename;
    } else {
        type_ = CONSOLE_LOGGER;
    }
}

void Logger::debug(const std::string& message)
{
    write(LOGGING_LEVEL_DEBUG, message);
}

void Logger::info(const std::string& message)
{
    write(LOGGING_LEVEL_INFO, message);
}

void Logger::warn(const std::string& message)
{
    write(LOGGING_LEVEL_WARN, message);
}

void Logger::error(const std::string& message, const std::string& what)
{
    write(LOGGING_LEVEL_ERROR, message, std::vector<std::string>({what}));
}

void Logger::error(const std::string& message, const err_trace& what)
{
	std::vector<std::string> m_what(what.size());
	for(std::pair<std::string, std::string> trace : what)
		m_what.push_back(std::string("ERROR: ") + trace.first + std::string(" :::: FUNCTION: ") + trace.second);
    write(LOGGING_LEVEL_ERROR, message, m_what);
}

void Logger::fatal(const std::string& message)
{
    write(LOGGING_LEVEL_FATAL, message);
}

void Logger::write(const uint8_t level, const std::string& message, const std::vector<std::string>& what)
{
    if(type_ == CONSOLE_LOGGER || type_ == HYBRID_LOGGER) {
        std::cout << formatMessage(level, message, what) << std::endl;
    }
    if((type_ == FILE_LOGGER) || (type_ == HYBRID_LOGGER)) {
        std::ofstream a(filename_, std::ios::app);
        if(!a.is_open() || !a.good()) {
			std::cerr << "Could not open file, logger will not work." << std::endl;
			return;
        }
        a << formatMessage(level, message, what, true) << std::endl;
        a.close();
    }
}

std::string Logger::formatMessage(const uint8_t level, const std::string& message,
								const std::vector<std::string>& what, bool forFile)
{
    time_t t = time(0);   // get time now
    char date[24];
    strftime(date, 24, "%F %T", localtime(&t));
    std::string defaultColor, color;
	if (forFile) {
		//No color in file
		color = "";
		defaultColor = "";
    } else {
    	color = color_[level];
		defaultColor = color_[LOGGING_LEVEL_DEFAULT];
    }

	std::string m_what = "";
	for (std::string trace : what) {
		m_what.append("\t");
		m_what.append(trace);
		m_what.append("\n");
	}

    std::regex reg_date("%date");
    std::regex reg_message("%msg");
    std::regex reg_color_begin("%highlight\\{");
    std::regex reg_color_end("(\\})");
    std::regex reg_level("%level");
    std::regex reg_ex("%ex"); //Exception / trace

    std::string output(std::regex_replace(pattern_, reg_level, getLevelStr(level)));
    output = std::regex_replace(output, reg_color_begin, color);
	output = std::regex_replace(output, reg_color_end, defaultColor, std::regex_constants::format_first_only);
	output = std::regex_replace(output, reg_date, std::string(date));
    output = std::regex_replace(output, reg_message, message);
    output = std::regex_replace(output, reg_ex, m_what);
    return output;
}

std::string Logger::getLevelStr(const uint8_t level)
{
	switch (level) {
		case LOGGING_LEVEL_DEBUG: {
			return std::string("DEBUG");
		}
		case LOGGING_LEVEL_INFO: {
			return std::string("INFO");
		}
		case LOGGING_LEVEL_WARN: {
			return std::string("WARN");
		}
		case LOGGING_LEVEL_ERROR: {
			return std::string("ERROR");
		}
		case LOGGING_LEVEL_FATAL: {
			return std::string("FATAL");
		}
		default: {
			return std::string("DEFAULT");
		}
	}
}
