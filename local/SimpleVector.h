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
    
    //! Basic initialization
    SimpleVector(unsigned int p_size) {
        this->m_size = p_size;
        m_vct_values = new ItemClass[p_size];
        for (unsigned int i = 0; i < p_size; i++)
            m_vct_values[i] = 0;
    }

    //! Resizes the vector with an option to allow shrinking if necessary
    //! Shrinking is not allowed by default
    void resize(unsigned int p_size, bool p_allow_shrink = false) {
        // Shrink is not allowed but requested size shrinks the vector
        assert(p_allow_shrink || p_size>m_size);
        ItemClass* temp = new ItemClass[p_size];
        unsigned int min_size = (p_size < m_size) ? p_size : m_size;
        // Copying the vector into temporary vector
        for (unsigned int i = 0; i < min_size; i++)
            temp[i] = m_vct_values[i];
        // Padding the vector if needed to be expanded
        for (unsigned int i = min_size; i < p_size; i++)
            m_vct_values[i] = 0;
        delete m_vct_values;
        m_vct_values = temp;
    }

    //! Adds an given element at specified index location
    //! Expansion is not allowed by default
    void add(unsigned int p_index, ItemClass p_value, bool p_allow_expansion = false) {
        // Check if expansion is allowed when adding out of bounds
        assert(p_allow_expansion || p_index < m_size);
        if (p_allow_expansion && p_index>=m_size) {
            resize(p_index+1);
        }
        m_vct_values[p_index] = p_value;
    }
    
    //! Gets the item present at the given p_index
    ItemClass get_element(unsigned int p_index){
        assert(p_index<m_size);
        return m_vct_values[p_index];
    }
    
    //! Gets the size of the vector
    unsigned int get_size(){
        return m_size;
    }
    
    //! operator[] overriding for fetching a value
    ItemClass operator[](unsigned int p_index) {
        return get_element(p_index);
    }
};

#endif	/* SIMPLEVECTOR_H */

