/* 
 * File:   ExtColVector.h
 * Author: sriram
 *
 * Created on 16 April, 2014, 11:15 AM
 */

#ifndef EXTCOLVECTOR_H
#define	EXTCOLVECTOR_H

#include "deprecated/PackedColVector.h"
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
    class iterator {
    private:
        bool is_packed;
        ExtVector<REAL>::iterator itrUnpacked;
        int unpackedIndex;
        ExtPackedVector<REAL>::iterator itrPacked;
        void initialize(ExtVector<REAL>::iterator itrUnpacked) {
            this->itrUnpacked = itrUnpacked;
            this->is_packed = false;
            unpackedIndex = 0;
        }
        void initialize(ExtPackedVector<REAL>::iterator itrPacked) {
            this->itrPacked = itrPacked;
            this->is_packed = true;
        }
    public:
        friend class ExtColVector;
        void operator=(iterator itr) {
            if (is_packed) {
                this->itrPacked = itr.itrPacked;
            } else {
                this->itrUnpacked = itr.itrUnpacked;
            }
        }
        iterator& operator++() { // Pre incrementation
            if (is_packed) {
                this->itrPacked++;
            } else {
                this->itrUnpacked++;
                unpackedIndex++;
            }
            return *this;
        }
        iterator& operator++(int) { // Post incrementation
            iterator* curItr = this;
            if (is_packed) {
                this->itrPacked++;
            } else {
                this->itrUnpacked++;
                unpackedIndex++;
            }
            return *curItr;
        }
        PackedElement<REAL> operator*() {
            if (is_packed) {
                return *itrPacked;
            } else {
                PackedElement<REAL> packedElement;
                packedElement.setIndex(unpackedIndex++);
                packedElement.setValue(*itrUnpacked);
                return packedElement;
            }
        }
        bool operator==(const iterator itr) const {
            return (is_packed && this->itrPacked == itr.itrPacked) ||
                    (!is_packed && this->itrUnpacked == itr.itrUnpacked);
        }
        bool operator!=(const iterator itr) const {
            return (is_packed && this->itrPacked != itr.itrPacked) ||
                    (!is_packed && this->itrUnpacked != itr.itrUnpacked);
        }
    };
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
            assert(m_vctPacked == NULL);
            m_vctPacked = new ExtPackedVector<REAL>(packedVector);
        } else {
            assert(m_vctUnpacked == NULL);
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
            m_vctUnpacked->allocate_cache();
            REAL value = (*m_vctUnpacked)[index];
            m_vctUnpacked->deallocate_cache();
            return value;
        } else if (m_vctPacked != NULL && m_vctUnpacked == NULL) {
            m_vctPacked->allocate_cache();
            REAL value = m_vctPacked->getAbsoluteItemValue(index);
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
        iterator itr;
        if(isPacked()){
            itr.initialize(m_vctPacked->begin());
        } else{
            itr.initialize(m_vctUnpacked->begin());
        }
        return itr;
    }
    
    //! Returns the ending iterator
    iterator end() {
        iterator itr;
        if(isPacked()){
            itr.initialize(m_vctPacked->end());
        } else{
            itr.initialize(m_vctUnpacked->end());
        }
        return itr;
    }
    //! Allocating cache 
    void allocate_cache(){
        if(isPacked()){
            m_vctPacked->allocate_cache();
        } else{
            m_vctUnpacked->allocate_cache();
        }
    }
    //! Deallocating cache
    void deallocate_cache(){
        if(isPacked()){
            m_vctPacked->deallocate_cache();
        } else{
            m_vctUnpacked->deallocate_cache();
        }
    }
    //! gets the real size of vector (size when unpacked)
    unsigned int real_size(){
        if(isPacked()){
            return m_vctPacked->size();
        } else{
            return m_vctUnpacked->size();
        }
    }
    //! gets the sparsity of the vector
    float get_sparsity(){
        if(isPacked()){
            return m_vctPacked->get_sparsity();
        } else{
            return m_vctUnpacked->get_sparsity();
        }
    }
    //! gets the total number of non-zero values in the vector
    unsigned int get_nnz(){
        if(isPacked()){
            return m_vctPacked->nnz();
        } else{
            return m_vctUnpacked->nnz();
        }
    }
};

#endif	/* EXTCOLVECTOR_H */

