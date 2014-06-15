/***************************************************************************
 *  main.cpp
 *
 *  Part of the EXTLP. See http://extlp.sourceforge.net
 *
 *  Copyright (C) 2014 Sriram Mahavadi <sriram.mahavadi@gmail.com>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************/

//! [example]
//#include <stxxl/bits/containers/matrix.h>
//#include <stxxl/vector>
//#include <iostream>
//#include <vector>
//
//#include "mpsinput.h"
//#include "GlobalDefines.h"
//#include "LPRow.h"
//#include "LPCol.h"
//#include "ExtLPMatrix.h"
//#include "PackedVectorUtil.h"
#include "ReadFileUtil.h"
#include "ExtNameMap.h"
//#include "ExtLPDSSet.h"
#include "Test.h"
//#include "ExtSequence.h"
//#include "PackedVector.h"
//#include "ExtStxxlVector.h"
#include "Console.h"
#include "ExtSimplex.h"

void print_io_statistics(stxxl::stats_data diff_data, ExtLPDSSet& extDataSet, unsigned int expected_read_io,
        unsigned int expected_write_io, bool print_header = false) {
    int width = 15;
    if (print_header) {
        std::stringstream header_stream;
        header_stream << std::setw(width) << "Act. Read IO" << ", ";
        header_stream << std::setw(width) << "Act. Write IO" << ", ";
        header_stream << std::setw(width) << "Est. Read IO" << ", ";
        header_stream << std::setw(width) << "Est. Write IO" << ", ";
        header_stream << std::setw(width) << "NNZ A" << ", ";
        header_stream << std::setw(width) << "Density A" << ", ";
        header_stream << std::setw(width) << "IO Wait Time" << ", ";
        header_stream << std::setw(width) << "Parl. Read T" << ", ";
        header_stream << std::setw(width) << "Parl. Write T" << ", ";
        header_stream << std::setw(width) << "Elapsed Time" << ", ";
        CONSOLE_PRINTLN(header_stream.str());
    }
    std::stringstream statistics_stream;
    statistics_stream << std::setw(width) << diff_data.get_reads() << ", ";
    statistics_stream << std::setw(width) << diff_data.get_writes() << ", ";
    statistics_stream << std::setw(width) << expected_read_io << ", "; // Todo Estimate
    statistics_stream << std::setw(width) << expected_write_io << ", "; // Todo Estimate
    statistics_stream << std::setw(width) << extDataSet.A.get_overall_nnz() << ", ";
    statistics_stream << std::setw(width) << extDataSet.A.get_overall_density() << ", ";
    statistics_stream << std::setw(width) << diff_data.get_io_wait_time() << ", ";
    statistics_stream << std::setw(width) << diff_data.get_pread_time() << ", ";
    statistics_stream << std::setw(width) << diff_data.get_pwrite_time() << ", ";
    statistics_stream << std::setw(width) << diff_data.get_elapsed_time() << ", ";
    CONSOLE_PRINTLN(statistics_stream.str());
}
unsigned int get_expected_read_io_currently() {
    return 0;
}

unsigned int get_expected_write_io_currently(ExtLPDSSet& extDataSet) {
    unsigned int a_blocks_count = ceil((double) extDataSet.A.get_element_size() * extDataSet.A.get_overall_nnz() / MATRIX_A_BLOCK_SIZE);
    unsigned int a_col_meta_blocks_count = ceil((double) extDataSet.A.get_col_meta_element_size()*((double) extDataSet.A.get_columns_count() / MATRIX_A_COL_DATA_BLOCK_SIZE));
    unsigned int a_row_meta_blocks_count = ceil((double) extDataSet.A.get_row_meta_element_size()*((double) extDataSet.A.get_rows_count() / MATRIX_A_ROW_DATA_BLOCK_SIZE));
    a_blocks_count += (a_col_meta_blocks_count + a_row_meta_blocks_count);
    return a_blocks_count;
}

