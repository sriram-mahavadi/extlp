/* 
 * File:   LinkedList.h
 * Author: Sriram Mahavadi
 *
 * Created on 22 April, 2014, 3:03 AM
 */

#ifndef LINKEDLIST_H
#define	LINKEDLIST_H
#include <stdio.h>
#include <assert.h>

#include "GlobalDefines.h"
template<class ItemClass>
class LinkedListNode {
private:
    ItemClass item;
    LinkedListNode<ItemClass>* next;
public:
    LinkedListNode(ItemClass item) {
        this->item = item;
        this->next = NULL;
    }
    void setNext(LinkedListNode<ItemClass>* next) {
        this->next = next;
    }
    void setItem(ItemClass item) {
        this->item = item;
    }
    LinkedListNode<ItemClass>* getNext() {
        return this->next;
    }
    ItemClass getItem() {
        return this->item;
    }
    ItemClass& getItemReference() {
        return this->item;
    }
};
template<class ItemClass>
class LinkedList {
private:
    LinkedListNode<ItemClass>* head;
    LinkedListNode<ItemClass>* tail;
    unsigned int m_size;
public:
    class iterator {
    private:
        //! Current position/LinkedListNode of the iterator
        LinkedListNode<ItemClass>* ptr;
    public:
        //! Allowing LinkedList to access the private components of it's iterator
        friend class LinkedList;
        void operator=(LinkedListNode<ItemClass>* ptr) {
            this->ptr = ptr;
        }
        void operator=(iterator itr) {
            this->ptr = itr.ptr;
        }
        iterator& operator++() { // Pre incrementation
            assert(ptr != NULL);
            ptr = ptr->getNext();
            return *this;
        }
        iterator& operator++(int) { // Post incrementation
            assert(ptr != NULL);
            ptr = ptr->getNext();
            return *this;
        }
        iterator operator+(int incr){
            LinkedListNode<ItemClass>* temp  = ptr;
            while(incr!=0){
                temp=temp->getNext();
                incr--;
            }
            iterator itr;
            itr.ptr = temp;
            return itr;
        }
        ItemClass& operator*() {
            return ptr->getItemReference();
        }
        bool operator==(const iterator itr) const {
            if (itr.ptr == this->ptr) return true;
            return false;
        }
        bool operator!=(const iterator itr) const {
            if (itr.ptr != this->ptr) return true;
            return false;
        }
    };
    //! Simple Initialization
    LinkedList() {
        head = NULL;
        tail = NULL;
        m_size = 0;
    }
    //! Copy Constructor. Does shallow copy by default hence need to be careful
    //! Overriding for creating a deep copy of LinkedList
    LinkedList(LinkedList& p_list){
        DEBUG_WARNING("Copying Linked List.");
        head = NULL;
        tail = NULL;
        m_size = 0;
        LinkedList<ItemClass>::iterator itr = p_list.begin();
        while(itr!=p_list.end()){
            add(*itr);
            itr++;
        }
    }
    //! Destroying LinkedList
    ~LinkedList() {
        clear();
    }
    //! Clears the complete list (deleting all the nodes in the list)
    void clear() {
        LinkedListNode< ItemClass >* ptrNode, *tmpNode;
        //        if (size() == 0)return; - Just a check if already no elements
        assert((head == NULL && tail == NULL) || tail->getNext() == NULL);
        ptrNode = head;
        unsigned int i = 0;
        while (ptrNode != NULL) {
            tmpNode = ptrNode;
            ptrNode = ptrNode->getNext();
            delete tmpNode;
            i++;
        }
        //        DEBUG("Deleted " << i << " nodes");
        head = tail = NULL;
        m_size = 0;
    }
    //! Adds the item at the end of the list
    void add(ItemClass item) {
        LinkedListNode<ItemClass>* newItem = new LinkedListNode<ItemClass>(item);
        //        DEBUG("Size of new Node: "<<sizeof(*newItem));
        if (tail == NULL) {
            head = tail = newItem;
        } else {
            tail->setNext(newItem);
            tail = newItem;
        }
        m_size++;
    }
    //! Adds the item after the given iterator/position of list
    void add(iterator itr, ItemClass item){
        LinkedListNode<ItemClass>* list_node = itr.ptr;
        // If the iterator shows null. possible when the list is empty
        if(list_node==NULL){
            add(item);
            return;
        }
        LinkedListNode<ItemClass>* new_node = new LinkedListNode<ItemClass>(item);
        new_node->setNext(list_node->getNext());
        list_node->setNext(new_node);
    }
    //! Returns the size of the linked List
    unsigned int size() {
        return this->m_size;
    }
    //! Returns the iterator pointing to the beginning element of the list
    iterator begin() const {
        iterator itr;
        itr = head;
        return itr;
    }
    //! Returns the iterator pointing to the ending element of the list
    iterator end() const {
        iterator itr;
        itr = NULL;
        return itr;
    }
};


#endif	/* LINKEDLIST_H */

