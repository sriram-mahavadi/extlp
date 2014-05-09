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
        LinkedListNode<ItemClass>* ptr;
    public:
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
    LinkedList() {
        head = NULL;
        tail = NULL;
        m_size = 0;
    }
    ~LinkedList() {
        clear();
    }
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
    unsigned int size() {
        return this->m_size;
    }
    iterator begin() const {
        iterator itr;
        itr = head;
        return itr;
    }
    iterator end() const {
        iterator itr;
        itr = NULL;
        return itr;
    }
};


#endif	/* LINKEDLIST_H */

