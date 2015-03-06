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

#ifndef BIO_H
#define BIO_H
/** \file bio.h
 *	Basic IO functions, HEX2BIN ... Base64...
 */

#include <string>

namespace cdpp {
    /********************************************//**
	* \brief Class for some basic IO functions
    ***********************************************/
	class bio {
		public:
		/********************************************//**
		 * \brief Hex string to unsigned char array.
		 * \param str Hexstring, e.g. "A0F445FF"
		 * \param data unsigned char array of unhexed data
		 * \exception std::invalid_argument If input string includes invalid characters
		 ***********************************************/
		static void hex2bytes(const std::string& str, unsigned char* data);
		/********************************************//**
		 * \brief Calculate size of decoded data
		 * \param b64input Base64 input c string
		 * \return Length of decoded data
		 ***********************************************/
		static int calcDecodeLength(const char* b64input);
		/********************************************//**
		 * \brief Decodes given base64 c string
		 * \param b64message Base64 input string
		 * \param buffer Output data array
		 * \exception std::length_error If calculated an actual size not match
		 ***********************************************/
		static void Base64Decode(const char* b64message, char* buffer);
		/********************************************//**
		 * \brief Decodes given base64 string
		 * \param b64string Base64 input string
		 * \return Decoded std::string
		 * \exception std::length_error If calculated an actual size not match
		 ***********************************************/
		static std::string Base64Decode(const std::string& b64string);
		/********************************************//**
		 * \brief Encodes an input string (0 terminated) to base64
		 * \param message Data to encode
		 * \return Encoded data as std::string
		 ***********************************************/
		static std::string Base64Encode(const char* message);
	};
}
#endif // BIO_H
