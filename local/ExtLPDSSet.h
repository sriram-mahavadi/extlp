/* 
 * File:   ExtLPDSSet.h
 * Author: sriram
 *
 * Created on 8 April, 2014, 12:40 PM
 */

#ifndef EXTLPDSSET_H
#define	EXTLPDSSET_H

#include "ExtNameMap.h"
#include "ExtMatrixA.h"
#include "ExtMatrixBInverse.h"
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
    ExtMatrixA &A;
    ExtMatrixBInverse &BInverse;
    // Names of the Rows and Cols involved in the 
    ExtNameMap &mapRowName;
    ExtNameMap &mapColName;


    
    /// Initialization 
    /// Better decide the data structures from the main itself.
    ExtLPDSSet(
            ExtMatrixA &p_A,ExtMatrixBInverse& p_BInverse,
            ExtNameMap &p_mapRowName, ExtNameMap &p_mapColName
            ) : A(p_A), BInverse(p_BInverse), mapRowName(p_mapRowName), mapColName(p_mapColName) {
    }
    
    //! Check for any sort of duplication. Completely unnecessary in whole project
    ExtLPDSSet(ExtLPDSSet& extDataSet):
    A(extDataSet.A), BInverse(extDataSet.BInverse), 
    mapRowName(extDataSet.mapRowName), mapColName(extDataSet.mapColName){
        DEBUG_WARNING("Trying to Copy/Duplicate Ext LP Data set.");
    }
    
    std::string get_problem_name() {
        return probName;
    }
    /// returns true if the problem is minimization problem
    bool get_objective_sense_bool() {
        return (objSense == MIN);
    }
    ObjSense get_objective_sense() {
        return objSense;
    }
    std::string get_objective_name() {
        return objName;
    }
    std::string get_rhs_name() {
        return rhsName;
    }
    void set_problem_name(std::string p_probName) {
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

