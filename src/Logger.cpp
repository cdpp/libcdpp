#include "Logger.h"
#include "Container/DecryptionException.h"

#include <regex>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace cdpp;

Logger& Logger::getLogger()
{
    static Logger logger;
    logger.setLoggingLevel(LOGGING_LEVEL);
    return logger;
}

/********************************************//**
 * \details Set up the logger by using the available logger types.
 *			Filename is only needed if you use Hybrid- or Filelogger.
 * \exception std::invalid_argument If filename is needed but empty.
 * \sa HYBRID_LOGGER
 * \sa FILE_LOGGER
 ***********************************************/

void Logger::setupLogger(uint8_t type, const uint8_t level, const std::string& filename)
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
    level_ = level;
}

void Logger::setLoggingLevel(const uint8_t level)
{
	level_ = level;
}

void Logger::debug(const std::string& message, const std::string& what)
{
	if (level_ <= LOGGING_LEVEL_DEBUG) {
		write(LOGGING_LEVEL_DEBUG, message, what);
    }
}

void Logger::info(const std::string& message)
{
	if (level_ <= LOGGING_LEVEL_INFO) {
		write(LOGGING_LEVEL_INFO, message);
	}
}

void Logger::warn(const std::string& message)
{
	if (level_ <= LOGGING_LEVEL_WARN) {
		write(LOGGING_LEVEL_WARN, message);
    }
}

void Logger::error(const std::string& message, const std::string& what)
{
	if (level_ <= LOGGING_LEVEL_ERROR) {
		write(LOGGING_LEVEL_ERROR, message, what);
	}
}

void Logger::error(const std::string& message, std::exception& throwable)
{
	if (level_ <= LOGGING_LEVEL_ERROR) {
		try {
			write(LOGGING_LEVEL_ERROR, message, dynamic_cast<CdppException&>(throwable).what());
		} catch (std::bad_cast bc) {
			write(LOGGING_LEVEL_ERROR, message, throwable.what());
		}
	}
}

void Logger::fatal(const std::string& message, const std::string& what)
{
	write(LOGGING_LEVEL_FATAL, message, what);
}

void Logger::fatal(const std::string& message, std::exception& throwable)
{
	try {
		write(LOGGING_LEVEL_FATAL, message, dynamic_cast<CdppException&>(throwable).what());
	} catch (std::bad_cast bc) {
		write(LOGGING_LEVEL_FATAL, message, throwable.what());
	}
}

void Logger::write(const uint8_t level, const std::string& message, const std::string& what)
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
								const std::string& what, bool forFile)
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

    std::regex reg_date("%date");
    std::regex reg_message("%msg");
    std::regex reg_color_begin("%highlight\\{");
    std::regex reg_color_end("(\\})");
    std::regex reg_level("%level");
    std::regex reg_ex("%ex\\{([\\r\\t\\n]*)\\}", std::regex_constants::ECMAScript); //Exception / trace
    std::smatch sm;
    std::regex_search(pattern_, sm, reg_ex);
    std::string prefix;
    if (sm.size() < 2) {
		prefix = "\n\t";
    } else {
		prefix = sm.str(1);
    }

	std::string what_m;
	if (!what.empty()) {
		std::regex nl("[\\n\\t\\r]*$");
		what_m = std::regex_replace(what, nl, "");
		nl = std::regex("\\n");
		what_m = prefix + std::regex_replace(what_m, nl, prefix);
	}

    std::string output(std::regex_replace(pattern_, reg_level, getLevelStr(level)));
    output = std::regex_replace(output, reg_ex, what_m);
    output = std::regex_replace(output, reg_color_begin, color);
	output = std::regex_replace(output, reg_color_end, defaultColor, std::regex_constants::format_first_only);
	output = std::regex_replace(output, reg_date, std::string(date));
    output = std::regex_replace(output, reg_message, message);
    return output;
}

std::string Logger::getLevelStr(const uint8_t level)
{
	switch (level) {
		case LOGGING_LEVEL_DEBUG: {
			return std::string("DEBUG  ");
		}
		case LOGGING_LEVEL_INFO: {
			return std::string("INFO   ");
		}
		case LOGGING_LEVEL_WARN: {
			return std::string("WARN   ");
		}
		case LOGGING_LEVEL_ERROR: {
			return std::string("ERROR  ");
		}
		case LOGGING_LEVEL_FATAL: {
			return std::string("FATAL  ");
		}
		default: {
			return std::string("DEFAULT");
		}
	}
}
