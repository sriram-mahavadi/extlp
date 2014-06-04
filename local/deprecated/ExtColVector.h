/* 
 * File:   ExtColVector.h
 * Author: sriram
 *
 * Created on 16 April, 2014, 11:15 AM
 */

#ifndef EXTCOLVECTOR_H
#define	EXTCOLVECTOR_H

#include "PackedColVector.h"
#include "ExtPackedVector.h"
class ExtColVector {
private:

    //------------------------------------
    /**@name Data */
    //@{
    REAL m_up; ///< upper bound
    REAL m_low; ///< lower bound
    REAL m_object; ///< objective value
    ExtPackedVector* m_vctPacked; /// column vector - packed version
    std::string m_name;
    //@}
    void deallocatePackedVector() {
        assert(m_vctPacked != NULL);
        delete m_vctPacked;
        m_vctPacked = NULL;
    }
public:
    //! Accessing iterator of the PackedVector
//    friend class ExtPackedVector::iterator;
    typedef ExtPackedVector::iterator iterator;
    //------------------------------------
    /**@name Construction / destruction */
    //@{
    /// default constructor.
    /** Construct LPCol with a column vector ready for taking \p defDim
     *  nonzeros.
     */
    explicit ExtColVector(PackedColVector& packedColVector)
    : m_up(packedColVector.get_upper_bound()), m_low(packedColVector.get_lower_bound()),
    m_object(packedColVector.get_objective_value()),
    m_name(packedColVector.get_name()) {
        PackedVector &packedVector = packedColVector;
        m_vctPacked = new ExtPackedVector(packedVector);
    }
    void storeFromPackedVector(PackedVector &packedVector) {
        // Recreating the whole vector for now... (Inefficient)
        // --TODO--     Need to implement incremental changes only getting written
        //              to the disk
        // Deallocate either of the version of vectors first
        if (m_vctPacked != NULL) {
            delete m_vctPacked;
            m_vctPacked = NULL;
        }
        // Allocate the new vector now
        m_vctPacked = new ExtPackedVector(packedVector);
    }
    //------------------------------------
    /**@name Access / modification */
    //@{
    /// get objective value.
    REAL obj() const {
        return m_object;
    }
    /// access objective value.
    void setObj(REAL p_object) {
        m_object = p_object;
    }

    /// get upper bound.
    REAL upper() const {
        return m_up;
    }
    /// access upper bound.
    void setUpper(REAL p_up) {
        m_up = p_up;
    }

    /// get lower bound.
    REAL lower() const {
        return m_low;
    }

    /// get name of the LP Column
    std::string getName() const {
        return m_name;
    }

    //! get the absolute index element of the current packed col vector
    REAL getRowElement(unsigned int index) {
        if (m_vctPacked != NULL) {
            m_vctPacked->allocate_cache();
            REAL value = m_vctPacked->get_absolute_item_value(index);
            m_vctPacked->deallocate_cache();
            return value;
        } else {
            DEBUG_ERROR("Unknown Packed-Format!!! Both formats are NULL or Filled");
            return 0.0F;
        }
    }

    /// access the name of the LP Column
    void setName(std::string p_name) {
        m_name = p_name;
    }

    /// access lower bound.
    void setLower(REAL p_low) {
        m_low = p_low;
    }

    //! Returns the beginning iterator
    iterator begin() {
        return m_vctPacked->begin();
    }

    //! Returns the ending iterator
    iterator end() {
        return m_vctPacked->end();
    }
    //! Allocating cache 
    void allocate_cache() {
        m_vctPacked->allocate_cache();
    }
    //! Deallocating cache
    void deallocate_cache() {
        m_vctPacked->deallocate_cache();
    }
    //! gets the real size of vector (size when unpacked)
    unsigned int real_size() {
        return m_vctPacked->size();
    }
    //! gets the sparsity of the vector
    float get_sparsity() {
            return m_vctPacked->get_sparsity();
    }
    //! gets the total number of non-zero values in the vector
    unsigned int get_nnz() {
            return m_vctPacked->nnz();
    }
};

#endif	/* EXTCOLVECTOR_H */

