//
//  AtomInfo.h
//  VolumeData
//
//  Created by Andrew Diggs on 4/16/25.
//

#ifndef AtomInfo_h
#define AtomInfo_h
#include "AMDmath.hpp"

struct atom_info{
    float rad = 0.;
    float mass = 0.;
    char ID[2];
    AMD::Vec4 clr;
};


inline int El_Hash(const char* el)
{
    int i = 0;
    int ret = 0;
    while(el[i]){
        int val = el[i] - 64;
        ret += val*val;
        i++;
    }
    return ret;
    
}


inline atom_info Get_Atom_Info(int hash)
{
    atom_info ret;
    switch (hash) {

        case 64:
            ret.rad = 0.5;
            ret.mass = 1.008;
            strcpy(ret.ID, "H");
            ret.clr = AMD::Vec4(1.0,1.0,1.0,1.0);
            break;
            
        case 1825:
            ret.rad = 0.9;
            ret.mass = 6.94;
            strcpy(ret.ID, "Li");
            ret.clr = AMD::Vec4(1.0,1.0,0.85,1.0);
            break;

        case 1373:
            ret.rad = 0.85;
            ret.mass = 9.01;
            strcpy(ret.ID, "Be");
            ret.clr = AMD::Vec4(1.0,0.85,0.85,1.0);
            break;

        case 4:
            ret.rad = 0.7;
            ret.mass = 10.81;
            strcpy(ret.ID, "B");
            ret.clr = AMD::Vec4(0.75,0.75,0.95,1.0);
            break;

        case 9:
            ret.rad = 0.7;
            ret.mass = 12.00;
            strcpy(ret.ID, "C");
            ret.clr = AMD::Vec4(0.75,0.75,0.75,1.0);
            break;
            
        case 196:
            ret.rad = 0.65;
            ret.mass = 14.00;
            strcpy(ret.ID, "N");
            ret.clr = AMD::Vec4(0.8,0.0,0.6,1.0);
            break;
            
        case 225:
            ret.rad = 0.6;
            ret.mass = 15.998;
            strcpy(ret.ID, "O");
            ret.clr = AMD::Vec4(0.8,0.4,0.4,1.0);
            break;
        case 36:
            ret.rad = 0.5;
            ret.mass = 19.00;
            strcpy(ret.ID, "F");
            ret.clr = AMD::Vec4(0.4,0.7,0.4,1.0);
            break;

        case 1285:
            ret.rad = 1.1;
            ret.mass = 22.99;
            strcpy(ret.ID, "Na");
            ret.clr = AMD::Vec4(1.0,1.0,0.75,1.0);
            break;

        case 1690:
            ret.rad = 0.9;
            ret.mass = 24.31;
            strcpy(ret.ID, "Mg");
            ret.clr = AMD::Vec4(1.0,0.85,0.75,1.0);
            break;

        case 1937:
            ret.rad = 0.9;
            ret.mass = 26.98;
            strcpy(ret.ID, "Al");
            ret.clr = AMD::Vec4(0.7,0.7,0.65,1.0);
            break;

        case 2042:
            ret.rad = 0.71;
            ret.mass = 28.09;
            strcpy(ret.ID, "Si");
            ret.clr = AMD::Vec4(0.7,0.7,0.8,1.0);
            break;

        case 256:
            ret.rad = 0.65;
            ret.mass = 30.97;
            strcpy(ret.ID, "P");
            ret.clr = AMD::Vec4(0.8,0.4,0.6,1.0);
            break;

        case 361:
            ret.rad = 0.65;
            ret.mass = 32.06;
            strcpy(ret.ID, "S");
            ret.clr = AMD::Vec4(0.8,0.8,0.4,1.0);
            break;

        case 1945:
            ret.rad = 0.6;
            ret.mass = 35.45;
            strcpy(ret.ID, "Cl");
            ret.clr = AMD::Vec4(0.4,0.9,0.4,1.0);
            break;

        case 1586:
            ret.rad = 1.40;
            ret.mass = 44.96;
            strcpy(ret.ID, "Sc");
            ret.clr = AMD::Vec4(0.8,0.6,0.6,1.0);
            break;

        case 2081:
            ret.rad = 1.40;
            ret.mass = 47.88;
            strcpy(ret.ID, "Ti");
            ret.clr = AMD::Vec4(0.8,0.6,0.6,1.0);
            break;

        case 484:
            ret.rad = 1.40;
            ret.mass = 50.94;
            strcpy(ret.ID, "V");
            ret.clr = AMD::Vec4(0.8,0.6,0.9,1.0);
            break;

        case 2509:
            ret.rad = 1.40;
            ret.mass = 52.00;
            strcpy(ret.ID, "Cr");
            ret.clr = AMD::Vec4(0.8,0.6,0.9,1.0);
            break;

        case 2285:
            ret.rad = 1.35;
            ret.mass = 54.94;
            strcpy(ret.ID, "Mn");
            ret.clr = AMD::Vec4(0.8,0.8,0.6,1.0);
            break;

        case 1405: //Fe
            ret.rad = 1.40;
            ret.mass = 55.85;
            strcpy(ret.ID, "Fe");
            ret.clr = AMD::Vec4(0.6,0.6,0.6,1.0);
            break;

        case 2218:
            ret.rad = 1.35;
            ret.mass = 58.93;
            strcpy(ret.ID, "Co");
            ret.clr = AMD::Vec4(0.7,0.7,0.6,1.0);
            break;

        case 1877:
            ret.rad = 1.35;
            ret.mass = 58.69;
            strcpy(ret.ID, "Ni");
            ret.clr = AMD::Vec4(0.7,0.7,0.7,1.0);
            break;

        case 2818: //Cu
            ret.rad = 1.35;
            ret.mass = 63.55;
            strcpy(ret.ID, "Cu");
            ret.clr = AMD::Vec4(0.8,0.8,0.6,1.0);
            break;

        case 2581: //Ir
            ret.rad = 1.35;
            ret.mass = 192.2;
            strcpy(ret.ID, "Ir");
            ret.clr = AMD::Vec4(0.6,0.6,0.9,1.0);
            break;
            
        case 2960: //Ir
            ret.rad = 1.35;
            ret.mass = 195.1;
            strcpy(ret.ID, "Ir");
            ret.clr = AMD::Vec4(0.6,0.6,0.9,1.0);
            break;

        case 2810: //Au
            ret.rad = 1.35;
            ret.mass = 197.0;
            strcpy(ret.ID, "Au");
            ret.clr = AMD::Vec4(0.8,0.7,0.34,1.0);
            break;
            
        case 1522: //Ag
            ret.rad = 1.65;
            ret.mass = 107.9;
            strcpy(ret.ID, "Ag");
            ret.clr = AMD::Vec4(0.8,0.8,0.8,1.0);
            break;
            
        default:
            break;
    }
    return ret;
}


#endif /* AtomInfo_h */
