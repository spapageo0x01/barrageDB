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
	std::cout << "dbname = "<< dbname << std::endl;
	std::cout << "user = " << user << std::endl;
	std::cout << "password = "<< password <<std::endl;
	std::cout << "hostaddr = "<< hostaddr <<std::endl;
	std::cout << "port = " << port << std::endl;
}

std::string db_metadata::generate_connection_string(void)
{
	std::string db_string;

	//std::string connection_string = "dbname=barrage_test user=postgres password=112358 hostaddr=127.0.0.1 port=5432";
	db_string = "dbname=" + dbname  + " user=" + user + " password=" + password + " hostaddr=" + hostaddr + " port=" + port;
	return db_string;
}