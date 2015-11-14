#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

class WorkerThread {
	private:
		int tid;
		std::string connection_string;
		boost::thread this_thread;

	public:
		WorkerThread(int);
		void start(int N);
		void join(void);
		void set_connection_string(std::string usr_str);
		int get_tid(void);
		int do_work(int N);
};

#endif