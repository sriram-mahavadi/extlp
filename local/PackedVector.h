/* 
 * File:   PackedVector2.h
 * Author: Sriram Mahavadi
 *
 * Created on 23 April, 2014, 1:51 AM
 */

#ifndef PACKEDVECTOR2_H
#define	PACKEDVECTOR2_H
#include <stdio.h>
#include <assert.h>
#include "LinkedList.h"
#include "GlobalDebug.h"
//! Struct needs to be accessed outside by the get() method
//! Basic <index, value> pair to represent array/vector in packed form
template <class ItemClass>
class PackedElement {
private:
    unsigned int index;
    ItemClass value;
public:
    void setIndex(unsigned int index) {
        this->index = index;
    }
    void setValue(ItemClass value) {
        this->value = value;
    }
    unsigned int getIndex() {
        return this->index;
    }
    ItemClass getValue() {
        return this->value;
    }
    bool operator==(const PackedElement& other) const {
        return (this->index == other.index && this->value == other.value);
    }
};

/// Checks if the vector has sparsity>0.5 and stores in packed form if it does
/// otherwise it will store the complete vector
/// This is the best form to store on disk to reduce the number of I/O(compressed storage)
template <class ItemClass>
class PackedVector {
private:
    //! Total Number of non zeros in the vector
    unsigned int m_nnz;
    //! Max size of the unpacked vector
    unsigned int m_realSize;
    //! Storage for unpacked full length vector
    ItemClass* m_vctUnpacked;
    //! Storage for packed compressed vector
    typedef LinkedList< PackedElement<ItemClass> > ll_packedelement_type;
    ll_packedelement_type m_vctPacked;
    //! Identifier for vector to qualify as Packed or Unpacked
    //! if true vector is in compressed form
    //! else vector is a full-length vector
    bool m_isPacked;
    //! Null Item Value 0 in case of integer, float, double. Blank incase of string
    ItemClass m_nullItem;
public:
    //! LinkedList iterator for iterating through Packed Vector
    typedef typename LinkedList< PackedElement<ItemClass> >::iterator llIterator_type;

    //! PackedVector Iterator for sequential iteration 
    //! across the elements of the Vector. The Iterator
    //! dereferences to a PackedElement (<index, value> pair).
    class iterator {
    private:
        //! Needed to decide upon how to perform the iterations
        bool isPacked;
        //! Index of the element for unpacked vector
        unsigned int index;
        //! Pointer for the element 
        ItemClass* ptr;
        //! Iterator of the packed vector
        llIterator_type itr;
        void initialize(ItemClass* ptr, bool isPacked, unsigned int index, llIterator_type itr) {
            this->ptr = ptr;
            this->isPacked = isPacked;
            this->index = index;
            this->itr = itr;
        }
    public:
        friend class PackedVector<ItemClass>;
        iterator() {
            // Initializing to the starting of both packed or unpacked vectors
            index = 0;
            isPacked = false;
            itr = NULL;
            ptr = NULL;
        }
        void operator=(iterator itr) {
            this->index = itr.index;
            this->itr = itr.itr;
        }
        iterator& operator++() { // Pre incrementation
            if (isPacked()) {
                itr++;
            } else {
                index++;
            }
            return *this;
        }
        iterator& operator++(int) { // Post incrementation
            iterator* curItr = this;
            if (isPacked) {
                itr++;
            } else {
                index++;
            }
            return *curItr;
        }
        PackedElement<ItemClass> operator*() {
            if (isPacked) {
                return *itr;
            } else {
                PackedElement<ItemClass> packedElement;
                packedElement.setIndex(index);
                packedElement.setValue(ptr[index]);
                return packedElement;
            }
        }
        bool operator==(const iterator itr) const {
            if (isPacked && this->itr == itr.itr) return true;
            if (!isPacked && this->index == itr.index)return true;
            return false;
        }
        bool operator!=(const iterator itr) const {
            if (isPacked && this->itr != itr.itr) return true;
            if (!isPacked && this->index != itr.index)return true;
            return false;
        }
    };
    iterator begin() {
        iterator itr;
        itr.initialize(m_vctUnpacked, m_isPacked, 0, m_vctPacked.begin());
        return itr;
    }
    iterator end() {
        iterator itr;
        itr.initialize(m_vctUnpacked, m_isPacked, m_realSize, m_vctPacked.end());
        return itr;
    }

