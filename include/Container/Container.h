#ifndef CONTAINER_H
#define CONTAINER_H

#include "Package.h"
#include <vector>
#include <string>

namespace cdpp {
    /********************************************//**
	 * \class Container
     * \brief Abstract class / interface for url container
     ***********************************************/
	class Container
	{
		public:
            /********************************************//**
			 * \brief Function to parse content of given container
             * \param content Content of container file
             * \return Decrypted / Decoded content
             ***********************************************/
			virtual std::vector<cdpp::Package> parse(const std::string &content) = 0;
			/********************************************//**
			 * \brief Function to parse given container file
             * \param filename File name of container-file
             * \return Decrypted / Decoded content of file
             ***********************************************/
            virtual std::vector<cdpp::Package> parseFile(const std::string &filename) = 0;
	};
}
#endif // CONTAINER_H
