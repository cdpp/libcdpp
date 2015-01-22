/*
* This is public domain code.
*/
#include "basics/aes.h"
#include <stdexcept>
#include <cstring>

using namespace cdpp;

AES::AES()
{
    OPENSSL_config(NULL);
    wasInit_ = false;
}

AES::AES(const uint8_t chipherType, const unsigned char* iv)
{
	OPENSSL_config(NULL);
	lastChipher_ = chipherType;
	if (iv != nullptr) {
		memcpy(iv_, iv, 16);
	}
	wasInit_ = true;
}

AES::~AES()
{
	EVP_cleanup();
}

/********************************************//**
 * \brief \copybrief decrypt
 * \details This function uses the EVP apis of the openssl library.
 *			Note that key must(!) fit the selected chipher (e.g. aes ecb 192 the key must be 24Byte == 192bit).
 *			You can select the type of chipher by setting type.
 * \sa CDPP_AES_ECB_192
 * \sa CDPP_AES_CFB_192.
 ***********************************************/

std::string AES::decrypt(unsigned char* encrypted, int length, const unsigned char* key,
						const unsigned char* iv, const uint8_t chipherType)
{
    int tmp_len, o_len = 0;
    unsigned char decrypted[length+16];
    const EVP_CIPHER* cipher = getChipher(chipherType);
    EVP_CIPHER_CTX* ctx;

	if (!wasInit_) {
		memcpy(iv_, iv, 16);
		wasInit_ = true;
	}

	if (!(ctx = EVP_CIPHER_CTX_new())) {
        logger_.error("Could not initalisize the Cipher Context.");
        throw std::runtime_error("openssl could not be initialisized.");
    }
	/* Initialise the decryption operation. */
	if(1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv_)) {
		logger_.error("Could not setup decryption, maybe key or iv are not valid.");
		return "";
	}
	EVP_CIPHER_CTX_set_padding(ctx, false);
	wasInit_ = true;

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_DecryptUpdate(ctx, decrypted, &tmp_len, encrypted, length)) {
        logger_.error("Failed to decrypt message.");
        return "";
    }
    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
	o_len += tmp_len;
    if (1 != EVP_DecryptFinal_ex(ctx, decrypted + tmp_len, &tmp_len)) {
        logger_.error("Failed to finalize the decryption.");
        return "";
    }
	//Clean context
	EVP_CIPHER_CTX_free(ctx);

    o_len += tmp_len;
    decrypted[o_len] = '\0';

	if (lastChipher_ == CDPP_AES_CFB_192) {
		// Setting IV for next turn in CFB Mode
		int offset = 16 - length;
		if(offset <= 0)
			memcpy(iv_, encrypted + (length - 16), 16);
		else {
			memmove(iv_, iv_ + (16 - offset), offset);
			memcpy(iv_ + offset, encrypted, 16 - offset);
		}
	}
    return std::string((char*)decrypted);
}

const EVP_CIPHER* AES::getChipher(const uint8_t chipherType)
{
	if (chipherType != CDPP_LAST_CHIPHER) {
		lastChipher_ = chipherType;
	}
	switch (lastChipher_) {
		case CDPP_AES_ECB_192: {
			return EVP_aes_192_ecb();
		}
		case CDPP_AES_CFB_192: {
			return EVP_aes_192_cfb8();
		}
		default: {
			return EVP_aes_192_ecb();
		}
	}
}
