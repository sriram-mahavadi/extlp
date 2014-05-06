/* 
 * File:   ExtVector2.h
 * Author: Sriram Mahavadi
 *
 * Created on 23 April, 2014, 7:33 PM
 */

#ifndef EXTVECTOR2_H
#define	EXTVECTOR2_H
#include "ExtStxxlVector.h"
#include "PackedVector.h"
//! 16MB Cache per vector
//! Always remember to allocate and deallocate cache pages as necessary
template<class ItemClass>
class ExtVector {
private:
    //! Vector to store the Items onto external memory
    typedef typename VECTOR_GENERATOR<ItemClass, 1, 1, VECTOR_BLOCK_SIZE>::result item_vector;
    item_vector m_vct_disk;
    //! Total number of non zeros in the vector
    unsigned int m_nnz;
    //! Total number of elements without compaction/compression
    unsigned int m_size;

public:
    //! Allowing iterator of item_vector for access 
    friend class item_vector::iterator;
    typedef typename item_vector::iterator iterator;

    //! Forced Initialization with default values
    //! Need to use it only when necessary
    ExtVector() : m_nnz(0), m_size(0) {
    }
    //! Simple initialization from the input packed
    ExtVector(PackedVector<ItemClass>& packedVector) {
        storeFromPackedVector(packedVector);
    }
    //! Storing from PackedVector
    void storeFromPackedVector(PackedVector<ItemClass>& packedVector) {
        m_vct_disk.resize(packedVector.size(), true);
        typename PackedVector<ItemClass>::iterator itr = packedVector.begin();
        while (itr != packedVector.end()) {
            PackedElement<ItemClass> packedElement = *itr;
            //            push_back(packedElement.getValue());
            m_vct_disk[packedElement.getIndex()] = packedElement.getValue();
            itr++;
        }
        deallocate_cache();
    }
    //! Adds the input element at the input index position
    void add(unsigned int index, ItemClass value) {
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
    ItemClass operator[](const unsigned int index) const {
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

