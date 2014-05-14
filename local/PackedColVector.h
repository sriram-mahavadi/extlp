/* 
 * File:   PackedColVector.h
 * Author: Sriram Mahavadi
 *
 * Created on 16 April, 2014, 10:56 AM
 */

#ifndef PACKEDCOLVECTOR_H
#define	PACKEDCOLVECTOR_H
#include "GlobalDefines.h"
#include "PackedVector.h"
class PackedColVector {
private:
    REAL m_upper_bound;
    REAL m_lower_bound;
    REAL m_objective_value;
    PackedVector m_vct_col;
    std::string m_name;
public:
    //    friend class PackedVector::iterator;
    typedef PackedVector::iterator iterator;
    //! Initialization with input as the real_size of the column vector
    explicit PackedColVector(int defDim = 0)
    : m_upper_bound(INFINITY_VALUE), m_lower_bound(0), m_objective_value(0), m_vct_col(defDim) {
    }
    //! Copy constructor 
    //! Highly uneccesary for assignment/initialization
    PackedColVector(PackedColVector& p_packed_col):m_vct_col(p_packed_col.get_col_vector()){
        DEBUG_WARNING("PackedColVector Copy Constructor... Getting Shallow copy of Linked list");
    }
    
    //!@ Simple Getters + Setters {
    REAL get_objective_value() const {
        return m_objective_value;
    }
    REAL get_upper_bound() const {
        return m_upper_bound;
    }
    REAL get_lower_bound() const {
        return m_lower_bound;
    }
    std::string get_name() const {
        return m_name;
    }
    //! Returns the reference to the Packed column
    PackedVector& get_col_vector() {
        return m_vct_col;
    }
    void set_objective_value(REAL p_object) {
        m_objective_value = p_object;
    }
    void set_upper_bound(REAL p_up) {
        m_upper_bound = p_up;
    }
    void set_name(std::string p_name) {
        m_name = p_name;
    }
    void set_lower_bound(REAL p_low) {
        m_lower_bound = p_low;
    }
    //! Sets the col vector with complete initialization to p_vec
    void set_col_vector(const std::vector<REAL>& p_vec) {
        m_vct_col.clear();
        m_vct_col.resize(p_vec.size());
        for (unsigned int i = 0; i < p_vec.size(); i++) {
            m_vct_col.add(i, p_vec[i], false);
        }
    }
    //! }
    // Gets the specific row value to the 
    // Highly inefficient. Always use sequential access with iterators
    REAL get_row_element(unsigned int row) const {
        return m_vct_col.get(row);
    }
};

#endif	/* PACKEDCOLVECTOR_H */

