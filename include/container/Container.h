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

#ifndef CONTAINER_H
#define CONTAINER_H

#include "data/Package.h"
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
