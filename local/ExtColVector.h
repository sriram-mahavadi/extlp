/* 
 * File:   ExtColVector.h
 * Author: sriram
 *
 * Created on 16 April, 2014, 11:15 AM
 */

#ifndef EXTCOLVECTOR_H
#define	EXTCOLVECTOR_H

#include "PackedColVector.h"
#include "ExtVector.h"
#include "ExtPackedVector.h"
class ExtColVector {
private:

    //------------------------------------
    /**@name Data */
    //@{
    REAL m_up; ///< upper bound
    REAL m_low; ///< lower bound
    REAL m_object; ///< objective value
    ExtVector<REAL>* m_vctUnpacked; /// the column vector - unpacked version
    ExtPackedVector<REAL>* m_vctPacked; /// column vector - packed version
    std::string m_name;
    //@}
    void deallocatePackedVector() {
        assert(m_vctPacked != NULL);
        delete m_vctPacked;
        m_vctPacked = NULL;
    }
    void deallocateUnpackedVector() {
        assert(m_vctUnpacked != NULL);
        delete m_vctUnpacked;
        m_vctUnpacked = NULL;
    }
public:

    //------------------------------------
    /**@name Construction / destruction */
    //@{
    /// default constructor.
    /** Construct LPCol with a column vector ready for taking \p defDim
     *  nonzeros.
     */
    explicit ExtColVector(PackedColVector& packedColVector)
    : m_up(packedColVector.upper()), m_low(packedColVector.lower()),
    m_object(packedColVector.obj()),
    m_name(packedColVector.getName()) {
        PackedVector<REAL> &packedVector = packedColVector.getPackedVector();
        if (packedVector.isPacked()) {
            m_vctPacked = new ExtPackedVector<REAL>(packedVector);
            m_vctUnpacked = NULL;
        } else {
            m_vctPacked = NULL;
            m_vctUnpacked = new ExtVector<REAL>(packedVector);
        }
    }
    bool isPacked() {
        if (m_vctPacked != NULL && m_vctUnpacked == NULL) return true;
        else if (m_vctPacked == NULL && m_vctUnpacked != NULL)return false;
        else DEBUG_ERROR("Invalid PackedVector State");
        return false; // Just in case of Invalid state
    }
    void storeFromPackedVector(PackedVector<REAL> &packedVector) {
        // Recreating the whole vector for now... (Inefficient)
        // --TODO--     Need to implement incremental changes only getting written
        //              to the disk

        // Deallocate either of the version of vectors first
        if (isPacked()) deallocatePackedVector();
        else deallocateUnpackedVector();

        // Allocate the new vector now
        if (packedVector.isPacked()) {
            assert(m_vctPacked==NULL);
            m_vctPacked = new ExtPackedVector<REAL>(packedVector);
        } else {
            assert(m_vctUnpacked==NULL);
            m_vctUnpacked = new ExtVector<REAL>(packedVector);
        }
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
        if (m_vctPacked == NULL && m_vctUnpacked != NULL) {
            REAL value = (*m_vctUnpacked)[index];
            return value;
        } else if (m_vctPacked != NULL && m_vctUnpacked == NULL) {
            m_vctPacked->getAbsoluteItemValue(index);
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
};

#endif	/* EXTCOLVECTOR_H */

