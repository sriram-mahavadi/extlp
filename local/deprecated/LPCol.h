/* 
 * File:   LPCol.h
 * Author: sriram
 *
 * Created on 18 March, 2014, 12:00 AM
 */

#ifndef LPCOL_H
#define	LPCOL_H

#include "../GlobalDefines.h"

/**@brief   LP column.
   @ingroup Algo

   Class LPCol provides a datatype for storing the column of an LP a the
   form similar to
   \f[
      \begin{array}{rl}
         \hbox{max}  & c^T x         \\
         \hbox{s.t.} & Ax \le b      \\
                     & l \le x \le u
      \end{array}
   \f]
   Hence, an LPCol consists of an objective value, a column DSVector and
   an upper and lower bound to the corresponding variable, which may include
   \f$\pm\infty\f$. However, it depends on the LP code to use, what values are
   actually treated as \f$\infty\f$.
 */
class LPCol {
private:

    //------------------------------------
    /**@name Data */
    //@{
    REAL up; ///< upper bound
    REAL low; ///< lower bound
    REAL object; ///< objective value
    std::vector<REAL> vec; ///< the column vector
    std::string name;
    //@}

public:

    //------------------------------------
    /**@name Construction / destruction */
    //@{
    /// default constructor.

    /** Construct LPCol with a column vector ready for taking \p defDim
     *  nonzeros.
     */
    explicit LPCol(int defDim = 0)
    : up(INFINITY_VALUE), low(0), object(0) {
        vec.resize(defDim);
        assert(isConsistent());
    }

    /// initializing constructor.
    /*  Construct LPCol with the given objective value \p obj, a column
     *  %vector \p vec, upper bound \p upper and lower bound \p lower.
     */
    //   LPCol(Real p_obj, const SVector& p_vector, Real p_upper, Real p_lower)
    //      : up(p_upper), low(p_lower), object(p_obj), vec(p_vector)
    //   {
    //      assert(isConsistent());
    //   }

    /// copy constructor.

//    LPCol(const LPCol& old)
//    : up(old.up), low(old.low), object(old.object), vec(old.vec) {
//        assert(isConsistent());
//    }

    /// destructor

    ~LPCol() {
    }
    //@}

    //------------------------------------
    /**@name Access / modification */
    //@{
    /// get objective value.

    REAL obj() const {
        return object;
    }
    /// access objective value.

    void setObj(REAL p_object) {
        object = p_object;
    }

    /// get upper bound.

    REAL upper() const {
        return up;
    }
    /// access upper bound.

    void setUpper(REAL p_up) {
        up = p_up;
    }

    /// get lower bound.

    REAL lower() const {
        return low;
    }

    /// get name of the LP Column

    std::string getName() const{
        return name;
    }

    /// get the input row element of the current column

    REAL getRowElement(unsigned int row) const{
        if (row < vec.size())
            return vec[row];
        else
            return INFINITY_VALUE;
//        return vec.size();
    }
    /// access the name of the LP Column

    void setName(std::string p_name) {
        name = p_name;
    }

    /// access lower bound.

    void setLower(REAL p_low) {
        low = p_low;
    }

    /// get constraint column vector.
    //   const SVector& colVector() const
    //   {
    //      return vec;
    //   }

    /// access constraint column vector.

    void setColVector(const std::vector<REAL>& p_vec) {
        vec = p_vec;
    }
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


#endif	/* LPCOL_H */

