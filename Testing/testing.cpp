
#include <stdio.h>
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cctype>
#include "AMDmath.hpp"


unsigned int Get_Num_El(std::string line){
    bool ws = true;
    unsigned int count = 0;
    for(int i = 0; i < line.size(); i++){
        if(std::isspace(line[i]) && ws){
            continue;
        }
        else if(!ws && !std::isspace(line[i])){
            continue;
        }
        else if(std::isspace(line[i]) && !ws){
            ws = true;
            count++;
        }
        else if(ws && !std::isspace(line[i])){
            ws = false;
        }
    }
    if(!ws){
        count ++;
    }
    return count;
}



int FT_Hash(const char* ft){
    int val = 0;
    for(int i =0; i<strlen(ft); i++){
        val +=ft[i];
    }
    return val % 10;
}





int main(int argc, const char * argv[]) 
{
    const char* fts[6] = {"lammps", "LAMMPS", "jdftx", "JDFTX", "ase", "ASE"};
    for(int i = 0; i<6; i++){
        int val = FT_Hash(fts[i]);
        printf("%s = %d\n",fts[i],val);
    }
    return 0;
}
