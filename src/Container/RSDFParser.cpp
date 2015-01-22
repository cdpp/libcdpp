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

/********************************************//**
 * \details Parse rsdf content with know key and decryption.
			RSDF is no a secur format, you see key is known.
			But it needs some lines of code, due to much encodings:
			HEX -> Bin / Base64 -> Bin -> AES decrypt -> URL
   \sa AES::decrypt
   \sa CDPP_AES_CFB_192
 ***********************************************/

std::vector<std::string> RSDFParser::parse(std::string content)
{
	static Logger logger = Logger::getLogger();
	AES aes(CDPP_AES_CFB_192, iv_);
	//Remove whitespaces
	content.erase(std::remove_if(content.begin(), content.end(), &::isspace), content.end());
	//It is a hex string so 2 chars in string == 1 Byte
	int length = content.length() / 2;
	char buffer[length + 1];
	//Convert content of file to binary (hex2bin)
	bio::hex_to_bytes(content, (unsigned char*)buffer);
	std::vector<std::string> links;
	int buffer_pos = 0;
	//While not reached end of content search new line (1 line == 1 URL)
	while(length > 0) {
		//Next new line (\n or \r or 0xDA (see header))
		int tmp_len = searchPattern(buffer+buffer_pos, length, std_patterns_);
		//Empty line? Jump over!
		if(tmp_len == 0) {
			length--;
			buffer_pos++;
			continue;
		}
		//Get string from start / last new line to found new line
		char result[tmp_len + 1];
		memcpy(result, buffer+buffer_pos, tmp_len);
		result[tmp_len] = '\0';
		buffer_pos += tmp_len + 1;
		//Decode line of Base64 to binary
		tmp_len = bio::calcDecodeLength(result);
		char decoded[tmp_len];
		bio::Base64Decode(result, decoded);
		//Decrypt line with AES CFB
		links.push_back(aes.decrypt((unsigned char*)decoded, tmp_len, key_));
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
