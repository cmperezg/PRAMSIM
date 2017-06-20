#include "prcpu.h"



void PRCPU::fillInstructionDescriptions(){
    /*
     * r: w sized register
     * i: immediate value, offset, address or base. can be replaced by a label.
     */

    INSTRUCTIONS["add"] = "rrr";
    INSTRUCTIONS["addi"] = "rri";
    INSTRUCTIONS["addu"] = "rrr";
    INSTRUCTIONS["and"] = "rrr";
    INSTRUCTIONS["andi"] = "rri";

    INSTRUCTIONS["beq"] = "rri";
    INSTRUCTIONS["bne"] = "rri";

    INSTRUCTIONS["j"] = "i";
    INSTRUCTIONS["jr"] = "r";

    INSTRUCTIONS["li"] = "ri";
    INSTRUCTIONS["lid"] = "r";
    INSTRUCTIONS["lw"] = "rri";

    INSTRUCTIONS["not"] = "rr";

    INSTRUCTIONS["or"] = "rrr";
    INSTRUCTIONS["ori"] = "rri";

    INSTRUCTIONS["sl"] = "rrr";
    INSTRUCTIONS["sli"] = "rri";
    INSTRUCTIONS["slt"] = "rrr";
    INSTRUCTIONS["slti"] = "rri";
    INSTRUCTIONS["sr"] = "rrr";
    INSTRUCTIONS["sri"] = "rri";
    INSTRUCTIONS["sub"] = "rrr";
    INSTRUCTIONS["subu"] = "rrr";
    INSTRUCTIONS["sw"] = "rri";

    INSTRUCTIONS["xor"] = "rrr";
    INSTRUCTIONS["xori"] = "rri";

    INSTRUCTIONS["halt"] = "";
}

PRCPU::PRCPU(int _wordsize, int _regs, RAM *_ram, int id)
{
    WORD_SIZE = _wordsize;
    NUM_REGS = _regs;
    ID = id;

    ramm = _ram;

    fillInstructionDescriptions();

    for(int i = 0; i<NUM_REGS; i++){
        REG.push_back(0);
    }
    halt = false;
}

std::map<std::string, std::string> *PRCPU::getInstructionDesc(){
    return &INSTRUCTIONS;
}

//1 if register number is valid
int PRCPU::validr(int r){
    if((r>=0) & (r<NUM_REGS)){
        return 1;
    }
    return 0;
}

//UTILITIES
std::string intToBits(int x){
    std::bitset<32> _x(x);
    return _x.to_string();
}


//INSTRUCTIONS



//ADD $rd, $r1, $r2 : $rd = $r1 + $r2;
std::tuple<int,std::string> PRCPU::ADD(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(ADD): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    //overflow
    if(((REG[r2]>0) & (REG[r1] > (std::numeric_limits<float>::max()-REG[r2]))) |
          ((REG[r1]>0) & (REG[r2] > (std::numeric_limits<float>::max()-REG[r1])))){
        OVF = 1;
        msg = "(ADD): Overflow detected";
    }

    //underflow
    if(((REG[r2]<0) & (REG[r1] < (std::numeric_limits<float>::min()-REG[r2]))) |
          ((REG[r1]<0) & (REG[r2] < (std::numeric_limits<float>::max()-REG[r1])))){
        OVF = 1;
        msg = "(ADD): Underflow detected";
    }


    REG[rd] = REG[r1] + REG[r2];

    return std::make_tuple(0,msg);
}