    //! Initialization section
    //! PackedVector need to know the real size of vector in advance
    //! PackedVector needs to know the element to be ignored(Null Item)
    void initializeDefaults(unsigned int p_realSize, ItemClass p_nullItem) {
        m_nnz = 0;
        m_realSize = p_realSize;
        m_isPacked = true;
        m_vctUnpacked = NULL;
        m_nullItem = p_nullItem;
    }

    //! Packed Vector is Packed by default.
    //! Can convert to be unpacked automatically based on sparsity.
    PackedVector(int p_realSize, ItemClass p_nullItem) {
        initializeDefaults(p_realSize, p_nullItem);
    }

    //! Forced instantiation of PackedVector
    //! This will force the vector to be packed/unpacked 
    PackedVector(int p_realSize, bool p_isPacked, ItemClass p_nullItem) {
        initializeDefaults(p_realSize, p_nullItem);
        m_isPacked = p_isPacked;
        if (!p_isPacked)
            initializeUnpackedVector();
    }

    //! Complete initialization. 
    //! No more modifications to the existing input vector of elements
    PackedVector(std::vector<ItemClass> &vct, ItemClass p_nullItem) {
        /// Calculating the number of non-zeros
        /// Adding the elements into Packed form by default
        initializeDefaults(vct.size(), true, p_nullItem);
        unsigned int i;
        for (i = 0; i < vct.size(); i++) {
            add(i, vct[i]);
        }
    }

    //! Destroying PackedVector
    ~PackedVector(){
        clear();
    }
    
    //! Returns the nullItem value pertaining to this type vector
    ItemClass getNullItem() {
        return m_nullItem;
    }

    //! Get the total number of non zeros in the vector
    unsigned int getPackedSize() {
        return m_nnz;
    }

    //! Get the max capacity of the vector including the zeros
    int getUnPackedSize() {
        return m_realSize;
    }

    //! Get the sparsity of the vector
    float getSparsity() {
        return (float) (m_realSize - m_nnz) / m_realSize;
    }
    unsigned int size() const {
        if (isPacked()) {
            return m_nnz;
        } else {
            return m_realSize;
        }
    }

    //! Forced conversion to Packed Form
    void convertToPacked() {
        if (isPacked())return;
        assert(m_vctUnpacked != NULL);
        m_isPacked = true;
        unsigned int i;
        m_nnz = 0;
        m_vctPacked.clear();
        for (i = 0; i < m_realSize; i++) {
            add(i, m_vctUnpacked[i], false);
        }
        delete[] m_vctUnpacked;
        m_vctUnpacked = NULL;
    }

    //! Initializing Unpacked Vector
    //! Requires initialization with zeros
    void initializeUnpackedVector() {
        assert(!isPacked() && m_vctUnpacked == NULL);
        m_vctUnpacked = new ItemClass[m_realSize];
        memset(m_vctUnpacked, 0, m_realSize * sizeof (ItemClass));
    }

    //! Deallocating/Destroying Unpacked Vector
    void deallocateUnpackedVector() {
        assert(m_vctUnpacked != NULL);
        delete []m_vctUnpacked;
        m_vctUnpacked = NULL;
    }
    //! Forced conversion to Unpacked Form
    void convertToUnPacked() {
        if (!isPacked())return;
        //        DEBUG_MEMORY("Vector is unpacking");
        assert(m_vctUnpacked == NULL);
        m_isPacked = false;
        m_nnz = 0;
        initializeUnpackedVector();

        llIterator_type itr = m_vctPacked.begin();
        while (itr != m_vctPacked.end()) {
            PackedElement<ItemClass> packedElement = *itr;
            add(packedElement.getIndex(), packedElement.getValue(), false);
            itr++;
        }
        //        DEBUG_MEMORY("Before Deallocating Packed Linked List");
        m_vctPacked.clear();
        //        DEBUG_MEMORY("After Deallocating Packed Linked List");
    }

    //! Store unpacked vector externally into vctUnpack
    //! Override the existing vector
    void storeUnpackedVector(std::vector<ItemClass> &vct) {
        if (isPacked()) {
            m_vctPacked.clear();
        } else {
            delete[] m_vctUnpacked;
        }
        initializeDefaults(vct.size(), m_nullItem);
        m_isPacked = false;
        initializeUnpackedVector();
        unsigned int i;
        for (i = 0; i < vct.size(); i++) {
            add(i, vct[i], false);
        }
    }

    //! Best for performing disk read
    //! Main intution is to reduce disk I/O
    void convertForDiskWrite() {
        if (m_nnz > m_realSize / 2) {
            convertToUnPacked();
        } else {
            convertToPacked();
        }
    }

