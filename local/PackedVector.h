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
#include "stxxl/vector"
#include "SimpleVector.h"

//! Basic <index, value> pair to represent array/vector in packed form.
//! Struct needs to be accessed outside by the get() and set() methods only.
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
    //! For sorting based on index.
    //! Used in LinkedList for adding in sorting order.
    bool operator<(const PackedElement& other) const {
        return (this->index < other.index);
    }
    //! Simple string with index and value
    std::string debug_contents(){
        std::stringstream packed_element_stream;
        packed_element_stream<<"[ Index: "<<index<<"; Value: "<<value<<" ]";
        return packed_element_stream.str();
    }
};

//! Container for storing the packed form of the vector.
//! Simple Vector of PackedElements.
class PackedVector {
private:
    //! Total Number of non zeros in the vector.
    unsigned int m_nnz;
    //! Max size of the unpacked vector.
    unsigned int m_real_size;
    //! Storage for packed compressed vector.
    typedef LinkedList< PackedElement > ll_packedelement_type;
    ll_packedelement_type m_vct_packed;
    //! Boolean Identifier to specify if the vector is sorted by index.
    bool m_is_sorted_by_index;
public:
    //! LinkedList iterator for iterating through Packed Vector.
    friend class LinkedList< PackedElement >::iterator;
    typedef typename LinkedList< PackedElement >::iterator iterator;

    //! Initialization section.
    //! PackedVector need to know the real size of vector in advance.
    void initializeDefaults(unsigned int p_realSize) {
        m_nnz = 0;
        m_real_size = p_realSize;
        m_is_sorted_by_index = true;
    }

    //! Copy Construction - to check Memory Leak.
    //! Do not allow copy constructor. May lead to shallow copy of linked list.
    //! This might lead to clearing linked list twice leading to error while freeing.
    PackedVector(PackedVector& p_packed_vector) : m_nnz(p_packed_vector.m_nnz),
    m_real_size(p_packed_vector.m_real_size), m_vct_packed(p_packed_vector.m_vct_packed),
    m_is_sorted_by_index(p_packed_vector.m_is_sorted_by_index) {
        DEBUG_WARNING("Copying Packed Vector.");
    }

    //! Simple initialization
    //! Sorted PackedVector by default
    PackedVector(unsigned int p_size, bool p_is_sorted_by_index = false) :
    m_real_size(p_size), m_is_sorted_by_index(p_is_sorted_by_index) {
        m_nnz = 0;
    }

    //! Complete initialization from std::vector.
    //! No more modifications to the existing input vector of elements.
    PackedVector(std::vector<REAL> &vct) {
        initializeDefaults(vct.size());
        unsigned int i;
        for (i = 0; i < vct.size(); i++) {
            add(i, vct[i], false);
        }
    }

    //! Complete initialization from Simple Vector.
    PackedVector(SimpleVector<REAL> &vct) {
        initializeDefaults(vct.get_size());
        unsigned int i;
        for (i = 0; i < vct.get_size(); i++) {
            add(i, vct[i], false);
        }
    }

    //! Destroying PackedVector.
    ~PackedVector() {
        clear();
    }

    //! Get the total number of non zeros in the vector.
    unsigned int get_nnz() {
        return m_nnz;
    }

    //! Get the max capacity of the vector including the zeros.
    unsigned int get_real_size() {
        return m_real_size;
    }

    //! Get the sparsity of the vector.
    float get_sparsity() {
        return (float) (m_real_size - m_nnz)*100.0 / m_real_size;
    }

    //! Complete Initialization.
    //! Override the existing vector
    void store_from_vector(std::vector<REAL> &vct) {
        clear();
        initializeDefaults(vct.size());
        for (unsigned int i = 0; i < vct.size(); i++) {
            add(i, vct[i], false);
        }
    }

    //! Complete initialization from Simple Vector.
    void store_from_vector(SimpleVector<REAL> &vct) {
        clear();
        initializeDefaults(vct.get_size());
        unsigned int i;
        for (i = 0; i < vct.get_size(); i++) {
            add(i, vct[i], false);
        }
    }
    //! Returns if the vector is sorted by index.
    bool is_sorted_by_index() const {
        return m_is_sorted_by_index;
    }

