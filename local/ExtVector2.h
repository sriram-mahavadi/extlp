/* 
 * File:   ExtVector2.h
 * Author: Sriram Mahavadi
 *
 * Created on 23 April, 2014, 7:33 PM
 */

#ifndef EXTVECTOR2_H
#define	EXTVECTOR2_H

template<class ItemClass>
class ExtVector2 : public stxxl::VECTOR_GENERATOR<ItemClass, 4, 2, 4 * 1024>::result{
private:
//    typedef stxxl::VECTOR_GENERATOR<ItemClass, 4, 2, 4 * 1024>::result item_vector_type;
//    item_vector_type vctDisk;
public:
//    friend class item_vector_type::iterator;
//    typedef item_vector_type::iterator iterator;
//    
//    void push_back(ItemClass item){
//        vctDisk.push_back(item);
//    }
//    
//    void resize(unsigned int size, bool shrinkCapacity=true){
//        vctDisk.resize(size, shrinkCapacity);
//    }
//    void allocate_page_cache(){
//        vctDisk.allocate_page_cache();
//    }
//    void deallocate_page_cache(){
//        vctDisk.deallocate_page_cache();
//    }
};


#endif	/* EXTVECTOR2_H */

