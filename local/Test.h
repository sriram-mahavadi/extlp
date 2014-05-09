/* 
 * File:   Test.h
 * Author: harsha
 *
 * Created on 22 April, 2014, 11:03 AM
 */

#ifndef TEST_H
#define	TEST_H
#include "LinkedList.h"
#include "GlobalDebug.h"
#include "PackedVector.h"
#include "deprecated/ExtVectorPrototype.h"
#include "ExtVector.h"
#include "ExtPackedVector.h"
#include "ExtNameMap.h"
#include "ExtLPDSSet.h"
class Test {
public:
    static void testExtNameMap() {
        fixed_name_map myFixedMap((fixed_name_map::node_block_type::raw_size)*5, (fixed_name_map::leaf_block_type::raw_size)*5);
        unsigned int testSize = 1000000;
        ExtNameMap nameMap(myFixedMap);
        //        ExtNameMap strMap(myMap);
        for (unsigned int i = 1; i < testSize; i++) { /// Inserting 1 million strings
            std::stringstream strStream;
            strStream << (i);
            //            FixedString fixedString = FixedStringUtil::getFixedString(strStream.str());
            //            CONSOLE_PRINTLN("Inserting: " + strStream.str());
            //            strMap.set(fixedString, i);
            if (i % (testSize / 10) == 0) {
                CONSOLE_PRINTLN("Number of Keys Written: " << i);
            }
            nameMap.set(strStream.str(), i);
            //            myFixedMap[fixedString] = i;
        }
        DEBUG_MEMORY("Completed Writing " << testSize << " keys.");
    }
    static void testLinkedList() {
        bool testIterator = false;
        bool testStorage = true;
        bool testSimpleStoragePointer = false;
        bool testStorageWithPointer = false;
        CONSOLE_PRINTLN("************************ Tesiting LinkedList ************************");
        if (testStorage) {
            LinkedList<REAL> linkedList;
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before LinkedList - Initialization of " << testSize << " elements");
            for (unsigned int i = 0; i < testSize; i++)
                linkedList.add((REAL) i);
            DEBUG("Added " << linkedList.size() << " elements in total");
            DEBUG_MEMORY("After LinkedList - Initialization of " << testSize << " elements");
            DEBUG_MEMORY("Before LinkedList - Clear of " << testSize << " elements");
            linkedList.clear();
            DEBUG_MEMORY("After LinkedList - Clear of " << testSize << " elements");

            LinkedList<REAL> linkedList2;
            DEBUG_MEMORY("Before LinkedList - Initialization of " << testSize << " elements");
            for (unsigned int i = 0; i < testSize; i++)
                linkedList2.add((REAL) i);
            DEBUG("Added " << linkedList2.size() << " elements in total");
            DEBUG_MEMORY("After LinkedList - Initialization of " << testSize << " elements");
            DEBUG_MEMORY("Before LinkedList - Clear of " << testSize << " elements");
            linkedList2.clear();
            DEBUG_MEMORY("After LinkedList - Clear of " << testSize << " elements");

            LinkedList<REAL> linkedList3;
            DEBUG_MEMORY("Before LinkedList - Initialization of " << testSize << " elements");
            for (unsigned int i = 0; i < testSize; i++)
                linkedList3.add((REAL) i);
            DEBUG("Added " << linkedList3.size() << " elements in total");
            DEBUG_MEMORY("After LinkedList - Initialization of " << testSize << " elements");
            DEBUG_MEMORY("Before LinkedList - Clear of " << testSize << " elements");
            linkedList3.clear();
            DEBUG_MEMORY("After LinkedList - Clear of " << testSize << " elements");

            LinkedList<REAL> linkedList4;
            DEBUG_MEMORY("Before LinkedList - Initialization of " << testSize << " elements");
            for (unsigned int i = 0; i < testSize; i++)
                linkedList4.add((REAL) i);
            DEBUG("Added " << linkedList4.size() << " elements in total");
            DEBUG_MEMORY("After LinkedList - Initialization of " << testSize << " elements");
            DEBUG_MEMORY("Before LinkedList - Clear of " << testSize << " elements");
            linkedList4.clear();
            DEBUG_MEMORY("After LinkedList - Clear of " << testSize << " elements");
        }
        if (testStorageWithPointer) {
            LinkedList<REAL> *linkedList = new LinkedList<REAL>();
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before LinkedList - Initialization of " << testSize << " elements");
            for (unsigned int i = 0; i < testSize; i++)
                linkedList->add((REAL) i);
            DEBUG_MEMORY("After LinkedList - Initialization of " << testSize << " elements");
            DEBUG_MEMORY("Before LinkedList - Clear of " << testSize << " elements");
            linkedList->clear();
            DEBUG_MEMORY("After LinkedList - Clear of " << testSize << " elements");
            DEBUG_MEMORY("Before LinkedList - Delete of " << testSize << " elements");
            delete linkedList;
            DEBUG_MEMORY("After LinkedList - Delete of " << testSize << " elements");
        }
        if (testSimpleStoragePointer) {
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before Allocating " << testSize << " elements");
            try {
                REAL *p = new REAL[testSize];
                for (unsigned int i = 0; i < testSize; i++) {
                    p[i] = i;
                }
                DEBUG_MEMORY("After Allocating " << testSize << " elements");
                DEBUG_MEMORY("Before Deleting " << testSize << " elements");
                delete []p;
                DEBUG_MEMORY("After Deleting " << testSize << " elements");
            } catch (std::bad_alloc &ba) {
                DEBUG_ERROR("Bad ALLOC: " << ba.what());
            }
        }
        if (testIterator) {
            LinkedList<REAL> linkedList;
            unsigned int testSize = 100000000;
            for (unsigned int i = 0; i < testSize; i++)
                linkedList.add((REAL) i);
            LinkedList<REAL>::iterator itr = linkedList.begin();
            while (itr != linkedList.end()) {
                double val = *itr;
                DEBUG("Value is: " << val);
                itr++;
            }
        }
    }

