/* 
 * File:   PackedVector.h
 * Author: sriram
 *
 * Created on 5 April, 2014, 9:54 AM
 */

#ifndef PACKEDVECTORPROTOTYPE_H
#define	PACKEDVECTORPROTOTYPE_H

#include "../GlobalDefines.h"


/// Struct needs to be accessed outside by the get() method
/// Basic <index, value> pair to represent array/vector in packed form
struct PackedElementPrototype {
public:
    unsigned int index;
    REAL value;
};

/// Checks if the vector has sparsity>0.5 and stores in packed form if it does
/// otherwise it will store the complete vector
/// This is the best form to store on disk to reduce the number of I/O(compressed storage)

class PackedVectorPrototype {
private:
    unsigned int nnz; /// Number of non-zeros, define the packed or unpacked form of vector
    /// It is the compressed size in Packed form
    unsigned int realSize; /// Max number of elements in unpacked form
    std::vector<REAL> vctUnpacked;
    std::vector<PackedElementPrototype> vctPacked;

public:

    enum VectorStatus {
        UNINITIALIZED, /// Vector status unknown and not yet initialized
        PACKED, /// Vector is in compressed form
        UNPACKED /// Vector is full-length vector
    };

    VectorStatus status;
    /// Initialization section

    PackedVectorPrototype(int p_realSize = 0) {
        nnz = 0;
        realSize = p_realSize;
        status = UNINITIALIZED;
    }

    PackedVectorPrototype(int p_realSize, bool isPacked) {
        nnz = 0;
        realSize = p_realSize;
        status = (isPacked) ? PACKED : UNPACKED;
        if (!isPacked)
            vctUnpacked.resize(p_realSize);
    }

    PackedVectorPrototype(std::vector<REAL> &vct) {
        /// Calculating the number of non-zeros
        /// Adding the elements into Packed form by default
        unsigned int i;
        status = PACKED;
        for (i = 0; i < vct.size(); i++) {
            if (vct[i] != 0.0) {
                PackedElementPrototype element;
                nnz++;
                element.index = i;
                element.value = vct[i];
                vctPacked.push_back(element);
            }
        }
        realSize = vct.size();
    }

    /// Get the total number of non zeros in the vector

    int getPackedSize() {
        validateStatus();
        if (status == UNINITIALIZED)
            return 0;
        return nnz;
    }

    /// Get the max capacity of the vector including the zeros

    int getUnPackedSize() {
        validateStatus();
        if (status == UNINITIALIZED)
            return 0;
        return realSize;
    }

    /// Get the sparsity of the vector

    float getSparsity() {
        validateStatus();
        if (status == UNINITIALIZED)
            return 0;
        return (float) (realSize - nnz) / realSize;
    }

    unsigned int size() const {
        if (status == PACKED) {
            return nnz;
        } else if (status == UNPACKED) {
            return realSize;
        } else if (status == UNINITIALIZED) {
            // Still Uninitialized
            return 0;
        } else {
            DEBUG_ERROR("Vector Status is Unknown");
            return -1;
        }
    }

    void convertToPacked() {
        if (status == PACKED || status == UNINITIALIZED)return;
        status = PACKED;
        unsigned int i;
        nnz = 0;
        vctPacked.clear();
        for (i = 0; i < vctUnpacked.size(); i++) {
            if (vctUnpacked[i] != 0.0) {
                PackedElementPrototype element;
                nnz++;
                element.index = i;
                element.value = vctUnpacked[i];
                vctPacked.push_back(element);
            }
        }
        vctUnpacked.clear();
    }

    void convertToUnPacked() {
        if (status == UNPACKED || status == UNINITIALIZED)return;
        status = UNPACKED;
        unsigned int i;
        vctUnpacked.clear();
        vctUnpacked.resize(realSize);
        for (i = 0; i < vctPacked.size(); i++) {
            if (vctPacked[i].value != 0.0) {
                int index = vctPacked[i].index;
                REAL value = vctPacked[i].value;
                vctUnpacked[index] = value;
            }
        }
        vctPacked.clear();
    }

    /// Store unpacked vector externally into vctUnpack

    void storeUnpackedVector(std::vector<REAL> &vctUnpack) {
        if (status == UNINITIALIZED)return;
        if (status == UNPACKED) {
            vctUnpack = vctUnpacked;
            return;
        }
        unsigned int i;
        vctUnpack.clear();
        vctUnpack.resize(realSize);
        for (i = 0; i < nnz; i++) {
            if (vctPacked[i].value != 0.0) {
                int index = vctPacked[i].index;
                REAL value = vctPacked[i].value;
                vctUnpack[index] = value;
            }
        }
    }

    /// Best for performing disk read
    /// Main intution is to reduce disk I/O

    void convertForDiskWrite() {
        if (nnz > realSize / 2) {
            convertToUnPacked();
        } else {
            convertToPacked();
        }
    }

    /// Adds into packed form without even checking,
    /// if the index already exists

