/* 
 * File:   ExtRowVector.h
 * Author: sriram
 *
 * Created on 16 April, 2014, 11:15 AM
 */

#ifndef EXTROWVECTOR_H
#define	EXTROWVECTOR_H

#include "PackedRowVector.h"
class ExtRowVector {
private:
    //------------------------------------
    /**@name Data */
    //@{
    REAL left; ///< left-hand side of the constraint
    REAL right; ///< right-hand side of the constraint
    ExtVector vctPack; ///< the row vector
    std::string name;
    //@}
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
    explicit ExtRowVector(PackedRowVector &packedRowVector)
    : left(packedRowVector.getLhs()), right(packedRowVector.getRhs()), vctPack(packedRowVector.getPackedVector()),
    name(packedRowVector.getName()) {
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
    void setType(LPRow::Type p_type) {
        switch (p_type) {
            case LESS_EQUAL:
                left = -INFINITY_VALUE;
                break;
            case EQUAL:
                if (getLhs() > -INFINITY_VALUE)
                    right = getLhs();
                else
                    left = getRhs();
                break;
            case GREATER_EQUAL:
                right = INFINITY_VALUE;
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
        return left;
    }

    /// get the name of the LP Row
    std::string getName() const {
        return name;
    }

    /// access the name of the LP row
    void setName(std::string p_name) {
        name = p_name;
    }

    /// access left-hand side value.
    void setLhs(REAL p_left) {
        left = p_left;
    }

    /// get right-hand side value of (in)equality.
    REAL getRhs() const {
        return right;
    }

    /// access right-hand side value.
    void setRhs(REAL p_right) {
        right = p_right;
    }

};

#endif	/* EXTROWVECTOR_H */

