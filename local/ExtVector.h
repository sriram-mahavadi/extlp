/* 
 * File:   ExtVector.h
 * Author: sriram
 *
 * Created on 7 April, 2014, 12:59 AM
 */

#ifndef EXTVECTOR_H
#define	EXTVECTOR_H
#include "GlobalDefines.h"
#include "PackedVector.h"
/**
 * This data structure is efficient for the purpose of disk I/O
 * The vector is in packed form if the sparsity is <= 50%
 * and in full form if the sparsity > 50%
 * This inturn reduces total number of I/Os 
 */
class ExtVector {
private:
    /**
     * Temporarily using the existing Disk Vector from stxxl
     * Better performance might be achieved through our own 
     * Blocking structure. -- TO DO --     
     */
    real_vector vctDisk;
    int nnz;
    int realSize;
    bool isVctPacked; // TO DO - Shift the packed identifier bit to physical memory
public:
    ExtVector(PackedVector &vctPacked) {
        // 1 extra element to specify if the vector is packed or not
        vctDisk.resize(vctPacked.getUnPackedSize() + 1);
        nnz = vctPacked.getPackedSize();
        realSize = vctPacked.getUnPackedSize();
        if (vctPacked.getSparsity() < 0.5f) {
            vctPacked.convertToUnPacked();
            unsigned int i = 0;
            vctDisk[i] = 0; // Vector is not packed
            for (i = 0; i < vctPacked.size(); i++) {
                PackedElement element = vctPacked.get(i);
                vctDisk[i + 1] = element.value;
            }
        } else {
            vctPacked.convertToPacked();
            unsigned int i = 0;
            vctDisk[i] = 1; // Vector is packed
            for (i = 0; i < vctPacked.size(); i++) {
                PackedElement element = vctPacked.get(i);
                vctDisk[2 * i + 1] = element.index;
                vctDisk[2 * i + 2] = element.value;
            }
        }
    }
    int size() {
        // Removing 1 element size for the packed identifier
        return (isPacked()) ? nnz : vctDisk.size() - 1;
    }
    bool isPacked() {
        return (vctDisk[0] == 1);
    }

    /// Externally store disk based vector into memory
    void storePackedVector(PackedVector &vctPacked) {
        if (isPacked()) { // If vector in disk is packed
            unsigned int i;
            for (i = 0; i < nnz; i++) {
                int index = vctDisk[2 * i + 1];
                int value = vctDisk[2 * i + 2];
                vctPacked.add(index, value);
            }
        } else { // otherwise. if it is unpacked.
            unsigned int i;
            for (i = 0; i < realSize; i++) {
                vctPacked.add(i, vctDisk[i+1]);
            }
        }

    }
     /// Get the sparsity of the vector
    float getSparsity() {
        return (float) (realSize-nnz) / realSize;
    }
    
    ///
    void displayVector() {
        Console::println("********* ExtVector Display ************");
        if (isPacked()) {
            Console::println("Vector Status is: Packed");
            Console::println("Vector size is: ", size());
            Console::println("Sparsity is: ", getSparsity());
            unsigned int i;
            for (i = 0; i < nnz; i++) {
                std::stringstream packedStr;
                packedStr << "Index: " << vctDisk[2*i+1];
                packedStr << ", Value: " << vctDisk[2*i+2];
                Console::println(packedStr.str());
            }
        } else{
            Console::println("Status is: Unpacked");
            Console::println("Vector size is: ", size());
            Console::println("Sparsity is: ", getSparsity());
            unsigned int i;
            for (i = 0; i < realSize; i++) {
                std::stringstream packedStr;
                packedStr << "Index: " << i;
                packedStr << ", Value: " << vctDisk[i+1];
                Console::println(packedStr.str());
            }
        } 
        Console::println(" ----------- *** --------------");
    }
    /// Testing the functionality of the Vector
    static void test() {
        // Sparse case
        Console::println("*** tesiting ExtVector ***");
        REAL arrTest[] = {1.0, 2.0, 5.0, 6.0, 0.0, 0.0, 0.0, 0.0, 7.1, 0.0, 0.0, 8.2, 6};
        unsigned int testSize = sizeof (arrTest) / sizeof (REAL);
        PackedVector vctPack(testSize);
        unsigned int i;
        for (i = 0; i < testSize; i++) {
            vctPack.add(i, arrTest[i]);
            //            vctPack.displayVector();
        }
        Console::println("Original Vector: ");
        vctPack.displayVector();
        // Saving vctPack1 to disk
        ExtVector extVector(vctPack);
        extVector.displayVector();
        PackedVector vctResult(testSize, extVector.isPacked());
        extVector.storePackedVector(vctResult);
        
        Console::println("External Stored Vector: ");
        vctResult.displayVector();
    }
};

#endif	/* EXTVECTOR_H */

