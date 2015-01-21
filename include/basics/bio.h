#ifndef BASE64_H
#define BASE64_H
/** \file bio.h
 *	Basic IO functions, HEX2BIN ... Base64...
 */

#include <string>

namespace cdpp {
	namespace bio {
		/** \brief Hex string to unsigned char array.
		 * \param str Hexstring, e.g. "A0F445FF"
		 * \param data unsigned char array of unhexed data
		 * \exception std::invalid_argument If input string includes invalid characters
		 */
		void hex_to_bytes(const std::string& str, unsigned char* data);
		int calcDecodeLength(const char* b64input);
		int Base64Decode(char* b64message, char* buffer);
		int Base64Encode(const char* message, char** buffer);
	}
}
#endif // BASE64_H
