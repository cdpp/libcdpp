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

#include "hoster/ZippyShareCom.h"
#include "container/DecryptionException.h"
#include "basics/html.h"

#include "curlcpp/include/curl_easy.h"
#include "curlcpp/include/curl_form.h"

#include <pugixml.hpp>
#include <sstream>
#include <cstring>

using namespace cdpp;

ZippyShareCom::ZippyShareCom()
{
	//ctor
}

ZippyShareCom::~ZippyShareCom()
{
	//dtor
}

bool ZippyShareCom::checkFile(FileInfo& file)
{
	std::ostringstream resultStream;
	curl::curl_writer writer(resultStream);
	curl::curl_easy easy(writer);
	try {
		// Add some options to our request, timeout must be a great value -> zippyshare has sometime a looong ping
		easy.add(curl_pair<CURLoption, long>(CURLOPT_TIMEOUT,			600));
		easy.add(curl_pair<CURLoption, string>(CURLOPT_URL,				file.url));
		easy.perform();
	} catch (curl_easy_exception exc) {
		// Print errors, if any
		std::string error;
		for(std::pair<std::string, std::string> trace : exc.get_traceback())
			error += std::string("ERROR: ") + trace.first + std::string(" :::: FUNCTION: ") + trace.second + std::string("\n\t");
		logger_.debug("requestKey(): Throws exception.", error);
		throw CdppIOException("DLCParser::requestKey(): Could not resolve key from network");
	}
	pugi::xml_document doc;
	//Convert HTML to XML with tidyp so we can process it with pugixml
	pugi::xml_parse_result result = doc.load_string(internal::html2xml(resultStream.str()).c_str());
	//check if pugi could read it
	if (!result) {
		logger_.debug(std::string("ZippyShareCom XML-Error: ") + result.description());
		throw CdppIOException("ZippyShareCom::checkFile(): ZippyShareCom XML-Error");
	}
	//Find node where information is saved
	pugi::xml_node content = doc.find_node(
									[](pugi::xml_node n) -> bool {
										return (strcmp(n.attribute("id").as_string(), "lrbox") == 0);
									});
	if (!content) {
		logger_.debug("Malformed");
		throw CdppParseException("Got malformed XML file.");
	}
	content = content.first_child().first_child();
	if (strcmp(content.name(), "font") == 0) {
		content = content.next_sibling();
	}
	content = content.next_sibling();
	std::string co(content.text().as_string());
	if (co.at(0) == '\n') {
		co.erase(0,1);
		if (co.at(0) == '\r') {
			co.erase(0,1);
		}
	}
	if (co.compare("Name:") == 0) {
		content = content.next_sibling();
		file.name = std::string(content.text().as_string());
		file.size = content.next_sibling().next_sibling().next_sibling().text().as_int() * 1024 * 1024;
		file.status = FileStatus::OK;
	} else if (co.compare("File has expired and does not exist anymore on this server") == 0) {
		file.status = FileStatus::NOT_FOUND;
	} else {
		file.status = FileStatus::FAILED;
		return false;
	}
	return true;
}
