/***************************************************************************
 *  mail.cpp
 *
 *  Part of the EXTLP. See http://extlp.sourceforge.net
 *
 *  Copyright (C) 2013 Sriram Mahavadi <sriram.mahavadi@gmail.com>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************/

//! [example]
#include <stxxl/bits/containers/matrix.h>
#include <stxxl/vector>
#include <iostream>
#include <vector>

#include "mpsinput.h"
#include "GlobalDefines.h"
#include "LPRow.h"
#include "LPCol.h"
#include "Console.h"
#include "Timer.h"
#include "ExtLPMatrix.h"
#include "PackedVector.h"
#include "PackedVectorUtil.h"
#include "ReadFileUtil.h"
#include "ExtVector.h"
#include "ExtNameMap.h"
#include "ExtLPDSSet.h"

int main(int argc, char *argv[]) {
    //    printUsage(argv);
    //    Resolving the problem of writing my own copy constructor for LPCol 
    //    which do not copy the name string... hence when going into vector string 
    //    is not copied
    //    char *arr[] = {"hello", "hell2", "h3llo3"};
    //    int i;
    //    vector<LPCol> arrCol;
    //    vector<LPRow> arrRow;
    //    for(i=0; i<3; i++){
    //        std::cout<<arr[i];
    //        std::string str = arr[i];
    //        LPCol col;
    //        LPRow row;
    //        row.setName(arr[i]);
    //        col.setName(arr[i]);
    //        arrCol.push_back(col);
    //        arrRow.push_back(row);
    //    }
    //    for(i=0; i<3; i++){
    //        std::cout<<"Try PrintCol: "<<arrCol[i].getName()<<"...\r\n";
    //                std::cout<<"Try PrintRow: "<<arrRow[i].getName()<<"...\r\n";
    //    }

    Console::printVersionInfo();
    const char* filename;
    std::cout << "Number of arguments: " << argc;
    try {
        int optidx;
        for (optidx = 1; optidx < argc; optidx++) {
            if (*argv[optidx] != '-')
                break;
            switch (argv[optidx][1]) {
                case 't':
                    std::cout << "timeout";
                    break;

                case 'L':
                    std::cout << "Iterations Limit";
                    break;
                case 'h':
                case '?':
                    Console::printVersionInfo();
                    //lint -fallthrough
                default:
                    Console::printUsage(argv);
            }

        }

        // generate stats instance
        stxxl::stats * Stats = stxxl::stats::get_instance();
        // start measurement here
        stxxl::stats_data stats_begin(*Stats);
        // some computation ...
        // substract current stats from stats at the beginning of the measurement


        filename = argv[optidx];
        DEBUG_SIMPLE("Accepted Input File: " << filename);
        ReadFileUtil inputReader;
//        inputReader.readFile(filename); // Reading using physical memory
        ///////////// - Initializing data structures
        std::vector<ExtRowVector> vctRows; // Set of Rows
        std::vector<ExtColVector> vctCols; // Set of Columns
        //    ExtVector &vctRhs;
        //    ExtVector &vctObj;
        PackedVector vctRhs; // Better to try and fit rhs 
        PackedVector vctObj; // and obj in memory as they are freq used
        std::vector<PackedRowVector> vctCacheRows;
        std::vector<PackedColVector> vctCacheCols;
        //        REAL * cacheMatrix[4]; // Try to cache B^-1
        // Names of the Rows and Cols involved in the 
        name_map myRowMap((name_map::node_block_type::raw_size)*3, (name_map::leaf_block_type::raw_size)*3);
        name_map myColMap((name_map::node_block_type::raw_size)*3, (name_map::leaf_block_type::raw_size)*3);
        ExtNameMap mapRowName(myRowMap);
        ExtNameMap mapColName(myColMap);
        ExtLPDSSet extDataSet(vctRows, vctCols, vctRhs, vctObj,
                vctCacheRows, vctCacheCols, mapRowName, mapColName);
//        inputReader.readFileUsingDisk(filename, extDataSet);

        ///////////////////// --- Test Sections
//        PackedVector::test();
        ExtVector::test();
//        ExtNameMap::test();

        //        ExtLPMatrix matrix;
        std::cout << (stxxl::stats_data(*Stats) - stats_begin);
        Console::println("Program Completed Execution!!!");
    } catch (int) {
        std::cout << "Error dude";
    }
    return 0;
}

/*
// Example for Matrix Multiplication
int main()
{
    // Matrix dimensions
    int height = 3;
    int width = 3;

    int internal_memory = 64 * 1024 * 1024;
    const int small_block_order = 32;  // must be multiple of matrix valueType in bits

    typedef stxxl::block_scheduler<stxxl::matrix_swappable_block<int, small_block_order> > block_schedular_type;
    typedef stxxl::matrix<int, small_block_order> matrix_type;

    block_schedular_type my_bs(internal_memory);

    // Create 3 matrices with given dimensions
    matrix_type A(my_bs, height, width);
    matrix_type B(my_bs, height, width);
    matrix_type C(my_bs, height, width);
    typedef matrix_type::col_major_iterator col_iterator;
    typedef matrix_type::row_major_iterator row_iterator;

    int i = 0;
    std::cout<<"****"<<std::endl<<A(1, 1)<<"***"<<std::endl;
    // Fill matrix A with values 0,1,2,3,...
    for (row_iterator it_A = A.begin(); it_A != A.end(); ++it_A, ++i)
    {
 *it_A = i;
    }

    i = 0;

    // Fill matrix B with values 0,2,4,8,...
    for (row_iterator it_B = B.begin(); it_B != B.end(); ++it_B, ++i)
    {
 *it_B = i * 2;
    }

    // Multiply matrix A and B and store result in matrix C
    C = A * B;

    C.transpose();

    // Print out matrix C
    for (row_iterator it_C = C.begin(); it_C != C.end(); ++it_C)
    {
        std::cout << *it_C << " ";
    }
    
    for(col_iterator it_C = C.begin(); it_C != C.end(); ++it_C){
        std::cout<< *it_C << " ";
    }
    return 0;
}
 */
//! [example]

/***************************************************************************
 *  local/test1.cpp
 *
 *  This is an example file included in the local/ directory of STXXL. All .cpp
 *  files in local/ are automatically compiled and linked with STXXL by CMake.
 *  You can use this method for simple prototype applications.
 *
 *  Part of the STXXL. See http://stxxl.sourceforge.net
 *
 *  Copyright (C) 2013 Timo Bingmann <tb@panthema.net>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************/
/*
#include <iostream>
#include <limits>

#include <stxxl/vector>
#include <stxxl/random>
#include <stxxl/sort>

struct my_less_int : std::less<int>
{
    int min_value() const { return std::numeric_limits<int>::min(); }
    int max_value() const { return std::numeric_limits<int>::max(); }
};

int main()
{
    // create vector
    stxxl::VECTOR_GENERATOR<int>::result vector;

    // fill vector with random integers
    stxxl::random_number32 random;

    for (size_t i = 0; i < 100 * 1024 * 1024; ++i) {
        vector.push_back(random());
    }

    // sort vector using 16 MiB RAM
    stxxl::sort(vector.begin(), vector.end(), my_less_int(), 16 * 1024 * 1024);

    // output first and last items:
    std::cout << vector.size() << " items sorted ranging from "
              << vector.front() << " to " << vector.back() << std::endl;

    return 0;
}
 */