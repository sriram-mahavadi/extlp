/* 
 * File:   GlobalDefines.h
 * Author: Sriram Mahavadi
 *
 * Created on 18 March, 2014, 3:59 PM
 */

#ifndef GLOBALDEFINES_H
#define	GLOBALDEFINES_H
#include "GlobalDebug.h"
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
//#define BLOCK_SIZE 1<<12;
// 48KB
//#define VECTOR_BLOCK_SIZE 12*4096
#define VECTOR_PACKED_BLOCK_SIZE 6*12*1024
#define VECTOR_BLOCK_SIZE 6*8*1024
#define MATRIX_A_BLOCK_SIZE 10*1024*1024
#define MATRIX_A_COL_DATA_BLOCK_SIZE 10*1024*1024
#define MATRIX_A_ROW_DATA_BLOCK_SIZE 10*1024*1024

//typedef stxxl::VECTOR_GENERATOR<REAL>::result real_vector;
// template parameters<ValueType, PageSize, CachePages, BlockSize, AllocStratg>
// Here vector caches 2 pages each of 4*4kb blocks => 8*32 kb cache overall 
//typedef stxxl::VECTOR_GENERATOR<REAL, 4, 1, 4*1024>::result real_vector;
//typedef stxxl::VECTOR_GENERATOR<std::string>::result string_vector;

//////////// - Defining map for storing rownames and colnames
#define DATA_NODE_BLOCK_SIZE (2*1024*1024)
#define DATA_LEAF_BLOCK_SIZE (2*1024*1024)
#define MAX_KEY_LEN 35

#endif	/* GLOBALDEFINES_H */