    //! Adds the simple <index, value> pair into the vector.
    //! Also allows duplication check if given index already exists. 
    //! Does not allow duplication by default.
    //! Ignores add of element if index already exists and index_duplication_check is true.
    //! Adds in sorted fashion if the PackedVector is sorted by index.
    void add(unsigned int index, REAL value, bool p_index_duplication_check) {
        // Check for non zero condition
        if (value == 0.0F)return;
        // Simply return if index is already present
        if (p_index_duplication_check) {
            iterator itr = m_vct_packed.begin();
            while (itr != m_vct_packed.end()) {
                if (index == (*itr).get_index()) {
                    DEBUG_WARNING("Trying to add an index already existing...");
                    return;
                }
                itr++;
            }
        }
        PackedElement element;
        element.set_index(index);
        element.set_value(value);
        m_nnz++;
        if (!is_sorted_by_index()) {
            m_vct_packed.add(element);
        } else {
            m_vct_packed.add_sorted(element);
        }
    }

    //    //! Adds the simple <index, value> pair into the vector
    //    //! Also allows duplication check if given index already exists. 
    //    //! Does not allow duplication by default
    //    //! Ignores add of element if index already exists and index_duplication_check is true
    //    void add(unsigned int index, REAL value, bool p_index_duplication_check = true) {
    //        // Check for non zero condition
    //        if (value == 0.0F)return;
    //        // Simply return if index is already present
    //        if (p_index_duplication_check) {
    //            iterator itr = m_vct_packed.begin();
    //            while (itr != m_vct_packed.end()) {
    //                if (index == (*itr).get_index())
    //                    return;
    //                itr++;
    //            }
    //        }
    //        PackedElement element;
    //        element.set_index(index);
    //        element.set_value(value);
    //        m_nnz++;
    //        m_vct_packed.add(element);
    //    }

    //! Not a Efficient operation to randomly access item
    //! from Packed Vector. Only need to be done if very much necessary.
    REAL get(unsigned int index) const {
        iterator itr = m_vct_packed.begin();
        while (itr != m_vct_packed.end()) {
            unsigned int current_index = (*itr).get_index();
            if (index == current_index)
                return (*itr).get_value();
            if (is_sorted_by_index() && index < current_index)
                return 0.0F;
            itr++;
        }
        return 0.0F;
    }

    //! Removes an element with given absolute index.
    //! Removal is done only if index is present
    void remove(PackedElement packed_element){
        bool is_removed = m_vct_packed.remove(packed_element);
        if(is_removed)
            m_nnz--;
    }
    
    // Not a Efficient operation to randomly access item.
    // from Packed Vector. Only need to be done if very much necessary.
    REAL operator[](unsigned int index) const {
        return get(index);
    }

    //! Improve the size of the exising vector. Expand the vector.
    //! The size need to be extended only. the vector cannot shrink.
    //! TODO - Check the case in which we might mistakenly shrink the vector.
    void resize(unsigned int p_size) {
        // For now resizing only allows expansion and no shrinking
        assert(p_size >= m_real_size);
        m_real_size = p_size;
    }

    //! Sorts the Packed Vector based on the index.
    //! Selection sort is employed for simple implementation.
    //! TODO - Simply expand and compress incase it is less of a computation.
    void sort_by_index() {
        iterator itr1 = m_vct_packed.begin();
        iterator itr2, min_itr;
        while ((itr1 + 1) != m_vct_packed.end()) {
            itr2 = itr1 + 1;
            min_itr = itr1;
            while (itr2 != m_vct_packed.end()) {
                if ((*min_itr).get_index() > (*itr2).get_index()) {
                    min_itr = itr2;
                }
                itr2++;
            }
            // Swapping the elements
            if (min_itr != itr1) {
                PackedElement packed_element = *itr1;
                *itr1 = *min_itr;
                *min_itr = packed_element;
            }
            itr1++;
        }
    }

    //! Clear all the elements in the packed vector.
    //! Initialize to defaults.
    void clear() {
        m_vct_packed.clear();
        initializeDefaults(m_real_size);
    }

    //! Returns iterator to the beginning(first) element in the vector.
    iterator begin() {
        return m_vct_packed.begin();
    }

    //! Returns iterator to the ending(last) of the vector.
    iterator end() {
        return m_vct_packed.end();
    }

    //! Simple display of vector elements.
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

