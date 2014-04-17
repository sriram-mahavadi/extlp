/* 
 * File:   ExtNameMap.h
 * Author: sriram
 *
 * Created on 8 April, 2014, 12:07 AM
 */

#ifndef EXTNAMEMAP_H
#define	EXTNAMEMAP_H

#include "GlobalDefines.h"
#include "Console.h"
class ExtNameMap {
private:
    name_map &nameMap; //((name_map::node_block_type::raw_size), (node));
    //    name_map nameMap((name_map::node_block_type::raw_size)*3, (name_map::leaf_block_type::raw_size)*3);
    ////    ((map_type::node_block_type::raw_size)*3, (map_type::leaf_block_type::raw_size)*3);
public:
    ExtNameMap(name_map &myMap):nameMap(myMap){
        
    }
    /// Check if the element exists
    bool contains(const std::string &key) {
        return (nameMap.find(key) != nameMap.end());
    }

    /// Get the element value
    /// Returns -1 if element does not exist
    unsigned int get(const std::string &key) {
        name_map::iterator itrMap = nameMap.find(key);
        if (itrMap == nameMap.end()) return -1;
        return itrMap->second;
    }
    void set(const std::string &key, unsigned int value) {
        //        nameMap[key] = value;
        nameMap.insert(std::pair<std::string, unsigned int>(key, value));
    }
    void displayMap(){
        Console::println("*** Map ***");
        for(name_map::iterator itr = nameMap.begin(); itr!=nameMap.end(); itr++){
            std::stringstream keyStream;
            keyStream << "Index: " << itr->first << ", ";
            keyStream << "Value: " << itr->second;
            Console::println(keyStream.str());
        }
    }
    static void test(){
        Console::println("*** Testing ExtNameMap ***");
//        string a = "hello", b="help";
//        if(a<b)Console::println(a);
//        else Console::println(b);
        name_map myMap((name_map::node_block_type::raw_size)*3, (name_map::leaf_block_type::raw_size)*3);
        ExtNameMap strMap(myMap);
        string arrTest[] = {"hello", "this", "is", "a", "simple", "test", "tryokdddddddddddddddddddddddddddddddddddddddddddddutthissizestringyouhero"};
        unsigned int i, testSize = 7;
        for(i=0; i<testSize; i++){
            Console::println("Inserting: " + arrTest[i]);
            strMap.set(arrTest[i], i);
        }
        for(i=testSize; i<1000000; i++){        /// Inserting 1 million strings
            
            std::stringstream strStream;
            strStream<<(i*100);
//            Console::println("Inserting: " + strStream.str());
            strMap.set(strStream.str(), i);
        }
        strMap.displayMap();
        Console::println("------------ *** --------------");
    }
};
#endif	/* EXTNAMEMAP_H */

