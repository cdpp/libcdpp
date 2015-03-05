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
