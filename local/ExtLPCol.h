/* 
 * File:   ExtLPCol.h
 * Author: sriram
 *
 * Created on 30 March, 2014, 5:52 PM
 */

#ifndef EXTLPCOL_H
#define	EXTLPCOL_H
#include "GlobalDefines.h"
class ExtLPCol{
private:
    real_vector vector;
public:
    ExtLPCol(){
        int i=0;
        for(i=0; i<10; i++){
            vector.push_back(i);
        }
    }
    void displayElements(){
        unsigned int i=0;
        for(i=0; i<vector.size(); i++){
            DEBUG_SIMPLE("Value at "<<i<<" is: "<<vector[i]);
        }
    }
};

#endif	/* EXTLPCOL_H */

