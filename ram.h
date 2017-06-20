#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <map>
#include <string>
#include <cmath>
class RAM
{
 public:
    //number of words
    int SIZE;

    //the RAM (stores strings for rep purposes)
    std::map<int,std::string> ram;

    RAM(int _size);
    std::string get(int memloc);
    void set(int memloc, std::string content);
    void RAM_info();
    void print_range(int from, int to);
};

#endif // RAM_H
