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
     * \brief Class for exceptions to throw on failed IO operations (base64)
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
}

#endif // DECRYPTIONEXCEPTION_H
