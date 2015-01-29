#include "Container/DLCParser.h"
#include "Container/DecryptionException.h"
#include "basics/bio.h"
#include "basics/aes.h"

#include "curlcpp/include/curl_easy.h"
#include "curlcpp/include/curl_form.h"
#include "curlcpp/include/curl_header.h"

#include <sstream>
#include <fstream>
#include <regex>

#include "pugixml.hpp"

using namespace cdpp;
using namespace curl;

DLCParser::DLCParser()
{
	//ctor
}

DLCParser::~DLCParser()
{
	//dtor
}

std::vector<Package> DLCParser::parseFile(const std::string& filename)
{
	std::ifstream file;
	file.open(filename);
    if(file.is_open()) {
    	std::string dlcData = "";
		std::string line = "";
		while (std::getline(file, line)) {
			dlcData.append(line);
		}
		file.close();
		return parse(dlcData);
	} else {
		logger_.error("Could not open file: " + filename);
	}
	return std::vector<cdpp::Package>(0);
}

std::vector<Package> DLCParser::parse(const std::string& content)
{
	std::vector<Package> links;
	std::string dlcData, dlcKey;
	dlcKey = content.substr(content.size() - 88);
	logger_.debug("DLC Key: " + dlcKey);
	dlcData = content.substr(0, content.size() - 88);
	logger_.debug("Requesting key.");
	try {
		dlcKey = requestKey(dlcKey);
	} catch (CdppIOException& io) {
		logger_.error("Catched in >DLCParser::parse()<: ", io);
		return links;
	}
	if(dlcKey.size() == 0) {
		logger_.error("Could not fetch key, maybe we are offline.");
		return links;
	} else if (dlcKey.compare("2YVhzRFdjR2dDQy9JL25aVXFjQ1RPZ") == 0) {
		logger_.error("DLC limit reached... Please try again later.");
		return links;
	}
	//Decrypting DLC with key
	try {
		logger_.debug("Decrypting DLC...");
		dlcData = decrypt(dlcData, dlcKey);
	} catch (std::exception& e) {
		logger_.error("Exception catched in DLCParser::parse():", e);
		return links;
	}
	//Handle XML:
	try{
		logger_.debug("Handling XML...");
		links =  getLinksFromXML(dlcData);
	} catch (CdppException ce) {
		logger_.error("Exception catched in DLCParser::getLinksFromXML():", ce);
	}
	catch (std::length_error le) {
		logger_.error("Exception catched in DLCParser::getLinksFromXML():", le);
	}
	return links;
}

std::string DLCParser::requestKey(const std::string& dlcKey)
{
	const std::string apiURL = "http://service.jdownloader.org/dlcrypt/service.php";
	const std::string apiData = "destType=jdtc5&b=last09&p=2009&srcType=dlc&data=" + dlcKey + "&v=9.581";
	//Headers the JDownloder send
	const std::initializer_list<std::string> headers = {
			"User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.10) Gecko/2009042523 Ubuntu/9.04 (jaunty) Firefox/3.0.10",
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
			"Accept-Language: de,en-gb;q=0.7, en;q=0.3",
			"Accept-Encoding: gzip,deflate",
			"Cache-Control: no-cache",
			"Pragma: no-cache",
			"rev: 9.581"};

	std::ostringstream resultStream;
	curl_writer writer(resultStream);
    curl_easy easy(writer);
    curl_header header(headers);
    try {
        // Add some options to our request
        easy.add(curl_pair<CURLoption, long>(CURLOPT_TIMEOUT,			120));
        easy.add(curl_pair<CURLoption, string>(CURLOPT_URL,				apiURL));
        easy.add(curl_pair<CURLoption, string>(CURLOPT_POSTFIELDS,		apiData));
        easy.add(curl_pair<CURLoption, curl_header>(CURLOPT_HTTPHEADER,	header));
        easy.perform();
    } catch (curl_easy_exception exc) {
        // Print errors, if any
        std::string error;
        for(std::pair<std::string, std::string> trace : exc.what())
			error += std::string("ERROR: ") + trace.first + std::string(" :::: FUNCTION: ") + trace.second + std::string("\n\t");

        logger_.debug("requestKey(): Throws exception.", error);
        throw CdppIOException("DLCParser::requestKey(): Could not resolve key from network");
    }
	std::smatch sm;
	//Response should contain the key wrapped in <rc></rc> tags
	std::regex tags("<rc>([A-Za-z0-9\\+\\/]+={0,2})<\\/rc>");
	std::string result = resultStream.str();
	std::regex_search(result, sm, tags);
	if(sm.size() < 2) {
		logger_.debug("Resolving key error: 0x0" + std::to_string(__LINE__));
		throw CdppIOException("DLCParser::requestKey(): Could not parse key from response.");
	}
    return sm.str(1);
}

