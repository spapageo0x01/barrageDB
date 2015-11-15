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