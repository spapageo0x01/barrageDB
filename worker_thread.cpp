#include <iostream>
#include "worker_thread.hpp"

WorkerThread::WorkerThread(int thread_id)
{
        std::cout << "Worker thread constructor | tid: " << thread_id << std::endl;
        tid = thread_id;
}

void WorkerThread::start(int N)
{
        this_thread = boost::thread(&WorkerThread::do_work, this, N);
}

void WorkerThread::join()
{
        this_thread.join();
}


void WorkerThread::set_connection_string(std::string str)
{
        connection_string = str;
}


int WorkerThread::get_tid(void)
{
        return tid;
}

int WorkerThread::do_work(int N)
{
        std::cout << "[tid: " << tid << "] Running.." << std::endl;

        boost::posix_time::seconds work_time(2);
        //Pretend to do something useful..
        boost::this_thread::sleep(work_time);

        std::cout << "[tid: " << tid << "] Finished running.." << std::endl;
}

