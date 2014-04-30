/* 
 * File:   ExtRowVector.h
 * Author: sriram
 *
 * Created on 16 April, 2014, 11:15 AM
 */

#ifndef EXTROWVECTOR_H
#define	EXTROWVECTOR_H

#include "deprecated/PackedRowVector.h"
#include "ExtVector.h"
#include "ExtPackedVector.h"
#include "PackedVector.h"
class ExtRowVector {
private:
    //------------------------------------
    /**@name Data */
    //@{
    REAL m_left; ///< left-hand side of the constraint
    REAL m_right; ///< right-hand side of the constraint
    ExtVector<REAL>* m_vctUnpacked; ///<  row vector unpacked version
    ExtPackedVector<REAL>* m_vctPacked; /// row vector - packed version 
    std::string m_name;
    //@}
    void deallocatePackedVector() {
        assert(m_vctPacked != NULL);
        delete m_vctPacked;
        m_vctPacked = NULL;
    }
    void deallocateUnpackedVector() {
        assert(m_vctUnpacked != NULL);
        delete m_vctUnpacked;
        m_vctUnpacked = NULL;
    }

public:
    //------------------------------------
    /**@name Types */
    //@{
    /// (In)Equality of an LP row.
    /** #LPRow%s may be of one of the above Types. This datatype may be
     *  used for constructing new #LPRow%s in the regular form.
     */
    enum Type {
        LESS_EQUAL, ///< \f$a^Tx \le \alpha\f$.   
        EQUAL, ///< \f$a^Tx = \alpha\f$.   
        GREATER_EQUAL, ///< \f$a^Tx \ge \alpha\f$.    
        RANGE ///< \f$\lambda \le a^Tx \le \rho\f$.
    };
    //@}

    //------------------------------------
    /**@name Construction / destruction */
    //@{
    /// Construct LPRow with a vector ready to hold \p defDim nonzeros
    /// The ExtRowVector is constructed from the existing PackedRowVector
    /// from the memory
    /// Not using row-wise representation of the vector for now
    explicit ExtRowVector(PackedRowVector &packedRowVector)
    : m_left(packedRowVector.getLhs()), m_right(packedRowVector.getRhs()),
    m_name(packedRowVector.getName()) {
        //        PackedVector<REAL> &packedVector = packedRowVector.getPackedVector();
        //        if (packedVector.isPacked()) {
        //            m_vctPacked = new ExtPackedVector<REAL>(packedVector);
        //            m_vctUnpacked = NULL;
        //        } else {
        //            m_vctPacked = NULL;
        //            m_vctUnpacked = new ExtVector<REAL>(packedVector);
        //        }
    }

    //@}

    //------------------------------------
    /**@name Access / modification */
    //@{
    /// get type of row.
    Type type() const {
        if (getRhs() >= INFINITY_VALUE)
            return GREATER_EQUAL;
        if (getLhs() <= -INFINITY_VALUE)
            return LESS_EQUAL;
        if (getLhs() == getRhs())
            return EQUAL;
        return RANGE;
    }

    /// set type of (in)equality
    void setType(Type p_type) {
        switch (p_type) {
            case LESS_EQUAL:
                m_left = -INFINITY_VALUE;
                break;
            case EQUAL:
                if (getLhs() > -INFINITY_VALUE)
                    m_right = getLhs();
                else
                    m_left = getRhs();
                break;
            case GREATER_EQUAL:
                m_right = INFINITY_VALUE;
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
    /** This method returns \f$\alpha\f$ for a LPRow in regular form.
     *  However, value() may only be called for LPRow%s with
     *  type() != \c RANGE.
     */
    REAL value() const {
        assert(type() != RANGE);
        return (getRhs() < INFINITY_VALUE) ? getRhs() : getLhs();
    }

    //    char* type_str() {
    //        switch (type()) {
    //
    //        }
    //    }


    /// get left-hand side value.
    REAL getLhs() const {
        return m_left;
    }

    /// get the name of the LP Row
    std::string getName() const {
        return m_name;
    }

    /// access the name of the LP row
    void setName(std::string p_name) {
        m_name = p_name;
    }

    /// access left-hand side value.
    void setLhs(REAL p_left) {
        m_left = p_left;
    }

    /// get right-hand side value of (in)equality.
    REAL getRhs() const {
        return m_right;
    }

    /// access right-hand side value.
    void setRhs(REAL p_right) {
        m_right = p_right;
    }

};

#endif	/* EXTROWVECTOR_H */

