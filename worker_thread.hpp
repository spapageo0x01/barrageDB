#include <iostream>

class WorkerThread {
	private:
		int tid;
		boost::thread thread;

	public:
		WorkerThread(int);
		void start(int N);
		void join(void);

		int get_tid(void);
		int do_work(int N);
};

WorkerThread::WorkerThread(int thread_id)
{
	std::cout << "Worker thread constructor | tid: " << thread_id << std::endl;
	tid = thread_id;
}

void WorkerThread::start(int N)
{
	thread = boost::thread(&WorkerThread::do_work, this, N);
}

void WorkerThread::join()
{
	thread.join();
}

int WorkerThread::get_tid(void)
{
	return tid;
}


//
int WorkerThread::do_work(int N){
	std::cout << "[tid: " << tid << "] Running.." << std::endl;

	boost::posix_time::seconds work_time(2);
	//Pretend to do something useful..
	boost::this_thread::sleep(work_time);

	std::cout << "[tid: " << tid << "] Finished running.." << std::endl;
}
