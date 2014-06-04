/* 
 * File:   ExtStringVector.h
 * Author: sriram
 *
 * Created on 17 April, 2014, 12:27 AM
 */

#ifndef EXTSTRINGVECTOR_H
#define	EXTSTRINGVECTOR_H

#include "../GlobalDefines.h"

class ExtStringVector{
private:
    string_vector vctDisk;
public:
    ExtStringVector(){
    }
    void add(unsigned int i, std::string str){
//        assert(i<vctDisk.size());
//        vctDisk.push_back(str);
        vctDisk[i] = str;
    }
    std::string get(unsigned int i){
        return vctDisk[i];
    }
    unsigned int size(){
        return vctDisk.size();
    }
    void setSize(unsigned int p_size){
        vctDisk.reserve(p_size);
    }
    static void test(){
        ExtStringVector strVector;
        unsigned int i=0;
        unsigned int maxTrial = 1000000;
        strVector.setSize(maxTrial);
        for(i=0; i<maxTrial; i++){
            std::stringstream strStream;
            strStream<<i;
            DEBUG("Inserting: "<<i<<", "<<strStream.str());
            strVector.add(i, strStream.str());
        }
        for(i=0; i<10;i++){
            std::stringstream strStream;
            strStream<<"Index: "<<i;
            strStream<<", Value: "<<strVector.get(i);
            Console::println(strStream.str());
        }
    }
};


#endif	/* EXTSTRINGVECTOR_H */

