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
#include "PackedVector2.h"
#include "ExtVector.h"
#include "ExtVector2.h"
#include "ExtPackedVector.h"
class Test {
public:
    static void testLinkedList() {
        bool testIterator = false;
        bool testStorage = true;
        bool testSimpleStoragePointer = false;
        bool testStorageWithPointer = false;
        CONSOLE_PRINTLN("************************ Tesiting LinkedList ************************");
        if(testStorage){
            LinkedList<REAL> linkedList;
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before LinkedList - Initialization of "<<testSize<<" elements");
            for(unsigned int i=0; i<testSize; i++)
                linkedList.add((REAL)i);
            DEBUG("Added "<<linkedList.size()<<" elements in total");
            DEBUG_MEMORY("After LinkedList - Initialization of "<<testSize<<" elements");
            DEBUG_MEMORY("Before LinkedList - Clear of "<<testSize<<" elements");
            linkedList.clear();
            DEBUG_MEMORY("After LinkedList - Clear of "<<testSize<<" elements");
            
            LinkedList<REAL> linkedList2;
            DEBUG_MEMORY("Before LinkedList - Initialization of "<<testSize<<" elements");
            for(unsigned int i=0; i<testSize; i++)
                linkedList2.add((REAL)i);
            DEBUG("Added "<<linkedList2.size()<<" elements in total");
            DEBUG_MEMORY("After LinkedList - Initialization of "<<testSize<<" elements");
            DEBUG_MEMORY("Before LinkedList - Clear of "<<testSize<<" elements");
            linkedList2.clear();
            DEBUG_MEMORY("After LinkedList - Clear of "<<testSize<<" elements");
            
            LinkedList<REAL> linkedList3;
            DEBUG_MEMORY("Before LinkedList - Initialization of "<<testSize<<" elements");
            for(unsigned int i=0; i<testSize; i++)
                linkedList3.add((REAL)i);
            DEBUG("Added "<<linkedList3.size()<<" elements in total");
            DEBUG_MEMORY("After LinkedList - Initialization of "<<testSize<<" elements");
            DEBUG_MEMORY("Before LinkedList - Clear of "<<testSize<<" elements");
            linkedList3.clear();
            DEBUG_MEMORY("After LinkedList - Clear of "<<testSize<<" elements");
            
            LinkedList<REAL> linkedList4;
            DEBUG_MEMORY("Before LinkedList - Initialization of "<<testSize<<" elements");
            for(unsigned int i=0; i<testSize; i++)
                linkedList4.add((REAL)i);
            DEBUG("Added "<<linkedList4.size()<<" elements in total");
            DEBUG_MEMORY("After LinkedList - Initialization of "<<testSize<<" elements");
            DEBUG_MEMORY("Before LinkedList - Clear of "<<testSize<<" elements");
            linkedList4.clear();
            DEBUG_MEMORY("After LinkedList - Clear of "<<testSize<<" elements");
        }
        if(testStorageWithPointer){
            LinkedList<REAL> *linkedList = new LinkedList<REAL>();
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before LinkedList - Initialization of "<<testSize<<" elements");
            for(unsigned int i=0; i<testSize; i++)
                linkedList->add((REAL)i);
            DEBUG_MEMORY("After LinkedList - Initialization of "<<testSize<<" elements");
            DEBUG_MEMORY("Before LinkedList - Clear of "<<testSize<<" elements");
            linkedList->clear();
            DEBUG_MEMORY("After LinkedList - Clear of "<<testSize<<" elements");
            DEBUG_MEMORY("Before LinkedList - Delete of "<<testSize<<" elements");
            delete linkedList;
            DEBUG_MEMORY("After LinkedList - Delete of "<<testSize<<" elements");
        }
        if(testSimpleStoragePointer){
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before Allocating "<<testSize<<" elements");
            try{
            REAL *p = new REAL[testSize];
            for(unsigned int i=0; i<testSize; i++){
                p[i] = i;
            }
            DEBUG_MEMORY("After Allocating "<<testSize<<" elements");
            DEBUG_MEMORY("Before Deleting "<<testSize<<" elements");
            delete []p;
            DEBUG_MEMORY("After Deleting "<<testSize<<" elements");
            }catch(std::bad_alloc &ba){
                DEBUG_ERROR("Bad ALLOC: "<<ba.what());
            }
        }
        if(testIterator){
            LinkedList<REAL> linkedList;
            unsigned int testSize = 100000000;
            for(unsigned int i=0; i<testSize; i++)
                linkedList.add((REAL)i);
            LinkedList<REAL>::iterator itr = linkedList.begin();
            while(itr!=linkedList.end()){
                double val = *itr;
                DEBUG("Value is: "<<val);
                itr++;
            }
        }
    }
    
