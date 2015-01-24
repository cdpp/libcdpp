#ifndef RSDFPARSER_H
#define RSDFPARSER_H
/********************************************//**
 * \file RSDFParser.h
***********************************************/
#include "Container.h"
#include <string>
#include <vector>

namespace cdpp {
    /********************************************//**
     * \class RSDFParser
	 * \brief Class for parsing RSDF link container
	 * \implements Container
     ***********************************************/
    class RSDFParser : public Container
    {
        public:
        	//! Standard constructor
            RSDFParser();
            //! Standard destructor
            virtual ~RSDFParser();
            /********************************************//**
			 * \brief Parse given content of rsdf file
             * \param content Content of file
             * \return List of decrypted URLs
             ***********************************************/
            std::vector<std::string> parse(std::string content);
            /********************************************//**
			 * \brief Parse given rsdf file
             * \param filename Name and path of file, which should be parsed
             * \return List of decrypted URLs
             ***********************************************/
            std::vector<std::string> parseFile(const std::string &filename);
        protected:
        private:
            const unsigned char key_[24] = {0x8C, 0x35, 0x19, 0x2D, 0x96, 0x4D,
                                            0xC3, 0x18, 0x2C, 0x6F, 0x84, 0xF3,
                                            0x25, 0x22, 0x39, 0xEB, 0x4A, 0x32,
                                            0x0D, 0x25, 0x00, 0x00, 0x00, 0x00};
            const unsigned char iv_[16] = { 0xa3, 0xd5, 0xa3, 0x3c, 0xb9, 0x5a,
                                            0xc1, 0xf5, 0xcb, 0xdb, 0x1a, 0xd2,
                                            0x5c, 0xb0, 0xa7, 0xaa};
			//! Characters to splitt lines, 0xDA because of some fails (linkcrypt.ws)
        	const std::vector<char> std_patterns_ = {'\n', '\r', '\xda'};
        	//! Returns position of the occurrence of the pattern
			int searchPattern(const char* str, const int length, const std::vector<char> &patterns);
    };
}
#endif // RSDFPARSER_H
