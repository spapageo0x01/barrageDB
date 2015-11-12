#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

class WorkerThread {
	private:
		int tid;
		boost::thread this_thread;

	public:
		WorkerThread(int);
		void start(int N);
		void join(void);

		int get_tid(void);
		int do_work(int N);
};