unsigned int get_expected_io_currently(ExtLPDSSet& extDataSet) {
    return get_expected_read_io_currently() + get_expected_write_io_currently(extDataSet);
}



int main(int argc, char *argv[]) {
    //    printUsage(argv);
    std::setvbuf(stdout, NULL, _IONBF, 0);
    std::setvbuf(stderr, NULL, _IONBF, 0);
    Console::printVersionInfo();
    const char* filename;
    CONSOLE_PRINT("Number of arguments: " << argc);
    try {
        int optidx;
        for (optidx = 1; optidx < argc; optidx++) {
            if (*argv[optidx] != '-')
                break;
            switch (argv[optidx][1]) {
                case 't':
                    CONSOLE_PRINT("timeout");
                    break;

                case 'L':
                    CONSOLE_PRINT("Iterations Limit");
                    break;
                case 'h':
                case '?':
                    Console::printVersionInfo();
                    //lint -fallthrough
                default:
                    Console::printUsage(argv);
            }
        }
        // substract current stats from stats at the beginning of the measurement
        filename = argv[optidx];
        ///////////// - Reading File Section
        DEBUG_SIMPLE("Accepted Input File: " << filename);
        ReadFileUtil inputReader;
        ///////////// - Initializing data structures
        // Names of the Rows and Cols involved in the 
        fixed_name_map myRowMap((fixed_name_map::node_block_type::raw_size) * DATA_NODE_CACHE_COUNT,
                (fixed_name_map::leaf_block_type::raw_size) * DATA_LEAF_CACHE_COUNT);
        fixed_name_map myColMap((fixed_name_map::node_block_type::raw_size) * DATA_NODE_CACHE_COUNT,
                (fixed_name_map::leaf_block_type::raw_size) * DATA_LEAF_CACHE_COUNT);
        ExtNameMap mapRowName(myRowMap);
        ExtNameMap mapColName(myColMap);
        ExtMatrixA A;
        ExtMatrixBInverse BInverse;
        ExtLPDSSet extDataSet(A, BInverse, mapRowName, mapColName);

        unsigned int /*expected_io = 0,*/ expected_read_io = 0, expected_write_io = 0;
        stxxl::stats * Stats = stxxl::stats::get_instance();
        stxxl::stats_data stats_begin(*Stats);

        inputReader.readFileUsingDisk(filename, extDataSet);

        //        expected_io = get_expected_io_currently(extDataSet);
        expected_read_io = get_expected_read_io_currently();
        expected_write_io = get_expected_write_io_currently(extDataSet);
        stxxl::stats_data diff_data = stxxl::stats_data(*Stats) - stats_begin;
        print_io_statistics(diff_data, extDataSet, expected_read_io, expected_write_io, true);
        //         print_solution();


        ExtSimplex ext_simplex(extDataSet);
        ext_simplex.solve();


        ///////////////////// --- Test Sections
        //                PackedVector::test();
        //        Test::testPackedVector2();
        //                Test::testExtVector();
        //        Test::testExtPackedVector();
        //        FixedStringUtil::test();
        //        ExtNameMap::test();
        //        ExtStringVector::test();
        //        Test::testLinkedList();
        //        Test::testExtNameMap();
        //        Test::testExtMatrixA(extDataSet);
        //        extDataSet.A.standardize_matrix();
        //        Test::testExtMatrixA(extDataSet);
        //        Test::testMatrixBInverse(extDataSet);
        //////////////////// --- Statistics Sections
        //        Console::println("--- * Map Statistics * ---");
        //        mapRowName.displayStorageStatus();
        //        mapColName.displayStorageStatus();
        //        Console::println("--- *** ---");
        CONSOLE_PRINTLN(stxxl::stats_data(*Stats) - stats_begin);
        CONSOLE_PRINTLN("Program Completed Execution!!!");
        DEBUG_MEMORY("Memory Left at the end of program.");
    } catch (int) {
        std::cout << "Error dude";
    }
    return 0;
}