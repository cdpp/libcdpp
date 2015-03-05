#ifndef HTML_H_INCLUDED
#define HTML_H_INCLUDED
/********************************************//**
* \file html.h
* \brief Basic function(s) to process HTML code
***********************************************/
#include <string>

namespace cdpp {
	namespace internal {
		/********************************************//**
		* \brief HTML to XML converting function
		* \param html Reference to html code
		* \return XML code generated from html code
		***********************************************/
		std::string html2xml(const std::string& html);
	}
}

#endif // HTML_H_INCLUDED
