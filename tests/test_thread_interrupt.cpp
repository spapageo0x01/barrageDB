#include <iostream>
#include <boost/thread.hpp>

void workerFunc()
{
    std::cout << "Worker: running" << std::endl;

    while(1) {
    	try {
    		sleep(2);
    		std::cout << "Worker: woke up!" << std::endl;	
    		boost::this_thread::interruption_point();
    	}
    	catch (boost::thread_interrupted const&)
    	{
    		std::cout << "Worker: interrupted!" << std::endl;
    		break;
    	}

    }
    std::cout << "Worker: finished" << std::endl;
}

int main(){
	std::cout << "Main: startup" << std::endl;

	boost::thread workerThread(workerFunc);
	std::cout << "Main: waiting for thread" << std::endl;

	sleep(9);

	std::cout << "Main: Woke up - interrupting thread" << std::endl;
	workerThread.interrupt();
	workerThread.join();

}