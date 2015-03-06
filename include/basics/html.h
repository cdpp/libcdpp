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
