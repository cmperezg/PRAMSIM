#include "ram.h"


int first_ram_add;
int last_ram_add;

RAM::RAM(int _size)
{
    SIZE = _size;

    first_ram_add = 100;
    last_ram_add = first_ram_add + SIZE;

    for(int i=first_ram_add; i<last_ram_add; i++){
        ram.insert(std::make_pair(i,""));
    }
}

std::string RAM::get(int memloc){
    if((memloc>=first_ram_add) & (memloc<=last_ram_add)){
        return ram.at(memloc);
    }

    std::cout<<"memloc out of range";
    return "ERROR";
}

void RAM::set(int memloc, std::string content){
    std::cout <<"RAM: " << memloc<< " , "<<content<< "\n";
    if((memloc>=first_ram_add) & (memloc<=last_ram_add)){
        ram.at(memloc) = content;
        return;
    }else{
    std::cout<<"memloc out of range: "<<memloc<<": "<<content<<"\n";
    }
}

void RAM::RAM_info(){
    std::cout << "SIZE: " << SIZE << "\n";

    std::cout << "RAM: " << first_ram_add << " - " << last_ram_add << "\n";

}

void RAM::print_range(int from, int to){
    if((from >= first_ram_add) & (to <= last_ram_add) & (from <= to)){
        for(int i = from; i<= to; i++){
            std::cout << i << " : " << ram[i] << "\n";
        }
    }else{
        std::cout<<"print_range: invalid range provided \n";
    }
}
