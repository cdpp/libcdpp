#include "hoster/ShareOnlineBz.h"
#include "curlcpp/include/curl_easy.h"
#include "curlcpp/include/curl_form.h"

#include <sstream>

using namespace cdpp;

ShareOnlineBz::ShareOnlineBz()
{
	//ctor
}

ShareOnlineBz::~ShareOnlineBz()
{
	//dtor
}

bool ShareOnlineBz::checkFile(FileInfo& file)
{
	std::vector<FileInfo> fi({file});
	bool ret = checkFiles(fi);
	file = fi.at(0);
	return ret;
}

bool ShareOnlineBz::checkFiles(std::vector<FileInfo>& files)
{
	const std::string api("http://api.share-online.biz/linkcheck.php?md5=1");
	std::string apiData("links=");
	for (FileInfo file : files) {
		 apiData.append(file.url + '\n');
	}

	std::ostringstream resultStream;
	curl::curl_writer writer(resultStream);
	curl::curl_easy easy(writer);
	try {
		// Add some options to our request
		easy.add(curl_pair<CURLoption, long>(CURLOPT_TIMEOUT,			180));
		easy.add(curl_pair<CURLoption, string>(CURLOPT_URL,				api));
		easy.add(curl_pair<CURLoption, string>(CURLOPT_POSTFIELDS,		apiData));
		easy.perform();
	} catch (curl_easy_exception exc) {
		// Print errors, if any
		std::string error;
		for(std::pair<std::string, std::string> trace : exc.what())
			error += std::string("ERROR: ") + trace.first + std::string(" :::: FUNCTION: ") + trace.second + std::string("\n\t");

		logger_.error("requestKey(): Throws exception.", error);
		for(unsigned char i = 0; i < files.size(); i++)
			files[i].status = FileStatus::FAILED;
		return false;
	}
	std::vector<std::string> info = split(resultStream.str(), ';');
	if (info.size() < 4) {
		for (unsigned char i = 0; i < files.size(); i++)
			files[i].status = FileStatus::FAILED;
		logger_.debug("Could not resolve info: Maybe wrong URL given.");
		return false;
	}
	for (unsigned char i = 0; i < files.size(); i++) {
		if (info.at(i*4+1) == "DELETED" || info.at(1) == "NOT FOUND") {
			files[i].status = FileStatus::NOT_FOUND;
		} else if (info.at(i*4+1) == "OK") {
			files[i].status = FileStatus::OK;
			files[i].name = info.at(i*4+2);
			files[i].size = std::stol(info.at(i*4+3));
			files[i].hash = info.at(i*4+4);
		} else {
			files[i].status = FileStatus::FAILED;
			return false;
		}
	}
	return true;
}

std::vector<std::string> ShareOnlineBz::split(const std::string& str, const char delim)
{
	std::vector<std::string> elems;
	std::stringstream ss(str);
    std::string item;
    while(std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}
