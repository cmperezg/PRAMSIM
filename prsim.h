#ifndef PRSIM_H
#define PRSIM_H

#include <map>
#include <string>
#include <vector>
#include <iostream>     // std::cout
#include <fstream>
#include <sstream>
#include <tuple>

#include "prcpu.h"
#include "ram.h"
#include "prassembler.h"

class PRsim
{
public:
    PRsim(std::string filename, int _p);

    void assembleAndLoad();
    int executeProgram();
    void runSimulation();

    void printFromRAM(int from, int to);
    void printRegister(int proc, int i);

    ~PRsim();
};

#endif // PRSIM_H
