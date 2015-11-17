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
#include <cmath>
#include "worker_thread.hpp"
#include "db_ops.hpp"
#include "crypto/sha256.hpp"


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

int WorkerThread::do_work(void)
{
        std::cout << "[tid: " << tid << "] Running.." << std::endl;

        // At this point we should randomly select among a specific
        // set of SQL queries
        //  1) Insert a new entry to the database
        //  2) Read (and validate) an entry
        //  3) Modify an existing entry (need locking or can we use transactions?)
        //  4) Delete an existing entry (same as above)
        //Could implement this by using a query_generator class, storing 4 function
        //pointers and rand()

        switch(type) {
            case VALIDATOR:
                {
                    //Read sequentialy
                    int ret = get_row_count_slow(connection_string);
                    if (ret == CONNECTION_ERROR) {
                        std::cout << "Unable to connect to database!" << std::endl;
                        break;
                    }

                    int db_rows = ret;
                    std::cout << ">Table rows: " << db_rows << std::endl;
                    std::cout << ">Validating.." << std::endl;

                    int validation_errors = 0;
                    for (int i = 1; i < db_rows; ++i){
                        if(!read_query_sequential(i)){
                            ++validation_errors;
                        }
                    }
                    std::cout << "============================" << std::endl;
                    std::cout << ">Validated " << db_rows << " tuples." << std::endl;
                    std::cout << ">>Total # of errors: " << validation_errors << std::endl;
                }
                break;
            case GENERATOR:
                {
                    std::random_device rd;       
                    std::default_random_engine rng(rd());
                    std::uniform_int_distribution<int> dist(0, QUERY_MAX - 1);

                    while(1) {
                        try {
                            int op = dist(rng);
                            switch(op) {
                                case INSERT_QUERY:
                                    insert_query();
                                    break;
                                case READ_QUERY:
                                    //read_query_random();
                                    break;
                                default:
                                    break;
                            }
                            boost::this_thread::interruption_point();
                        }
                        catch (boost::thread_interrupted const&)
                        {
                            std::cout << "Worker: interrupted!" << std::endl;
                            break;
                        }
                    }
                }
                break;
            default:
                break;
        }

        std::cout << "[tid: " << tid << "] Finished running.." << std::endl;
}


uint64_t __64bit_generator()
{
    std::random_device rd;

    std::mt19937_64 e2(rd());

    std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2,61)), std::llround(std::pow(2,62)));
    return dist(e2);
}

char __string_generator() {
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
        std::generate_n(std::back_inserter(rand_string), length, __string_generator);
        return rand_string;
}

int WorkerThread::insert_query(void)
{
    struct row_data data;

    data.num =  __64bit_generator();
    data.string_a = __generate_rand_string(256);
    data.string_b = __generate_rand_string(256);
    data.sha_digest = sha256(data.string_a + data.string_b + std::to_string(data.num));

    // uint64_t num = __64bit_generator();
    // std::string str1 = __generate_rand_string(256);
    // std::string str2 = __generate_rand_string(256);
    // std::string sha_digest = sha256(str1+str2+std::to_string(num));
    //insert_entry(connection_string, num, str1, str2, sha_digest);
    insert_entry(connection_string, data);

    return 0;
}

int WorkerThread::read_query_random(void)
{
    struct row_data data;

    read_entry_random(connection_string, &data);

    std::string validation_sha = sha256(data.string_a + data.string_b + std::to_string(data.num));

    if (validation_sha.compare(data.sha_digest)) {
        std::cout << "VALIDATION ERROR: sha256 checksum mismatch (expected: '" << validation_sha << "', found: '" << data.sha_digest << "'" << std::endl;
    }

    return 0;
}

int WorkerThread::read_query_sequential(int row_number)
{
    struct row_data data;

    read_entry_sequential(connection_string, row_number, &data);

    std::string validation_sha = sha256(data.string_a + data.string_b + std::to_string(data.num));

    if (validation_sha.compare(data.sha_digest)) {
        std::cout << "VALIDATION ERROR @ row " << row_number <<"  : sha256 checksum mismatch (expected: '" << validation_sha << "', found: '" << data.sha_digest << "'" << std::endl;
        return 0;
    }

    return 1;
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
    type = work_type;

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
    switch(type) {
        case GENERATOR:
            sleep(run_time);

            // Raise interrupts
            for (int i = 0; i < thread_count; ++i) {
                (threads[i]->get_thread_descriptor())->interrupt();
            }

            // Join all
            for (int i = 0; i < thread_count; ++i) {
                threads[i]->join();
            }
            break;
        case VALIDATOR:
            for (int i = 0; i < thread_count; ++i) {
                threads[i]->join();
            }
            break;
        default:
            break;
    }
}