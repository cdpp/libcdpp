#ifndef DLCPARSER_H
#define DLCPARSER_H

#include "Container.h"
#include "Package.h"
#include "../Logger.h"

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
			std::string requestKey(const std::string& dlcKey);
			std::string decrypt(const std::string& dlcContent, const std::string& requestedKey);
			std::vector<Package> getLinksFromXML(const std::string& xmlData);
			Logger logger_ = Logger::getLogger();

			const unsigned char key_[16] = {0xeb, 0xda, 0x23, 0x7a, 0x3d, 0x87,
											0xac, 0xcc, 0xf7, 0x2d, 0xcb, 0x61,
											0x57, 0xfe, 0xe3, 0x14};
	};
}

#endif // DLCPARSER_H
