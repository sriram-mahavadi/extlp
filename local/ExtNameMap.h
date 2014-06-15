/* 
 * File:   ExtNameMap.h
 * Author: Sriram Mahavadi
 *
 * Created on 8 April, 2014, 12:07 AM
 */

#ifndef EXTNAMEMAP_H
#define	EXTNAMEMAP_H
#include "stxxl/map"
#include "FixedString.h"
#include "GlobalDefines.h"
#include "GlobalDebug.h"
// template parameter <KeyType, DataType, CompareType, RawNodeSize, RawLeafSize, PDAllocStrategy (optional)>
typedef stxxl::map<FixedString, unsigned int, comp_Fixed_String, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> fixed_name_map;
class ExtNameMap {
private:
    fixed_name_map &nameMap;

public:
    ExtNameMap(fixed_name_map &myMap) : nameMap(myMap) {
    }

    //! Check if the element exists
    bool contains(const std::string &key) {
        FixedString fixedStringKey = FixedString::getFixedString(key);
        fixed_name_map::const_iterator itrMap = nameMap.find(fixedStringKey);
        return ( itrMap != nameMap.end());
    }

    //! Get the element value
    //! Returns -1 if element does not exist
    unsigned int get(const std::string &key) {
        FixedString fixedStringKey = FixedString::getFixedString(key);
        fixed_name_map::const_iterator itrMap = nameMap.find(fixedStringKey);
        if (itrMap == nameMap.end()) {
            DEBUG_WARNING("Requesting for a row/col name not present.");
            return -1;
        }
        return itrMap->second;
    }
    void set(const std::string &key, unsigned int value) {
        FixedString fixedStringKey = FixedString::getFixedString(key);
        nameMap[fixedStringKey] = value;
        //        nameMap.insert(std::pair<FixedString, unsigned int>(fixedStringKey, value));
    }
    void displayStorageStatus() {
        std::stringstream strStream;
        nameMap.print_statistics(strStream);
        CONSOLE_PRINTLN(strStream.str());
    }
    void displayMap() {
        CONSOLE_PRINTLN("*** Map ***");
        for (fixed_name_map::iterator itr = nameMap.begin(); itr != nameMap.end(); itr++) {
            std::stringstream keyStream;
            std::string key = FixedString::getNormalString(itr->first);
            keyStream << "Index: " << key << ", ";
            keyStream << "Value: " << itr->second;
            CONSOLE_PRINTLN(keyStream.str());
        }
    }
    static void test() {
        CONSOLE_PRINTLN("*** Testing ExtNameMap ***");
        //        string a = "hello", b="help";
        //        if(a<b)CONSOLE_PRINTLN(a);
        //        else CONSOLE_PRINTLN(b);
        fixed_name_map myFixedMap((fixed_name_map::node_block_type::raw_size)*5, (fixed_name_map::leaf_block_type::raw_size)*5);
        //        ExtNameMap strMap(myMap);
        for (unsigned int i = 1; i < 1000; i++) { /// Inserting 1 million strings
            std::stringstream strStream;
            strStream << (i);
            FixedString fixedString = FixedString::getFixedString(strStream.str());
            //            CONSOLE_PRINTLN("Inserting: " + strStream.str());
            //            strMap.set(fixedString, i);
            if (i % 1000000 == 0) {
                CONSOLE_PRINTLN("Number of Keys Written: " << i);
            }
            myFixedMap[fixedString] = i;
        }
        FixedString fxString = FixedString::getFixedString("111");
        DEBUG("From Fixed String(111): " << myFixedMap[fxString]);
        fxString = FixedString::getFixedString("1000000");
        myFixedMap.find(fxString);
        DEBUG("From Fixed String(1000000): " << myFixedMap[fxString]);
        if (myFixedMap.find(fxString) == myFixedMap.end()) {
            CONSOLE_PRINTLN("111 element present");
        } else {
            CONSOLE_PRINTLN("ERROR: 111 element not present");
        }
        unsigned int i = 0;
        for (fixed_name_map::iterator itr = myFixedMap.begin(); itr != myFixedMap.end(); itr++) {
            std::stringstream myStream;
            FixedString fixedString = itr->first;
            myStream << "Key: " << FixedString::getNormalString(fixedString);
            myStream << ", Value: " << itr->second;
            i++;
            if (i % 1000000 == 0) {
                CONSOLE_PRINTLN("Number of Keys Read: " << i);
            }
            //            CONSOLE_PRINTLN(myStream.str());
            //            DEBUG(myStream.str());
        }
        //        strMap.displayMap();
        //        int_map myIntMap((int_map::node_block_type::raw_size)*3, (int_map::leaf_block_type::raw_size)*3);
        //        for(int i=0; i<1000000; i++){
        //                myIntMap[i] = i;
        //        }
        //        if(myIntMap.find(1111)!=myIntMap.end()){
        //            CONSOLE_PRINTLN("Found the Element 1111");
        //        }
        //        for(int_map::iterator itr = myIntMap.begin(); itr!=myIntMap.end(); itr++){
        //            std::stringstream myStream;
        //            myStream<<"Key: "<<itr->first;
        //            myStream<<", Value: "<<itr->second;
        ////            CONSOLE_PRINTLN(myStream.str());
        //        }        
        CONSOLE_PRINTLN("------------ *** --------------");
    }
};
#endif	/* EXTNAMEMAP_H */

