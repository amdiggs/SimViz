//
//  FileIO.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/21/22.
//

#include "FileIO.hpp"
#include "Atomic.hpp"


int num_lines;
int block_len;

std::string m_dig = "[[:digit:]]+";
std::string m_float = "[[:digit:]]+\\.[[:digit:]]+";
std::string m_space = "[[:blank:]]";
std::string m_exp_float = "[[:digit:]]+\\.[[:digit:]]+e\\+[[:digit:]]+";
std::string m_comment = "#.*";

bool match(std::string input, std::string m_type){
    std::regex reg (m_type);
    return std::regex_match(input,reg);
}

bool match_int(std::string input){
    return match(input, m_dig);
}
bool match_float(std::string input){
    return match(input, m_float) || match(input, m_exp_float);
    
}
bool match_space(std::string input){
    return match(input, m_space);
}


bool match_comment(std::string input){
    return  match(input, m_comment);
}



int get_int(std::string int_str)
{
    if(match_int(int_str)){
        return std::atoi(int_str.c_str());
    }
    else{std::cout << int_str << " is not compatable with type int"  << std::endl; exit(2);}
    
}

float get_float(std::string fl_str)
{
    
 if(match_float(fl_str)){
     return std::atoi(fl_str.c_str());
 }
 else{std::cout << fl_str << " is not compatable with type float" << std::endl; exit(2);}
}



unsigned int Hash(const char* word){
    unsigned int hash_int = 0;
    for (int i = 0; i<std::strlen(word); i++){
        hash_int+=word[i];
    }
    return hash_int;
}


bool ITEM(std::string str, std::string reg_ex)
{
    std::regex reg(reg_ex);
    std::smatch m;
    std::regex_search(str, m, reg);
    return !m.empty();
}


char** Read_File(const char* file){
    char** contents = nullptr;
    char** tmp = nullptr;
    std::ifstream infile (file);
    std::string line;
    int count = 0;
    int ts[2];
    int num_matches = 0;
    if (!infile.is_open()){
        std::cout << "File did not open!!" << std::endl;
        exit(9);
    }
    while (getline(infile, line)){
        count ++;
        if(num_matches < 2 && match(line, TIMESTEP)){
            ts[num_matches] = count;
            num_matches++;
        }
        tmp = (char**)realloc(contents, count*sizeof(char*));
        if(tmp != NULL){
            contents  = tmp;
        }
        else{
            std::cout << "FUCK THIS" << std::endl;
            exit(9);
        }
        contents[count - 1] = (char*)malloc(line.length()*sizeof(char));
        for(int i = 0; i< line.length(); i++){
            contents[count - 1][i] = line[i];
        }
    }// while
    block_len = ts[1] - ts[0];
    num_lines = count;
    infile.close();
    return contents;
}


void Delete_Char_Array(char *** arr){
    for (int i = 0; i < num_lines; i++){
        free((*arr)[i]);
    }
    free(*arr);
}

Sim_Block read_block(char** dat, int& start){
    Sim_Block tmp;
    std::stringstream ss;
    tmp.timestep = atoi(dat[start + 1]);
    tmp.num_atoms = atoi(dat[start + 3]);
    int count = 0;
    AMD::Vec3 BB;
    float lo, hi;
    for(int i = start + 5; i< start + 8; i++){
        ss << dat[i];
        ss >> lo >> hi;
        tmp.sim_box[count][0] = lo;
        tmp.sim_box[count][1] = hi;
        BB[count] = hi - lo;
        count++;
        ss.str(std::string());
        ss.clear();
        
    }
    count = 0;
    int _id, _type;
    float xs,ys,zs;
    for (int i = start + 9; i< start + block_len; i++){
        ss << dat[i];
        ss >> _id >> _type >> xs >> ys >> zs;
        tmp.atoms[count] = Atom(_id, _type, xs*BB.x, ys*BB.y, zs*BB.z);
        count++;
        ss.str(std::string());
        ss.clear();
    }
    start += block_len;
    return tmp;
}

Simulation read_dump(std::string file)
{
    Simulation sim;
    
    char** data = Read_File(file.c_str());
    int num_blocks = num_lines/block_len;
    sim.Set_Num_Blocks(num_blocks);
    sim.Set_Blocks(data);
    sim.Update_Sim(0);
    Delete_Char_Array(&data);
    
    
    return sim;
}

