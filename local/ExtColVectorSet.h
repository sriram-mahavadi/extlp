/* 
 * File:   ExtColVectorSet.h
 * Author: sriram
 *
 * Created on 17 April, 2014, 1:37 AM
 */

#ifndef EXTCOLVECTORSET_H
#define	EXTCOLVECTORSET_H
#include "GlobalDefines.h"
#include "ExtColVector.h"

//! Used to store Group of ExtColVector
//! Mainly used to Store the LP Tableau as set of columns
class ExtColVectorSet{
private:
    std::vector< ExtColVector* > vctExtVector;
public:
};


#endif	/* EXTCOLVECTORSET_H */