/********************************************//**
 * \brief Decrypts DLC Content with requested key
 * \param dlcContent Content of DLC file (file - 88Byte)
 * \param requestedKey Key requested from jdownloader.org
 * \return Decrypted content of container
 ***********************************************/
std::string DLCParser::decrypt(const std::string& dlcContent, const std::string& requestedKey)
{
	//Initi AES with given iv (iv == key) and mode = ECB
	AES aesECB(CDPP_AES_ECB_128, key_); //Init
	//Key and IV are decrypted DLCKey
	//dlcKey gets base64 decoded before (inline in this decrypt method)
	std::string dlcKey;
	try {
		dlcKey = aesECB.decrypt(requestedKey, key_);
	}
	catch (std::invalid_argument& ia) {
		throw std::invalid_argument(ia.what() + std::string("\nCalled by: DLCParser::decrypt()"));
	}
	catch (std::logic_error& lo) {
		throw std::logic_error(lo.what() + std::string("\nCalled by: DLCParser::decrypt()"));
	}
	catch (std::runtime_error& rt) {
		throw std::runtime_error(rt.what() + std::string("\nCalled by: DLCParser::decrypt()"));
	}
	char* buffer = new char[bio::calcDecodeLength(dlcKey.c_str()) + 1];
	try {
		bio::Base64Decode(dlcKey.c_str(), buffer);
	} catch (std::length_error& le) {
		throw std::length_error(le.what() + std::string("\nCalled by: DLCParser::decrypt()"));
	}
	AES aesCBC(CDPP_AES_CBC_128, (unsigned char*)buffer);
	std::string data;
	try {
		data = aesCBC.decrypt(dlcContent, (unsigned char*)buffer);
	}
	catch (std::invalid_argument& ia) {
		throw std::invalid_argument(ia.what() + std::string("\nCalled by: DLCParser::decrypt()"));
	}
	catch (std::logic_error& lo) {
		throw std::logic_error(lo.what() + std::string("\nCalled by: DLCParser::decrypt()"));
	}
	catch (std::runtime_error& rt) {
		throw std::runtime_error(rt.what() + std::string("\nCalled by: DLCParser::decrypt()"));
	}
	delete[] buffer;
	try {
		data = bio::Base64Decode(data);
	} catch (std::length_error& le) {
		throw std::length_error(le.what() + std::string("\nCalled by: DLCParser::decrypt()"));
	}
	return data;
}

std::vector<Package> DLCParser::getLinksFromXML(const std::string& xmlData)
{
	std::vector<Package> packages;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(xmlData.c_str());
	if (!result) {
		logger_.debug(std::string("DLC XML-Error: ") + result.description());
		throw CdppIOException("DLCParser::getLinksFromXML(): DLC XML-Error");
	}
	pugi::xml_node content = doc.child("dlc").child("content");
	if(!content) {
		logger_.debug("Malformed");
		throw CdppParseException("Got malformed XML file.");
	}
	for (pugi::xml_node pkg = content.child("package"); pkg; pkg = content.next_sibling("package")) {
		Package package;
		try {
			package.setName(bio::Base64Decode(pkg.attribute("name").value()));
			package.setPassword(bio::Base64Decode(pkg.attribute("passwords").value()));
		} catch (std::length_error le) {
			throw CdppParseException(le.what() + std::string("Called by: DLCParser::getLinksFromXML()"));
		}
		for (pugi::xml_node file = pkg.child("file"); file; file = file.next_sibling("file")) {
            FileLink m_file;
            try {
				m_file.name = bio::Base64Decode(file.child_value("filename"));
				m_file.url = bio::Base64Decode(file.child_value("url"));
				if(file.child("size"))
					m_file.size = std::stol(bio::Base64Decode(file.child_value("size")));
            } catch (std::length_error le) {
				throw std::length_error(le.what() + std::string("Called by: DLCParser::getLinksFromXML()"));
            }
            package.addFile(m_file);
		}
		packages.push_back(package);
	}
	return packages;
}
