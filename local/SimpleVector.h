/* 
 * File:   SimpleVector.h
 * Author: Sriram Mahavadi
 *
 * Created on 10 May, 2014, 11:46 PM
 */

#ifndef SIMPLEVECTOR_H
#define	SIMPLEVECTOR_H
#include "GlobalDefines.h"

//! Simple Vector container for better storage allocation and deallocation
//! Generic Vector to store any type of data. 
template <class ItemClass>
class SimpleVector {
private:
    //! Fixed array of ItemClass values
    ItemClass* m_vct_values;
    //! Size of the vector
    unsigned int m_size;

public:
    //! Basic initialization with empty vector
    //    SimpleVector() {
    //        m_vct_values = NULL;
    //        m_size = 0;
    //    }

    //! Basic initialization with fixed size
    SimpleVector(unsigned int p_size) {
        this->m_size = p_size;
        m_vct_values = new ItemClass[p_size];
        for (unsigned int i = 0; i < p_size; i++)
            m_vct_values[i] = 0;
    }
    
    //! Deep copy for SimpleVector copy constructor
    //! Shallow copy would cause problems for SimpleVector
    SimpleVector(SimpleVector& other){
        m_vct_values = new ItemClass[other.get_size()];
        m_size = other.get_size();
        for(unsigned int i=0; i<m_size; i++){
            m_vct_values[i] = other[i];
        }
    }
    
    //! Simple Deallocation
    ~SimpleVector(){
        clear();
    }

    //! Deep copy when during assignment
    void operator=(const SimpleVector& other){
        clear();
        m_vct_values = new ItemClass[other.get_size()];
        m_size = other.get_size();
        for(unsigned int i=0; i<m_size; i++){
            m_vct_values[i] = other.get_element(i);
        }
    }
    
    //! Resizes the vector with an option to allow shrinking if necessary
    //! Shrinking is not allowed by default
    void resize(unsigned int p_size, bool p_allow_shrink = false) {
        if (p_size == m_size) return; // If already of the same size
        // Shrink is not allowed but requested size shrinks the vector
        assert(p_allow_shrink || p_size > m_size);
        ItemClass* temp = new ItemClass[p_size];
        unsigned int min_size = (p_size < m_size) ? p_size : m_size;
        // Copying the vector into temporary vector
        for (unsigned int i = 0; i < min_size; i++) {
            temp[i] = m_vct_values[i];
        }
        // Padding the vector if needed to be expanded
        for (unsigned int i = min_size; i < p_size; i++) {
            temp[i] = 0;
        }
        if (m_vct_values != NULL) {
            // Handling the possibility of double free after clear and resize
            delete []m_vct_values;
        }
        m_vct_values = temp;
        m_size = p_size;
    }

    // Just assign all the elements to value zero.
    void nullify() {
        for (unsigned int i = 0; i < m_size; i++) {
            m_vct_values[i] = 0;
        }
    }

    //! Adds an given element at specified index location
    //! Expansion is not allowed by default
    void add(unsigned int p_index, ItemClass p_value, bool p_allow_expansion = false) {
        // Check if expansion is allowed when adding out of bounds
        assert(p_allow_expansion || p_index < m_size);
        if (p_allow_expansion && p_index >= m_size) {
            DEBUG("Trying to Expand");
            resize(p_index + 1);
        }
        m_vct_values[p_index] = p_value;
    }

    //! Gets the item present at the given p_index
    ItemClass get_element(unsigned int p_index) const{
        assert(p_index < m_size);
        return m_vct_values[p_index];
    }

    //! Gets the size of the vector
    unsigned int get_size() const{
        return m_size;
    }

    //! operator[] overriding for fetching a value
    ItemClass& operator[](unsigned int p_index) {
        assert(p_index < m_size);
        return m_vct_values[p_index];
    }

    //! Remove/Clear all the elements of the vector
    void clear() {
        // If already empty vector
        if (m_size == 0)return;
        // If some elements are present already
        delete []m_vct_values;
        m_vct_values = NULL;
        m_size = 0;
    }
};

#endif	/* SIMPLEVECTOR_H */

