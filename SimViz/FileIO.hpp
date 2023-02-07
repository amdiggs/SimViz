//
//  FileIO.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/21/22.
//

#ifndef FileIO_hpp
#define FileIO_hpp

#include <stdio.h>
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#define MAX_WORD 250
#define MAX_TS 1000


#define TIMESTEP "ITEM: TIMESTEP"
#define NUM "ITEM: NUMBER OF ATOMS"
#define BOX_BOUNDS "ITEM: BOX BOUNDS pp pp pp"
#define ATOM_LINE "ITEM: ATOMS id type xs ys zs"


bool match(std::string input, std::string m_type);
bool search(std::string input, std::string m_type);
bool match_int(std::string input);
bool match_float(std::string input);
bool match_space(std::string input);
bool match_atom_file(std::string input);
bool match_comment(std::string input);



unsigned int Hash(const char* line);

int get_int(std::string int_str);
float get_float(std::string fl_str);

bool ITEM(std::string str, std::string reg_ex);
char* Read_Line(std::ifstream& infile);
char** Read_File(const char* file, int& num_lines, int& bloc_len);






#endif /* FileIO_hpp */
