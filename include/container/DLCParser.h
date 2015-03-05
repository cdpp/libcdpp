#ifndef DLCPARSER_H
#define DLCPARSER_H

#include "Container.h"
#include "data/Package.h"
#include "../basics/Logger.h"

#include <vector>
#include <string>

namespace cdpp {
	/********************************************//**
	* \class DLCParser
	* \brief Class for DLC decryption
	* \implements Container
	***********************************************/
	class DLCParser : public Container
	{
		public:
			/** Default constructor */
			DLCParser();
			/** Default destructor */
			virtual ~DLCParser();
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
			/* Methods */
			/********************************************//**
			 * \brief Request key from dlc server
             * \param dlcKey key from dlc file
             * \return key returned from server
             ***********************************************/
			std::string requestKey(const std::string& dlcKey);
			/********************************************//**
			 * \brief Decrypt given dlc content with key
             * \param dlcContent Content of DLC file
             * \param requestedKey Key requested from DLC server
             * \return Decrypted DLC content
             ***********************************************/
			std::string decrypt(const std::string& dlcContent, const std::string& requestedKey);
			/********************************************//**
			 * \brief Get links from decrypted DLC file
             * \param xmlData decrypted DLC data (is XML)
             * \return A vector containing all packages in DLC
             ***********************************************/
			std::vector<Package> getLinksFromXML(const std::string& xmlData);

			/* Attributes */
			Logger logger_ = Logger::getLogger();
			const unsigned char key_[16] =  {	0xc4, 0x6e, 0x50, 0x3a, 0x43, 0x6e,
												0xc2, 0x14, 0x0e, 0x45, 0x8f, 0x3d,
												0x46, 0xa0, 0x30, 0x77};
	};
}

#endif // DLCPARSER_H
