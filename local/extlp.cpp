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
        // generate stats instance
        stxxl::stats * Stats = stxxl::stats::get_instance();
        // start measurement here
        stxxl::stats_data stats_begin(*Stats);
        // some computation ...
        // substract current stats from stats at the beginning of the measurement
        filename = argv[optidx];
        ///////////// - Reading File Section
        DEBUG_SIMPLE("Accepted Input File: " << filename);
        ReadFileUtil inputReader;
        ///////////// - Initializing data structures
        // Names of the Rows and Cols involved in the 
        fixed_name_map myRowMap((fixed_name_map::node_block_type::raw_size)*5, (fixed_name_map::leaf_block_type::raw_size)*5);
        fixed_name_map myColMap((fixed_name_map::node_block_type::raw_size)*5, (fixed_name_map::leaf_block_type::raw_size)*5);
        ExtNameMap mapRowName(myRowMap);
        ExtNameMap mapColName(myColMap);
        ExtMatrixA A;
        ExtLPDSSet extDataSet(A, mapRowName, mapColName);
        inputReader.readFileUsingDisk(filename, extDataSet);

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