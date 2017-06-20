#ifndef PRCPU_H
#define PRCPU_H

#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <limits>
#include <bitset>

#include "ram.h"

class PRCPU
{
public:
    PRCPU(int _wordsize, int _regs, RAM *_ram, int id);

    std::map<std::string,std::string> *getInstructionDesc();

    //instructions
    std::tuple<int,std::string> ADD(int rd, int r1, int r2);
    std::tuple<int,std::string> ADDI(int rd, int r1, int imm);
    std::tuple<int,std::string> ADDU(int rd, int r1, int r2);
    std::tuple<int,std::string> AND(int rd, int r1, int r2);
    std::tuple<int,std::string> ANDI(int rd, int r1, int imm);
    std::tuple<int,std::string> BEQ(int r1, int r2, int add);
    std::tuple<int,std::string> BNE(int r1, int r2, int add);
    std::tuple<int,std::string> J(int add);
    std::tuple<int,std::string> JR(int r1);
    std::tuple<int,std::string> LI(int rd, int imm);

    std::tuple<int,std::string> LID(int rd);
    std::tuple<int,std::string> LW(int rd, int r1, int off, RAM *_ram);
    std::tuple<int,std::string> NOT(int rd, int r1);
    std::tuple<int,std::string> OR(int rd, int r1, int r2);
    std::tuple<int,std::string> ORI(int rd, int r1, int imm);
    std::tuple<int,std::string> SL(int rd, int r1, int r2);
    std::tuple<int,std::string> SLI(int rd, int r1, int imm);
    std::tuple<int,std::string> SLT(int rd, int r1, int r2);
    std::tuple<int,std::string> SLTI(int rd, int r1, int imm);
    std::tuple<int,std::string> SR(int rd, int r1, int r2);

    std::tuple<int,std::string> SRI(int rd, int r1, int imm);
    std::tuple<int,std::string> SUB(int rd, int r1, int r2);
    std::tuple<int,std::string> SUBU(int rd, int r1, int r2);
    std::tuple<int,std::string> SW(int r1, int r2, int off, RAM *_ram);
    std::tuple<int,std::string> XOR(int rd, int r1, int r2);
    std::tuple<int,std::string> XORI(int rd, int r1, int imm);

    void printRegister(int i);


    std::vector<std::string> tokenize(std::string str, std::string separator);
    std::tuple<int,std::string> handleInstruction();
    bool isHalted();
    void setPC(int _pc);
    int getPC();
    int getID();
    void updateIR();

private:
    //CONSTANTS
    int WORD_SIZE;
    int NUM_REGS;
    int ID;

    //REGISTERS
    std::vector<int> REG;
    int PC= 0;
    std::string IR;
    int OVF;
    int CAR;

    //OTHER
    std::map<std::string,std::string> INSTRUCTIONS;
    RAM *ramm;
    bool halt;


    void fillInstructionDescriptions();
    int validr(int r);
};

#endif // PRCPU_H
