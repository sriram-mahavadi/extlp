/* 
 * File:   PackedVector.h
 * Author: Sriram Mahavadi
 *
 * Created on 23 April, 2014, 1:51 AM
 */

#ifndef PACKEDVECTOR_H
#define	PACKEDVECTOR_H
#include <stdio.h>
#include <assert.h>
#include "LinkedList.h"
#include "GlobalDefines.h"

//! Struct needs to be accessed outside by the get() method
//! Basic <index, value> pair to represent array/vector in packed form
class PackedElement {
private:
    unsigned int index;
    REAL value;
public:
    PackedElement() {
    }
    PackedElement(unsigned int index, REAL value) {
        this->index = index;
        this->value = value;
    }
    void set_index(unsigned int index) {
        this->index = index;
    }
    void set_value(REAL value) {
        this->value = value;
    }
    unsigned int get_index() const {
        return this->index;
    }
    REAL get_value() const {
        return this->value;
    }
    bool operator==(const PackedElement& other) const {
        return (this->index == other.index && this->value == other.value);
    }
};

//! Container for storing the packed form of the vector
//! Simple Vector of PackedElements
class PackedVector {
private:
    //! Total Number of non zeros in the vector
    unsigned int m_nnz;
    //! Max size of the unpacked vector
    unsigned int m_real_size;
    //! Storage for packed compressed vector
    typedef LinkedList< PackedElement > ll_packedelement_type;
    ll_packedelement_type m_vct_packed;
public:
    //! LinkedList iterator for iterating through Packed Vector
    friend class LinkedList< PackedElement >::iterator;
    typedef typename LinkedList< PackedElement >::iterator iterator;

    //! Initialization section
    //! PackedVector need to know the real size of vector in advance
    //! PackedVector needs to know the element to be ignored(Null Item)
    void initializeDefaults(unsigned int p_realSize) {
        m_nnz = 0;
        m_real_size = p_realSize;
    }

    //! Copy Construction - to check Memory Leak
    //! Do not allow copy constructor. May lead to shallow copy of linked list
    //! This might lead to clearing linked list twice leading to error while freeing
    PackedVector(PackedVector& p_packed_vector) {
        DEBUG_WARNING("Got Called... Getting Shallow copy of Linked list");
    }

    //! Simple initialization
    PackedVector(unsigned int p_size) {
        m_real_size = p_size;
    }

    //! Complete initialization. 
    //! No more modifications to the existing input vector of elements
    PackedVector(std::vector<REAL> &vct) {
        initializeDefaults(vct.size());
        unsigned int i;
        for (i = 0; i < vct.size(); i++) {
            add(i, vct[i]);
        }
    }

    //! Destroying PackedVector
    ~PackedVector() {
        clear();
    }

    //! Get the total number of non zeros in the vector
    unsigned int get_nnz() {
        return m_nnz;
    }

    //! Get the max capacity of the vector including the zeros
    unsigned int get_real_size() {
        return m_real_size;
    }

    //! Get the sparsity of the vector
    float get_sparsity() {
        return (float) (m_real_size - m_nnz)*100.0 / m_real_size;
    }

    //! Complete Initialization.
    //! Override the existing vector
    void store_from_vector(std::vector<REAL> &vct) {
        clear();
        for (unsigned int i = 0; i < vct.size(); i++) {
            add(i, vct[i]);
        }
    }

    //! Adds the simple <index, value> pair into the vector
    //! Also allows duplication check if given index already exists. 
    //! Does not allow duplication by default
    //! Ignores add of element if index already exists and index_duplication_check is true
    void add(unsigned int index, REAL value, bool p_index_duplication_check = true) {
        // Check for non zero condition
        if (value == 0.0F)return;
        // Simply return if index is already present
        if (p_index_duplication_check) {
            iterator itr = m_vct_packed.begin();
            while (itr != m_vct_packed.end()) {
                if (index == (*itr).get_index())
                    return;
                itr++;
            }
        }
        PackedElement element;
        element.set_index(index);
        element.set_value(value);
        m_nnz++;
        m_vct_packed.add(element);
    }

    // Not a Efficient operation to randomly access item 
    // from Packed Vector. Only need to be done if very much necessary.
    REAL get(unsigned int index) const {
        iterator itr = m_vct_packed.begin();
        while (itr != m_vct_packed.end()) {
            if (index == (*itr).get_index())
                return (*itr).get_value();
            itr++;
        }
        return 0.0F;
    }

    // Not a Efficient operation to randomly access item 
    // from Packed Vector. Only need to be done if very much necessary.
    REAL operator[](unsigned int index) const {
        return get(index);
    }

    //! Improve the size of the exising vector. Expand the vector.
    //! The size need to be extended only. the vector cannot shrink.
    //! TODO - Check the case in which we might mistakenly shrink the vector
    void resize(unsigned int p_size) {
        m_real_size = p_size;
    }

    //! Clear all the elements in the packed vector
    //! Initialize to defaults
    void clear() {
        m_vct_packed.clear();
        initializeDefaults(m_real_size);
    }

    //! Returns iterator to the beginning(first) element in the vector
    iterator begin() {
        return m_vct_packed.begin();
    }

    //! Returns iterator to the ending(last) of the vector
    iterator end() {
        return m_vct_packed.end();
    }

    //! Simple display of vector elements
    void displayVector() {
        CONSOLE_PRINTLN("********* Vector Display ************");
        CONSOLE_PRINTLN("Vector Status is: Packed");
        CONSOLE_PRINTLN("Vector Real size is: " << get_real_size());
        CONSOLE_PRINTLN("Vector No of Non-Zeros is: " << get_nnz());
        CONSOLE_PRINTLN("Sparsity is: " << get_sparsity());
        iterator itr = m_vct_packed.begin();
        while (itr != m_vct_packed.end()) {
            std::stringstream packedStr;
            packedStr << "Index: " << (*itr).get_index();
            packedStr << ", Value: " << (*itr).get_value();
            CONSOLE_PRINTLN(packedStr.str());
            itr++;
        }
        CONSOLE_PRINTLN(" ----------- *** --------------");
    }
};
#endif	/* PACKEDVECTOR2_H */

