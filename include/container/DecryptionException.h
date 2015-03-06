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

#ifndef DECRYPTIONEXCEPTION_H
#define DECRYPTIONEXCEPTION_H

#include "../cdpp.h"
#include <exception>
#include <string>
#include <utility>

namespace cdpp {
    /********************************************//**
	 * \class CdppException
     * \brief Class for exceptions to throw on failed decryption
     ***********************************************/
	class CdppException : public std::exception
	{
		public:
			/** Default constructor */
			CdppException() noexcept {};
			/** Default destructor */
			virtual ~CdppException() noexcept {};
			using std::exception::what;
            /********************************************//**
			 * \brief Reason for this exception (if set)
             * \return Reason as error trace
             ***********************************************/
			std::string what() noexcept {
				return what_.second.empty() ? what_.first : what_.first + std::string(": ") + what_.second;
			};
		protected:
			std::pair<std::string, std::string> what_;
	};

    /********************************************//**
	 * \class CdppIOException
     * \brief Class for exceptions to throw on failed IO operations
     ***********************************************/
	class CdppIOException : public CdppException
	{
		public:
			/** Default constructor */
			CdppIOException() noexcept : CdppIOException(std::string()) {};
            /********************************************//**
			 * \brief Set Error (message) and function in constructor
             * \param error Error which occured
             ***********************************************/
			CdppIOException(const std::string& error) noexcept {
				what_ = std::make_pair(std::string("CdppIOException"), error);
			};
	};

	    /********************************************//**
	 * \class CdppParseException
     * \brief Class for exceptions to throw on failed parsing (base64)
     ***********************************************/
	class CdppParseException : public CdppException
	{
		public:
			/** Default constructor */
			CdppParseException() noexcept : CdppParseException(std::string()) {};
            /********************************************//**
			 * \brief Set Error (message) and function in constructor
             * \param error Error which occured
             ***********************************************/
			CdppParseException(const std::string& error) noexcept {
				what_ = std::make_pair(std::string("CdppParseException"), error);
			};
	};
}

#endif // DECRYPTIONEXCEPTION_H