    void add(unsigned int index, REAL value) {
        // Check for non zero condition
        if (value == 0.0)return;

        if (status == UNINITIALIZED) {
            status = PACKED;
            PackedElementPrototype element;
            element.index = index;
            element.value = value;
            vctPacked.push_back(element);
        } else if (status == PACKED) {
            PackedElementPrototype element;
            element.index = index;
            element.value = value;
            vctPacked.push_back(element);
        } else if (status == UNPACKED) {
            assert(index < size());
            vctUnpacked[index] = value;
        } else {
            DEBUG_ERROR("Vector Status is Unknown");
            return;
        }
        nnz++;
    }

    /// Gets the packed or unpacked element present in the 
    /// absolute index position input within corresponding vector
    /// When element index is returned to be -1, there is some error
    /// Which is either that vector is unintizlized, array out of bounds,
    /// or that vector status is unknown

    PackedElementPrototype get(unsigned int index) const {
        PackedElementPrototype element;
        if (status == UNINITIALIZED) {
            element.index = -1;
            DEBUG_WARNING("Vector is uninitialized!!!");
        } else if (status == PACKED) {
            if (index >= nnz) { // Array out of bounds
                element.index = -1;
            } else {
                element.index = vctPacked[index].index;
                element.value = vctPacked[index].value;
            }
        } else if (status == UNPACKED) {
            if (index >= realSize) { // Array out of bounds
                element.index = -1;
            } else {
                element.index = index;
                element.value = vctUnpacked[index];
            }
        } else {
            element.index = -1;
            DEBUG_ERROR("Vector Status is Unknown");
        }
        return element;
    }

    /// checks if the status of the vector is packed

    bool isPacked() const {
        if (status == PACKED)return true;
        return false;
    }

    /// checks if the status of the vector is initialized

    bool isInitialized() const {
        if (status != UNINITIALIZED)return true;
        return false;
    }

    ///

    void resize(unsigned int p_size) {
        if (status == PACKED) {
            vctPacked.resize(p_size);
            // Unknown real size in this case
            // Just size is increased. the other 
        } else if (status == UNPACKED) {
            vctUnpacked.resize(p_size);
            realSize = p_size;
        }
    }

    /// 

    void setRealSize(unsigned int p_realSize) {
        realSize = p_realSize;
        if (status == UNPACKED)
            vctUnpacked.reserve(p_realSize);
    }

    void clear() {
        if (isPacked()) {
            vctPacked.clear();
            nnz = 0;
        } else {
            vctUnpacked.clear();
            vctUnpacked.reserve(realSize);
            nnz = 0;
        }

    }

    ///

    void displayVector() {
        CONSOLE_PRINTLN("********* Vector Display ************");
        if (status == PACKED) {
            CONSOLE_PRINTLN("Vector Status is: Packed");
            CONSOLE_PRINTLN("Vector size is: "<< size());
            CONSOLE_PRINTLN("Sparsity is: "<< getSparsity());
            unsigned int i;
            for (i = 0; i < size(); i++) {
                std::stringstream packedStr;
                packedStr << "Index: " << vctPacked[i].index;
                packedStr << ", Value: " << vctPacked[i].value;
                CONSOLE_PRINTLN(packedStr.str());
            }
        } else if (status == UNPACKED) {
            CONSOLE_PRINTLN("Status is: Unpacked");
            CONSOLE_PRINTLN("Vector size is: "<< size());
            CONSOLE_PRINTLN("Sparsity is: "<< getSparsity());
            unsigned int i;
            for (i = 0; i < size(); i++) {
                std::stringstream packedStr;
                packedStr << "Index: " << i;
                packedStr << ", Value: " << vctUnpacked[i];
                CONSOLE_PRINTLN(packedStr.str());
            }
        } else if (status == UNINITIALIZED) {
            CONSOLE_PRINTLN("Status is: Uninitialized");
        }
        CONSOLE_PRINTLN(" ----------- *** --------------");
    }

    /// Status should take either of the values in the Status enum

    void validateStatus() {
        assert(status == PACKED || status == UNINITIALIZED || status == UNPACKED);
    }

    /// Testing the functionality of the Vector

    static void test() {
        CONSOLE_PRINTLN("*** Testing Packed Vector ***");
        REAL arrTest[] = {1.0, 2.0, 5.0, 6.0, 0.0, 0.0, 0.0, 0.0, 7.1, 0.0, 0.0, 8.2};
        PackedVectorPrototype vctPack(sizeof (arrTest) / sizeof (REAL));
        //        vctPack.setRealSize(sizeof(arrTest)/sizeof(REAL));
        vctPack.displayVector();
        unsigned int i;
        for (i = 0; i<sizeof (arrTest) / sizeof (REAL); i++) {
            vctPack.add(i, arrTest[i]);
            //            vctPack.displayVector();
        }
        vctPack.displayVector();
        // Converting Mode
        vctPack.convertToUnPacked();
        vctPack.displayVector();
    }
};


#endif	/* PACKEDVECTOR_H */

