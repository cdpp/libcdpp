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
			bool checkFiles(std::vector<FileInfo>& files);
		private:
			Logger logger_ = Logger::getLogger();
			std::vector<std::string> split(const std::string& str, const char delim);
	};
}
#endif // SHAREONLINEBZ_H
