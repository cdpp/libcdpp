#ifndef CDPP_H_INCLUDED
#define CDPP_H_INCLUDED

#include <string>
#include <vector>

#if __cplusplus<201103L
	#error A C++ Compiler with C++11 support is required!
#endif // __cplusplus
namespace cdpp {
	typedef std::vector<std::pair<std::string,std::string>> err_trace;
}
#endif // CDPP_H_INCLUDED
