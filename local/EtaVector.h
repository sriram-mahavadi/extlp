/* 
 * File:   EtaVector.h
 * Author: Sriram Mahavadi
 *
 * Created on 12 May, 2014, 3:27 AM
 */

#ifndef ETAVECTOR_H
#define	ETAVECTOR_H

#include "PackedVector.h"

//! Eta Vector is an simple representation of elementary matrix.
//! The non-identity column along with column_index better represents the elementary matrix.
class EtaVector{
private:
    //! Non identity column index in elementary matrix.
    //! Generally the leaving variable index from the basis matrix
    unsigned int m_col_r_index;
    PackedVector m_vct_col;
    
    //! Performs the required transformations for generating Elementary matrix 
    //! used to update B-Inverse and Rhs
    void _transform_for_b_inverse(){
        REAL col_r_value = 0.0F;
        PackedVector::iterator itr = m_vct_col.begin();
        while(itr!=m_vct_col.end()){
            PackedElement packed_element = *itr;
            if(packed_element.get_index() == m_col_r_index){
                col_r_value = packed_element.get_value();
                break;
            }
            itr++;
        }
        assert(col_r_value!=0.0F);
        itr = m_vct_col.begin();
        while(itr!=m_vct_col.end()){
            PackedElement packed_element = *itr;
            REAL value = packed_element.get_value();
            if(packed_element.get_index() == m_col_r_index){
                packed_element.set_value(1/col_r_value);
            }else{
                packed_element.set_value(-value/col_r_value);
            }
            *itr = packed_element;
            itr++;
        }
    }
public:
    //! Simple Initialization with PackedVector.
    EtaVector(unsigned int col_index, PackedVector& y):m_col_r_index(col_index), m_vct_col(y){
        _transform_for_b_inverse();
    }
    //! Simple Initialization with SimpleVector.
    EtaVector(unsigned int col_index, SimpleVector<REAL>& y):m_col_r_index(col_index), m_vct_col(y){
        _transform_for_b_inverse();
    }
    //! Returns the reference to the non identity column of Elementary Matrix
    PackedVector& get_col_vector(){
        return m_vct_col;
    }
    //! Returns the index of the non identity column
    unsigned int get_col_index(){
        return m_col_r_index;
    }
};


#endif	/* ETAVECTOR_H */

