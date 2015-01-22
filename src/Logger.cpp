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

void Logger::setupLogger(uint8_t type, std::string filename)
{
    if (type == CONSOLE_LOGGER) {
        type_ = CONSOLE_LOGGER;
    } else if (type == FILE_LOGGER || type == HYBRID_LOGGER) {
        if(filename.empty())
            throw new std::invalid_argument("filename is empty!");
        type_ = type;
        filename_ = filename;
    } else {
        type_ = CONSOLE_LOGGER;
    }
}

void Logger::debug(std::string message) {
    write(message, LOGGING_LEVEL_DEBUG);
}
void Logger::warn(std::string message) {
    write(message, LOGGING_LEVEL_WARN);
}
void Logger::error(std::string message) {
    write(message, LOGGING_LEVEL_ERROR);
}
void Logger::fatal(std::string message) {
    write(message, LOGGING_LEVEL_FATAL);
}

void Logger::write(const std::string message, const uint8_t level)
{
    if(type_ == CONSOLE_LOGGER || type_ == HYBRID_LOGGER) {
        std::cout << formatMessage(message, level) << std::endl;
    }
    if(type_ == FILE_LOGGER || type_ == HYBRID_LOGGER) {
        std::fstream a;
        a.open(filename_);
        a << formatMessage(message, level, true) << '\n';
        a.close();
    }
}

std::string Logger::formatMessage(const std::string message, const uint8_t level, bool forFile)
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

    std::string output(std::regex_replace(pattern_, reg_level, getLevelStr(level)));
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
			return std::string("DEBUG");
		}
		case LOGGING_LEVEL_WARN: {
			return std::string("WARN");
		}
		case LOGGING_LEVEL_ERROR: {
			return std::string("ERRO");
		}
		case LOGGING_LEVEL_FATAL: {
			return std::string("FATAL");
		}
		default: {
			return std::string("DEFAULT");
		}
	}
}
