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
		static void hex_to_bytes(const std::string& str, unsigned char* data);
		/********************************************//**
		 * \brief Calculate size of decoded data
		 * \param b64input Base64 input c string
		 * \return Length of decoded data
		 ***********************************************/
		static int calcDecodeLength(const char* b64input);
		/********************************************//**
		 * \brief Decodes given base64 string
		 * \param b64message Base64 input string
		 * \param buffer Output data array
		 * \return 0 if success
		 ***********************************************/
		static int Base64Decode(char* b64message, char* buffer);
		/********************************************//**
		 * \brief Encodes an input string (0 terminated) to base64
		 * \param message Data to encode
		 * \return Encoded data as std::string
		 ***********************************************/
		static std::string Base64Encode(const char* message);
	};
}
#endif // BIO_H