    //! Testing Packed Vector
    static void testPackedVector2() {
        bool testConversion = true;
        CONSOLE_PRINTLN("************************ Tesiting PackedVector2 ************************");
        if (testConversion) {
            CONSOLE_PRINTLN("--- Tesiting PackedVector2 Conversion ---");
            REAL arrTest[] = {1.0, 2.0, 5.0, 6.0, 0.0, 0.0, 0.0, 0.0, 7.1, 0.0, 0.0, 8.2, 9, 9, 9, 9, 9, 9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
            PackedVector vctPack(sizeof (arrTest) / sizeof (REAL));
            //        vctPack.setRealSize(sizeof(arrTest)/sizeof(REAL));
            vctPack.displayVector();
            unsigned int i;
            for (i = 0; i<sizeof (arrTest) / sizeof (REAL); i++) {
                vctPack.add(i, arrTest[i]);
                vctPack.displayVector();
            }
            // Converting Mode Forcedly
            vctPack.displayVector();
        }
        CONSOLE_PRINTLN("************************ End ************************");
    }
    //! Test template
    static void test() {
        // Section for deciding which testcases need to be checked
        CONSOLE_PRINTLN("************************ Tesiting Title ************************");
        // Section for Testing all different testcases
        CONSOLE_PRINTLN("************************ End ************************");


    }
    //! Testing ExtVector
    static void testExtVectorPrototype() {
        bool testConversion = false;
        bool testStorage = true;
        CONSOLE_PRINTLN("************************ Tesiting ExtVector ************************");
        if (testStorage) {
            CONSOLE_PRINTLN("--- Tesiting ExtVector Storage ---");
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before PackedVector - Initialization of " << testSize << " elements");
            PackedVectorPrototype vctPack(testSize);
            //            PackedVector2<REAL> vctPack(testSize, 0);
            unsigned int i;
            for (i = 0; i < testSize; i++) { // Adding 1 mil numbers
                vctPack.add(i, i);
            }
            DEBUG_MEMORY("After PackedVector - Initialization of " << testSize << " elements");
            DEBUG_MEMORY("Before ExtVector - Initialization of " << testSize << " elements");
            ExtVectorPrototype extVector(vctPack);
            DEBUG_MEMORY("After ExtVector - Initialization of " << testSize << " elements");


        }
        if (testConversion) {

        }
        // Takes up space of 8*2*(10^7) => 160MB sofar :)
        //        Console::println("Original Vector: ");
        //        vctPack.displayVector();
        // PackedVector to ExtVector
        //        ExtVector extVector(vctPack);
        //        DEBUG("Vector Size(in bytes): "<<extVector.getDiskStorageCapacity());
        //        Console::println("Value of index 2: ", extVector.getAbsoluteIndexElement(2));
        //        Console::println("Value of index 4: ", extVector.getAbsoluteIndexElement(4));
        //        extVector.displayVector();
        /// ExtVector to PackedVector
        //        PackedVector vctResult(testSize, extVector.isPacked());
        //        PackedVector vctResult(10000000, extVector.isPacked());
        //        extVector.storePackedVector(vctResult);
        //        std::vector<ExtVector*> vctExtStorage;
        //        /// Mutilple ExtVector Test
        //        int noTrials = 1000; /// Approx 8GB
        //        for(int j=0; j<noTrials; j++){
        //            ExtVector* tempVector = new ExtVector(vctPack);
        //            // 230-160 => 70MB for unpacking in this case
        //            vctExtStorage.push_back(tempVector);
        //            CONSOLE_PRINTLN("Number of Vectors Added: ", j);
        //        }
        //        vector<REAL>().swap(vctDisk);

        // Each ExtVector when getting typecasted could increase the size
        // temporarily for now as PackedVector can unpack/pack and increase size
        // Each vector has 32KB cache hence total of 32MB cache is expected
        //        CONSOLE_PRINTLN("External Stored Vector Recieved Back: ");
        //        vctResult.displayVector();
        CONSOLE_PRINTLN("************************ End ************************");
    }
    static void testExtVector() {
        //        bool testConversion = false;
        bool testStorage = false;
        bool testArray = true;
        CONSOLE_PRINTLN("************************ Tesiting ExtVector ************************");
        if (testStorage) {
            CONSOLE_PRINTLN("--- Tesiting ExtVector Storage ---");
            unsigned int testSize = 1000;
            DEBUG_MEMORY("Before ExtVector - Initialization of " << testSize << " elements");
            ExtVector extVector;
            extVector.resize(testSize);
            for (unsigned int i = 0; i < testSize; i++) {
                extVector.add(i, (REAL) i);
            }
            DEBUG_MEMORY("After ExtVector - Initialization of " << testSize << " elements");
            DEBUG_MEMORY("Before ExtVector - Deallocate Pagecache " << testSize << " elements");
            extVector.deallocate_cache();
            DEBUG_MEMORY("After ExtVector - Deallocate Pagecache of " << testSize << " elements");
        }
        if (testArray) {
            CONSOLE_PRINTLN("--- Tesiting ExtVector Array ---");
            unsigned int testArraySize = 1000; // Total number of vectors/columns
            unsigned int testSize = 1000000; // Number of elements in individual vector
            DEBUG_MEMORY("Before ExtVector - Adding " << testArraySize << " vectos of " << testSize << " elements");
            std::vector<ExtVector*> vctExtVector;
            for (unsigned int i = 0; i < testArraySize; i++) {
                ExtVector *extVector = new ExtVector();
                extVector->resize(testSize, true);
                extVector->allocate_cache();
                vctExtVector.push_back(extVector);
                for (unsigned int j = 0; j < testSize; j++) {
                    extVector->add(j, (REAL) j);
                }
                extVector->deallocate_cache();
                if (i % (testArraySize / 10) == 0) {
                    DEBUG_MEMORY("After ExtVector - Adding " << i << " vectos of " << testSize << " elements");
                }
            }
            DEBUG_MEMORY("After ExtVector - Adding " << testArraySize << " vectos of " << testSize << " elements");
        }
        CONSOLE_PRINTLN("************************ End ************************");
    }
    static void testExtPackedVector() {
        //        bool testConversion = false;
        bool testStorage = true;
        CONSOLE_PRINTLN("************************ Tesiting ExtPackedVector ************************");
        if (testStorage) {
            CONSOLE_PRINTLN("--- Tesiting ExtPackedVector Storage ---");
            unsigned int testSize = 100;
            DEBUG_MEMORY("Before PackedVector2 - Initialization of " << testSize << " elements");
            // Forced initialization of PackedVector into packedform
            PackedVector vctPacked(testSize);
            for (unsigned int i = 0; i < testSize; i++) {
                // Always add with autoConversion false if you donot want to convert
                // This is typical while incase of forced initialization
                vctPacked.add(i, (REAL) i);
            }
            DEBUG_MEMORY("Before ExtVector2 - Initialization of " << testSize << " elements");
            ExtPackedVector extPacked(vctPacked);
            ExtPackedVector::iterator itr = extPacked.begin();
            while (itr != extPacked.end()) {
                PackedElement packedElement = *itr;
                DEBUG("Index: " << packedElement.get_index() << ", Value: " << packedElement.get_value());
                itr++;
            }
            DEBUG_MEMORY("After ExtVector2 - Initialization of " << testSize << " elements");
        }
        CONSOLE_PRINTLN("************************ End ************************");
    }
    static void testExtMatrixA(ExtLPDSSet extDataSet) {

        DEBUG_SIMPLE("Number of Rows imported: " << extDataSet.A.get_rows_count());
        DEBUG_SIMPLE("Number of Columns imported: " << extDataSet.A.get_columns_count());
        DEBUG_SIMPLE("Writing Simplex Tableau to the Log!!!");


        DEBUG_FILE_WITH_TIMESTAMP(" *** Exporting LP Tableau *** ");
        DEBUG_FILE("Number of Rows imported: " << extDataSet.A.get_rows_count());
        DEBUG_FILE("Number of Columns imported: " << extDataSet.A.get_columns_count());
        DEBUG_FILE("Problem Name: " << extDataSet.get_problem_name());
        DEBUG_FILE("Partial Simplex Tableau is shown as follows: ");
        unsigned int i = 0, j = 0;

        //! Row-wise display of the LP Tableau
        //        std::stringstream titleStream;
        //        titleStream << std::setw(10) << "Row/Col" << ": ";
        //        for (j = 0; j < extDataSet.vctCols.size(); j++)
        //            titleStream << std::setw(10) << extDataSet.vctCols[j].getName() << ", ";
        //        titleStream << std::setw(10) << extDataSet.getRhsName() << ", ";
        //        DEBUG_FILE(titleStream.str());
        //
        //        std::stringstream objStream;
        //        std::string objTitle("OBJ-");
        //        objTitle = objTitle + mpsInput.objName();
        //        objStream << std::setw(10) << objTitle << ": ";
        //        for (i = 0; i < extDataSet.vctCols.size(); i++) {
        //            objStream << std::setw(10) << extDataSet.vctCols[i].obj() << ", ";
        //        }
        //        /// Rhs Column in objective constraint
        //        objStream << std::setw(10) << 0 << ", ";
        //        DEBUG_FILE(objStream.str());
        //        for (i = 0; i < extDataSet.vctRows.size(); i++) {
        //            //            string rowName(arrRow[i].)
        //            std::stringstream rowStream;
        //            rowStream << std::setw(10) << extDataSet.vctRows[i].getName() << ": ";
        //            for (j = 0; j < extDataSet.vctCols.size(); j++) {
        //                //                mapColNumber[]
        //                rowStream << std::setw(10) << extDataSet.vctCols[j].getRowElement(i) << ", ";
        //            }
        //            rowStream << std::setw(10) << extDataSet.vctRows[i].getRhs() << ", ";
        //            DEBUG_FILE(rowStream.str());
        //        }
        //        DEBUG_FILE(" ------------------------- *** -----------------------------");


        //! Column-wise display of LP Tableau
        unsigned int width = MAX_KEY_LEN;
        std::stringstream titleStream;
        titleStream << std::setw(width) << "Col/Row" << ": ";
        for (j = 0; j < extDataSet.A.get_rows_count(); j++)
            titleStream << std::setw(width) << extDataSet.A.get_row_attr(j).get_row_name() << ", ";
        //        titleStream << std::setw(width) << extDataSet.getRhsName() << ", ";
        titleStream << std::setw(width) << extDataSet.get_objective_name() << ", ";
        DEBUG_FILE(titleStream.str());

        for (i = 0; i < extDataSet.A.get_columns_count(); i++) {
            // Adding cache
            PackedVector packed_vector(extDataSet.A.get_rows_count());
            extDataSet.A.store_column(i, packed_vector, false);
            DEBUG("Size of the packed Vector: " << packed_vector.get_nnz() << "; Total Size: " << packed_vector.get_real_size());
            MatrixAColAttr col_attr = extDataSet.A.get_col_attr(i);
            std::stringstream colStream;
            colStream << std::setw(width) << col_attr.get_col_name() << ": ";
            PackedVector::iterator itr = packed_vector.begin();
            unsigned int prev_row_index = 0;
            while (itr != packed_vector.end()) {
                PackedElement packed_element = *itr;
                // Check if the PackedElements are ordered based on index
                assert(((int) packed_element.get_index() - (int) prev_row_index) >= 0);
                // Pad zeros between the current and previous index
                while (prev_row_index < packed_element.get_index()) {
                    colStream << std::setw(width) << 0.0F << ", ";
                    prev_row_index++;
                }
                // Now log the packed element
                colStream << std::setw(width) << packed_element.get_value() << ", ";
                prev_row_index++;
                itr++;
            }
            // Padding NULL values until the end of col vector
            //            DEBUG("Previous Index: "<<prevRowIndex<<", Size of Vector " << i<<" is: "<<extCurrentVector.real_size()<<", Packed="<<extCurrentVector.isPacked());
            while (prev_row_index < packed_vector.get_real_size()) {
                colStream << std::setw(width) << 0.0F << ", ";
                prev_row_index++;
            }
            colStream << std::setw(width) << col_attr.get_objective_value() << ", ";
            DEBUG_FILE(colStream.str());
        }

        std::stringstream rhsStream;
        std::string rhsTitle("");
        rhsTitle = rhsTitle + extDataSet.get_rhs_name();
        rhsStream << std::setw(width) << rhsTitle << ": ";
        for (i = 0; i < extDataSet.A.get_rows_count(); i++) {
            rhsStream << std::setw(width) << extDataSet.A.get_row_attr(i).get_rhs_value() << ", ";
        }
        /// Obj Row in Rhs Column
        rhsStream << std::setw(width) << 0 << ", ";
        DEBUG_FILE(rhsStream.str());
        //! ---------------- Sparsity Analysis
        float overall_vector_sparsity = 0;
        unsigned int overall_nnz = 0;
        unsigned int overall_size = (extDataSet.A.get_columns_count())*(extDataSet.A.get_columns_count());
        DEBUG_FILE_WITH_TIMESTAMP(" *** Sparsity Analysis *** ");
        for (i = 0; i < extDataSet.A.get_columns_count(); i++) {
            float vector_sparsity = extDataSet.A.get_col_sparsity(i);
            DEBUG_FILE("Vector[" << i << "]: " << vector_sparsity);
            overall_vector_sparsity += vector_sparsity;
            overall_nnz += extDataSet.A.get_col_nnz(i);
        }
        overall_vector_sparsity /= extDataSet.A.get_columns_count();
        float overall_tableau_sparsity = (float) (overall_size - overall_nnz)*100.0 / overall_size;

        DEBUG_FILE("Avg sparsity of a vector in LP Tableau: " << overall_vector_sparsity);
        DEBUG_FILE("Overall sparsity of the LP Tableau: " << overall_tableau_sparsity);


        DEBUG_FILE_WITH_TIMESTAMP(" ------------------------- *** -----------------------------");
        /////////////////////////////////////////////////////////////////////////
    }
};


#endif	/* TEST_H */