    //! Testing Packed Vector
    static void testPackedVector2() {
        bool testConversion = true;
        CONSOLE_PRINTLN("************************ Tesiting PackedVector2 ************************");
        if(testConversion){
            CONSOLE_PRINTLN("--- Tesiting PackedVector2 Conversion ---");
            REAL arrTest[] = {1.0, 2.0, 5.0, 6.0, 0.0, 0.0, 0.0, 0.0, 7.1, 0.0, 0.0, 8.2, 9, 9, 9, 9, 9, 9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
            PackedVector2<REAL> vctPack(sizeof (arrTest) / sizeof (REAL), 0);
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
    static void test(){
        // Section for deciding which testcases need to be checked
        CONSOLE_PRINTLN("************************ Tesiting Title ************************");
        // Section for Testing all different testcases
        CONSOLE_PRINTLN("************************ End ************************");


    }
    //! Testing ExtVector
    static void testExtVector() {
        bool testConversion=false;
        bool testStorage=true;
        CONSOLE_PRINTLN("************************ Tesiting ExtVector ************************");
        if(testStorage){
            CONSOLE_PRINTLN("--- Tesiting ExtVector Storage ---");
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before PackedVector - Initialization of "<<testSize<<" elements");
            PackedVector vctPack(testSize);
//            PackedVector2<REAL> vctPack(testSize, 0);
            unsigned int i;
            for (i = 0; i < testSize; i++) { // Adding 1 mil numbers
                vctPack.add(i, i);
            }   
            DEBUG_MEMORY("After PackedVector - Initialization of "<<testSize<<" elements");
            DEBUG_MEMORY("Before ExtVector - Initialization of "<<testSize<<" elements");
            ExtVector extVector(vctPack);
            DEBUG_MEMORY("After ExtVector - Initialization of "<<testSize<<" elements");


        }
        if(testConversion){
            
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
    static void testExtVector2(){
        bool testConversion=false;
        bool testStorage=true;
        CONSOLE_PRINTLN("************************ Tesiting ExtVector ************************");
        if(testStorage){
            CONSOLE_PRINTLN("--- Tesiting ExtVector Storage ---");
            unsigned int testSize = 100000000;
            DEBUG_MEMORY("Before ExtVector2 - Initialization of "<<testSize<<" elements");
            ExtVector2<REAL> extVector;
            for(unsigned int i=0; i<testSize; i++){
                extVector.push_back(i);
            }
            DEBUG_MEMORY("After ExtVector2 - Initialization of "<<testSize<<" elements");
            DEBUG_MEMORY("Before ExtVector2 - Deallocate Pagecache "<<testSize<<" elements");       
            extVector.deallocate_page_cache();
            DEBUG_MEMORY("After ExtVector2 - Deallocate Pagecache of "<<testSize<<" elements");
        }
        CONSOLE_PRINTLN("************************ End ************************");
    }
    static void testExtPackedVector(){
        bool testConversion=false;
        bool testStorage=true;
        CONSOLE_PRINTLN("************************ Tesiting ExtPackedVector ************************");
        if(testStorage){
            CONSOLE_PRINTLN("--- Tesiting ExtPackedVector Storage ---");
            unsigned int testSize = 100;
            DEBUG_MEMORY("Before PackedVector2 - Initialization of "<<testSize<<" elements");
            PackedVector2<REAL> vctPacked(testSize, 0);
            for(unsigned int i=0; i<testSize; i++){
                vctPacked.add(i, (REAL)i);
            }
            DEBUG_MEMORY("Before ExtVector2 - Initialization of "<<testSize<<" elements");
            ExtPackedVector<REAL> extPacked(vctPacked);
            ExtPackedVector<REAL>::iterator itr = extPacked.begin();
            while(itr!=extPacked.end()){
                PackedElement2<REAL> packedElement = *itr;
                DEBUG("Index: "<<packedElement.getIndex()<<", Value: "<<packedElement.getValue());
                itr++;
            }
            DEBUG_MEMORY("After ExtVector2 - Initialization of "<<testSize<<" elements");
        }
        CONSOLE_PRINTLN("************************ End ************************");
    }
};


#endif	/* TEST_H */

