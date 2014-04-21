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
    unsigned int nnz;
    unsigned int realSize;
    bool isVctPacked; // TO DO - Shift the packed identifier bit to physical memory
public:
    /// Donot explicitly call this constructor
    /// unless very much necessary
    ExtVector(int p_realSize = 0) {
        nnz = 0;
        realSize = p_realSize;
    }
    /// Always use this constructor for storing packed vector 
    /// into the disk
    ExtVector(PackedVector &vctPacked) : vctDisk(vctPacked.getUnPackedSize() + 1) {
        // 1 extra element to specify if the vector is packed or not
        //        vctDisk.reserve(vctPacked.getUnPackedSize() + 1);
        //        DEBUG("Vector Unpacked Size: "<<vctPacked.getUnPackedSize() + 1);
        //        DEBUG("ExtVector Size: "<<vctDisk.capacity());
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
                vctDisk[2 * i + 1] = (REAL) element.index;
                vctDisk[2 * i + 2] = element.value;
            }
        }
    }
    REAL getAbsoluteIndexElement(unsigned int absoluteIndex) {
        unsigned int i = 0;
        //        DEBUG("NNZ: "<<nnz<<", Size: "<<vctDisk.size()<<", Index: "<<absoluteIndex<<", Real Size: "<<realSize);
        bool isSafe = (isPacked() && (2 * (nnz - 1) + 2) < vctDisk.size());
        isSafe = isSafe || (!isPacked() && (absoluteIndex + 1) < vctDisk.size());
        assert(isSafe);
        if (isPacked()) {
            //            DEBUG("getAbsoluteIndexElement(): Packed ExtVector");
            for (i = 0; i < nnz; i++) {
                unsigned int index = vctDisk[2 * i + 1];
                REAL value = vctDisk[2 * i + 2];
                if (index == absoluteIndex)
                    return value;
            }
            return 0.0F;
        } else {
            //            DEBUG("getAbsoluteIndexElement(): UnPacked ExtVector");
            return vctDisk[absoluteIndex + 1];
        }
    }
    int size() const {
        // Removing 1 element size for the packed 
        return (isPacked()) ? nnz : vctDisk.size() - 1;
    }
    unsigned int getDiskStorageCapacity(){
        return vctDisk.raw_capacity();
    }
    bool isPacked() const {
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
                vctPacked.add(i, vctDisk[i + 1]);
            }
        }

    }
    /// Get the sparsity of the vector
    float getSparsity() {
        return (float) (realSize - nnz) / realSize;
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
                packedStr << "Index: " << vctDisk[2 * i + 1];
                packedStr << ", Value: " << vctDisk[2 * i + 2];
                Console::println(packedStr.str());
            }
        } else {
            Console::println("Status is: Unpacked");
            Console::println("Vector size is: ", size());
            Console::println("Sparsity is: ", getSparsity());
            unsigned int i;
            for (i = 0; i < realSize; i++) {
                std::stringstream packedStr;
                packedStr << "Index: " << i;
                packedStr << ", Value: " << vctDisk[i + 1];
                Console::println(packedStr.str());
            }
        }
        Console::println(" ----------- *** --------------");
    }
    void displayStorageStatus() {
    }
    /// Testing the functionality of the Vector
    static void test() {
        // Sparse case
        Console::println("*** tesiting ExtVector ***");
        REAL arrTest[] = {1.0, 2.0, 5.0, 6.0, 0.0, 0.0, 0.0, 0.0, 7.1, 0.0, 0.0, 8.2, 6};
        unsigned int testSize = sizeof (arrTest) / sizeof (REAL);
        PackedVector vctPack(testSize);
        vctPack.setRealSize(100000000);
        unsigned int i;
        for (i = 0; i < testSize; i++) {
            vctPack.add(i, (REAL) arrTest[i]);
            //            vctPack.displayVector();
        }
        for (i = testSize; i < 100000000; i++) { // Adding 1 mil numbers
            vctPack.add(i, i);
        }
        // Takes up space of 8*2*(10^7) => 160MB sofar :)
        Console::println("Original Vector: ");
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
        std::vector<ExtVector*> vctExtStorage;
        /// Mutilple ExtVector Test
        int noTrials = 1000; /// Approx 8GB
        for(int j=0; j<noTrials; j++){
            ExtVector* tempVector = new ExtVector(vctPack);
            // 230-160 => 70MB for unpacking in this case
            vctExtStorage.push_back(tempVector);
            Console::println("Number of Vectors Added: ", j);
        }
        // Each ExtVector when getting typecasted could increase the size
        // temporarily for now as PackedVector can unpack/pack and increase size
        // Each vector has 32KB cache hence total of 32MB cache is expected
        Console::println("External Stored Vector Recieved Back: ");
        //        vctResult.displayVector();
    }
};

#endif	/* EXTVECTOR_H */

