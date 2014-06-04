/* 
 * File:   ExtFixedMatrix.h
 * Author: Sriram Mahavadi
 *
 * Created on 6 May, 2014, 12:49 AM
 */

#ifndef EXTFIXEDMATRIX_H
#define	EXTFIXEDMATRIX_H
#include "GlobalDefines.h"
#include "PackedVector.h"

//! Limits for the start and end of the column in ExtFixedMatrix container
//! The limits to the column is [start, end) start includes and end excludes
class ColAttr {
private:
    unsigned int start, end;
public:
    ColAttr(unsigned int start, unsigned int end) {
        this->start = start;
        this->end = end;
    }
    void set_start(unsigned int start) {
        this->start = start;
    }
    void set_end(unsigned int end) {
        this->end = end;
    }
    unsigned int get_start() {
        return this->start;
    }
    unsigned int get_end() {
        return this->end;
    }
};

//! Storage Container for Fixed Matrix
//! The contents of the matrix do not change after assignment/initialization
//! Storage is done in column-wise fashion
class ExtFixedMatrix {
private:
    //! Vector to store the Items onto external memory
    //! 10MB page/block size
    typedef typename stxxl::VECTOR_GENERATOR<PackedElement, 1, 1, 10 * 1024 * 1024>::result item_vector;
    item_vector m_vct_disk;

    //! Vector to keep track of columns in the matrix
    std::vector<ColAttr> m_vct_col_limits;
    //! Vector to keep singleton columns in the matrix
    std::vector< PackedElement > m_vct_singleton_columns;
public:
    //! Adds a new Column into the matrix
    void add_column(PackedVector p_packed_col) {
        unsigned int no_cols = m_vct_col_limits.size();
        PackedVector::iterator itr = p_packed_col.begin();
        while (itr != p_packed_col.end()) {
            PackedElement packed_element = *itr;
            m_vct_disk.push_back(packed_element);
            itr++;
        }
        ColAttr col_limits(no_cols, no_cols + p_packed_col.get_nnz());
        m_vct_col_limits.push_back(col_limits);
    }
    //! Adds n number of slack variables as singleton columns
    void add_slack_columns(unsigned int n) {
        for (unsigned int i = 0; i < n; i++) {
            unsigned int no_singleton_cols = m_vct_singleton_columns.size();
            PackedElement packed_element(no_singleton_cols, 1.0F);
            m_vct_singleton_columns.push_back(packed_element);
        }
    }
    //! Get an Entire column 
    //! Send by value... Hence new PackedVector will be created after call
    PackedVector get_column(unsigned int p_column_idx) {
        //! Limits of the requested column
        ColAttr col_attr = m_vct_col_limits[p_column_idx];
        unsigned int nnz = col_attr.get_end() - col_attr.get_start() + 1;
        PackedVector packed_col(nnz);
        for (unsigned int i = col_attr.get_start(); i < col_attr.get_end(); i++) {
            const PackedElement packed_element = m_vct_disk[i];
            packed_col.add(packed_element.get_index(), packed_element.get_value());
        }
        return packed_col;
    }

    //! Store an Entire column 
    //! Send by Reference... Hence parameter PackedVector will be affected
    void store_column(unsigned int p_column_idx, PackedVector& p_packed_col) {
        //! Limits of the requested column
        ColAttr col_attr = m_vct_col_limits[p_column_idx];
        for (unsigned int i = col_attr.get_start(); i < col_attr.get_end(); i++) {
            const PackedElement packed_element = m_vct_disk[i];
            p_packed_col.add(packed_element.get_index(), packed_element.get_value());
        }
    }
};


#endif	/* EXTFIXEDMATRIX_H */

