#include "container/CCFParser.h"

#include "container/DecryptionException.h"
#include "basics/aes.h"

#include <pugixml.hpp>
#include <fstream>
#include <cstring>

using namespace cdpp;

CCFParser::CCFParser()
{
	//ctor
}

CCFParser::~CCFParser()
{
	//dtor
}

std::vector<Package> CCFParser::parseFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
		std::string data;
		data.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		if (!file.read(&data[ 0 ], data.size())) {
			logger_.error("Could not read from file: " + filename);
			return std::vector<cdpp::Package>(0);
		}
		file.close();
		return parse(data);
	} else {
		logger_.error("Could not open file: " + filename);
	}
	return std::vector<cdpp::Package>(0);
}

std::vector<Package> CCFParser::parse(const std::string& content)
{
	std::vector<cdpp::Package> packages;
	if (content.substr(0, 6) == "CCF3.0") {
		logger_.debug("CCF 3.0");
		try {
			packages = parseCCF3(content.substr(6));
		} catch (std::length_error le) {
			logger_.error("Catched exception in CCFParser::parse():", le);
		}
		return packages;
	} else {
		try {
			packages = parseCCF5(content);
		} catch (CdppParseException cp) {
			//logger_.
		}
		if (packages.empty()) {
			//packages = parseWithDLC();
		}
	}
	if (packages.empty())
		logger_.debug("CCF Not Known");
	return packages;
}

std::vector<Package> CCFParser::parseCCF3(const std::string& content)
{
	uint32_t magic = 0;
	magic  = content.at(3) << 24;
	magic |= content.at(2) << 16;
	magic |= content.at(1) << 8;
	magic |= content.at(0);
	uint8_t padding = 64 - content.at(4);

	if ((content.size() - 5) % 64 != 0) {
		throw std::length_error("CCF Content length is not a multiple of 64");
	}
	std::string decoded;
	for (size_t i = 0; i < (content.size() - 5) / 64; i++) {
		std::string block = ccf3DoBlock(content.substr(5 + i*64, 64), magic);
		for(size_t j = 0; j < 8; j++) {
			decoded += ccf3DoLine(block.substr(j * 8, 8), magic);
		}
	}
	std::ofstream of;
	of.open("/home/ferdinand/ccf-decoded.ccf");
	of << decoded;
	of.close();
	std::vector<Package> packages;
	try {
		packages = ccfDoXML(decoded.substr(0, decoded.length() - padding));
	} catch (CdppException ex) {
		logger_.error("Catched exception in CCFParser::parseCCF3():", ex);
	}
	return packages;
}

std::string CCFParser::ccf3DoBlock(std::string block, uint32_t& magic)
{
	for (size_t i = 0; i < 64; i++) {
		if ((magic & 1) == 0) {
			uint8_t bits = (magic & 0xff) % 9;
			rotateLeft(magic, bits);
		} else {
			uint8_t bits = (magic & 0xff) % 12;
			rotateRight(magic, bits);
		}
		// XOR every byte with the last byte of magic
		block[i] = block[i] ^ ((uint8_t)(magic & 0xFF));
		//Swap rows and collums
		for (size_t y = 0; y < 7; y++) {
			for (size_t x = (y + 1); x < 8; x++) {
				std::swap(block[y*8+x], block[x*8+y]);
			}
		}
	}
	return block;
}

std::string CCFParser::ccf3DoLine(std::string line, uint32_t& magic)
{
	for (size_t i = 0; i < 8; i++) {
		// XOR every byte with the last byte of magic
		line[i] = line.at(i) ^ ((uint8_t)(magic & 0xFF));
		//Swap bits (8x8 bit matrix)
		swapBits(line);
		if ((magic & 1) == 0) {
			rotateLeft(magic, magic & 9);
		} else {
			rotateRight(magic, magic & 12);
		}
	}
	return line;
}

std::vector<Package> CCFParser::ccfDoXML(const std::string& xmlData)
{
	std::vector<Package> packages;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(xmlData.c_str());
	if (!result) {
		logger_.debug(std::string("CCF XML-Error: ") + result.description());
		throw CdppIOException("CCFParser::ccfDoXML(): CCF XML-Error");
	}
	pugi::xml_node content = doc.child("CryptLoad");
	if(!content) {
		logger_.debug("Malformed");
		throw CdppParseException("Got malformed XML file.");
	}
	for (pugi::xml_node pkg = content.child("Package"); pkg; pkg = content.next_sibling("Package")) {
		Package package;
		package.setName(pkg.attribute("name").value());
		pugi::xml_node tmp = pkg.child("Options");
		if (tmp) {
			tmp = tmp.child("Passwort");
			if (tmp) {
				package.setPassword(tmp.value());
			}
		}
		for (pugi::xml_node download = pkg.child("Download"); download; download = download.next_sibling("Download")) {
			FileInfo m_file;
			m_file.url = download.attribute("Url").value();
			if (m_file.url.empty() ||
				m_file.url.size() < strlen(download.child_value("Url")))
			{
				m_file.url = download.child_value("Url");
			}
			m_file.name = download.child_value("FileName");
			m_file.size = std::stol(download.child_value("FileSize"));
			package.addFile(m_file);
		}
		packages.push_back(package);
	}
	return packages;
}

void CCFParser::swapBits(std::string& matrix)
{
	// Swap rows and collums
//	for (uint8_t y = 0; y < 7; y++) {
//		for (uint8_t x = (y+1); x < 8; x++) {
//			uint8_t buffer = matrix.at(x);
//			matrix[y] &= ~(1 << x);
//			matrix[y] |= (1 << x)
//
//		}
//	}
	const char mask[] = { 1, 2, 4, 8, 16, 32, 64, (char)128 };
	std::string ret;
	ret.resize(8);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((matrix[i] & mask[j]) != 0) {
				ret[j] |= mask[i];
			}
		}
	}
	matrix = ret;
}

std::vector<Package> CCFParser::parseCCF5(const std::string& content)
{
	const unsigned char key[] = {	0x5F, 0x67, 0x9C, 0x00, 0x54, 0x87, 0x37, 0xE1,
									0x20, 0xE6, 0x51, 0x8A, 0x98, 0x1B, 0xD0, 0xBA,
									0x11, 0xAF, 0x5C, 0x71, 0x9E, 0x97, 0x50, 0x29,
									0x83, 0xAD, 0x6A, 0xA3, 0x8E, 0xD7, 0x21, 0xC3
								};

	const unsigned char iv[] =	{	0xE3, 0xD1, 0x53, 0xAD, 0x60, 0x9E, 0xF7, 0x35,
									0x8D, 0x66, 0x68, 0x41, 0x80, 0xC7, 0x33, 0x1A
								};
	AES aes(CDPP_AES_CBC_256, iv);
	std::string decrypted;
	decrypted = aes.decrypt((unsigned char*)content.c_str(), content.length(), key);
	std::vector<Package> packages;
	try {
		packages = ccfDoXML(decrypted);
	} catch (CdppException cd) {
		logger_.error("Catched Exception in CCFParser::parseCCF5()", cd);
	}
	return packages;
}
