/* 
 * File:   ExtColVector.h
 * Author: sriram
 *
 * Created on 16 April, 2014, 11:15 AM
 */

#ifndef EXTCOLVECTOR_H
#define	EXTCOLVECTOR_H

#include "PackedColVector.h"
#include "ExtVector.h"
class ExtColVector {
private:

    //------------------------------------
    /**@name Data */
    //@{
    REAL up; ///< upper bound
    REAL low; ///< lower bound
    REAL object; ///< objective value
    ExtVector vctCol; ///< the column vector
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
    explicit ExtColVector(PackedColVector& vctPack)
    : up(vctPack.upper()), low(vctPack.lower()), object(vctPack.obj()),
    vctCol(vctPack.getPackedVector()), name(vctPack.getName()) {
    }

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
    std::string getName() const {
        return name;
    }

    /// get the input row element of the current column
    REAL getRowElement(unsigned int row) {
        return vctCol.getAbsoluteIndexElement(row);
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

    //    /// access constraint column vector.
    //    void setColVector(const std::vector<REAL>& p_vec) {
    //        vctCol.setRealSize(p_vec.size());
    //        for (unsigned int i = 0; i < p_vec.size(); i++) {
    //            vctCol.add(i, p_vec[i]);
    //        }
    //    }
};

#endif	/* EXTCOLVECTOR_H */