//ADDI $rd, $r1, imm : $rd = $r1 + imm;
std::tuple<int,std::string> PRCPU::ADDI(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(ADDI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    //overflow
    if(((imm>0) & (REG[r1] > (std::numeric_limits<float>::max()-imm))) |
          ((REG[r1]>0) & (imm > (std::numeric_limits<float>::max()-REG[r1])))){
        OVF = 1;
        msg = "(ADDI): Overflow detected";
    }

    //underflow
    if(((imm<0) & (REG[r1] < (std::numeric_limits<float>::min()-imm))) |
          ((REG[r1]<0) & (imm < (std::numeric_limits<float>::max()-REG[r1])))){
        OVF = 1;
        msg = "(ADDI): Underflow detected";
    }
    REG[rd] = REG[r1] + imm;

    return std::make_tuple(0,msg);
}

//ADDU $rd, $r1, $r2 : $rd = $r1 + $r2; (unsigned, no overflow)
std::tuple<int,std::string> PRCPU::ADDU(int rd, int r1, int r2){
    std::string msg = "";

    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(ADDU): Invalid register(s)";
        return std::make_tuple(1,msg);
    }


    REG[rd] = (unsigned int)REG[r1] + (unsigned int)REG[r2];

    return std::make_tuple(0,msg);
}

//AND $rd, $r1, $r2 : $rd = $r1 & $r2;
std::tuple<int,std::string> PRCPU::AND(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(AND): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] & REG[r2];

    return std::make_tuple(0,msg);
}

//ANDI $rd, $r1, imm : $rd = $r1 & imm;
std::tuple<int,std::string> PRCPU::ANDI(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(ANDI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] & imm;

    return std::make_tuple(0,msg);
}

//BEQ $r1, $r2, off : if($r1 == $r2) then branch to off;
std::tuple<int,std::string> PRCPU::BEQ(int r1, int r2, int add){
    std::string msg = "";
    if((validr(r1)==0) | (validr(r2)==0)){
        msg = "(BEQ): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    if(REG[r1] == REG[r2]){
        PC = add;
    }

    return std::make_tuple(0,msg);
}


//BNE $r1, $r2, off : if($r1 != $r2) then branch to off;
std::tuple<int,std::string> PRCPU::BNE(int r1, int r2, int add){
    std::string msg = "";
    if((validr(r1)==0) | (validr(r2)==0)){
        msg = "(BNE): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    if(REG[r1] != REG[r2]){
        PC = add;
    }

    return std::make_tuple(0,msg);
}

//J add: PC = add
std::tuple<int,std::string> PRCPU::J(int add){
    std::string msg = "";

    PC = add;

    return std::make_tuple(0,msg);
}

