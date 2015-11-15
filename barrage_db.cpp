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

#include "worker_thread.hpp"
#include "db_metadata.hpp"

#include "boost/program_options.hpp"

int main()
{
	int i = 0;
	db_metadata db;
	std::vector<WorkerThread *> threads;

	std::cout << "Main: startup" << std::endl;

	try {
		db.load("config.ini");
		db.print_contents();

		for (i = 0; i < 10; i++) {
			threads.push_back(new WorkerThread(i));
			threads[i]->set_connection_string(db.generate_connection_string());
			threads[i]->start(10+i);
		}

		for (i = 0; i < 10; i++) {
			threads[i]->join();
		}
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}

	std::cout << "Main: done" << std::endl;

	return 0;
}