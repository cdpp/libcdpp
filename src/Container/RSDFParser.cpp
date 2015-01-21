#include "Container/RSDFParser.h"
#include "basics/aes.h"
#include "Logger.h"
#include "basics/base64.h"

#include <iostream>
#include <regex>
#include <fstream>
#include <climits>
#include <type_traits>

using namespace cdpp;

RSDFParser::RSDFParser()
{
    //ctor
}

RSDFParser::~RSDFParser()
{
    //dtor
}

std::vector<std::string*> RSDFParser::parseFile(std::string filename)
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

std::vector<std::string*> RSDFParser::parse(std::string content)
{
	static Logger logger = Logger::getLogger();
	content.erase(std::remove_if(content.begin(), content.end(), &::isspace), content.end());
	int length = content.length() / 2;
	char buffer[length + 1];
	hex_to_bytes(content, (unsigned char*)buffer);
	std::vector<std::string*> links;
	int buffer_pos = 0;
	char foo[] = {'\x8f', '\x7d', 0x36, 0x66, 0x26, 0x24, 0x30, 0x42, 0xb5, 0x93, 0x80, 0x7e, 0x8d, 0x75};
	char bar[32];
	int kkk = aes_decrypt((unsigned char*)bar, (unsigned char*)foo, 14, key_, iv_, CDPP_AES_CFB);
	bar[kkk] = '\0';
	std::cout << bar << std::endl;
	while(length > 0) {
		int tmp_len = searchPattern(buffer+buffer_pos, length, std_patterns_);
		logger.error(std::to_string(tmp_len));
		char result[tmp_len + 1];
		memcpy(result, buffer+buffer_pos, tmp_len);
		result[tmp_len] = '\0';
		char decoded[base64::calcDecodeLength(result)];
		base64::Base64Decode(result, decoded);
		unsigned char decrypted[base64::calcDecodeLength((char*)result) + 16];
		int decrypted_len = aes_decrypt(decrypted, (unsigned char*)decoded, tmp_len-1, key_, iv_, CDPP_AES_CFB);
		decrypted[decrypted_len] = '\0';
		links.push_back(new std::string((char*)decrypted));
		tmp_len++;
		buffer_pos = tmp_len;
		length -= tmp_len;
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

/** \brief Hex string to unsigned char array.
 * Used a modified version of http://www.codeproject.com/Articles/99547/Hex-strings-to-raw-data-and-back
 * Codeprojects version is licensed under the BSD licencse.
 *
 * \param str Hexstring, e.g. "A0F445FF"
 * \param data unsigned char array of unhexed data
 *
 */

void RSDFParser::hex_to_bytes(const std::string& str, unsigned char* data)
{
	// Sanity check
	static_assert(8 == CHAR_BIT, "CHAR_BIT must / should be 8");
	if (str.empty())
		return;

	// Must be prepared that string can have odd number of
	// nybbles, in which case the first is treated like the low
	// nybble of the first byte
	size_t lengthOverflow = str.length() % 2;

	// This also affects the length of the data buffer we
	// allocate (need full  byte for nybble)
	const size_t length = lengthOverflow + str.length() / 2;

	// Buffer for byte conversion
	static char buf[3];
	buf[2] = 0;
	// End of input
	char* pend = &buf[2];

	// Iterators for input and output
	size_t i = 0;
	size_t c = 0;

	// If the first nybble is a low, we'll do it separately
	if (1 == lengthOverflow) {
		buf[0] = '0';
		buf[1] = str[c++];
		unsigned char x = static_cast<unsigned char> (strtoul(buf, &pend, 16));

		// Parsing should stop at terminating zero
		if (pend != &buf[2]) {
			std::string e = "Invalid character in hex string: \'";
			e += *(pend);
			e += "'";
			throw std::invalid_argument(e);
		}
		data[i++] = x;
	}

	// For each output byte, we use two input characters for
	// high and low nybble, respectively
	for (; i < length; ++i) {
		buf[0] = str[c++];
		// strtoul accepts initial whitespace or sign, we can't
		if (!isxdigit(buf[0])) {
			std::string e = "Invalid character in hex string: \'";
			e += buf[0];
			e += "'";
			throw std::invalid_argument(e);
		}

		buf[1] = str[c++];
		unsigned char x = static_cast<unsigned char>(strtoul(buf, &pend, 16));

		// Parsing should stop at terminating zero
		if (pend != &buf[2]) {
			std::string e = "Invalid character in hex string: \'";
			e += *(pend);
			e += "'";
			throw std::invalid_argument(e);
		}
		data[i] = x;
	}
}
