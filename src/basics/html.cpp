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

#include "basics/html.h"
#include <tidyp/buffio.h>
#include <tidyp/tidyp.h>

using namespace cdpp::internal;

/********************************************//**
 * \brief \copybrief html2xml
 * \details This function converts HTML code to XML code.
 *          By using tidyp.
 *			This is needed to process HTML code with pugixml (HTML is not allways valid XML).
 ***********************************************/
std::string html2xml(const std::string &html){
    // Initialize a Tidy document
    TidyDoc tidyDoc = tidyCreate();
    TidyBuffer tidyOutputBuffer({0, 0, 0, 0, 0});

    // Configure Tidy
    // The flags tell Tidy to output XML and disable showing warnings
    bool configSuccess = tidyOptSetBool(tidyDoc, TidyXmlOut, yes)
        && tidyOptSetBool(tidyDoc, TidyQuiet, yes)
        && tidyOptSetBool(tidyDoc, TidyNumEntities, yes)
        && tidyOptSetBool(tidyDoc, TidyShowWarnings, no);

    int tidyResponseCode = -1;

    // Parse input
    if (configSuccess)
        tidyResponseCode = tidyParseString(tidyDoc, html.c_str());

    // Process HTML
    if (tidyResponseCode >= 0)
        tidyResponseCode = tidyCleanAndRepair(tidyDoc);

    // Output the HTML to our buffer
    if (tidyResponseCode >= 0)
        tidyResponseCode = tidySaveBuffer(tidyDoc, &tidyOutputBuffer);

    // Any errors from Tidy?
    if (tidyResponseCode < 0)
        throw ("Tidy encountered an error while parsing an HTML response. Tidy response code: " + tidyResponseCode);

    // Grab the result from the buffer and then free Tidy's memory
    std::string tidyResult = (char*)tidyOutputBuffer.bp;
    tidyBufFree(&tidyOutputBuffer);
    tidyRelease(tidyDoc);

    return tidyResult;
}
