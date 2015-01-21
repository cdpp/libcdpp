/**
* AES decryption using OpenSSL EVP apis
* this is public domain code.
**/
#include "Logger.h"
#include "basics/aes.h"

#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>

/********************************************//**
 * \brief Decrypts an input string with given key and iv by using AES ECB 192
 *
 * \param encrypted Encrypted data as c-string (char*)
 * \param length Length of encrypted data
 * \param key AES Key
 * \param iv Initalization Vector
 * \return Decrypted data
 *
 ***********************************************/

int aes_decrypt(unsigned char* decrypted, unsigned char* encrypted, int length, unsigned char* key, unsigned char* iv, uint8_t type)
{
    static cdpp::Logger logger = cdpp::Logger::getLogger();
    int o_len = length;
    int tmp_len;

    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    EVP_CIPHER_CTX *ctx;
	const EVP_CIPHER *cipher;

	switch (type) {
		case CDPP_AES_ECB: {
			cipher = EVP_aes_192_ecb();
			break;
		}
		case CDPP_AES_CFB: {
			cipher = EVP_aes_192_cfb();
			break;
		}
		default: {
			cipher = EVP_aes_192_ecb();
		}
	}
    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        logger.error("Could not initalisize the Cipher Context.");
        return -1;
    }

    /* Initialise the decryption operation. */
    if(1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv)) {
        logger.error("Could not setup decryption, maybe key or iv are not valid.");
        return -1;
    }
    EVP_CIPHER_CTX_set_padding(ctx, false);
    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_DecryptUpdate(ctx, decrypted, &o_len, encrypted, length)) {
        logger.error("Failed to decrypt message.");
        return -1;
    }
    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, decrypted + o_len, &tmp_len)) {
        logger.error("Failed to finalize the decryption.");
        return -1;
    }
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    /* Clean up */
    EVP_cleanup();
    ERR_free_strings();

    return (o_len + tmp_len);
}
