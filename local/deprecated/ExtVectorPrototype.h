/* 
 * File:   ExtVector.h
 * Author: sriram
 *
 * Created on 7 April, 2014, 12:59 AM
 */

#ifndef EXTVECTOR_H
#define	EXTVECTOR_H
#include "../GlobalDefines.h"
#include "PackedVectorPrototype.h"
/**
 * This data structure is efficient for the purpose of disk I/O
 * The vector is in packed form if the sparsity is <= 50%
 * and in full form if the sparsity > 50%
 * This inturn reduces total number of I/Os 
 */
class ExtVectorPrototype {
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
    ExtVectorPrototype(int p_realSize = 0) {
        nnz = 0;
        realSize = p_realSize;
    }
    /// Always use this constructor for storing packed vector 
    /// into the disk
    ExtVectorPrototype(PackedVectorPrototype &vctPacked) : vctDisk(vctPacked.getUnPackedSize() + 1) {
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
                PackedElementPrototype element = vctPacked.get(i);
                vctDisk[i + 1] = element.value;
            }
        } else {
            vctPacked.convertToPacked();
            unsigned int i = 0;
            vctDisk[i] = 1; // Vector is packed
            for (i = 0; i < vctPacked.size(); i++) {
                PackedElementPrototype element = vctPacked.get(i);
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
    unsigned int getDiskStorageCapacity() {
        return vctDisk.raw_capacity();
    }
    bool isPacked() const {
        return (vctDisk[0] == 1);
    }

    /// Externally store disk based vector into memory
    void storePackPackedVector(PackedVectorPrototype &vctPacked) {
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
        CONSOLE_PRINTLN("********* ExtVector Display ************");
        if (isPacked()) {
            CONSOLE_PRINTLN("Vector Status is: Packed");
            CONSOLE_PRINTLN("Vector size is: " << size());
            CONSOLE_PRINTLN("Sparsity is: " << getSparsity());
            unsigned int i;
            for (i = 0; i < nnz; i++) {
                std::stringstream packedStr;
                packedStr << "Index: " << vctDisk[2 * i + 1];
                packedStr << ", Value: " << vctDisk[2 * i + 2];
                CONSOLE_PRINTLN(packedStr.str());
            }
        } else {
            CONSOLE_PRINTLN("Status is: Unpacked");
            CONSOLE_PRINTLN("Vector size is: " << size());
            CONSOLE_PRINTLN("Sparsity is: " << getSparsity());
            unsigned int i;
            for (i = 0; i < realSize; i++) {
                std::stringstream packedStr;
                packedStr << "Index: " << i;
                packedStr << ", Value: " << vctDisk[i + 1];
                CONSOLE_PRINTLN(packedStr.str());
            }
        }
        CONSOLE_PRINTLN(" ----------- *** --------------");
    }
    void displayStorageStatus() {
    }

};

#endif	/* EXTVECTOR_H */

