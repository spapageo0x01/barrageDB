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
#include <vector>
#include <boost/program_options.hpp>

#include "worker_thread.hpp"
#include "db_metadata.hpp"
#include "db_ops.hpp"

int main(int argc, char *argv[])
{
	int i = 0, ret;
	int thread_count;
	int cleanup;
	int duration;
	std::string conf_file;
	db_metadata db;
	//std::vector<WorkerThread *> threads;
	//WorkGroup group1;
	std::cout << "Main: startup" << std::endl;

	namespace po = boost::program_options;
	po::options_description desc("barrageDB options");

	desc.add_options()
		("help", "generates help message")
		("threads", po::value<int>(&thread_count)->default_value(10), "set number of threads")
		("time", po::value<int>(&duration)->default_value(60), "set time to run (in seconds)")
		("conf-file", po::value<std::string>(&conf_file)->default_value("confing.ini"), "configuration file name")
		("validate-offline", "validate database contents offline")
		("cleanup", po::value<int>(&cleanup)->default_value(1), "cleanup database on exit");

	po::variables_map var_map;
	po::store(po::parse_command_line(argc, argv, desc), var_map);
	po::notify(var_map);

	if (var_map.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}


	try {
		std::cout << "========= barrageDB ========" << std::endl;
		std::cout << ">Number of threads  : " << thread_count << std::endl;
		std::cout << ">Test duration      : " << duration << std::endl;
		std::cout << ">configuration file : " << conf_file << std::endl;

		db.load(conf_file);
		db.print_contents();

		ret = check_table_exists(db.generate_connection_string());
		if (!ret) {
			create_table(db.generate_connection_string());
		} else {
			// Failure handling
		}

		WorkGroup group1(db.generate_connection_string(), thread_count); 
		group1.start();
		group1.wait_all();

		ret = drop_table(db.generate_connection_string());
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}

	std::cout << "Main: done" << std::endl;

	return 0;
}