//JR add: PC = $r1
std::tuple<int,std::string> PRCPU::JR(int r1){
    std::string msg = "";
    if(validr(r1)==0){
        msg = "(BNE): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    PC = REG[r1];

    return std::make_tuple(0,msg);
}

//LI $rd, imm: $rd = imm
std::tuple<int,std::string> PRCPU::LI(int rd, int imm){
    std::string msg = "";
    if(validr(rd)==0){
        msg = "(LI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = imm;

    return std::make_tuple(0,msg);
}

//LID $rd: $rd = ID
std::tuple<int,std::string> PRCPU::LID(int rd){
    std::string msg = "";
    if(validr(rd)==0){
        msg = "(LUW): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = ID;

    return std::make_tuple(0,msg);
}

//LW $rd, $r1, off :   $rd = MEM[$r1 + off];
std::tuple<int,std::string> PRCPU::LW(int rd, int r1, int off, RAM *_ram){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(LW): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = atoi((_ram->get(REG[r1]+off)).c_str());

    return std::make_tuple(0,msg);
}

//NOT $rd,$r1: $rd = ~$r1
std::tuple<int,std::string> PRCPU::NOT(int rd, int r1){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(UWNOT): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = ~(REG[r1]);

    return std::make_tuple(0,msg);
}

//OR $rd, $r1, $r2 : $rd = $r1 | $r2;
std::tuple<int,std::string> PRCPU::OR(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(OR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] | REG[r2];

    return std::make_tuple(0,msg);
}

//ORI $rd, $r1, imm : $rd = $r1 | imm;
std::tuple<int,std::string> PRCPU::ORI(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(ORI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] | imm;

    return std::make_tuple(0,msg);
}


//SL $rd, $r1, $r2 : $rd = $r1 << $r2;
std::tuple<int,std::string> PRCPU::SL(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SL): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] << REG[r2];

    return std::make_tuple(0,msg);
}

//SLI $rd, $r1, imm : $rd = $r1 << imm;
std::tuple<int,std::string> PRCPU::SLI(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(SLI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(imm >= WORD_SIZE){
        msg = "(SLI): Shift larger than WORD_SIZE is invalid";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] << imm;

    return std::make_tuple(0,msg);
}

//SLT $rd, $r1, $r2 : $rd = ($r1 == $r2);
std::tuple<int,std::string> PRCPU::SLT(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SLT): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(REG[r1] == REG[r2]){
        REG[rd] = 1;
    }else{
        REG[rd] = 0;
    }


    return std::make_tuple(0,msg);
}

//SLTI $rd, $r1, imm : $rd = ($r1 == imm);
std::tuple<int,std::string> PRCPU::SLTI(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(SLTI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(REG[r1] == imm){
        REG[rd] = 1;
    }else{
        REG[rd] = 0;
    }


    return std::make_tuple(0,msg);
}

//SRV $rd, $r1, $r2 : $rd = $r1 >> imm;
std::tuple<int,std::string> PRCPU::SR(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] >> REG[r2];

    return std::make_tuple(0,msg);
}

//SRI $rd, $r1, imm : $rd = $r1 >> imm;
std::tuple<int,std::string> PRCPU::SRI(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(SRI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }
    if(imm >= WORD_SIZE){
        msg = "(SRI): Shift larger than WORD_SIZE is invalid";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] >> imm;

    return std::make_tuple(0,msg);
}

//SUB $rd, $r1, $r2 : $rd = $r1 - $r2;
std::tuple<int,std::string> PRCPU::SUB(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SUB): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    //overflow
    if(((REG[r2]<0) & (REG[r1] > (std::numeric_limits<float>::max()+REG[r2]))) |
          ((REG[r1]<0) & (REG[r2] > (std::numeric_limits<float>::max()+REG[r1])))){
        OVF = 1;
        msg = "(SUB): Overflow detected";
    }

    //underflow
    if(((REG[r2]>0) & (REG[r1] < (std::numeric_limits<float>::min()+REG[r2]))) |
          ((REG[r1]>0) & (REG[r2] < (std::numeric_limits<float>::min()+REG[r1])))){
        OVF = 1;
        msg = "(SUB): Underflow detected";
    }


    REG[rd] = REG[r1] - REG[r2];

    return std::make_tuple(0,msg);
}


//SUBU $rd, $r1, $r2 : $rd = $r1 - $r2; (unsigned, no overflow)
std::tuple<int,std::string> PRCPU::SUBU(int rd, int r1, int r2){
    std::string msg = "";

    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(SUBU): Invalid register(s)";
        return std::make_tuple(1,msg);
    }


    REG[rd] = REG[r1] - REG[r2];

    return std::make_tuple(0,msg);
}

//SW $r1, $r2, off : MEM[$r2 + off] = $r1;
std::tuple<int,std::string> PRCPU::SW(int r1, int r2, int off, RAM *_ram){
    std::string msg = "";
    if((validr(r1)==0) | (validr(r2)==0)){
        msg = "(SW): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    _ram->set(REG[r2]+off, std::to_string(REG[r1]));

    return std::make_tuple(0,msg);
}

//XOR $rd, $r1, $r2 : $rd = $r1 ^ $r2;
std::tuple<int,std::string> PRCPU::XOR(int rd, int r1, int r2){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0) | (validr(r2)==0)){
        msg = "(XOR): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] ^ REG[r2];

    return std::make_tuple(0,msg);
}

//XORI $rd, $r1, imm : $rd = $r1 ^ imm;
std::tuple<int,std::string> PRCPU::XORI(int rd, int r1, int imm){
    std::string msg = "";
    if((validr(rd)==0) | (validr(r1)==0)){
        msg = "(ORI): Invalid register(s)";
        return std::make_tuple(1,msg);
    }

    REG[rd] = REG[r1] ^ imm;

    return std::make_tuple(0,msg);
}



