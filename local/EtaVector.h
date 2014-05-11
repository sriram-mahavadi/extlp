/* 
 * File:   EtaVector.h
 * Author: Sriram Mahavadi
 *
 * Created on 12 May, 2014, 3:27 AM
 */

#ifndef ETAVECTOR_H
#define	ETAVECTOR_H

#include "PackedVector.h"

//! Eta Vector is an simple representation of elementary matrix
//! The non-identity column along with column_index better represents the elementary matrix
class EtaVector{
private:
    unsigned int m_col_index;
    PackedVector m_vct_col;
public:
};


#endif	/* ETAVECTOR_H */

