#include <iostream>
#include <vector>

#include "worker_thread.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	int i = 0;
	vector<WorkerThread *> threads;

	cout << "Main: startup" << endl;

	for (i = 0; i < 10; i++) {
		threads.push_back(new WorkerThread(i));
		threads[i]->start(10+i);
	}

	for (i = 0; i < 10; i++) {
		threads[i]->join();
	}

	cout << "Main: done" << endl;

	return 0;
}
