/*
 * pc_shared_ofstream.cpp - this source file contains shared ofstream
 * to synchronize output of several threads to console
 * (c) 2020 Anna Chertova
 */

#include "pc_shared_ostream.h"

// Create synchronized output entities
pc_shared_ostream shared_cout(std::cout);
pc_shared_ostream shared_cerr(std::cerr);