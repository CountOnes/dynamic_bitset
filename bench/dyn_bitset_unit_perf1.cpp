// -----------------------------------------------------------
//             Copyright (c) 2016 Wojciech Muła and Daniel Lemire
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// -----------------------------------------------------------


// assuming a working boost implementation, this can be built manually, e.g. as g++ -O3 -o dyn_bitset_unit_perf1 dyn_bitset_unit_perf1.cpp -I../include -lboost_chrono -lboost_system

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <boost/chrono.hpp>
#include <boost/cstdint.hpp>
#include <boost/dynamic_bitset.hpp>

#define BEST_TIME_NS(test, repeat)                               \
        do {                                                            \
            std::cout << std::setw(20) << #test << ":";                                   \
            fflush(NULL);                                               \
            long int min_diff = 0xFFFFFF;                               \
            for (int i = 0; i < repeat; i++) {                          \
                __asm volatile("" ::: /* pretend to clobber */ "memory"); \
                boost::chrono::high_resolution_clock::time_point start = boost::chrono::high_resolution_clock::now(); \
                test;                                                   \
                boost::chrono::nanoseconds ns = boost::chrono::duration_cast<boost::chrono::nanoseconds> (boost::chrono::high_resolution_clock::now() - start); \
                long int cycles_diff = ns.count();              \
                if (cycles_diff < min_diff) min_diff = cycles_diff;       \
            }                                                             \
            std::cout << std::setw(20) << min_diff <<" ns"<< std::endl;                       \
 } while (0)

#define BEST_TIME_NS_CHECKED(test, correct, repeat)                               \
        do {                                                            \
            std::cout << std::setw(20) << #test << ":";                                   \
            fflush(NULL);                                               \
            long int min_diff = 0xFFFFFF;                               \
            for (int i = 0; i < repeat; i++) {                          \
                __asm volatile("" ::: /* pretend to clobber */ "memory"); \
                boost::chrono::high_resolution_clock::time_point start = boost::chrono::high_resolution_clock::now(); \
                if (test != correct) std::cout << "bug";                                                   \
                boost::chrono::nanoseconds ns = boost::chrono::duration_cast<boost::chrono::nanoseconds> (boost::chrono::high_resolution_clock::now() - start); \
                long int cycles_diff = ns.count();              \
                if (cycles_diff < min_diff) min_diff = cycles_diff;       \
            }                                                             \
            std::cout << std::setw (20) << min_diff <<" ns"<< std::endl;                       \
 } while (0)



void create() {
    boost::dynamic_bitset<> b1;
    b1.resize(100000000);
    for(uint32_t k = 0; k < 100000000; k += 100) {
      b1.set(k);
    }
}

size_t iterate(boost::dynamic_bitset<> & b1) {
    size_t count = 0;
    for(size_t index = b1.find_first(); index != b1.npos; index = b1.find_next(index)) {
      ++count;
    }
    return count;
}




int main(int argc,  char** agv) {
  int repeat = 10;
  BEST_TIME_NS(create(),repeat);
  boost::dynamic_bitset<> b1;
  b1.resize(100000000);
  size_t c = 0;
  for(uint32_t k = 0; k < 100000000; k += 100) {
    b1.set(k);
    c++;
  }
  BEST_TIME_NS_CHECKED(iterate(b1),c,repeat);
  BEST_TIME_NS_CHECKED(b1.count(),c,repeat);
  
  return 0;
}
