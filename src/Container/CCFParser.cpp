#include "Container/CCFParser.h"

using namespace cdpp;

CCFParser::CCFParser()
{
	//ctor
}

CCFParser::~CCFParser()
{
	//dtor
}

std::vector<std::string> CCFParser::parse(const std::string& content)
{
	return std::vector<std::string>();
}

std::vector<std::string> CCFParser::parseFile(const std::string& filename)
{
	return parse("");
}
