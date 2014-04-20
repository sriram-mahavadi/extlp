/* 
 * File:   FixedStringUtil.h
 * Author: sriram
 *
 * Created on 18 April, 2014, 12:03 PM
 */

#ifndef FIXEDSTRINGUTIL_H
#define	FIXEDSTRINGUTIL_H
#include "FixedString.h"
//#define MAX_KEY_LEN 12
//class FixedStringUtil;
//typedef class FixedString{
//public:
//    char charStr[MAX_KEY_LEN];
//    bool operator==(const FixedString& fixedString) const{
//        std::string strNormal1(this->charStr);
//        std::string strNormal2(fixedString.charStr);
//        return (strNormal1.compare(strNormal2)==0);
//    }
//    bool operator!=(const FixedString& fixedString) const{
//        std::string strNormal1(this->charStr);
//        std::string strNormal2(fixedString.charStr);
//        return (strNormal1.compare(strNormal2)!=0);
//    }
//}FixedString;
class FixedStringUtil{
public:
    static FixedString getFixedString(std::string strNormal){
        // For now we may assume  that str length is always
        // less than or equal to the MAX_KEY_LEN
        FixedString fixedString;
        strcpy(fixedString.charStr, strNormal.c_str());
//        MAX_KEY_LEN
        return fixedString;
    }
    static std::string getNormalString(FixedString fixedString){
        std::string strNormal(fixedString.charStr);
        return strNormal;
    }
    static FixedString getMaxFixedString(){
        std::string strMax = std::string(MAX_KEY_LEN-1, std::numeric_limits<unsigned char>::min());
        return getFixedString(strMax);
    }
    static void test(){
        std::cout<<" --- Fixed String Util Test --- "<<std::endl;
        FixedString fixedString = getFixedString("1");
        std::string strNormal = getNormalString(fixedString);
        std::cout<<"Normal String is: "<<strNormal<<std::endl;
    }
};


#endif	/* FIXEDSTRINGUTIL_H */

