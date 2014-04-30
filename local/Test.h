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
class Test {
public:
    static void testExtNameMap() {
        fixed_name_map myFixedMap((fixed_name_map::node_block_type::raw_size)*5, (fixed_name_map::leaf_block_type::raw_size)*5);
        unsigned int testSize=1000000;
        ExtNameMap nameMap(myFixedMap);
        //        ExtNameMap strMap(myMap);
        for (unsigned int i = 1; i < testSize; i++) { /// Inserting 1 million strings
            std::stringstream strStream;
            strStream << (i);
            FixedString fixedString = FixedStringUtil::getFixedString(strStream.str());
            //            CONSOLE_PRINTLN("Inserting: " + strStream.str());
            //            strMap.set(fixedString, i);
            if (i % (testSize/10) == 0) {
                CONSOLE_PRINTLN("Number of Keys Written: " << i);
            }
            nameMap.set(strStream.str(), i);
//            myFixedMap[fixedString] = i;
        }
        DEBUG_MEMORY("Completed Writing "<<testSize<<" keys.");
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
            PackedVector<REAL> vctPack(sizeof (arrTest) / sizeof (REAL), 0);
            //        vctPack.setRealSize(sizeof(arrTest)/sizeof(REAL));
            vctPack.displayVector();
            unsigned int i;
            for (i = 0; i<sizeof (arrTest) / sizeof (REAL); i++) {
                vctPack.add(i, arrTest[i]);
                vctPack.displayVector();
            }
            // Converting Mode Forcedly
            vctPack.convertToPacked();
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
            ExtVector<REAL> extVector;
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
            std::vector<ExtVector<REAL>*> vctExtVector;
            for (unsigned int i = 0; i < testArraySize; i++) {
                ExtVector<REAL> *extVector = new ExtVector<REAL>();
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
            PackedVector<REAL> vctPacked(testSize, true, 0);
            for (unsigned int i = 0; i < testSize; i++) {
                // Always add with autoConversion false if you donot want to convert
                // This is typical while incase of forced initialization
                vctPacked.add(i, (REAL) i, false);
            }
            DEBUG_MEMORY("Before ExtVector2 - Initialization of " << testSize << " elements");
            ExtPackedVector<REAL> extPacked(vctPacked);
            ExtPackedVector<REAL>::iterator itr = extPacked.begin();
            while (itr != extPacked.end()) {
                PackedElement<REAL> packedElement = *itr;
                DEBUG("Index: " << packedElement.getIndex() << ", Value: " << packedElement.getValue());
                itr++;
            }
            DEBUG_MEMORY("After ExtVector2 - Initialization of " << testSize << " elements");
        }
        CONSOLE_PRINTLN("************************ End ************************");
    }
};


#endif	/* TEST_H */

