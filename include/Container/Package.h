#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <vector>

namespace cdpp {
	/* Why I am using a struct?
	 * Well in my opinion a class is for encapsulate data.
	 * And a struct is only a data container --> so this is only a data container
	 * and I think a struct is better style then using so many getters and setters. */
    /********************************************//**
     * \struct FileLink
     * \brief Data container for a simple link in a package
     * \details Contains URL of the link, name of the file (if known)
	 *			and size of the file (if known)
    ***********************************************/
	struct FileLink
	{
		/** Name of the file */
		std::string name;
		/** Url to file */
		std::string url;
		/** Size of file */
		long size;
		/*****************************************//**
		* \brief Initialize a instance of this struct
		* \param name Name of File
		* \param url URL to file
		* \param size Size of file
		**********************************************/
		explicit
			FileLink(std::string name = "", std::string url = "", long size = -1)
				: name(name), url(url), size(size) {};
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
			 * \sa FileLink
			 ********************************/
			void addFile(FileLink file);
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
			 * \return std::vector<FileLink>
			 * \sa FileLink
			 ********************************/
			std::vector<FileLink> getFiles();
		private:
			std::vector<FileLink> files_;
			std::string password_;
			std::string name_;
	};
}

#endif // PACKAGE_H
