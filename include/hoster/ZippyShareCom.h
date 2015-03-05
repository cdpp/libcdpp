#ifndef ZIPPYSHARECOM_H
#define ZIPPYSHARECOM_H

#include "hoster/Hoster.h"
#include "basics/Logger.h"

namespace cdpp {
	class ZippyShareCom : public Hoster
	{
		public:
			/** Default constructor */
			ZippyShareCom();
			/** Default destructor */
			virtual ~ZippyShareCom();
			bool checkFile(FileInfo& file);
		private:
			Logger logger_ = Logger::getLogger();
	};
}
#endif // ZIPPYSHARECOM_H
