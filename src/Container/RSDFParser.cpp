#include "Container/RSDFParser.h"
#include "basics/aes.h"
#include "Logger.h"
#include "basics/bio.h"

#include <iostream>
#include <fstream>
#include <regex>
using namespace cdpp;

RSDFParser::RSDFParser()
{
    //ctor
}

RSDFParser::~RSDFParser()
{
    //dtor
}

std::vector<std::string> RSDFParser::parseFile(std::string filename)
{
	static Logger logger = Logger::getLogger();
	std::string content = "";
	std::ifstream file;
	file.open(filename);
	if(file.is_open()) {
		std::string line = "";
		while (std::getline(file, line)) {
			content.append(line);
		}
		file.close();
	} else {
		logger.error("Could not open file: " + filename);
	}
	return parse(content);
}

std::vector<std::string> RSDFParser::parse(std::string content)
{
	static Logger logger = Logger::getLogger();
	content.erase(std::remove_if(content.begin(), content.end(), &::isspace), content.end());
	int length = content.length() / 2;
	char buffer[length + 1];
	bio::hex_to_bytes(content, (unsigned char*)buffer);
	std::vector<std::string> links;
	int buffer_pos = 0;
	while(length > 0) {
		int tmp_len = searchPattern(buffer+buffer_pos, length, std_patterns_);
		if(tmp_len == 0) {
			length--;
			buffer_pos++;
			continue;
		}
		char result[tmp_len + 1];
		memcpy(result, buffer+buffer_pos, tmp_len);
		result[tmp_len] = '\0';
		char decoded[bio::calcDecodeLength(result)];
		bio::Base64Decode(result, decoded);
		links.push_back(aes_decrypt((unsigned char*)decoded, tmp_len, key_, iv_, CDPP_AES_CFB_192));
		buffer_pos += tmp_len + 1;
		length -= buffer_pos;
	}
	return links;
}

int RSDFParser::searchPattern(const char* str, const int length, const std::vector<char> patterns)
{
	int i = 0;
	for(; i < length; i++) {
		for(char pattern : patterns) {
			if(pattern == *(str+i)) {
				return i;
			}
		}
	}
	return i;
}
