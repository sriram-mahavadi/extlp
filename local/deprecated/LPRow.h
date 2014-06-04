/* 
 * File:   LPRow.h
 * Author: sriram
 *
 * Created on 18 March, 2014, 12:00 AM
 */

#ifndef LPROW_H
#define	LPROW_H


#include "../GlobalDefines.h"

/**@brief   (In)equality for LPs.
   @ingroup Algo

   Class LPRow provides constraints for linear programs in the form
   \f[
                       l \le a^Tx \le r,
   \f]
   where \em a is a DSVector. \em l is referred to as 
   %left hand side,
   \em r as %right hand side and \em a as \em row \em vector or 
   the constraint vector. \em l and \em r may also take values 
   \f$\pm\f$ #infinity. 
   This static member is predefined, but may be overridden to meet 
   the needs of the LP solver to be used.
 
   LPRows allow to specify regular inequalities of the form 
   \f[
                           a^Tx \sim \alpha,
   \f]
   where \f$\sim\f$ can take any value 
   of \f$\le, =, \ge\f$, by setting rhs and
   lhs to the same value or setting one of them to \f$\infty\f$.

   Since constraints in the regular form occur often, LPRows offers methods
   type() and value() for retreiving \f$\sim\f$ and \f$\alpha\f$ of 
   an LPRow in this form, respectively. Also, a constructor for 
   LPRows given in regular form is provided.
 */
class LPRow {
private:

    //------------------------------------
    /**@name Data */
    //@{
    REAL left; ///< left-hand side of the constraint
    REAL right; ///< right-hand side of the constraint
    std::vector<REAL> vec; ///< the row vector
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

    explicit LPRow(int defDim = 0)
    : left(0), right(INFINITY_VALUE), vec(defDim) {
        assert(isConsistent());
    }

    //    /// copy constructor
    //
    //    LPRow(const LPRow& row)
    //    : left(row.left), right(row.right), vec(row.vec) {
    //        assert(isConsistent());
    //    }
    //
    //    /// Construct LPRow with the given left-hand side, right-hand side
    //    /// and rowVector.
    //
    //    LPRow(REAL p_lhs, const SVector& p_rowVector, REAL p_rhs)
    //    : left(p_lhs), right(p_rhs), vec(p_rowVector) {
    //        assert(isConsistent());
    //    }

    /// Construct LPRow from passed \p rowVector, \p type and \p value
    //    LPRow(const SVector& p_rowVector, Type p_type, Real p_value);

    /// destructor

    ~LPRow() {
    }
    //@}

    //------------------------------------
    /**@name Access / modification */
    //@{
    /// get type of row.

    Type type() const {
        if (rhs() >= INFINITY_VALUE)
            return GREATER_EQUAL;
        if (lhs() <= -INFINITY_VALUE)
            return LESS_EQUAL;
        if (lhs() == rhs())
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
                if (lhs() > -INFINITY_VALUE)
                    right = lhs();
                else
                    left = rhs();
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

        return (rhs() < INFINITY_VALUE) ? rhs() : lhs();
    }

//    char* type_str() {
//        switch (type()) {
//
//        }
//    }


    /// get left-hand side value.

    REAL lhs() const {
        return left;
    }

    /// get the name of the LP Row
    
    std::string getName() const{
        return name;
    }
    
    /// access the name of the LP row
    
    void setName(std::string p_name){
        name = p_name;
    }
    
    /// access left-hand side value.

    void setLhs(REAL p_left) {
        left = p_left;
    }

    /// get right-hand side value of (in)equality.

    REAL rhs() const {
        return right;
    }

    /// access right-hand side value.

    void setRhs(REAL p_right) {
        right = p_right;
    }

    /// get constraint row vector

    //    const SVector& rowVector() const {
    //        return vec;
    //    }

    /// access constraint row vector.

    //    void setRowVector(const DSVector& p_vec) {
    //        vec = p_vec;
    //    }
    //@}

    //------------------------------------
    /**@name Consistency check */
    //@{
    /// check consistency.

    bool isConsistent() const {
#ifdef ENABLE_CONSISTENCY_CHECKS
        return vec.isConsistent();
#else
        return true;
#endif
    }
    //@}
};

#endif	/* LPROW_H */

