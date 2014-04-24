/* 
 * File:   MemoryManager.h
 * Author: harsha
 *
 * Created on 23 April, 2014, 7:41 PM
 */

#ifndef MEMORYMANAGER_H
#define	MEMORYMANAGER_H

#include "GlobalDebug.h"

class MemoryManager{
public:
    static void check_point(){
        char ch;
        CONSOLE_PRINTLN("Look At the physical Memory status!!!");
        CONSOLE_PRINTLN("Press any key to continue!!!");
        char ch = getchar();
    }
};


#endif	/* MEMORYMANAGER_H */

