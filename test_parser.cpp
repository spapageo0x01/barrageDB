#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

//Check http://www.boost.org/doc/libs/1_59_0/doc/html/property_tree/tutorial.html

struct db_info {
	std::string dbname;
	std::string user;
	std::string password;
	std::string hostaddr;
	std::string port;

	void load(const std::string &filename);
	void print_contents(void);
};


// TODO: Add error handling code. What happens if the file does not exist?
void db_info::load(const std::string &filename)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(filename, pt);

	dbname = pt.get<std::string>("database.dbname");
	user = pt.get<std::string>("database.user");
	password = pt.get<std::string>("database.password");
	hostaddr = pt.get<std::string>("database.hostaddr");
	port = pt.get<std::string>("database.port");
}

void db_info::print_contents(void)
{
	std::cout << "dbname = "<< dbname << std::endl;
	std::cout << "user = " << user << std::endl;
	std::cout << "password = "<< password <<std::endl;
	std::cout << "hostaddr = "<< hostaddr <<std::endl;
	std::cout << "port = " << port << std::endl;

}

int main()
{
	db_info db;
	db.load("config.ini");
	db.print_contents();
}

