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


int execute_query(std::string connection_string, std::string sql_query)
{
	try {
		pqxx::connection connection(connection_string);

		if (connection.is_open()) {
			std::cout << "Opened database succesffully: " << connection.dbname() << std::endl;
		} else {
			std::cerr << "Can't open database" << std::endl;
			return CONNECTION_ERROR;
		}

		pqxx::work work(connection);

		work.exec(sql_query);
		work.commit();

		connection.disconnect();
		return 0;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return CONNECTION_ERROR;
	}	
}


int create_table(std::string connection_string)
{
	int ret;
	std::string query;

	query = "CREATE TABLE barrage_data("			\
	"sid SERIAL PRIMARY KEY NOT NULL,"	\
	"num bigint NOT NULL,"	\
	"string_a VARCHAR(256) NOT NULL,"	\
	"string_b VARCHAR(256) NOT NULL,"	\
	"sha256 VARCHAR(64) NOT NULL);";

	return execute_query(connection_string, query);
}

// User should provide data or should they be generated here?
int insert_entry(std::string connection_string,
				 unsigned long long num,
				 std::string str1,
				 std::string str2,
				 std::string sha_digest)
{
	std::string query;

	query = "INSERT INTO integrity_data"
			"(num, string_a, string_b, sha256)" \
			"VALUES (" + std::to_string(num) + ", "+ str1 +", " + str2 +", " + sha_digest +")";

	return execute_query(connection_string, query);
}

int drop_table(std::string connection_string)
{
	return execute_query(connection_string, "DROP TABLE barrage_data");
}


/*
	Check if the default "barrage_data" table exists in the database.
	If the table exists => return 1
	If the table does not exist => return 0
	If we are not able to connect to the database => return CONNECTION_ERROR
*/
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
		
		sql_query = "SELECT relname FROM pg_class WHERE relname='barrage_data';";

		pqxx::nontransaction work(connection);
		pqxx::result result(work.exec(sql_query));

		connection.disconnect();

		if (!result.empty()) {
			std::cout << "Table exists" << std::endl;
			return 1;
		}

		return 0;
		
		/*for (pqxx::result::const_iterator iter = result.begin(); iter != result.end(); iter++) {
			std::cout << "erm" << iter[0] << std::endl;
		}*/
		
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return CONNECTION_ERROR;
	}	
}