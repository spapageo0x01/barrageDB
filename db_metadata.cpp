/*
    This file is part of barrageDB
    Copyright (C) 2015  Spyridon Papageorgiou

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "db_metadata.hpp"

void db_metadata::load(const std::string &filename)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(filename, pt);

	dbname = pt.get<std::string>("database.dbname");
	user = pt.get<std::string>("database.user");
	password = pt.get<std::string>("database.password");
	hostaddr = pt.get<std::string>("database.hostaddr");
	port = pt.get<std::string>("database.port");
}

void db_metadata::save(const std::string &filename)
{
	return;	
}

void db_metadata::print_contents(void)
{
	std::cout << "dbname   = "<< dbname << std::endl;
	std::cout << "user     = " << user << std::endl;
	std::cout << "password = "<< password <<std::endl;
	std::cout << "hostaddr = "<< hostaddr <<std::endl;
	std::cout << "port     = " << port << std::endl;
}

std::string db_metadata::generate_connection_string(void)
{
	std::string db_string = "dbname=" + dbname  + " user=" + user + " password=" + password + " hostaddr=" + hostaddr + " port=" + port;
	return db_string;
}