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
#include "crypto/sha256.hpp"
#include "worker_thread.hpp"

WorkerThread::WorkerThread(int thread_id, WorkerType work_type)
{
        std::cout << "Worker thread constructor | tid: " << thread_id << std::endl;
        tid = thread_id;
        type = work_type;
}

void WorkerThread::start()
{
        this_thread = boost::thread(&WorkerThread::do_work, this);
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

int WorkerThread::do_work(void)
{
        int op;
        std::cout << "[tid: " << tid << "] Running.." << std::endl;

        // At this point we should randomly select among a specific
        // set of SQL queries
        //  1) Insert a new entry to the database
        //  2) Read (and validate) an entry
        //  3) Modify an existing entry (need locking or can we use transactions?)
        //  4) Delete an existing entry (same as above)
        //Could implement this by using a query_generator class, storing 4 function
        //pointers and rand()

       
        if (type == VALIDATOR) {
            //Read sequentialy
        } else {
            std::random_device rd;       
            std::default_random_engine rng(rd());
            std::uniform_int_distribution<int> dist(0, QUERY_MAX - 1);

            op = dist(rng);
            switch(op) {
                case INSERT_QUERY:
                    insert_query();
                case READ_QUERY:
                    read_query();
                default:
                    break;
            }
        }

        std::cout << "[tid: " << tid << "] Finished running.." << std::endl;
}

int WorkerThread::insert_query(void)
{
    //rand uint64_t
    std::string str1 = __generate_rand_string(256);
    std::string str2 = __generate_rand_string(256);
    //std::digest = sha256(str1+str2+to_string(num));

    return 0;
}

int WorkerThread::read_query(void)
{
    return 0;
}

boost::thread *WorkerThread::get_thread_descriptor(void)
{
    return &this_thread;
}

/*********************** WorkGroup **************************/

WorkGroup::WorkGroup(std::string db_string, int number_of_threads, int duration, WorkerType work_type)
{
    run_time = duration;
    thread_count = number_of_threads;

    for (int i = 0; i < number_of_threads; ++i) {
        threads.push_back(new WorkerThread(i, work_type));
        threads[i]->set_connection_string(db_string);
    }
}

void WorkGroup::start(void)
{
    for (int i = 0; i < thread_count; ++i) {
        threads[i]->start();
    }
    
}

void WorkGroup::wait_all(void)
{
    sleep(run_time);

    // Raise interrupts
    for (int i = 0; i < thread_count; ++i) {
        (threads[i]->get_thread_descriptor())->interrupt();
    }

    // Join all
    for (int i = 0; i < thread_count; ++i) {
        threads[i]->join();
    }
}