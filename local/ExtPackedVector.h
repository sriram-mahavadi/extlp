/* 
 * File:   ExtPackedVector.h
 * Author: Sriram Mahavadi
 *
 * Created on 24 April, 2014, 3:54 PM
 */

#ifndef EXTPACKEDVECTOR_H
#define	EXTPACKEDVECTOR_H
#include "PackedVector.h"
#include "ExtStxxlVector.h"
//! This is used only for storing packed-form of the vector
//! PackedVector is recommended be stored into ExtPackedVector only 
//! when it is in packed-form
class ExtPackedVector {
private:
    //! Vector to store the Items onto external memory
    // template parameters<ValueType, PageSize, CachePages, BlockSize, AllocStratg>
    typedef VECTOR_GENERATOR< PackedElement, 1, 1, VECTOR_PACKED_BLOCK_SIZE>::result item_vector;
    item_vector m_vct_disk;
    //! Total number of non zeros in the vector
    unsigned int m_nnz;
    //! Total number of elements without compaction/compression
    unsigned int m_size;

public:
    //! Defining iterator for the purpose of iteration
    //    friend class item_vector::iterator;
    typedef typename item_vector::iterator iterator;

    //! Initialization from the PackedVector
    ExtPackedVector(PackedVector& packedVector) {
        storeFromPackedVector(packedVector);
    }
    //! Publishing Packed Vector into External memory
    void storeFromPackedVector(PackedVector& p_packed_vector) {
        resize(p_packed_vector.get_nnz(), true);
        m_nnz = 0;
        m_size = p_packed_vector.get_real_size();
        PackedVector::iterator itr = p_packed_vector.begin();
        unsigned int i = 0;
        while (itr != p_packed_vector.end()) {
            PackedElement packed_element = *itr;
            add(i, packed_element);
            itr++;
            i++;
        }
        deallocate_cache();
    }
    //! Adding an PackedElement into the PackedVector
    void add(unsigned int p_index, PackedElement &p_packed_element) {
        add(p_index, p_packed_element.get_index(), p_packed_element.get_value());
    }
    //! Adding an element into the PackedVector
    void add(unsigned int p_index, unsigned int p_absoulute_index, REAL p_value) {
        if (p_value != 0.0F) {
            PackedElement packed_element;
            packed_element.set_index(p_absoulute_index);
            packed_element.set_value(p_value);
            m_vct_disk[p_index] = packed_element;
            m_nnz++;
        }
    }
    //! Resizes the vector with an option to allow_shrink. 
    //! Shrinking is allowed by default.
    void resize(unsigned int p_size, bool p_allow_shrink = true) {
        m_vct_disk.resize(p_size, p_allow_shrink);
    }
    //! Return the number of non zeros
    unsigned int nnz() {
        return m_nnz;
    }
    //! Return the number actual size of the vector (in unpacked version)
    unsigned int size() {
        return m_size;
    }
    //! Returns the ItemValue pertaining to the specific absolute index
    //! Here the value from <index, value> pair is returned for a given index
    REAL get_absolute_item_value(unsigned int absolute_index) {
        iterator itr = begin();
        while (itr != end()) {
            PackedElement &packedElement = *itr;
            if (absolute_index == packedElement.get_index()) {
                return packedElement.get_value();
            }
            itr++;
        }
        return 0.0F;
    }
    //! Allocates the cache desired by an individual vector
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


#endif	/* EXTPACKEDVECTOR_H */

