/* 
 * File:   ExtStringVector.h
 * Author: sriram
 *
 * Created on 17 April, 2014, 12:27 AM
 */

#ifndef EXTSTRINGVECTOR_H
#define	EXTSTRINGVECTOR_H

#include "GlobalDefines.h"

class ExtStringVector{
private:
    unsigned int realSize;
    string_vector vctDisk;
public:
    unsigned int size(){
        return realSize;
    }
    void setSize(unsigned int p_realSize){
        realSize = p_realSize;
    }
};


#endif	/* EXTSTRINGVECTOR_H */

