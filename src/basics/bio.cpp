#include "basics/bio.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <cmath>
#include <climits>
#include <cstring>
#include <stdexcept>

/** \brief \copybrief hex_to_bytes
 *  \details Secur method to convert a hex-string to binary, checks if characters are valid.
 *			 Throws exception if not.
 *			 Used a modified version of http://www.codeproject.com/Articles/99547/Hex-strings-to-raw-data-and-back
 * 			 Codeprojects version is licensed under the BSD licencse.
 */
 void cdpp::bio::hex_to_bytes(const std::string& str, unsigned char* data)
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

//Calculates the length of a decoded base64 string
int cdpp::bio::calcDecodeLength(const char* b64input)
{
	int len = strlen(b64input);
	int padding = 0;
	//last two chars are =
	if (b64input[len-1] == '=' && b64input[len-2] == '=') {
		padding = 2;
	} else if (b64input[len-1] == '=') {
		//last char is =
		padding = 1;
	}
	return (int)len*0.75 - padding;
}

//Decodes a base64 encoded string
int cdpp::bio::Base64Decode(char* b64message, char* buffer)
{
	BIO *bio, *b64;
	int decodeLen = calcDecodeLength((const char*)b64message);
	int len = 0;
	FILE* stream = fmemopen(b64message, strlen(b64message), "r");

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(stream, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	len = BIO_read(bio, buffer, strlen(b64message));
	if(len != decodeLen)
		return -1;
	buffer[len] = '\0';

	BIO_free_all(bio);
	fclose(stream);

	return (0); //success
}

//Encodes a string to base64
int cdpp::bio::Base64Encode(const char* message, char** buffer)
{
	BIO *bio, *b64;
	FILE* stream;
	int encodedSize = 4*ceil((double)strlen(message)/3);
	*buffer = (char *)malloc(encodedSize+1);

	stream = fmemopen(*buffer, encodedSize+1, "w");
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(stream, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
	BIO_write(bio, message, strlen(message));
	BIO_flush(bio);
	BIO_free_all(bio);
	fclose(stream);

	return (0); //success
}