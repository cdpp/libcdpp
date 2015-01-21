#include "basics/base64.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
#include <cmath>

//Calculates the length of a decoded base64 string
int cdpp::base64::calcDecodeLength(const char* b64input)
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
int cdpp::base64::Base64Decode(char* b64message, char* buffer)
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
int cdpp::base64::Base64Encode(const char* message, char** buffer)
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
