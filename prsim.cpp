#include "prsim.h"

std::vector<PRCPU*> procs;
RAM *ram;
PRassembler *assem;
std::string simfile;
int firstadd;
int p; //number of processors

int CLOCK;

PRsim::PRsim(std::string filename, int _p)
{
    ram = new RAM(2000);
    //cpu = new PRCPU(32,32,32,ram);
    ram->RAM_info();
    simfile = filename;
    firstadd = 100;
    p = _p;

    for(int i = 0; i<p; i++){
        procs.push_back(new PRCPU(32,32,ram,i));
        std::cout << "Added proc "<< procs[i]->getID() <<"\n";
    }
    assem = new PRassembler(procs[0]->getInstructionDesc());
    CLOCK = 0;
}


void PRsim::assembleAndLoad(){
    //std::cout << "Assemble and Load\n";
    std::vector<std::tuple<int,std::string>> assembled =  assem->assemble(simfile,firstadd);
    if(assembled.size() == 0){
        std::cout << "UWsim: Error assembling file \n";
    }else{
        for(unsigned int i = 0; i < assembled.size(); i++){
            ram->set(std::get<0>(assembled[i]), std::get<1>(assembled[i]));
        }
    }
}

int PRsim::executeProgram(){
    //set starting PC for all processors
    for(int i =0; i<p; i++){
        procs[i]->setPC(firstadd);
    }
    //finish when p0 halts
    bool finished = false;

    while(!finished){
        std::tuple<int,std::string> check;
        for(unsigned int i = 0; i<procs.size(); i++){
            if(procs[i]->isHalted()){
                continue;
            }
            procs[i]->updateIR();
            int npc = procs[i]->getPC() +1;
            procs[i]->setPC(npc);
            //std::cout<< "proc "<<i<<" sent to handle instruction. \n";
            check = procs[i]->handleInstruction();

            if(std::get<0>(check) != 0){
                std::cout<< "ERROR: " <<"P"<<i<<": "<< procs[i]->getPC() << " , " << std::get<1>(check)<< "\n";
                return CLOCK;
            }
        }
        CLOCK++;
        if(procs[0]->isHalted()){
            finished = true;
        }
    }

    return CLOCK;
}

void PRsim::runSimulation(){
    assembleAndLoad();
    int clock  = executeProgram();
    std::cout<<"Number of instructions: " <<clock<<"\n";
}


void PRsim::printFromRAM(int from, int to){
    ram->print_range(from,to);
}

void PRsim::printRegister(int proc,int i){
    procs[proc]->printRegister(i);
}

PRsim::~PRsim(){
    for(int i = 0; i<p; i++){
        delete(procs[i]);
    }
    delete ram;
    delete assem;
}
