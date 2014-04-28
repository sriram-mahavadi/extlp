/* 
 * File:   ExtPackedVector.h
 * Author: harsha
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
template <class ItemClass>
class ExtPackedVector {
private:
    //! Vector to store the Items onto external memory
    typedef typename VECTOR_GENERATOR< PackedElement<ItemClass> , 4, 2, 2 * 1024 * 1024>::result item_vector;
    item_vector m_vct_disk;
    //! Total number of non zeros in the vector
    unsigned int m_nnz;
    //! Total number of elements without compaction/compression
    unsigned int m_size;
    //! Null type for the specific ItemClass
    ItemClass m_nullItem;
    

    //! Identifier Variable to specify if vector is packed 
    // bool m_isPacked; (Not necessary cause ExtPackedVector is packed by default)

public:
    //! Defining iterator for the purpose of iteration
    friend class item_vector::iterator;
    typedef typename item_vector::iterator iterator;
    
    //! Initialization from the PackedVector
    ExtPackedVector(PackedVector<ItemClass>& packedVector) {
        storeFromPackedVector(packedVector);
    }
    //! Publishing Packed Vector into External memory
    void storeFromPackedVector(PackedVector<ItemClass>& p_packed_vector) {
        assert(p_packed_vector.isPacked());
        resize(p_packed_vector.size(), true);
        m_nullItem = p_packed_vector.getNullItem();
        m_nnz = 0;
        typename PackedVector<ItemClass>::iterator itr = p_packed_vector.begin();
        unsigned int i = 0;
        while (itr != p_packed_vector.end()) {
            PackedElement<ItemClass> packed_element = *itr;
            add(i, packed_element);
            itr++;
            i++;
        }
    }
    //! Adding an PackedElement into the PackedVector
    void add(unsigned int p_index, PackedElement<ItemClass> &p_packed_element) {
        m_vct_disk[p_index] = p_packed_element;
    }
    //! Adding an element into the PackedVector
    void add(unsigned int p_index, unsigned int p_absoulute_index, ItemClass p_value) {
        PackedElement<ItemClass> packed_element;
        packed_element.setIndex(p_absoulute_index);
        packed_element.setValue(p_value);
        m_vct_disk[p_index] = packed_element;
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
    ItemClass getAbsoluteItemValue(unsigned int index) {
        iterator itr = begin();
        while (itr != end()) {
            PackedElement<ItemClass> &packedElement = *itr;
            if (index == packedElement.getIndex()) {
                return packedElement.getValue();
            }
            itr++;
        }
        return m_nullItem;
    }
    //! Returns iterator to the beginning(first) element in the vector
    iterator begin() {
        return m_vct_disk.begin();
    }
    //! Returns iterator to the ending(last) of the vector
    iterator end() {
        return m_vct_disk.end();
    }
};


#endif	/* EXTPACKEDVECTOR_H */

