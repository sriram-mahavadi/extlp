/* 
 * File:   GlobalDefines.h
 * Author: Sriram Mahavadi
 *
 * Created on 18 March, 2014, 3:59 PM
 */

#ifndef GLOBALDEFINES_H
#define	GLOBALDEFINES_H
#include "GlobalDebug.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// Data types and constants that are useful across the project  //
//////////////////////////////////////////////////////////////////

//! Datatype for the cells of matrix and column cells of simplex
//! algorithm - Can be replaced with Fraction, MixedFraction
#define REAL double

//! Used to represent infinity values in the matrix and column 
//! cells of simplex algorithm
#define INFINITY_VALUE 1e100

//! Vector template parameters are set for configuring external memory operations performance are as follows:
//! template parameters<ValueType, PageSize, CachePages, BlockSize, AllocStratg>

//! Matrix B Block Configurations
//! Total Size taken in physical memory => 2*(5MB*10) + 1MB + 1MB => 102MB
#define MATRIX_B_BLOCK_SIZE (10*1024*1024)
#define MATRIX_B_COL_BLOCK_SIZE MATRIX_B_BLOCK_SIZE
#define MATRIX_B_ROW_BLOCK_SIZE MATRIX_B_BLOCK_SIZE
#define MATRIX_B_COL_PAGE_CACHE 1
#define MATRIX_B_ROW_PAGE_CACHE 1
#define MATRIX_B_COL_BLOCKS_PER_PAGE 1
#define MATRIX_B_ROW_BLOCKS_PER_PAGE 1
#define MATRIX_B_COL_DATA_BLOCK_SIZE (10*1024*1024)
#define MATRIX_B_ROW_DATA_BLOCK_SIZE (10*1024*1024)

//! Matrix A Block Configurations
//! Total Size taken in physical memory => (10MB*10) + 1MB + 1MB => 102MB
#define MATRIX_A_BLOCK_SIZE (10*1024*1024)
#define MATRIX_A_PAGE_CACHE 1
#define MATRIX_A_BLOCKS_PER_PAGE 1
#define MATRIX_A_COL_DATA_BLOCK_SIZE (10*1024*1024)
#define MATRIX_A_ROW_DATA_BLOCK_SIZE (10*1024*1024)

//! Map Block Configurations for storing rownames and colnames
//! Two maps are maintained for purpose of mapping rows and columns respectively
//! Total Size taken in physical memory => 2*(2MB*5) => 20MB
#define DATA_NODE_BLOCK_SIZE (10*1024*1024)
#define DATA_LEAF_BLOCK_SIZE (10*1024*1024)
#define DATA_NODE_CACHE_COUNT 10
#define DATA_LEAF_CACHE_COUNT 10
#define MAX_KEY_LEN 35

#endif	/* GLOBALDEFINES_H */

/**
 * Additional Data structures residing on physical memory through out the program
 * ```````````````````````````````````````````````````````````````````````````````
 * SimpleVector<unsigned int> for storing base indices => O(m)
 * SimpleVector<REAL> for storing rhs values => O(m)
 * SimpleVector<REAL> for storing base obj values => O(m)
 */

