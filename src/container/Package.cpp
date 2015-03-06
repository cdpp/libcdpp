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

#include "data/Package.h"

using namespace cdpp;

Package::Package()
{
	name_ = "unknown";
}

Package::~Package()
{
	//dtor
}

void Package::addFile(FileInfo file)
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

std::vector<FileInfo> Package::getFiles()
{
	return files_;
}
