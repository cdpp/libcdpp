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

#ifndef CCFPARSER_H
#define CCFPARSER_H

#include "Container.h"
#include "../basics/Logger.h"
#include <string>

//Check if x86 architecture (GCC and MSVC) used for inline asm
#if (defined __i386__ || defined __amd64__ || defined _M_IX86 || defined _M_X64)
	#define X86
#endif

namespace cdpp {
	/********************************************//**
	* \class CCFParser
	* \brief Decrypts CCF Container, native decryption for CCF 3.0
	* \implements Container
	***********************************************/
	class CCFParser : public Container
	{
		public:
			/** Default constructor */
			CCFParser();
			/** Default destructor */
			virtual ~CCFParser();
			/********************************************//**
			 * \brief Function to parse content of given container
             * \param content Content of container file
             * \return Decrypted / Decoded content
             ***********************************************/
			std::vector<cdpp::Package> parse(const std::string& content);
			/********************************************//**
			 * \brief Function to parse given container file
             * \param filename File name of container-file
             * \return Decrypted / Decoded content of file
             ***********************************************/
            std::vector<cdpp::Package> parseFile(const std::string& filename);
		private:
			/* Attributes */
			Logger logger_ = Logger::getLogger();
			/* Methodes */
			std::vector<Package> parseCCF3(const std::string& content);
			std::vector<Package> parseCCF5(const std::string& content);
			std::string ccf3DoBlock(std::string block, uint32_t& magic);
			std::string ccf3DoLine(std::string line, uint32_t& magic);
			std::vector<Package> ccfDoXML(const std::string& data);
			void swapBits(std::string& matrix);
			/* Basic functions -> maybe asm optimized */
			inline void rotateLeft(uint32_t& in, uint8_t bits)
			{
				#ifdef X86
				//I know I have fun.
				asm ( "roll %[bi], %0;\n\t"
					: "=a" (in)
					: "0"(in), [bi]"cI" (bits)
				);
				#else
					in = (in << bits) | (in >> (32-bits));
				#endif // X86
			}
			inline void rotateRight(uint32_t& in, const uint8_t bits)
			{
				#ifdef X86
				asm ( "rorl %[bi], %0;"
					: "=a" (in)
					: "0"(in), [bi]"cI"(bits)
				);
				#else
					in = (in >> bits) | (in << (32-bits));
				#endif // X86
			}
	};
}

#endif // CCFPARSER_H
