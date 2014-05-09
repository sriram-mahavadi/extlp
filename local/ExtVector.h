/* 
 * File:   ExtVector2.h
 * Author: Sriram Mahavadi
 *
 * Created on 23 April, 2014, 7:33 PM
 */

#ifndef EXTVECTOR2_H
#define	EXTVECTOR2_H
#include "PackedVector.h"
#include "ExtStxxlVector.h"
//#include "deprecated/ExtStxxlVector.h"
//! Cache is allocated on per vector basis
//! Always remember to allocate and deallocate cache pages as necessary
class ExtVector {
private:
    //! Vector to store the Items onto external memory
    //! template parameters<ValueType, PageSize, CachePages, BlockSize, AllocStratg>
    typedef typename VECTOR_GENERATOR<REAL, 1, 1, VECTOR_BLOCK_SIZE>::result item_vector;
    typedef typename item_vector::iterator iterator_type;
    item_vector m_vct_disk;
    //! Total number of non zeros in the vector
    unsigned int m_nnz;
    //! Total number of elements without compaction/compression
    unsigned int m_size;

public:
    //! Allowing iterator of item_vector for access 
    //    friend class iterator_type;
    //    vector_iterator<REAL, alloc_strategy_type, size_type,
    //                            difference_type, block_size, pager_type, page_size>
    typedef typename item_vector::iterator iterator;

    //! Forced Initialization with default values
    //! Need to use it only when necessary
    ExtVector() : m_nnz(0), m_size(0) {
    }
    //! Simple initialization from the input packed
    ExtVector(PackedVector& packedVector) {
        storeFromPackedVector(packedVector);
    }
    //! Storing from PackedVector
    void storeFromPackedVector(PackedVector& packedVector) {
        m_vct_disk.resize(packedVector.get_real_size(), true);
        PackedVector::iterator itr = packedVector.begin();
        while (itr != packedVector.end()) {
            PackedElement packedElement = *itr;
            //            push_back(packedElement.getValue());
            m_vct_disk[packedElement.get_index()] = packedElement.get_value();
            itr++;
        }
        deallocate_cache();
    }
    //! Adds the input element at the input index position
    void add(unsigned int index, REAL value) {
        assert(index < m_size);
        m_vct_disk[index] = value;
    }
    //! Resizes the vector, also has an option to shrink_necessary
    void resize(unsigned int p_size, bool p_allow_shrink = true) {
        m_vct_disk.resize(p_size, p_allow_shrink);
        m_size = p_size;
    }
    //! Return the number of non zeros
    unsigned int nnz() {
        return m_nnz;
    }
    //! Return the number actual size of the vector
    unsigned int size() {
        return m_size;
    }
    //! Simple access to the element at index position
    //! Always preferably use the iterator instead for better performance
    REAL operator[](const unsigned int index) const {
        return m_vct_disk[index];
    }
    //! Allocates the 16MB cache desired by an individual vector
    void allocate_cache() {
        m_vct_disk.allocate_page_cache();
    }
    //! Deallocates all the cache pertaining to this vector
    void deallocate_cache() {
        m_vct_disk.deallocate_page_cache();
    }
    //! Returns iterator to the beginning(first) element in the vector
    iterator begin() {
        return m_vct_disk.begin();
    }
    //! Returns iterator to the ending(last) of the vector
    iterator end() {
        return m_vct_disk.end();
    }
    //! Returns the sparsity of the vector
    float get_sparsity() {
        return ((float) (m_size - m_nnz) / m_size)*100;
    }
};


#endif	/* EXTVECTOR2_H */

