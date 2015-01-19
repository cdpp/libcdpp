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
    write(formatMessage(message, std::string("DEBUG"), std::string(LOGGING_COLOR_DEBUG)));
}
void Logger::warn(std::string message) {
    write(formatMessage(message, std::string("WARN"), std::string(LOGGING_COLOR_WARN)));
}
void Logger::error(std::string message) {
    write(formatMessage(message, std::string("ERROR"), std::string(LOGGING_COLOR_ERROR)));
}
void Logger::fatal(std::string message) {
    write(formatMessage(message, std::string("FATAL"), std::string(LOGGING_COLOR_FATAL)));
}

void Logger::write(std::string message)
{
    if(type_ == CONSOLE_LOGGER || type_ == HYBRID_LOGGER) {
        std::cout << message << std::endl;
    }
    if(type_ == FILE_LOGGER || type_ == HYBRID_LOGGER) {
        std::fstream a;
        a.open(filename_);
        a << message << '\n';
        a.close();
    }
}

std::string Logger::formatMessage(std::string message, std::string level, std::string color)
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime(&t);
    std::regex reg_date("(DATE)");
    std::regex reg_message("(MESSAGE)");
    std::regex reg_level("LEVEL");
    std::regex reg_color_begin("(\\[COLOR\\])");
    std::regex reg_color_end("(\\[/COLOR\\])");
    char time[10];
    strftime(time, 10, "%T", now);
    char date[12];
    strftime(date, 12, "%F", now);
    std::string output(std::regex_replace(pattern_, std::regex("TIME"), std::string(time)));
    output = std::regex_replace(output, reg_level, level);
    output = std::regex_replace(output, reg_color_begin, color);
    output = std::regex_replace(output, reg_color_end, std::string(LOGGING_COLOR_DEFAULT));
    output = std::regex_replace(output, reg_date, std::string(date));
    output = std::regex_replace(output, reg_message, message);
    return output;
}
