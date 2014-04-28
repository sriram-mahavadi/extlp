/* 
 * File:   ExtLPDSSet.h
 * Author: sriram
 *
 * Created on 8 April, 2014, 12:40 PM
 */

#ifndef EXTLPDSSET_H
#define	EXTLPDSSET_H

#include "ExtNameMap.h"
#include "ExtRowVector.h"
#include "ExtColVector.h"
#include "PackedRowVector.h"
#include "PackedColVector.h"
/**
 * Contains the total set of problem inputs or data structures
 * that will be required at each step of simplex method
 */
enum ObjSense {
    MIN, MAX
};
class ExtLPDSSet {
private:
    std::string probName;
    ObjSense objSense;
    std::string objName;
    std::string rhsName;
public:
    /// Data structures are accessed across the project and only one instance of them
    /// is  maintained through out. Make sure we only keep sending the reference of
    /// ExtLPDSSet object.
    std::vector<ExtRowVector> &vctRows;
    std::vector<ExtColVector> &vctCols;
    // Now Rhs and Obj are parts of RowVector and ColVector respectively
    //    ExtVector &vctRhs;
    //    ExtVector &vctObj;
    //    PackedVectorPrototype &vctRhs; // Better to try and fit PackedVectorPrototypekedVector &vctObj; // and obj in memory as they are freq used
    //    std::vector<PackedRowVector> &vctCacheRows;
    //    std::vector<PackedColVector> &vctCacheCols;
    //    REAL *cacheMatrix[4]; // Try to cache B^-1

    // Names of the Rows and Cols involved in the 
    ExtNameMap &mapRowName;
    ExtNameMap &mapColName;

    /// Initialization 
    /// Better decide the data structures from the main itself.
    ExtLPDSSet(
            std::vector<ExtRowVector> &p_vctRows,
            std::vector<ExtColVector> &p_vctCols,
            ExtNameMap &p_mapRowName, ExtNameMap &p_mapColName
            ) :
    vctRows(p_vctRows), vctCols(p_vctCols),
    mapRowName(p_mapRowName), mapColName(p_mapColName) {
    }
    std::string getProbName() {
        return probName;
    }

    /// returns true if the problem is minimization problem
    bool getObjSenseAsBool() {
        return (objSense == MIN);
    }
    ObjSense getObjSense() {
        return objSense;
    }
    std::string getObjName() {
        return objName;
    }
    std::string getRhsName() {
        return rhsName;
    }
    void setProbName(std::string p_probName) {
        probName = p_probName;
    }
    void setObjSense(ObjSense p_objSense) {
        objSense = p_objSense;
    }

    /// parameter isMin=true when the problem is minimization, false otherwise
    void setObjSense(bool isMin) {
        objSense = (isMin) ? MIN : MAX;
    }
    void setObjName(std::string p_objName) {
        objName = p_objName;
    }
    void setRhsName(std::string p_rhsName) {
        rhsName = p_rhsName;
    }
};


#endif	/* EXTLPDSSET_H */

