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
