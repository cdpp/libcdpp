/*  Copyright (C) 2015 CDPP

	This file is part of libcdpp.

    libcdpp is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    libcdpp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef RSDFPARSER_H
#define RSDFPARSER_H
/********************************************//**
 * \file RSDFParser.h
***********************************************/
#include "Container.h"
#include "data/Package.h"
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
            std::vector<cdpp::Package> parse(const std::string& content);
            /********************************************//**
			 * \brief Parse given rsdf file
             * \param filename Name and path of file, which should be parsed
             * \return List of decrypted URLs
             ***********************************************/
            std::vector<cdpp::Package> parseFile(const std::string &filename);
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
