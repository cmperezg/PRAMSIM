#include <map>
#include <string>
#include <vector>
#include <iostream>     // std::cout
#include <fstream>
#include <sstream>
#include <tuple>

#include "ram.h"
#include "prcpu.h"
#include "prassembler.h"
#include "prsim.h"


int main(int argc, char *argv[])
{
    std::string fname = "C://Users//cmpgt//OneDrive//Documentos//code//pramsim//zerocomp.prasm";
    std::string fname2 = "C://Users//cmpgt//OneDrive//Documentos//code//pramsim//simplest.prasm";

    std::cout<< "about to create sim\n";
    int numregs = 8;
    PRsim sim(fname2,numregs);
    sim.runSimulation();

    for(int i =0; i<numregs; i++){
        sim.printRegister(i,3);
    }
    return 0;
}
