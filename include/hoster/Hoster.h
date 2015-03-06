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

#ifndef HOSTER_H_INCLUDED
#define HOSTER_H_INCLUDED

#include "data/Package.h"
#include <vector>

namespace cdpp {
	class Hoster
	{
		public:
			virtual bool checkFile(FileInfo& file) = 0;
			bool checkFiles(std::vector<FileInfo>& files)
			{
				bool failed = false;
				for (auto file_itr = files.begin(); file_itr != files.end(); file_itr++) {
					failed = !checkFile(*file_itr);
				}
				return !failed;
			};
	};
}
#endif // HOSTER_H_INCLUDED
