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

#include "basics/aes.h"
#include "basics/bio.h"

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
 * \sa CDPP_AES_ECB_192 CDPP_AES_ECB_128
 * \sa CDPP_AES_CBC_128 CDPP_AES_CFB_192.
 ***********************************************/

std::string AES::decrypt(const std::string& b64String, const unsigned char* key,
						const unsigned char* iv, const uint8_t chipherType)
{
	int tmp_len = bio::calcDecodeLength(b64String.c_str());
	char* decoded = new char[tmp_len + 1];
	try {
		bio::Base64Decode(b64String.c_str(), decoded);
	} catch (std::length_error& le) {
		logger_.debug("Length error in decrypt AES -> from B64");
		delete[] decoded;
		throw std::length_error(le.what() + std::string("\nCalled by: AES::decrypt(str)"));
	}
	std::string ret;
	try {
		ret = decrypt((unsigned char*)decoded, tmp_len, key, iv, chipherType);
	}
	catch (std::invalid_argument& ia) {
		delete[] decoded;
		throw std::invalid_argument(ia.what() + std::string("\nCalled by: AES::decrypt(str)"));
	}
	catch (std::logic_error& lo) {
		delete[] decoded;
		throw std::logic_error(lo.what() + std::string("\nCalled by: AES::decrypt(str)"));
	}
	catch (std::runtime_error& rt) {
		delete[] decoded;
		throw std::runtime_error(rt.what() + std::string("\nCalled by: AES::decrypt(str)"));
	}
	delete[] decoded;
	return ret;
}

/********************************************//**
 * \brief \copybrief decrypt
 * \details This function uses the EVP apis of the openssl library.
 *			Note that key must(!) fit the selected chipher (e.g. aes ecb 192 the key must be 24Byte == 192bit).
 *			You can select the type of chipher by setting type.
 * \sa CDPP_AES_ECB_192 CDPP_AES_ECB_128
 * \sa CDPP_AES_CBC_128 CDPP_AES_CFB_192.
 ***********************************************/

std::string AES::decrypt(unsigned char* encrypted, int length, const unsigned char* key,
						const unsigned char* iv, const uint8_t chipherType)
{
    int tmp_len, o_len = 0;
    unsigned char* decrypted = new unsigned char[length+16];
    const EVP_CIPHER* cipher = getChipher(chipherType);
    EVP_CIPHER_CTX* ctx;

	if (!wasInit_) {
		if(iv != nullptr)
			memcpy(iv_, iv, 16);
		wasInit_ = true;
	}

	if (!(ctx = EVP_CIPHER_CTX_new())) {
        logger_.debug("Could not initalisize the Cipher Context.");
        delete[] decrypted;
        throw std::runtime_error("AES::decrypt(): Openssl could not be initialisized.");
    }
	/* Initialise the decryption operation. */
	if(1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv_)) {
		logger_.debug("Could not setup decryption, maybe key or iv are not valid.");
		cleanup(ctx);
		delete[] decrypted;
		throw std::invalid_argument("AES::decrypt(): Key or IV are not valid.");
	}
	EVP_CIPHER_CTX_set_padding(ctx, false);
	wasInit_ = true;

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_DecryptUpdate(ctx, decrypted, &tmp_len, encrypted, length)) {
        logger_.debug("Failed to decrypt message.");
        cleanup(ctx);
        delete[] decrypted;
        throw std::logic_error("AES::decrypt(): Can not decrypt message");
    }
    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
	o_len += tmp_len;
    if (1 != EVP_DecryptFinal_ex(ctx, decrypted + tmp_len, &tmp_len)) {
        logger_.debug("Failed to finalize the decryption.");
        cleanup(ctx);
        delete[] decrypted;
        throw std::logic_error("AES::decrypt(): Can not finalize decryption.");
    }
	cleanup(ctx);

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
    std::string ret((char*)decrypted);
    delete[] decrypted;
    return ret;
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
		case CDPP_AES_ECB_128: {
			return EVP_aes_128_ecb();
		}
		case CDPP_AES_CBC_128: {
			return EVP_aes_128_cbc();
		}
		case CDPP_AES_CBC_256: {
			return EVP_aes_256_cbc();
		}
		case CDPP_AES_CFB_192: {
			return EVP_aes_192_cfb8();
		}
		default: {
			return EVP_aes_192_ecb();
		}
	}
}

void AES::cleanup(EVP_CIPHER_CTX* ctx)
{
	//Clean context
	EVP_CIPHER_CTX_free(ctx);
}
