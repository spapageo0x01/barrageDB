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

#include "db_ops.hpp"

#define CONNECTION_ERROR -1

//TODO: Command line arguments instead of static string
//TODO: Create .hpp file instead of using all these defines here

int execute_query(std::string connection_string, std::string sql_query)
{
	try {
		pqxx::connection connection(connection_string);

		if (!connection.is_open()) {
			//std::cerr << "Can't open database" << std::endl;
			return CONNECTION_ERROR;
		}

		pqxx::work work(connection);

		work.exec(sql_query);
		work.commit();

		connection.disconnect();
		return 1;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return CONNECTION_ERROR; // or 0?
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
int insert_entry(std::string connection_string, struct row_data data)
{
	std::string query;

	query = "INSERT INTO barrage_data"
			"(num, string_a, string_b, sha256)" \
			"VALUES ('" + std::to_string(data.num) + "', '"+ data.string_a +"', '" + data.string_b +"', '" + data.sha_digest +"')";

	return execute_query(connection_string, query);
}

int get_row_count_slow(std::string connection_string)
{

	try {
		pqxx::connection connection(connection_string);

		if (!connection.is_open()) {
			//std::cerr << "Can't open database" << std::endl;
			return CONNECTION_ERROR;
		}

		pqxx::work work(connection);

		pqxx::result result(work.exec("SELECT count(sid) FROM barrage_data"));

		connection.disconnect();

		std::cout << "Rows: " << result[0][0].c_str() << std::endl;

		return atoi(result[0][0].c_str());
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return CONNECTION_ERROR; 
	}

}

int read_entry_random(std::string connection_string, struct row_data *data)
{
	std::string query;

	// Get number of tuples
	// Select a row at random
	// Query and get data for that row

	//return execute_query(connection_string, query);
}

int read_entry_sequential(std::string connection_string, unsigned int row_number, struct row_data *data)
{
	try {
		pqxx::connection connection(connection_string);

		if (!connection.is_open()) {
			//std::cerr << "Can't open database" << std::endl;
			return CONNECTION_ERROR;
		}

		pqxx::work work(connection);

		pqxx::result result(work.exec("SELECT num,string_a,string_b,sha256 FROM barrage_data WHERE sid="+ std::to_string(row_number)));

		connection.disconnect();

		for (int row_num = 0; row_num < result.size(); ++row_num) {
			const pqxx::result::tuple row = result[row_num];
			for (int col_num = 0; col_num < row.size(); ++col_num) {
				const pqxx::field field = row[col_num];

				std::string temp = field.c_str();
				switch(col_num){
					case 0:	//num
					{
						std::string::size_type sz = 0;
						data->num = std::stoull(temp, &sz, 0);
					}
						break;
					case 1: //string_a
						data->string_a = temp;
						break;
					case 2: //string_b
						data->string_b = temp;
						break;
					case 3: //sha256
						data->sha_digest = temp;
						break;
					default:
						std::cout << "Error: unrecognized column type" << std::endl;
						break;
				}
			}
		}

		return 1;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return CONNECTION_ERROR;
	}
}


/*
	Drop the barrage_data table from the database.
	On success => return 1
	On failure => return CONNECTION_ERROR
*/
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