    //! Adds <index, value> pair into the vector in packed/unpacked form
    //! autoConversion can be set or unset to convert among packed and unpacked forms
    //! While adding in packed form, it does not check if the index already exists
    //! While adding in unpacked form, it overrides the existing value in the index
    void add(unsigned int index, ItemClass value, bool p_autoConversion = true) {
        // Check for non zero condition
        if (value == m_nullItem)return;

        if (isPacked()) {
            PackedElement<ItemClass> element;
            element.setIndex(index);
            element.setValue(value);
            m_vctPacked.add(element);
        } else {
            assert(index < size());
            m_vctUnpacked[index] = value;
        }
        m_nnz++;
        if (p_autoConversion)
            convertForDiskWrite();
    }

    ItemClass operator[](unsigned int index) const{
        if (!isPacked()) {
            assert(index<m_realSize);
            return m_vctUnpacked[index];
        } else {
            llIterator_type itr = m_vctPacked.begin();
            while (itr != m_vctPacked.end()) {
                if (index == (*itr).getIndex())
                    return (*itr).getValue();
                itr++;
            }
            return m_nullItem;
        }
    }
    /// Gets the packed or unpacked element present in the 
    /// absolute index position input within corresponding vector
    /// When element index is returned to be -1, there is some error
    /// Which is either that vector is unintizlized, array out of bounds,
    /// or that vector status is unknown
    //    PackedElement get(unsigned int index) const {
    //        PackedElement element;
    //        if (status == UNINITIALIZED) {
    //            element.index = -1;
    //            DEBUG_WARNING("Vector is uninitialized!!!");
    //        } else if (status == PACKED) {
    //            if (index >= m_nnz) { // Array out of bounds
    //                element.index = -1;
    //            } else {
    //                element.index = m_vctPacked[index].index;
    //                element.value = m_vctPacked[index].value;
    //            }
    //        } else if (status == UNPACKED) {
    //            if (index >= m_realSize) { // Array out of bounds
    //                element.index = -1;
    //            } else {
    //                element.index = index;
    //                element.value = m_vctUnpacked[index];
    //            }
    //        } else {
    //            element.index = -1;
    //            DEBUG_ERROR("Vector Status is Unknown");
    //        }
    //        return element;
    //    }

    //! Returns if the status of the vector is packed
    bool isPacked() const{
        return m_isPacked;
    }

    //! Returns whether the vector was successfully resized or not
    bool resize(unsigned int p_size) {
        if (isPacked()) {
            // TODO Remove all the elements that have index >= p_size
        } else {
            ItemClass *tempPack = m_vctUnpacked;
            try {
                initializeUnpackedVector();
            } catch (std::bad_alloc& ba) {
                DEBUG_ERROR("Bad Alloc Caught: " << ba.what());
                m_vctUnpacked = tempPack;
                return false;
            }
            unsigned int minSize = (p_size < m_realSize) ? p_size : m_realSize;
            for (unsigned int i = 0; i < minSize; i++) {
                m_vctUnpacked[i] = tempPack[i];
            }
            delete[] tempPack;
        }
        m_realSize = p_size;
        return true;
    }
    void clear() {
        if (isPacked()) {
            m_vctPacked.clear();
        } else {
            deallocateUnpackedVector();
        }
        initializeDefaults(m_realSize, m_nullItem);
    }
    void displayVector() {
        CONSOLE_PRINTLN("********* Vector Display ************");
        if (isPacked()) {
            CONSOLE_PRINTLN("Vector Status is: Packed");
            CONSOLE_PRINTLN("Vector size is: " << size());
            CONSOLE_PRINTLN("Sparsity is: " << getSparsity());
            llIterator_type itr = m_vctPacked.begin();
            while (itr != m_vctPacked.end()) {
                std::stringstream packedStr;
                packedStr << "Index: " << (*itr).getIndex();
                packedStr << ", Value: " << (*itr).getValue();
                CONSOLE_PRINTLN(packedStr.str());
                itr++;
            }
        } else {
            CONSOLE_PRINTLN("Status is: Unpacked");
            CONSOLE_PRINTLN("Vector size is: " << size());
            CONSOLE_PRINTLN("Sparsity is: " << getSparsity());
            unsigned int i;
            for (i = 0; i < size(); i++) {
                std::stringstream packedStr;
                packedStr << "Index: " << i;
                packedStr << ", Value: " << m_vctUnpacked[i];
                CONSOLE_PRINTLN(packedStr.str());
            }
        }
        CONSOLE_PRINTLN(" ----------- *** --------------");
    }
};
#endif	/* PACKEDVECTOR2_H */

