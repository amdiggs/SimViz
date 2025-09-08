
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







int main(int argc, const char * argv[]) 
{
    int num = Get_Num_El(argv[1]);
    printf("num = %d\n",num);
    return 0;
}
