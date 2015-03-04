#ifndef SHAREONLINEBZ_H
#define SHAREONLINEBZ_H

#include "hoster/Hoster.h"
#include "basics/Logger.h"

#include <string>

namespace cdpp {
	class ShareOnlineBz : public Hoster
	{
		public:
			/** Default constructor */
			ShareOnlineBz();
			/** Default destructor */
			virtual ~ShareOnlineBz();
			bool checkFile(FileInfo& file);
			bool checkFile(std::vector<FileInfo>& files);
		private:
			Logger logger_ = Logger::getLogger();
			std::vector<std::string> split(const std::string& str, const char delim);
	};
}
#endif // SHAREONLINEBZ_H
