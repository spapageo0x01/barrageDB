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

