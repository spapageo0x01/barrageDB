#ifndef DB_METADATA_H
#define DB_METADATA_H

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

class db_metadata {
	std::string dbname;
	std::string user;
	std::string password;
	std::string hostaddr;
	std::string port;

	public:
		void load(const std::string &filename);
		void save(const std::string &filename);	// Not implemented yet
		std::string generate_connection_string(void);
		void print_contents(void);
};

#endif