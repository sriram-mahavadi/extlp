/* 
 * File:   Test.h
 * Author: harsha
 *
 * Created on 22 April, 2014, 11:03 AM
 */

#ifndef TEST_H
#define	TEST_H
#include "LinkedList.h"
#include "GlobalDebug.h"
class Test {
public:
    static void testLinkedList() {
        Console::println("----------- Testing Linked List --------------");
        LinkedList<double> linkedList;
        for(unsigned int i=0; i<100; i++)
            linkedList.add((double)i);
        LinkedList<double>::iterator itr = linkedList.begin();
        while(itr!=linkedList.end()){
            double val = *itr;
            DEBUG("Value is: "<<val);
            itr++;
        }
    }
};

#endif	/* TEST_H */

