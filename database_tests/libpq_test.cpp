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

#include <iostream>
#include <string>
#include <pqxx/pqxx>

#define CONNECTION_ERROR -1

//TODO: Command line arguments instead of static string
//TODO: Create .hpp file instead of using all these defines here


int create_table(std::string connection_string)
{
	std::string sql_query;

	try {
		pqxx::connection connection(connection_string);

		if (connection.is_open()) {
			std::cout << "Opened database succesffully: " << connection.dbname() << std::endl;
		} else {
			std::cerr << "Can't open database" << std::endl;
			return CONNECTION_ERROR;
		}
		
		sql_query = "CREATE TABLE tb1("			\
		"ID 	INT 	PRIMARY KEY NOT NULL,"	\
		"NAME 	TEXT 				NOT NULL,"	\
		"AGE	INT 				NOT NULL,"	\
		"ADDR	CHAR(50),"						\
		"SALARY	REAL);";

		pqxx::work work(connection);

		work.exec(sql_query);
		work.commit();
		std::cout << "Table created successfully" << std::endl;

		connection.disconnect();
		return 0;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return CONNECTION_ERROR;
	}	
}

int drop_table(std::string connection_string)
{
	std::string sql_query;

	try {
		pqxx::connection connection(connection_string);

		if (connection.is_open()) {
			std::cout << "Opened database succesffully: " << connection.dbname() << std::endl;
		} else {
			std::cerr << "Can't open database" << std::endl;
			return CONNECTION_ERROR;
		}
		
		sql_query = "DROP TABLE tb1";

		pqxx::work work(connection);

		work.exec(sql_query);
		work.commit();
		std::cout << "Table dropped successfully" << std::endl;

		connection.disconnect();
		return 0;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return CONNECTION_ERROR;
	}	
}

int check_table_exists(std::string connection_string)
{
	std::string sql_query;
	int ret;

	try {
		pqxx::connection connection(connection_string);

		if (connection.is_open()) {
			std::cout << "Opened database succesffully: " << connection.dbname() << std::endl;
		} else {
			std::cerr << "Can't open database" << std::endl;
			return CONNECTION_ERROR;
		}
		
		sql_query = "SELECT relname FROM pg_class WHERE relname='tb1';";

		pqxx::nontransaction work(connection);
		pqxx::result result(work.exec(sql_query));

		connection.disconnect();

		if (!result.empty()) {
			std::cout << "Table exists" << std::endl;
			return 0;
		}

		return 1;
		
		/*for (pqxx::result::const_iterator iter = result.begin(); iter != result.end(); iter++) {
			std::cout << "erm" << iter[0] << std::endl;
		}*/
		
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return CONNECTION_ERROR;
	}	
}


int main(int argc, char * argv[])
{
	int ret;
	std::string connection_string = "dbname=barrage_test user=postgres password=112358 hostaddr=127.0.0.1 port=5432";

	std::cout << ">>libpq test<<" << std::endl;

	ret = check_table_exists(connection_string);

	if (ret && ret != CONNECTION_ERROR) {
		create_table(connection_string);
	}

	//drop_table(connection_string);
}