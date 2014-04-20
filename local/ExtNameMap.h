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
#include "FixedStringUtil.h"
class ExtNameMap {
private:
    fixed_name_map &nameMap; //((name_map::node_block_type::raw_size), (node));
    //    name_map nameMap((name_map::node_block_type::raw_size)*3, (name_map::leaf_block_type::raw_size)*3);
    ////    ((map_type::node_block_type::raw_size)*3, (map_type::leaf_block_type::raw_size)*3);
public:
    ExtNameMap(fixed_name_map &myMap) : nameMap(myMap) {
    }
    /// Check if the element exists
    bool contains(const std::string &key) {
        FixedString fixedStringKey = FixedStringUtil::getFixedString(key);
        return (nameMap.find(fixedStringKey) != nameMap.end());
    }

    /// Get the element value
    /// Returns -1 if element does not exist
    unsigned int get(const std::string &key) {
        FixedString fixedStringKey = FixedStringUtil::getFixedString(key);
        fixed_name_map::iterator itrMap = nameMap.find(fixedStringKey);
        if (itrMap == nameMap.end()) return -1;
        return itrMap->second;
    }
    void set(const std::string &key, unsigned int value) {
        FixedString fixedStringKey = FixedStringUtil::getFixedString(key);
        nameMap[fixedStringKey] = value;
        //        nameMap.insert(std::pair<std::string, unsigned int>(key, value));
    }
    //    void set(const FixedString &key, unsigned int value) {
    //        nameMap[key] = value;
    //        //        nameMap.insert(std::pair<std::string, unsigned int>(key, value));
    //    }
    void displayMap() {
        Console::println("*** Map ***");
        for (fixed_name_map::iterator itr = nameMap.begin(); itr != nameMap.end(); itr++) {
            std::stringstream keyStream;
            std::string key = FixedStringUtil::getNormalString(itr->first);
            keyStream << "Index: " << key << ", ";
            keyStream << "Value: " << itr->second;
            Console::println(keyStream.str());
        }
    }
    static void test() {
        Console::println("*** Testing ExtNameMap ***");
        //        string a = "hello", b="help";
        //        if(a<b)Console::println(a);
        //        else Console::println(b);
        fixed_name_map myFixedMap((fixed_name_map::node_block_type::raw_size)*5, (fixed_name_map::leaf_block_type::raw_size)*5);
        //        ExtNameMap strMap(myMap);
        for (unsigned int i = 1; i < 1000; i++) { /// Inserting 1 million strings
            std::stringstream strStream;
            strStream << (i);
            FixedString fixedString = FixedStringUtil::getFixedString(strStream.str());
            //            Console::println("Inserting: " + strStream.str());
            //            strMap.set(fixedString, i);
            if (i % 1000000 == 0) {
                Console::println("Number of Keys Written: ", i);
            }
            myFixedMap[fixedString] = i;
        }
        FixedString fxString = FixedStringUtil::getFixedString("111");
        DEBUG("From Fixed String(111): " << myFixedMap[fxString]);
        fxString = FixedStringUtil::getFixedString("1000000");
        myFixedMap.find(fxString);
        DEBUG("From Fixed String(1000000): " << myFixedMap[fxString]);
        if (myFixedMap.find(fxString) == myFixedMap.end()) {
            Console::println("111 element present");
        } else {
            Console::println("ERROR: 111 element not present");
        }
        unsigned int i = 0;
        for (fixed_name_map::iterator itr = myFixedMap.begin(); itr != myFixedMap.end(); itr++) {
            std::stringstream myStream;
            FixedString fixedString = itr->first;
            myStream << "Key: " << FixedStringUtil::getNormalString(fixedString);
            myStream << ", Value: " << itr->second;
            i++;
            if (i % 1000000 == 0) {
                Console::println("Number of Keys Read: ", i);
            }
            //            Console::println(myStream.str());
            //            DEBUG(myStream.str());
        }
        //        strMap.displayMap();
        //        int_map myIntMap((int_map::node_block_type::raw_size)*3, (int_map::leaf_block_type::raw_size)*3);
        //        for(int i=0; i<1000000; i++){
        //                myIntMap[i] = i;
        //        }
        //        if(myIntMap.find(1111)!=myIntMap.end()){
        //            Console::println("Found the Element 1111");
        //        }
        //        for(int_map::iterator itr = myIntMap.begin(); itr!=myIntMap.end(); itr++){
        //            std::stringstream myStream;
        //            myStream<<"Key: "<<itr->first;
        //            myStream<<", Value: "<<itr->second;
        ////            Console::println(myStream.str());
        //        }        
        Console::println("------------ *** --------------");
    }
};
#endif	/* EXTNAMEMAP_H */

