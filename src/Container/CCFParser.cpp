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

std::vector<cdpp::Package> CCFParser::parse(const std::string& content)
{
	return std::vector<cdpp::Package>();
}

std::vector<cdpp::Package> CCFParser::parseFile(const std::string& filename)
{
	return parse("");
}
