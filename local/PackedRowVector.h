/* 
 * File:   PackedRowVector.h
 * Author: sriram
 *
 * Created on 16 April, 2014, 10:56 AM
 */

#ifndef PACKEDROWVECTOR_H
#define	PACKEDROWVECTOR_H
#include "GlobalDefines.h"
#include "PackedVector.h"
//! PackedRowVector: Meant to store the row of the LP Tableau
//! Only meant for keeping track of REAL values pertaining to the LP Column
class PackedRowVector {
private:
    REAL m_lhs_value;
    REAL m_rhs_value;
    std::string m_name;
public:
    enum Type {
        LESS_EQUAL,
        EQUAL,
        GREATER_EQUAL,
        RANGE
    };

    //! Initialization
    explicit PackedRowVector()
    : m_lhs_value(0), m_rhs_value(INFINITY_VALUE) {
    }

    //! TODO - Return proper row type as per the Matrix A
    //! Integer identifier for the row type 
    //! 0 Equals; 1 greater than Equals; 2 Less than Equals
    //! 3 Less than; 4 Greater ; 5 Range
    int get_row_type() {
        /// Used in ExtMatrixA to externally store row attributes.
        switch(get_type()){
            case EQUAL: return 0;
            case GREATER_EQUAL: return 1;
            case LESS_EQUAL: return 2;
            case RANGE: return 5;
        }
        return -1; // Unknown or Unidentified Type
    }
    Type get_type() const {
        if (get_rhs() >= INFINITY_VALUE)
            return GREATER_EQUAL;
        if (get_lhs() <= -INFINITY_VALUE)
            return LESS_EQUAL;
        if (get_lhs() == get_rhs())
            return EQUAL;
        return RANGE;
    }
    void set_type(Type p_type) {
        switch (p_type) {
            case LESS_EQUAL:
                m_lhs_value = -INFINITY_VALUE;
                break;
            case EQUAL:
                if (get_lhs() > -INFINITY_VALUE)
                    m_rhs_value = get_lhs();
                else
                    m_lhs_value = get_rhs();
                break;
            case GREATER_EQUAL:
                m_rhs_value = INFINITY_VALUE;
                break;
            case RANGE:
                DEBUG_ERROR("ELPROW01 RANGE not supported in LPRow::setType()"
                        << std::endl)
                        //                        throw SPxInternalCodeException("XLPROW01 This should never happen.");
            default:
                DEBUG_ERROR("XLPROW02 This should never happen.");
                //                throw SPxInternalCodeException("XLPROW02 This should never happen.");
        }
    }
    REAL get_value() const {
        assert(get_type() != RANGE);
        return (get_rhs() < INFINITY_VALUE) ? get_rhs() : get_lhs();
    }
    REAL get_lhs() const {
        return m_lhs_value;
    }
    std::string get_name() const {
        return m_name;
    }
    void set_name(std::string p_name) {
        m_name = p_name;
    }
    void set_lhs(REAL p_left) {
        m_lhs_value = p_left;
    }
    REAL get_rhs() const {
        return m_rhs_value;
    }
    void set_rhs(REAL p_right) {
        m_rhs_value = p_right;
    }
};

#endif	/* PACKEDROWVECTOR_H */

