/* 
 * File:   ExtPackedVector.h
 * Author: harsha
 *
 * Created on 24 April, 2014, 3:54 PM
 */

#ifndef EXTPACKEDVECTOR_H
#define	EXTPACKEDVECTOR_H

#include "PackedVector2.h"



template <class ItemClass>
class ExtPackedVector : public stxxl::VECTOR_GENERATOR<PackedElement2<ItemClass>, 4, 2, 2 * 1024 * 1024>::result{
private:
    //! Total number of non zeros in the vector
    unsigned int nnz;   
    //! Total number of elements without compaction/compression
    unsigned int m_size;
    //! Identifier Variable to specify if vector is packed
    bool isPacked;
    //! Rest of all private data members and methods are inherited from vector class
public:
    ExtPackedVector(PackedVector2<ItemClass> packedVector){
        typename PackedVector2<ItemClass>::iterator itr = packedVector.begin();
        while(itr!=packedVector.end()){
            PackedElement2<ItemClass> packedElement = *itr;
            push_back(packedElement);
            itr++;
        }
    }
    //! Rest of all the public data members and methods are inherited from vector class
};


#endif	/* EXTPACKEDVECTOR_H */

