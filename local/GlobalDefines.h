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

#define ELEMENT_SIZE sizeof(REAL)
#define PACKED_ELEMENT_SIZE ELEMENT_SIZE + sizeof(unsigned int)

// Matrix B Block Configurations
#define VECTOR_B_TOTAL_SIZE 100*1024*1024
#define VECTOR_B_DIMENSION 200
//#define VECTOR_B_PACKED_BLOCK_SIZE (VECTOR_B_TOTAL_SIZE/2)/VECTOR_B_DIMENSION + ((VECTOR_B_TOTAL_SIZE/2)/VECTOR_B_DIMENSION)%(4*1024)
#define VECTOR_B_PACKED_BLOCK_SIZE VECTOR_B_DIMENSION*(sizeof(REAL)+4)/100

#define VECTOR_PACKED_BLOCK_SIZE 6*(sizeof(REAL)+4)*1024
#define VECTOR_BLOCK_SIZE 6*8*1024

// Matrix A Block Configurations
// Total Size taken in physical memory => (10MB*10) + 10MB + 10MB => 120MB
#define MATRIX_A_BLOCK_SIZE 1*1024*1024
#define MATRIX_A_PAGE_CACHE 1
#define MATRIX_A_BLOCKS_PER_PAGE 10
#define MATRIX_A_COL_DATA_BLOCK_SIZE 1*1024*1024
#define MATRIX_A_ROW_DATA_BLOCK_SIZE 1*1024*1024

// template parameters<ValueType, PageSize, CachePages, BlockSize, AllocStratg>
// Here vector caches 2 pages each of 4*4kb blocks => 8*32 kb cache overall 
//typedef stxxl::VECTOR_GENERATOR<REAL, 4, 1, 4*1024>::result real_vector;

//////////// - Map Block Configurations for storing rownames and colnames
#define DATA_NODE_BLOCK_SIZE (2*1024*1024)
#define DATA_LEAF_BLOCK_SIZE (2*1024*1024)
#define MAX_KEY_LEN 10

#endif	/* GLOBALDEFINES_H */

