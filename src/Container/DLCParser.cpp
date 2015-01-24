#include "Container/DLCParser.h"
#include "curlcpp/include/curl_easy.h"
#include "curlcpp/include/curl_form.h"
#include "curlcpp/include/curl_header.h"

#include <sstream>
#include <fstream>

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

std::vector<std::string> DLCParser::parseFile(const std::string& filename)
{
	std::vector<std::string> links;
	std::string dlcData, dlcKey;
	std::ifstream file;
	file.open(filename);
    if(file.is_open()) {
		std::string line = "";
		while (std::getline(file, line)) {
			dlcData.append(line);
		}
		file.close();
		dlcKey = dlcData.substr(dlcData.size() - 88);
		dlcData = dlcData.substr(0, dlcData.size() - 88);
		logger_.debug("Key: " + dlcKey);
		logger_.debug("Requestes Key: " + requestKey(dlcKey));
	} else {
		logger_.error("Could not open file: " + filename);
	}
	return links;
}

std::vector<std::string> DLCParser::parse(const std::string& content)
{
	std::vector<std::string> links;
	return links;
}

std::string DLCParser::requestKey(const std::string& dlcKey)
{
	//service.jdownloader.org
	const static std::string apiURL = "http://localhost/dlcrypt/service.php";
	const static std::initializer_list<std::string> headers = {
				"User-Agent: Mozilla/5.0",
				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
				"Accept-Language: de,en-gb;q=0.7, en;q=0.3",
				"Accept-Encoding: gzip,deflate",
				"Cache-Control: no-cache",
				"Pragma: no-cache",
				"rev: 26303"};

	std::ostringstream resultStream;
	curl_writer writer(resultStream);
	curl_form form;
    curl_easy easy(writer);
    //curl_header header(headers);
	logger_.debug("LEBE VOR MICH HIN");

    // Forms creation
    const curl_pair<CURLformoption,string> dtype_form(	CURLFORM_COPYNAME,		"destType");
    const curl_pair<CURLformoption,string> dtype_cont(	CURLFORM_COPYCONTENTS,	"jdtc6");
    const curl_pair<CURLformoption,string> app_form(	CURLFORM_COPYNAME,		"b");
    const curl_pair<CURLformoption,string> app_cont(	CURLFORM_COPYCONTENTS,	"JD");
	const curl_pair<CURLformoption,string> stype_form(	CURLFORM_COPYNAME,		"srcType");
    const curl_pair<CURLformoption,string> stype_cont(	CURLFORM_COPYCONTENTS,	"dlc");
    const curl_pair<CURLformoption,string> data_form(	CURLFORM_COPYNAME, 		"data");
    const curl_pair<CURLformoption,string> data_cont(	CURLFORM_COPYCONTENTS,	dlcKey);
    const curl_pair<CURLformoption,string> ver_form(	CURLFORM_COPYNAME,		"v");
    const curl_pair<CURLformoption,string> ver_cont(	CURLFORM_COPYCONTENTS,	"26303");
	logger_.debug("LEBE VOR TRY");
    try {
        // Form adding
        form.add(dtype_form,dtype_cont);
        form.add(app_form,app_cont);
        form.add(stype_form,stype_cont);
        form.add(data_form,data_cont);
        form.add(ver_form,ver_cont);

        // Add some options to our request
        easy.add(curl_pair<CURLoption,string>(CURLOPT_URL,apiURL));
        easy.add(curl_pair<CURLoption,bool>(CURLOPT_SSL_VERIFYPEER,false));
        easy.add(curl_pair<CURLoption,curl_form>(CURLOPT_HTTPPOST,form));
        //easy.add(curl_pair<CURLoption,curl_header>(CURLOPT_HTTPHEADER,header));
        easy.perform();
    } catch (curl_easy_exception error) {
        // Print errors, if any
        logger_.debug("LEBE");
        logger_.error("requestKey(): Throws exception.", error.what());
    }
    return resultStream.str();
}
