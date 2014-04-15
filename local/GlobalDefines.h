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
typedef stxxl::VECTOR_GENERATOR<REAL>::result real_vector;
typedef stxxl::VECTOR_GENERATOR<REAL>::result real_vector;

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
        return "";
    }
};
// template parameter <KeyType, DataType, CompareType, RawNodeSize, RawLeafSize, PDAllocStrategy (optional)>
typedef stxxl::map<int, char, CompareGreaterInt, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> int_map;
typedef stxxl::map<std::string, unsigned int, CompareGreaterString, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> name_map;


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// Console output/Debug operations that are useful across the project  ////
///////////////////////////////////////////////////////////////////////////
// Used for development purpose debugging
#define COMMON_DEBUG true
// Development purpose debug only related to parsing
#define PARSER_DEBUG true
// Simple debugging messages also may be visible to users
#define SIMPLE_DEBUG true
// Warning messages from all across the project
#define WARNING_DEBUG true
// Error messages from all across the project
#define ERROR_DEBUG true

#define DEBUG(x) \
if(COMMON_DEBUG == true){ \
        std::cout <<"COMMON MSG- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout.flush();\
}

#define DEBUG_SIMPLE(x) \
if(SIMPLE_DEBUG == true){ \
        std::cout << "*** " << x << " ***"<<std::endl;\
        std::cout.flush();\
}

#define DEBUG_PARSER(x) \
if(PARSER_DEBUG == true){ \
	std::cout <<"PARSER MSG- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout.flush();\
}

#define DEBUG_ERROR(x) \
if(ERROR_DEBUG == true){ \
        std::cout <<"GENERAL ERR- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout.flush();\
}

#define DEBUG_WARNING(x) \
if(WARNING_DEBUG == true){ \
        std::cout <<"GENERAL WRN- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout.flush();\
}

#endif	/* GLOBALDEFINES_H */

