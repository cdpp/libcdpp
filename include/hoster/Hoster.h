#ifndef HOSTER_H_INCLUDED
#define HOSTER_H_INCLUDED

#include "data/Package.h"
#include <vector>

namespace cdpp {
	class Hoster
	{
	public:
		virtual bool checkFile(FileInfo& file) = 0;
		virtual bool checkFile(std::vector<FileInfo>& files) = 0;
	};
}
#endif // HOSTER_H_INCLUDED
