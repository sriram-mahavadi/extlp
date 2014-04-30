/* 
 * File:   PackedColVector.h
 * Author: sriram
 *
 * Created on 16 April, 2014, 10:56 AM
 */

#ifndef PACKEDCOLVECTOR_H
#define	PACKEDCOLVECTOR_H
#include "../GlobalDefines.h"
#include "../PackedVector.h"
class PackedColVector {
private:

    //------------------------------------
    /**@name Data */
    //@{
    REAL up; ///< upper bound
    REAL low; ///< lower bound
    REAL object; ///< objective value
    PackedVector<REAL> vctCol; ///< the column vector
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
    explicit PackedColVector(int defDim = 0)
    : up(INFINITY_VALUE), low(0), object(0), vctCol(defDim, 0) {
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
    //! TODO return for unpacked and packed condition
    REAL getRowElement(unsigned int row) const {
        return vctCol[row];
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
        vctCol.clear();
        vctCol.resize(p_vec.size());
        for (unsigned int i = 0; i < p_vec.size(); i++) {
            vctCol.add(i, p_vec[i]);
        }
    }
    PackedVector<REAL>& getPackedVector() {
        return vctCol;
    }
};

#endif	/* PACKEDCOLVECTOR_H */

