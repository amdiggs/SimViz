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
        ret += el[i];
        i++;
    }
    return ret;
    
}


inline atom_info Get_Atom_Info(int hash)
{
    atom_info ret;
    switch (hash) {
        case 72:
            ret.rad = 0.5;
            ret.mass = 1.008;
            strcpy(ret.ID, "H");
            ret.clr = AMD::Vec4(1.0,1.0,1.0,1.0);
            break;
            
        case 67:
            ret.rad = 0.7;
            ret.mass = 12.00;
            strcpy(ret.ID, "C");
            ret.clr = AMD::Vec4(0.75,0.75,0.75,1.0);
            break;
            
        case 79:
            ret.rad = 0.6;
            ret.mass = 15.998;
            strcpy(ret.ID, "O");
            ret.clr = AMD::Vec4(0.8,0.0,0.0,1.0);
            break;
            
        case 78:
            ret.rad = 0.65;
            ret.mass = 14.00;
            strcpy(ret.ID, "N");
            ret.clr = AMD::Vec4(0.8,0.0,0.6,1.0);
            break;
        case 171: //Fe
            ret.rad = 1.40;
            ret.mass = 1.008;
            strcpy(ret.ID, "Fe");
            ret.clr = AMD::Vec4(0.8,0.6,0.6,1.0);
            break;
        case 184: //Cu
            ret.rad = 1.35;
            ret.mass = 1.008;
            strcpy(ret.ID, "Cu");
            ret.clr = AMD::Vec4(0.8,0.8,0.6,1.0);
            break;
        case 187: //Ir
            ret.rad = 1.35;
            ret.mass = 1.008;
            strcpy(ret.ID, "Ir");
            ret.clr = AMD::Vec4(0.6,0.6,0.9,1.0);
            break;
            
        case 182: //Au
            ret.rad = 1.35;
            ret.mass = 1.008;
            strcpy(ret.ID, "Au");
            ret.clr = AMD::Vec4(0.8,0.9,0.6,1.0);
            break;
            
        case 168: //Ag
            ret.rad = 1.65;
            ret.mass = 1.008;
            strcpy(ret.ID, "Ag");
            ret.clr = AMD::Vec4(0.6,0.6,0.6,1.0);
            break;
            
        case 196: //Pt
            ret.rad = 1.35;
            ret.mass = 100.8;
            strcpy(ret.ID, "Pt");
            ret.clr = AMD::Vec4(0.6,0.6,0.6,1.0);
            break;
            
        default:
            break;
    }
    return ret;
}


#endif /* AtomInfo_h */
