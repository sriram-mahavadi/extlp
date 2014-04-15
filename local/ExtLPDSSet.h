/* 
 * File:   ExtLPDSSet.h
 * Author: sriram
 *
 * Created on 8 April, 2014, 12:40 PM
 */

#ifndef EXTLPDSSET_H
#define	EXTLPDSSET_H

#include "ExtVector.h"
#include "ExtNameMap.h"
/**
 * Contains the total set of problem inputs or data structures
 * that will be required at each step of simplex method
 */
class ExtLPDSSet {
private:
    std::vector<ExtVector> &vctCols;
    std::vector<ExtVector> &vctRows;
    //    ExtVector &vctRhs;
    //    ExtVector &vctObj;
    PackedVector &vctRhs; // Better to try and fit rhs 
    PackedVector &vctObj; // and obj in memory as they are freq used
    std::vector<PackedVector> &vctCacheCols;
    std::vector<PackedVector> &vctCacheRows;
    REAL *cacheMatrix[4]; // Try to cache B^-1

    // Names of the Rows and Cols involved in the 
    ExtNameMap &mapRowName;
    ExtNameMap &mapColName;

public:
    /// Initialization 
    /// Better decide the data structures from the main itself.
    ExtLPDSSet(
            std::vector<ExtVector> &p_vctCols, std::vector<ExtVector> &p_vctRows,
            PackedVector &p_vctRhs, PackedVector &p_vctObj,
            std::vector<PackedVector> &p_vctCacheCols,
            std::vector<PackedVector> &p_vctCacheRows,
            ExtNameMap &p_mapRowName, ExtNameMap &p_mapColName
            ) :
    vctCols(p_vctCols), vctRows(p_vctRows), vctRhs(p_vctRhs),
    vctObj(p_vctObj), vctCacheCols(p_vctCacheCols), vctCacheRows(p_vctCacheRows),
    mapRowName(p_mapRowName), mapColName(p_mapColName) {
    }
};


#endif	/* EXTLPDSSET_H */

