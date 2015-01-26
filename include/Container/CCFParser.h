#ifndef CCFPARSER_H
#define CCFPARSER_H

#include "Container.h"

namespace cdpp {
	/********************************************//**
	* \class CCFParser
	* \brief Decrypts CCF Container, native decryption for CCF 3.0
	* \implements Container
	***********************************************/
	class CCFParser : public Container
	{
		public:
			/** Default constructor */
			CCFParser();
			/** Default destructor */
			virtual ~CCFParser();
			/********************************************//**
			 * \brief Function to parse content of given container
             * \param content Content of container file
             * \return Decrypted / Decoded content
             ***********************************************/
			std::vector<cdpp::Package> parse(const std::string& content);
			/********************************************//**
			 * \brief Function to parse given container file
             * \param filename File name of container-file
             * \return Decrypted / Decoded content of file
             ***********************************************/
            std::vector<cdpp::Package> parseFile(const std::string& filename);
		private:
			std::vector<std::string> decryptNative(std::string content);
			std::vector<std::string> decryptByJdo(std::string content);
	};
}

#endif // CCFPARSER_H
