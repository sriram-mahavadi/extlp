/* 
 * File:   PackedVector.h
 * Author: sriram
 *
 * Created on 5 April, 2014, 9:54 AM
 */

#ifndef PACKEDVECTOR_H
#define	PACKEDVECTOR_H

#include "GlobalDefines.h"

/// Struct needs to be accessed outside by the get() method
/// Basic <index, value> pair to represent array/vector in packed form
struct PackedElement {
public:
    int index;
    REAL value;
};

/// Checks if the vector has sparsity>0.5 and stores in packed form if it does
/// otherwise it will store the complete vector
/// This is the best form to store on disk to reduce the number of I/O(compressed storage)
class PackedVector {
private:
    int nnz; /// Number of non-zeros, define the packed or unpacked form of vector
    /// It is the compressed size in Packed form
    int realSize; /// Max number of elements in unpacked form
    std::vector<REAL> vctUnpacked;
    std::vector<PackedElement> vctPacked;

public:
    enum VectorStatus {
        UNINITIALIZED, /// Vector status unknown and not yet initialized
        PACKED, /// Vector is in compressed form
        UNPACKED /// Vector is full-length vector
    };

    VectorStatus status;
    /// Initialization section
    PackedVector() {
        nnz = 0;
        realSize = 0;
        status = UNINITIALIZED;
    }
    PackedVector(std::vector<REAL> &vct) {
        /// Calculating the number of non-zeros
        /// Adding the elements into Packed form by default
        unsigned int i;
        status = PACKED;
        for (i = 0; i < vct.size(); i++) {
            if (vct[i] != 0.0) {
                PackedElement element;
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
        return (float) nnz / realSize;
    }
    int size() {
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
                PackedElement element;
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
        for (i = 0; i < vctPacked.size(); i++) {
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
    void add(int index, REAL value) {
        if (status == UNINITIALIZED) {
            status = PACKED;
            PackedElement element;
            element.index = index;
            element.value = value;
            vctPacked.push_back(element);
        } else if (status == PACKED) {
            PackedElement element;
            element.index = index;
            element.value = value;
            vctPacked.push_back(element);
        }
    }

    /// Gets the packed or unpacked element present in the 
    /// absolute index position input within corresponding vector
    /// When element index is returned to be -1, there is some error
    /// Which is either that vector is unintizlized, array out of bounds,
    /// or that vector status is unknown
    PackedElement get(int index) {
        PackedElement element;
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
    bool isPacked() {
        if (status == PACKED)return true;
        return false;
    }

    // Status should take either of the values in the Status enum
    void validateStatus() {
        assert(status == PACKED || status == UNINITIALIZED || status == UNPACKED);
    }

};


#endif	/* PACKEDVECTOR_H */

