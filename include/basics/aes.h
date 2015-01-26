#ifndef AES_H_INCLUDED
#define AES_H_INCLUDED
/********************************************//**
* \file aes.h
* \brief Wrapper for openssl AES functions
***********************************************/
#include <string>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include "Logger.h"

namespace cdpp {
	/********************************************//**
	* \def CDPP_AES_ECB_192
	* Define used to indicate that decrypt should use AES in ECB mode with 192bit key.
	***********************************************/
	#define CDPP_AES_ECB_192 0x01
	/********************************************//**
	* \def CDPP_AES_ECB_128
	* Define used to indicate that decrypt should use AES in ECB mode with 128bit key.
	***********************************************/
	#define CDPP_AES_ECB_128 0x02
	/********************************************//**
	* \def CDPP_AES_CBC_128
	* Define used to indicate that decrypt should use AES in CBC mode with 128bit key.
	***********************************************/
	#define CDPP_AES_CBC_128 0x03
	/********************************************//**
	* \def CDPP_AES_CFB_192
	* Define used to indicate that decrypt should use AES in CFB mode with 192bit key.
	***********************************************/
	#define CDPP_AES_CFB_192 0x04
	/********************************************//**
	* \def CDPP_LAST_CHIPHER
	* Define used to indicate that decrypt should use the last chipher that was used.
	***********************************************/
	#define CDPP_LAST_CHIPHER 0xFF

	/********************************************//**
	* \class AES
	* \brief Wrapper for openssl AES functions
	***********************************************/
	class AES
	{
		public:
            /********************************************//**
			* \brief Standard constructor
            ***********************************************/
			AES();
			/********************************************//**
			* \brief Constructor: Sets chipher type and initialization vector
			* \param chipherType Chipher to use
			* \param iv Initialization vector
			***********************************************/
			AES(const uint8_t chipherType, const unsigned char* iv = nullptr);
			virtual ~AES();
			/********************************************//**
			* \brief Decrypts input data with given key and (if needed) iv by using AES
			* \param encrypted Encrypted data as c-string (char*)
			* \param length Length of encrypted data
			* \param key AES Key
			* \param iv Initalization Vector
			* \param chipherType Type of AES (e.g. cfb or ecb)
			* \return Decrypted data
			***********************************************/
			std::string decrypt(unsigned char* encrypted, int length, const unsigned char* key,
								const unsigned char* iv = nullptr, const uint8_t chipherType = CDPP_LAST_CHIPHER);
			/********************************************//**
			* \brief Decrypts an input string with given key and (if needed) iv by using AES
			* \param b64String Encrypted data as base64 string
			* \param key AES Key
			* \param iv Initalization Vector
			* \param chipherType Type of AES (e.g. cfb or ecb)
			* \return Decrypted data
			***********************************************/
			std::string decrypt(const std::string& b64String, const unsigned char* key,
						const unsigned char* iv = nullptr, const uint8_t chipherType = CDPP_LAST_CHIPHER);
		private:
			bool wasInit_;
			unsigned char iv_[16];
			Logger logger_ = Logger::getLogger();
			uint8_t lastChipher_ = CDPP_AES_CFB_192;

			inline void cleanup(EVP_CIPHER_CTX* ctx);
			const EVP_CIPHER* getChipher(const uint8_t chipherType = CDPP_LAST_CHIPHER);
	};

}
#endif // AES_H_INCLUDED
