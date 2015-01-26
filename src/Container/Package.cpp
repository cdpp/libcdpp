#include "Container/Package.h"

using namespace cdpp;

Package::Package()
{
	name_ = "unknown";
}

Package::~Package()
{
	//dtor
}

void Package::addFile(FileLink file)
{
	files_.push_back(file);
}

void Package::setPassword(std::string password)
{
	password_ = password;
}

void Package::setName(std::string name)
{
	name_ = name;
}

std::string Package::getName()
{
	return name_;
}

std::string Package::getPassword()
{
	return password_;
}

std::vector<FileLink> Package::getFiles()
{
	return files_;
}
