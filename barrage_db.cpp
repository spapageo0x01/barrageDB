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