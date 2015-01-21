#ifndef AES_H_INCLUDED
#define AES_H_INCLUDED

#include <string>

/********************************************//**
 * \file aes.h
 * \brief Wrapper for openssl AES functions
 ***********************************************/

/********************************************//**
 * \def CDPP_AES_ECB_192
 * Define used to indicate that decrypt should use AES in ECB mode with 192bit key.
 ***********************************************/
#define CDPP_AES_ECB_192 1
/********************************************//**
 * \def CDPP_AES_CFB_192
 * Define used to indicate that decrypt should use AES in CFB mode with 192bit key.
 ***********************************************/
#define CDPP_AES_CFB_192 2

/********************************************//**
 * \brief Decrypts an input string with given key and (if needed) iv by using AES
 * \param encrypted Encrypted data as c-string (char*)
 * \param length Length of encrypted data
 * \param key AES Key
 * \param iv Initalization Vector
 * \param type Type of AES (e.g. cfb or ecb)
 * \return Decrypted data
 ***********************************************/
std::string aes_decrypt(unsigned char* encrypted, int length, const unsigned char* key,
				const unsigned char* iv, const uint8_t type);

#endif // AES_H_INCLUDED
