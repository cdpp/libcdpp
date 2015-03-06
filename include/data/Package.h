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

#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <vector>

namespace cdpp {
	enum FileStatus
	{
		UNKNOWN = 0,
		NOT_FOUND,
		FAILED,
		OK
	};
	/* Why I am using a struct?
	 * Well in my opinion a class is for encapsulate data.
	 * And a struct is only a data container --> so this is only a data container
	 * and I think a struct is better style then using so many getters and setters. */
    /********************************************//**
     * \struct FileInfo
     * \brief Data container for a simple link in a package
     * \details Contains URL of the link, name of the file (if known)
	 *			and size of the file (if known)
    ***********************************************/
	struct FileInfo
	{
		/** Name of the file */
		std::string name;
		/** Url to file */
		std::string url;
		FileStatus status;
		/** Size of file */
		long size;
		std::string hash;
		/*****************************************//**
		* \brief Initialize a instance of this struct
		* \param name Name of File
		* \param url URL to file
		* \param size Size of file
		**********************************************/
		explicit
			FileInfo(std::string name = "", std::string url = "", long size = -1, std::string hash = "")
				: name(name), url(url), status(UNKNOWN), size(size), hash(hash) {};
	};
	/********************************************//**
	* \class Package
	* \brief Class for link packages (used when decrypting container files)
	***********************************************/
	class Package
	{
		public:
			/** Default constructor */
			Package();
            /********************************************//**
			 * \brief Constructor used to set name and password of package at creation
             * \param name Name of this package
             * \param password Password of this package (default = "")
             ***********************************************/
			Package(std::string name, std::string password = "");
			/** Default destructor */
			virtual ~Package();
			/** Adding a file to this package
			 * \param file File to add to this package
			 * \sa FileInfo
			 ********************************/
			void addFile(FileInfo file);
			/** Setting password of this package
			 * \param password Password to set
			 ********************************/
			void setPassword(std::string password);
			/** Setting name of this package
			 * \param name Name to set
			 ********************************/
			void setName(std::string name);
			/** Getting the name of this package
			* \return std::string
			*********************************/
			std::string getName();
			/** Getting password of this package
			 * \return std::string
			 ********************************/
			std::string getPassword();
			/** Getting files saved in this package
			 * \return std::vector<FileInfo>
			 * \sa FileInfo
			 ********************************/
			std::vector<FileInfo> getFiles();
		private:
			std::vector<FileInfo> files_;
			std::string password_;
			std::string name_;
	};
}

#endif // PACKAGE_H
