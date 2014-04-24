/* 
 * File:   GlobalDefines.h
 * Author: sriram
 *
 * Created on 18 March, 2014, 3:59 PM
 */

#ifndef GLOBALDEFINES_H
#define	GLOBALDEFINES_H
#include <stxxl/map>
#include <stxxl/vector>
#include <stxxl/map>
#include "GlobalDebug.h"
#include "FixedString.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// Data types and constants that are useful across the project  //
//////////////////////////////////////////////////////////////////

// Datatype for the cells of matrix and column cells of simplex
// algorithm - Can be replaced with Fraction, MixedFraction
#define REAL double

// Used to represent infinity values in the matrix and column 
// cells of simplex algorithm
#define INFINITY_VALUE 1e100

// Used to represent the block size of various I/O operations
// Storing 4 KB = 1<<12 block size by default
#define BLOCK_SIZE 1<<12;
//typedef stxxl::VECTOR_GENERATOR<REAL>::result real_vector;
// template parameters<ValueType, PageSize, CachePages, BlockSize, AllocStratg>
// Here vector caches 2 pages each of 4*4kb blocks => 8*32 kb cache overall 
typedef stxxl::VECTOR_GENERATOR<REAL, 4, 1, 4*1024>::result real_vector;

typedef stxxl::VECTOR_GENERATOR<std::string>::result string_vector;

//////////// - Defining map for storing rownames and colnames
#define DATA_NODE_BLOCK_SIZE (4096)
#define DATA_LEAF_BLOCK_SIZE (4096)

//! [comparator]
struct CompareGreaterInt {
    bool operator () (const int& a, const int& b) const {
        return a > b;
    }
    static int max_value() {
        return std::numeric_limits<int>::min();
    }
};
struct CompareGreaterString {
    bool operator () (const std::string& a, const std::string& b) const {
        return a > b;
    }
    static std::string max_value() {
        return std::string(MAX_KEY_LEN, std::numeric_limits<unsigned char>::max());
    }
};
//struct CompareGreaterFixedString {
//    bool operator () (const FixedString& a, const FixedString& b) const {
//        return FixedStringUtil::getNormalString(a) > FixedStringUtil::getNormalString(b);
//    }
//    static FixedString max_value() {
//        return FixedStringUtil::getMaxFixedString();
//    }
//};
//std::string getFixedLengthString(const std::string &strVariable)
//{
//    std::ostringstream ostr;
//    ostr << std::setfill('0') << std::setw(MAX_KEY_LEN) << strVariable;
//    return ostr.str();
//}
//struct CompareGreaterCharArray {
//    bool operator () (const std::string& a, const std::string& b) const {
//        return a < b;
//    }
//    //    static std::string max_value() {
//    //        return "";
//    //    }
//    static char[] max_value() {
//        //return std::string(MAX_KEY_LEN, std::numeric_limits<unsigned char>::max());
//        return std::string(MAX_KEY_LEN, std::numeric_limits<unsigned char>::max());
//    }
//};
// template parameter <KeyType, DataType, CompareType, RawNodeSize, RawLeafSize, PDAllocStrategy (optional)>
typedef stxxl::map<int, int, CompareGreaterInt, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> int_map;
typedef stxxl::map<std::string, unsigned int, CompareGreaterString, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> name_map;
//typedef stxxl::map<FixedString, unsigned int, CompareGreaterFixedString, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> fixed_name_map;
typedef stxxl::map<FixedString, unsigned int, comp_Fixed_String, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> fixed_name_map;




#endif	/* GLOBALDEFINES_H */

