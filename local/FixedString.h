/* 
 * File:   FixedString.h
 * Author: sriram
 *
 * Created on 19 April, 2014, 12:54 PM
 */

#ifndef FIXEDSTRING_H
#define	FIXEDSTRING_H
#include "GlobalDefines.h"
class FixedString {
public:
    char charStr[MAX_KEY_LEN];
    FixedString() : charStr({0}) {
    }
    bool operator<(const FixedString& fixedString) const {
        return std::lexicographical_compare(charStr, charStr + MAX_KEY_LEN,
                fixedString.charStr, fixedString.charStr + MAX_KEY_LEN);
        //        std::string str1(charStr);
        //        std::string str2(fixedString.charStr);
        //        return str1<str2;
    }
    bool operator==(const FixedString& fixedString) const {
        return std::equal(charStr, charStr + MAX_KEY_LEN, fixedString.charStr);
        //        std::string str1(charStr);
        //        std::string str2(fixedString.charStr);
        //        return (str1.compare(str2) == 0);
    }
    bool operator!=(const FixedString& fixedString) const {
        return !std::equal(charStr, charStr + MAX_KEY_LEN, fixedString.charStr);
        //        std::string str1(charStr);
        //        std::string str2(fixedString.charStr);
        //        return (str1.compare(str2) != 0);
    }
    static FixedString getFixedString(std::string strNormal) {
        // For now we may assume  that str length is always
        // less than or equal to the MAX_KEY_LEN
        FixedString fixedString;
        strcpy(fixedString.charStr, strNormal.c_str());
        //        MAX_KEY_LEN
        return fixedString;
    }
    static std::string getNormalString(FixedString fixedString) {
        std::string strNormal(fixedString.charStr);
        return strNormal;
    }
};
struct comp_Fixed_String : public std::less<FixedString> {
    static FixedString max_value() {
        FixedString s;
        std::fill(s.charStr, s.charStr + MAX_KEY_LEN, 0x7f);
        return s;
    }
};

//typedef stxxl::btree::btree<FixedString, double, comp_type, 4096, 4096, stxxl::SR> btree_type;
//
//// forced instantiation
//template class stxxl::btree::btree<FixedString, double, comp_type, 4096, 4096, stxxl::SR>;
//


#endif	/* FIXEDSTRING_H */