void PRCPU::printRegister(int i){

        if(validr(i)==0){
            std::cout<< "(printRegister): Invalid register number";
            return;
        }
        std::cout <<"P"<<ID<< " : R"<<i<<": "<<"\n";
        std::cout << intToBits(REG[i]) << "\n";
        return;


}



// PROGRAM EXECUTION AND INSTRUCTION HANDLING

std::vector<std::string> PRCPU::tokenize(std::string str, std::string separator){
    std::vector<std::string> ret;
    int index = 0;
    while(index!=-1){
        if(str.size()==0){
            return ret;
        }
        index = str.find(separator);
        if(index==-1){
            ret.push_back(str);
            return ret;
        }else{
            if(index==0){
                str = str.substr(index+separator.size());
                continue;
            }else{
                std::string seg = str.substr(0,index);
                str = str.substr(index+separator.size());
                ret.push_back(seg);
            }
        }
    }
    return ret;
}

//EXECUTE SINGLE INSTRUCTION CURRENTLY IN IR
std::tuple<int,std::string> PRCPU::handleInstruction(){

    std::string instructionline = IR;
    std::cout <<"Handling: "<<ID<<": "<<instructionline<<"\n";
    //at this point we are sure instructions exist and parameters are the correct format
    if(instructionline.empty()){
        halt = true;
        return std::make_tuple(0,"");
    }

    std::vector<std::string> sepinst = tokenize(instructionline," ");
    std::string instruction = sepinst[0];
    std::vector<std::string> params;

    if (sepinst.size() > 1){
        params = tokenize(sepinst[1],",");
    }
     std::tuple<int,std::string> ret;
    //Giant if-else to find instruction
    if(instruction == "add"){
        ret = ADD(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "addi"){
        ret = ADDI(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "addu"){
        ret = ADDU(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "and"){
        ret = AND(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "andi"){
        ret = ANDI(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "beq"){
        ret = BEQ(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "bne"){
        ret = BNE(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "j"){
        ret = J(atoi(params[0].c_str()));
        return ret;
    }else if(instruction == "jr"){
        ret = JR(atoi(params[0].c_str()));
        return ret;
    }else if(instruction == "li"){
        ret = LI(atoi(params[0].c_str()),atoi(params[1].c_str()));
        return ret;
    }else if(instruction == "lid"){
        ret = LID(atoi(params[0].c_str()));
        return ret;
    }else if(instruction == "lw"){
        ret = LW(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()),ramm);
        return ret;
    }else if(instruction == "not"){
        ret = NOT(atoi(params[0].c_str()),atoi(params[1].c_str()));
        return ret;
    }else if(instruction == "or"){
        ret = OR(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "ori"){
        ret = ORI(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "sl"){
        ret = SL(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "sli"){
        ret = SLI(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "slt"){
        ret = SLT(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "slti"){
        ret = SLTI(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "sr"){
        ret = SR(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "sri"){
        ret = SRI(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "sub"){
        ret = SUB(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "subu"){
        ret = SUBU(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "sw"){
        ret = SW  (atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()),ramm);
        return ret;
    }else if(instruction == "xor"){
        ret = XOR(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "xori"){
        ret = XORI(atoi(params[0].c_str()),atoi(params[1].c_str()),atoi(params[2].c_str()));
        return ret;
    }else if(instruction == "halt"){
        halt = true;
        return std::make_tuple(0,"");
    }
    return ret;
}

bool PRCPU::isHalted(){
    return halt;
}

void PRCPU::setPC(int _pc){
    PC = _pc;
}

int PRCPU::getPC(){
    return PC;
}

int PRCPU::getID(){
    return ID;
}

void PRCPU::updateIR(){
    IR = ramm->get(PC);
}
