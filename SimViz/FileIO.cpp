//
//  FileIO.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/21/22.
//

#include "FileIO.hpp"
#include "AMDmath.hpp"
#include <math.h>
namespace fs = std::filesystem;

std::string m_dig = "[[:digit:]]+";
std::string m_float = "[[:digit:]]+\\.[[:digit:]]+";
std::string m_space = "[[:blank:]]";
std::string m_exp_float = "[[:digit:]]+\\.[[:digit:]]+e\\+[[:digit:]]+";
std::string m_comment = "^#.*";
std::string dump_file = ".dump";
std::string dat_file = ".dat";

enum Coords_Type {lattice, cartiesian};
Coords_Type coords_type;
AMD::Mat3 Scale;

bool match(std::string input, std::string m_type){
    std::regex reg (m_type);
    return std::regex_match(input,reg);
}

bool search(std::string input, std::string m_type){
    std::regex reg (m_type);
    return std::regex_search(input,reg);
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

bool match_atom_file(std::string input){
    return search(input, dump_file) || search(input, dat_file);
}

bool match_number(std::string input){
    return match_int(input) || match_float(input);
}


bool match_TS(const char* line){return std::regex_match(line,re_TS);}
bool match_Num_Atoms(const char* line){return std::regex_match(line,re_NA);}
bool match_Box_Bounds(const char* line){return std::regex_match(line,re_BB);}
bool match_Atom_Line(const char* line){return std::regex_match(line,re_AL);}
bool match_Atom_Line2(const char* line){return std::regex_match(line,re_AL2);}
bool match_Atom_Line3(const char* line){return std::regex_match(line,re_AL3);}




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


void Get_Vals(std::string line, float* tmp, int* num){
    unsigned int count = 0;
    char word[100];
    unsigned int num_vals = 0;
    for(int i = 0; i < line.size(); i++){
        count = 0;
        while(!std::isspace(line[i]) && (i < line.size())){
            char test = line[i];
            word[count] = line[i];
            count++;
            i++;
        }
        if(count){
            tmp[num_vals] = std::atof(word);
            num_vals++;
        }
    }
    *num = num_vals;
    
}

String_List::String_List(std::string line){
    bool ws = true;
    unsigned int count = 0;
    unsigned int word_indx = 0;
    for(int i = 0; i < line.size(); i++){
        char word[100] = "";
        if(std::isspace(line[i])){
            continue;
        }
        else{
            word_indx = 0;
            while((i < line.length()) && (!std::isspace(line[i]))){
                word[word_indx] = line[i];
                word_indx++;
                i++;
            }
            m_words[m_num_el].assign(word);
            m_num_el++;
        }
       
    }
}

void Set_Scale(AMD::Vec3* latt){
    for(int i = 0; i<3; i++){
        Scale.assign_col(i,latt[i]);
    }
}

AMD::Vec3 Scaled_Coords(AMD::Vec3 vec){
    AMD::Vec3 ret = Scale*vec;
    return ret;
}


Dump::Dump()
:init(false), dump_num_atoms(0), scale(1.0,1.0,1.0)
{}

Dump::~Dump(){
    if(init){
        free(Atom_Lines);
    }
}

void Dump::Set_Lattice(std::ifstream& file_stream, size_t &pos){
    std::string line;
    std::stringstream ss;
    file_stream.seekg(pos);
    float x,y,z;
    for(int i = 0; i<3; i++){
        std::getline(file_stream, line);
        int num_el = Get_Num_El(line);


    }
}

void Dump::Set_Params_LAMMPS(std::string line){
    std::stringstream ss;
    ss << line;
    std::string out;
    while(ss >> out){
        if(out.compare("id") == 0){has_id = true;}
        if(out.compare("xs") == 0){scale.x = lattice[0][0];}
        if(out.compare("ys") == 0){scale.y = lattice[1][1];}
        if(out.compare("zs") == 0){scale.z = lattice[2][2];}
    }
}



void Dump::Set_Params_JDFTX(std::string line){
    std::stringstream ss;
    ss << line;
    std::string out;
    while(ss >> out){
        if(out.compare("id") == 0){has_id = true;}
        if(out.compare("xs") == 0){scale.x = lattice[0][0];}
        if(out.compare("ys") == 0){scale.y = lattice[1][1];}
        if(out.compare("zs") == 0){scale.z = lattice[2][2];}
    }
}
void Dump::Init(std::ifstream& file_stream, size_t& pos){
    std::string line;
    std::stringstream ss;
    file_stream.seekg(pos);
    std::getline(file_stream, line);
    
    
    float lo = 0.0;float hi = 0.0;
    
    if(!match_TS(line.c_str())){
        printf("First Line did not match TIMESTEP:\n");
        exit(3);
    }
    else{
        std::getline(file_stream, line);
        timestep = get_int(line);
    }
    while(std::getline(file_stream, line)){
        if(match_TS(line.c_str())){
            pos = file_stream.tellg();
            pos -= line.length() + 1;
            file_stream.seekg(pos);
            break;
        }
        
        else if(match_Num_Atoms(line.c_str())){
            std::getline(file_stream, line);
            dump_num_atoms = get_int(line);
        }
        
        else if(match_Box_Bounds(line.c_str())){
            for(int i = 0; i< 3; i ++){
                std::getline(file_stream, line);
                ss << line;
                ss >> lo >> hi;
                lattice[i][i] = hi - lo;
                ss.str("");
                ss.clear();
                }
            
        }// end of box else if
        
        else if(match_Atom_Line(line.c_str())){
            int id;
            int type;
            std::string str_type;
            float x,y,z;
            Set_Params_LAMMPS(line);
            Atom_Lines = (Atom_Line*)malloc(dump_num_atoms*sizeof(Atom_Line));
            std::stringstream ss;
            for(int i = 0; i< dump_num_atoms; i ++){
                std::getline(file_stream, line);
                ss << line;
                if(has_id){
                    ss >> id >> str_type >> x >> y >> z;
                }
                else{
                    id=0;
                    ss >> str_type >> x >> y >> z;
                }
                if(match_int(str_type)){type = atoi(str_type.c_str());}
                else{
                    char t = str_type[0];
                    switch (t) {
                        case 83:
                            type = 1;
                            break;
                            
                        case 72:
                            type = 3;
                            break;
                            
                        case 66:
                            type = 3;
                            break;
                            
                        case 73:
                            type = 1;
                            break;
                            
                        case 79:
                            type = 2;
                            break;
                            
                        default:
                            type = 0;
                            break;
                    }
                }
                Atom_Lines[i] = Atom_Line(id,type,AMD::Vec3(x*scale.x,y*scale.y,z*scale.z));
                ss.clear();
                }
            
        } // end of atom line else if
      
    }// end of while loop
    
    init = true;
    
}



String_List::~String_List(){}

std::string& String_List::operator[](const int index){
    return this->m_words[index];
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


char* Read_Line(std::ifstream& in_file){
    char* line = nullptr;
    char temp[100];
    in_file.getline(temp, 100);
    int num_c = (int)in_file.gcount();
    line = (char*)malloc(num_c*sizeof(char));
    for(int i = 0; i< num_c; i++){
        line[i] = temp[i];
    }
    return line;
}


char** Read_File(const char* file,int& num_blocks, int* block_idx, int& num_lines){
    char** contents = nullptr;
    std::ifstream infile(file, std::ios_base::in);
    int count = 0;
    if (!infile.is_open()){
        std::cout << "File did not open!!" << std::endl;
        exit(9);
    }
    
    int ts_count = 0;
    std::regex ts(TIMESTEP);
    while (!infile.eof()){
        count ++;
        contents = (char**)realloc(contents, count*sizeof(char*));
        contents[count - 1] = Read_Line(infile);
        if(std::regex_match(contents[count - 1], ts)){
            count ++;
            contents = (char**)realloc(contents, count*sizeof(char*));
            contents[count - 1] = Read_Line(infile);
            block_idx[ts_count] = get_int(contents[count - 1]);
            ts_count++;
        }
    }// while
    num_lines = count;
    num_blocks = ts_count;
    infile.close();
    return contents;
}

Data_2D::Data_2D(const char* file){
    
    std::ifstream infile(file, std::ios_base::in);
    if (!infile.is_open()){
        std::cout << "File did not open!!" << std::endl;
        exit(9);
    }
    std::string line;
    
    int num_el = 0;
    int count = 0;
    bool num_el_set = false;
    
    float tmp[25];
    while (std::getline(infile, line)) {
        if (!match_comment(line)) {
            if(!num_el_set){
                num_el = Get_Num_El(line);
            }
            m_dat = (float**)realloc(m_dat, (count+1)*sizeof(float*));
            m_dat[count] = (float*)malloc(num_el*sizeof(float));
            Get_Vals(line, tmp, &num_el);
            for(int i = 0; i < num_el; i++){
                m_dat[count][i] = tmp[i];
            }
            
            count++;
            
        }
    }
    
    m_num_el = num_el;
    m_num_rows = count;
}



Data_2D::~Data_2D(){
    if(m_dat!=NULL){
        for(int i = 0; i<m_num_rows; i++) {
            free(m_dat[i]);
        }
        free(m_dat);
    }
}


Data_3D::Data_3D(){}

Data_3D::~Data_3D(){
    if(init){
        for (int i = 0; i<m_ny; i++) {
            for(int j = 0; j < m_nz; j++){
                free(m_dat[i][j]);
            }
            free(m_dat[i]);
        }
    }
    free(m_dat);
}


Rho_Data_3D::Rho_Data_3D(){}

Rho_Data_3D::~Rho_Data_3D(){
    if(at_init){
        free(at_types);
        free(at_coords);
    }
}

Rho_Data_3D Read_Charge_Density(const char* file){
    Rho_Data_3D dat;
    std::ifstream infile(file, std::ios_base::in);
    if (!infile.is_open()){
        std::cout << "File did not open!!" << std::endl;
        exit(9);
    }
    
    
    std::string line;
    int num_el = 0;
    float tmp[20];
    // first two lines ar comments third line is num ats
    bool start = false;
    while(!start){
        std::getline(infile, line);
        String_List str_lst(line);
        start = match_number(str_lst[0]);
    }
    Get_Vals(line, tmp, &num_el);
    int num_ats = tmp[0];
    dat.num_ats = num_ats;
    float b_to_A = 1.0; //0.529177;
    //next three lines are ni ei for i = 1,2,3
    std::getline(infile, line);
    Get_Vals(line, tmp, &num_el);
    dat.m_nx = round(tmp[0]);
    dat.m_bin_widths[0] = tmp[1]*b_to_A;
    std::getline(infile, line);
    Get_Vals(line, tmp, &num_el);
    dat.m_ny = round(tmp[0]);
    dat.m_bin_widths[1] = tmp[2]*b_to_A;
    std::getline(infile, line);
    Get_Vals(line, tmp, &num_el);
    dat.m_nz = round(tmp[0]);
    dat.m_bin_widths[2] = tmp[3]*b_to_A;
    
    dat.at_coords = (AMD::Vec3*)malloc(num_ats*sizeof(AMD::Vec3));
    dat.at_types = (float*)malloc(num_ats*sizeof(float));
    dat.at_init = true;
    for(int i = 0; i<num_ats;i++){
        std::getline(infile, line);
        Get_Vals(line, tmp, &num_el);
        dat.at_types[i] = tmp[1];
        dat.at_coords[i] = AMD::Vec3(tmp[2],tmp[3],tmp[4]);
        
    }
    
    
    float* oneD = (float*)malloc(dat.m_nx*dat.m_ny*dat.m_nz*sizeof(float));
    int count = 0;
    while (std::getline(infile, line)) {
            Get_Vals(line, tmp, &num_el);
            for(int i = 0; i < num_el; i++){
                oneD[count] = tmp[i];
                count++;
            }
            
            
        }
    
    count = 0;
    dat.m_dat = (float***)malloc(dat.m_nx*sizeof(float**));
    for(int i = 0; i < dat.m_nx; i++){
        dat.m_dat[i] = (float**)malloc(dat.m_ny*sizeof(float*));
        for(int j = 0; j< dat.m_ny; j++){
            dat.m_dat[i][j] = (float*)malloc(dat.m_nz*sizeof(float));
            for(int k = 0; k < dat.m_nz; k++){
                dat.m_dat[i][j][k] = oneD[count];
                count++;
                
            }
        }
    }
    dat.init = true;
    free(oneD);
    return dat;
}



void Write_Dat(float* dat, int num, const char* file_name){
    std::ofstream dat_file;
    const char* out_file = "/Users/diggs/Desktop/";
    
    dat_file.open(file_name, std::ios::out);
    
    for(int i = 0; i< num; i++){
        dat_file << dat[i] << "\n";
       
    }
    
    
    
    dat_file.close();
    
}

