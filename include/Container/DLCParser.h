#ifndef DLCPARSER_H
#define DLCPARSER_H

#include "Container.h"
#include "../Logger.h"

#include <vector>
#include <string>

namespace cdpp {
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
			std::vector<std::string> parse(const std::string& content);
			/********************************************//**
			 * \brief Function to parse given container file
             * \param filename File name of container-file
             * \return Decrypted / Decoded content of file
             ***********************************************/
            std::vector<std::string> parseFile(const std::string& filename);
		private:
			std::string requestKey(const std::string& dlcKey);
			std::vector<std::string> getLinksFromXML(std::string xmlData);
			Logger logger_ = Logger::getLogger();
			const unsigned char key_[16] = {57, 48, 57, 99, 99, 100, 49, 101,
											56, 50, 50, 55, 98, 50,  98, 51};
			const unsigned char iv_[16] = {	57, 48, 57, 99, 99, 100, 49, 101,
											56, 50, 50, 55, 98,  50, 98,  51};
	};
}

#endif // DLCPARSER_H
