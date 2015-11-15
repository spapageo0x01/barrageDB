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
#include <random>
#include <algorithm>
#include <iterator>
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

char __generator() {
        static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz"
                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                       "0123456789";
        std::random_device rd;
        std::default_random_engine rng(rd());
        std::uniform_int_distribution<> dist(0, sizeof(alphabet)/sizeof(*alphabet)-2);

        return alphabet[dist(rng)];
}

std::string __generate_rand_string(int length)
{
        std::string rand_string;
        std::generate_n(std::back_inserter(rand_string), length, __generator);
        return rand_string;
}

int WorkerThread::do_work(int N)
{
        std::string str1;
        std::cout << "[tid: " << tid << "] Running.." << std::endl;

        boost::posix_time::seconds work_time(2);
        //Pretend to do something useful..
        boost::this_thread::sleep(work_time);
        str1 = __generate_rand_string(256);
        std::cout << "str1: " << str1 << std::endl;


        std::cout << "[tid: " << tid << "] Finished running.." << std::endl;
}

