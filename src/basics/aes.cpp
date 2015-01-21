/*
* This is public domain code.
*/
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>

#include "Logger.h"
#include "basics/aes.h"

/********************************************//**
 * \brief \copybrief aes_decrypt
 * \details This function uses the EVP apis of the openssl library.
 *			Note that key must(!) fit the selected chipher (e.g. aes ecb 192 the key must be 24Byte == 192bit).
 *			You can select the type of chipher by setting type.
 * \sa CDPP_AES_ECB_192
 * \sa CDPP_AES_CFB_192.
 ***********************************************/

std::string aes_decrypt(unsigned char* encrypted, int length, const unsigned char* key,
						const unsigned char* iv, const uint8_t type)
{
    static cdpp::Logger logger = cdpp::Logger::getLogger();
    int o_len = length;
    int tmp_len;
    unsigned char decrypted [length+16];

    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    EVP_CIPHER_CTX *ctx;
	const EVP_CIPHER *cipher;

	switch (type) {
		case CDPP_AES_ECB_192: {
			cipher = EVP_aes_192_ecb();
			break;
		}
		case CDPP_AES_CFB_192: {
			cipher = EVP_aes_192_cfb8();
			break;
		}
		default: {
			cipher = EVP_aes_192_ecb();
		}
	}
    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        logger.error("Could not initalisize the Cipher Context.");
        return "";
    }

    /* Initialise the decryption operation. */
    if(1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv)) {
        logger.error("Could not setup decryption, maybe key or iv are not valid.");
        return "";
    }
    EVP_CIPHER_CTX_set_padding(ctx, false);
    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_DecryptUpdate(ctx, decrypted, &o_len, encrypted, length)) {
        logger.error("Failed to decrypt message.");
        return "";
    }
    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, decrypted + o_len, &tmp_len)) {
        logger.error("Failed to finalize the decryption.");
        return "";
    }
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    EVP_cleanup();
    ERR_free_strings();
    return std::string((char*)decrypted);
}
