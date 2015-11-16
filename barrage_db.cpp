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

int main()
{
	int i = 0, ret;
	db_metadata db;
	//std::vector<WorkerThread *> threads;
	//WorkGroup group1;
	std::cout << "Main: startup" << std::endl;

	/*
	namespace po = boost::program_options;
	po::options_description desc("barrageDB options");

	desc.add_options()
		("help", "generates help message")
		("threads", po::value<int>(), "set number of threads")
		("time", po::value<unsigned int>(), "set time to run (in seconds)")
		("conf", string, "configuration file name")
		("validate-offline", "validate database contents offline")
		("cleanup", po::value<int>(), "cleanup database on exit");

	po::variables_map var_map;
	po::store(po::parse_command_line(ac, av, desc), var_map);
	po::notify(var_map);

	if (vm.count("help")) {
		count << desc << "\n";
		return 1;
	}
	*/

	try {
		db.load("config.ini");
		db.print_contents();

		ret = check_table_exists(db.generate_connection_string());
		if (!ret) {
			create_table(db.generate_connection_string());
		} else {
			// Failure handling
		}

		WorkGroup group1(db.generate_connection_string(), 10); 
		group1.start();
		group1.wait_all();

		/*
		for (i = 0; i < 10; i++) {
			threads.push_back(new WorkerThread(i));
			threads[i]->set_connection_string(db.generate_connection_string());
			threads[i]->start(10+i);
		}

		for (i = 0; i < 10; i++) {
			threads[i]->join();
		}
		*/

		ret = drop_table(db.generate_connection_string());
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}

	std::cout << "Main: done" << std::endl;

	return 0;
}