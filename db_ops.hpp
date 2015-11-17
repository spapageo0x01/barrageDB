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

#ifndef DB_OPS_HPP
#define DB_OPS_HPP
#include <iostream>
#include <string>
#include <pqxx/pqxx>

#define CONNECTION_ERROR -1

struct row_data {
    uint64_t num;
    std::string string_a;
    std::string string_b;
    std::string sha_digest;
};

int create_table(std::string connection_string);
int drop_table(std::string connection_string);
int check_table_exists(std::string connection_string);
int insert_entry(std::string connection_string, struct row_data data);
int read_entry_random(std::string connection_string, struct row_data *data);
int read_entry_sequential(std::string connection_string, unsigned int row_number, struct row_data *data);

int get_row_count_slow(std::string connection_string);
#endif