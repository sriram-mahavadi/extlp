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
#include "ExtLPCol.h"
#include "ExtLPMatrix.h"
#include "PackedVector.h"
#include "PackedVectorUtil.h"
#include "ReadFileUtil.h"
/// Process NAME section.

//static void readName(MPSInput& mps) {
//    do {
//        // This has to be the Line with the NAME section.
//        if (!mps.readLine()
//                || (mps.field0() == 0) || strcmp(mps.field0(), "NAME"))
//            break;
//        //      bool value = mps.readLine();
//        //      std::cout<<value<<"\n";
//        // Sometimes the name is omitted.
//        mps.setProbName((mps.field1() == 0) ? "_MPS_" : mps.field1());
//        //      std::cout<<"Field 0: "<<mps.field0()<<"\n";
//        //      std::cout<<"Field 1: "<<mps.field1()<<"\n";
//        //      
//        //      std::cout<<"Problem Name: "<<mps.probName()<<"\n";
//        //      MSG_INFO2( spxout << "IMPSRD01 Problem name   : " << mps.probName()
//        //                           << std::endl; )
//
//        // This hat to be a new section
//        if (!mps.readLine() || (mps.field0() == 0))
//            break;
//
//        if (!strcmp(mps.field0(), "ROWS"))
//            mps.setSection(MPSInput::ROWS);
//        else if (!strncmp(mps.field0(), "OBJSEN", 6))
//            mps.setSection(MPSInput::OBJSEN);
//        else if (!strcmp(mps.field0(), "OBJNAME"))
//            mps.setSection(MPSInput::OBJNAME);
//        else
//            break;
//
//        return;
//    } while (false);
//
//    mps.syntaxError();
//}
//
///// Process OBJSEN section. This Section is an ILOG extension.
//
//static void readObjsen(MPSInput& mps) {
//    do {
//        // This has to be the Line with MIN or MAX.
//        if (!mps.readLine() || (mps.field1() == 0))
//            break;
//
//        if (!strcmp(mps.field1(), "MIN"))
//            mps.setObjSense(false);
//        else if (!strcmp(mps.field1(), "MAX"))
//            mps.setObjSense(true);
//        else
//            break;
//
//        // Look for ROWS or OBJNAME Section
//        if (!mps.readLine() || (mps.field0() == 0))
//            break;
//
//        if (!strcmp(mps.field0(), "ROWS"))
//            mps.setSection(MPSInput::ROWS);
//        else if (!strcmp(mps.field0(), "OBJNAME"))
//            mps.setSection(MPSInput::OBJNAME);
//        else
//            break;
//
//        return;
//    } while (false);
//
//    mps.syntaxError();
//}
//
///// Process OBJNAME section. This Section is an ILOG extension.
//
//static void readObjname(MPSInput& mps) {
//    do {
//        // This has to be the Line with the name.
//        if (!mps.readLine() || (mps.field1() == 0))
//            break;
//
//        mps.setObjName(mps.field1());
//
//        // Look for ROWS Section
//        if (!mps.readLine() || (mps.field0() == 0))
//            break;
//
//        if (strcmp(mps.field0(), "ROWS"))
//            break;
//
//        mps.setSection(MPSInput::ROWS);
//        return;
//    } while (false);
//
//    mps.syntaxError();
//}
//
///// Process ROWS section. 
//
//static void readRows(
//        MPSInput& mps,
//        std::vector<LPRow> &arrRow,
//        std::map<std::string, int> &mapRowNumber) {
//    //    LPRow row;
//    DEBUG_PARSER("Started Importing/Reading Rows")
//    LPRow row;
//    int i = 0;
//    while (mps.readLine()) {
//        if (mps.field0() != 0) {
//            //            MSG_INFO2(spxout << "IMPSRD02 Objective name : " << mps.objName()
//            //                    << std::endl;)
//
//            if (strcmp(mps.field0(), "COLUMNS"))
//                break;
//
//            mps.setSection(MPSInput::COLUMNS);
//            goto endReadRows;
//        }
//        if (*mps.field1() == 'N') {
//            if (*mps.objName() == '\0')
//                mps.setObjName(mps.field2());
//        } else {
//            // getting the row already existing
//            //            if (rnames.has(mps.field2()))
//            //                break;
//            if (mapRowNumber.find(mps.field2()) != mapRowNumber.end())
//                break;
//            //            rnames.add(mps.field2());
//            //            arrRow.push_back(mps.field2());
//            mapRowNumber[mps.field2()] = i++;
//            row.setName(mps.field2());
//            switch (*mps.field1()) {
//                case 'G':
//                    row.setLhs(0.0);
//                    row.setRhs(INFINITY_VALUE);
//                    break;
//                case 'E':
//                    row.setLhs(0.0);
//                    row.setRhs(0.0);
//                    break;
//                case 'L':
//                    row.setLhs(-INFINITY_VALUE);
//                    row.setRhs(0.0);
//                    break;
//                default:
//                    goto endReadRowsWithError;
//            }
//            //            rset.add(row);
//            arrRow.push_back(row);
//        }
//        //        assert((*mps.field1() == 'N')
//        //                || (rnames.number(mps.field2()) == rset.num() - 1));
//    }
//endReadRowsWithError:
//    mps.syntaxError();
//endReadRows:
//    DEBUG_PARSER("End of Importing/Reading Rows")
//    return;
//}
//
///// Process COLUMNS section. 
//
//static void readCols(
//        MPSInput& mps,
//        std::vector<LPRow> &arrRow,
//        std::map<std::string, int> &mapRowNumber,
//        std::vector<LPCol> &arrCol,
//        std::map<std::string, int> &mapColNumber
//        ) {
//    DEBUG_PARSER("Started Reading Columns")
//    REAL val;
////    int idx;
//    char colname[MPSInput::MAX_LINE_LEN] = {'\0'};
//    LPCol col(arrRow.size());
//    //    DSVector vec;
//    std::vector<REAL> vec(arrRow.size());
//
//    DEBUG_PARSER("Size of vector while Initialization: " << vec.size());
//    col.setObj(0.0F);
//    //    vec.clear();
//
//    int i = 0, lines = 0;
//    while (mps.readLine()) {
//        DEBUG_PARSER("Lines Read: " << lines++ << "; Columns Read: " << i)
//        if (mps.field0() != 0) {
//            if (strcmp(mps.field0(), "RHS")) {
//                DEBUG_PARSER("Next Section is RHS")
//                break;
//            }
//
//            if (colname[0] != '\0') {
//                col.setColVector(vec);
//                arrCol.push_back(col);
//            }
//            mps.setSection(MPSInput::RHS);
//            return;
//        }
//        if ((mps.field1() == 0) || (mps.field2() == 0) || (mps.field3() == 0))
//            break;
//
//        // new column?
//        if (strcmp(colname, mps.field1())) {
//            // first column?
//            //            DEBUG_PARSER("Reading New Column");
//            if (colname[0] != '\0') {
//                col.setColVector(vec);
//                arrCol.push_back(col);
//            }
//            // save copy of string (make sure string ends with \0)
//            strncpy(colname, mps.field1(), MPSInput::MAX_LINE_LEN - 1);
//            colname[MPSInput::MAX_LINE_LEN - 1] = '\0';
//            mapColNumber[colname] = i++;
//            col.setName(colname);
//
//            //            cnames.add(colname);
//            // Initializing back to default values
//            vec.clear();
//            vec.resize(arrRow.size());
//            col.setObj(0.0);
//            col.setLower(0.0);
//            col.setUpper(INFINITY_VALUE);
//
//            //            if (mps.isInteger()) {
//            //                assert(cnames.number(colname) == cset.num());
//            //
//            //                if (intvars != 0)
//            //                    intvars->addIdx(cnames.number(colname));
//            //
//            //                // For Integer variable the default bounds are 0/1 
//            //                col.setUpper(1.0);
//            //            }
//        }
//        val = atof(mps.field3());
//
//        if (!strcmp(mps.field2(), mps.objName()))
//            col.setObj(val);
//        else {
//            // Getting row number for given row
//            //            if ((idx = rnames.number(mps.field2())) < 0)
//            if (mapRowNumber.find(mps.field2()) == mapRowNumber.end())
//                mps.entryIgnored("Column", mps.field1(), "row", mps.field2());
//            else {
//                int rowNumber = mapRowNumber[mps.field2()];
//                if (val != 0.0)
//                    vec[rowNumber] = val; //.add(idx, val);
//            }
//        }
//        if (mps.field5() != 0) {
//            assert(mps.field4() != 0);
//
//            val = atof(mps.field5());
//
//            if (!strcmp(mps.field4(), mps.objName()))
//                col.setObj(val);
//            else {
//                if (mapRowNumber.find(mps.field4()) == mapRowNumber.end())
//                    mps.entryIgnored("Column", mps.field1(), "row", mps.field2());
//                else {
//                    int rowNumber = mapRowNumber[mps.field4()];
//                    if (val != 0.0)
//                        vec[rowNumber] = val; //.add(idx, val);
//                }
//            }
//        }
//    }
//    DEBUG_PARSER("End of Reading Columns")
//    mps.syntaxError();
//}
//
///// Process RHS section. 
//
//static void readRhs(
//        MPSInput& mps,
//        std::vector<LPRow> &arrRow,
//        std::map<std::string, int> &mapRowNumber
//        ) {
//    char rhsname[MPSInput::MAX_LINE_LEN] = {'\0'};
//    char addname[MPSInput::MAX_LINE_LEN] = {'\0'};
//    int idx;
//    REAL val;
//
//    while (mps.readLine()) {
//        if (mps.field0() != 0) {
//            //            MSG_INFO2(spxout << "IMPSRD03 RHS name       : " << rhsname
//            //                    << std::endl;);
//
//            if (!strcmp(mps.field0(), "RANGES"))
//                mps.setSection(MPSInput::RANGES);
//            else if (!strcmp(mps.field0(), "BOUNDS"))
//                mps.setSection(MPSInput::BOUNDS);
//            else if (!strcmp(mps.field0(), "ENDATA"))
//                mps.setSection(MPSInput::ENDATA);
//            else
//                break;
//            goto endReadRhs;
//            //            return;
//        }
//        if (((mps.field2() != 0) && (mps.field3() == 0))
//                || ((mps.field4() != 0) && (mps.field5() == 0)))
//            mps.insertName("_RHS_");
//
//        if ((mps.field1() == 0) || (mps.field2() == 0) || (mps.field3() == 0))
//            break;
//
//        if (*rhsname == '\0')
//            strcpy(rhsname, mps.field1());
//
//        if (strcmp(rhsname, mps.field1())) {
//            if (strcmp(addname, mps.field1())) {
//                assert(strlen(mps.field1()) < MPSInput::MAX_LINE_LEN);
//                strcpy(addname, mps.field1());
//                DEBUG_PARSER("Duplicate RHS ignored: " << addname);
//                //                        << std::endl;);
//                //                MSG_INFO3(spxout << "IMPSRD07 RHS ignored    : " << addname
//                //                        << std::endl;);
//            }
//        } else {
//            //            if ((idx = rnames.number(mps.field2())) < 0)
//            if (mapRowNumber.find(mps.field2()) == mapRowNumber.end())
//                mps.entryIgnored("RHS", mps.field1(), "row", mps.field2());
//            else {
//                idx = mapRowNumber[mps.field2()];
//                val = atof(mps.field3());
//                // LE or EQ
//                //                if (rset.rhs(idx) < infinity)
//                //                    rset.rhs_w(idx) = val;
//                if (arrRow[idx].rhs() < INFINITY_VALUE)
//                    arrRow[idx].setRhs(val);
//                // GE or EQ
//                if (arrRow[idx].lhs() > -INFINITY_VALUE)
//                    arrRow[idx].setLhs(val);
//                //                if (rset.lhs(idx) > -infinity)
//                //                    rset.lhs_w(idx) = val;
//            }
//            if (mps.field5() != 0) {
//                //                if ((idx = rnames.number(mps.field4())) < 0)
//                if (mapRowNumber.find(mps.field4()) == mapRowNumber.end())
//                    mps.entryIgnored("RHS", mps.field1(), "row", mps.field4());
//                else {
//                    idx = mapRowNumber[mps.field4()];
//                    val = atof(mps.field5());
//                    // LE or EQ
//                    //                    if (rset.rhs(idx) < infinity)
//                    //                        rset.rhs_w(idx) = val;
//                    if (arrRow[idx].rhs() < INFINITY_VALUE)
//                        arrRow[idx].setRhs(val);
//                    // GE or EQ
//                    //                    if (rset.lhs(idx) > -infinity)
//                    //                        rset.lhs_w(idx) = val;
//                    if (arrRow[idx].lhs() > -INFINITY_VALUE)
//                        arrRow[idx].setLhs(val);
//                }
//            }
//        }
//    }
//endReadRhsWithError:
//    mps.syntaxError();
//endReadRhs:
//    mps.setRhsName(rhsname);
//}
//
//bool readMPS(std::istream &is, std::vector<std::string> &rowNames, std::vector<std::string> &colNames) {
//    //MPS
//    DEBUG_PARSER("Reading MPS File");
//    MPSInput mpsInput(is);
//    readName(mpsInput);
//    //    mpsSectionData = mpsSectionData + "Name: " + mpsInput.probName() + "\r\n";
//    if (mpsInput.section() == MPSInput::OBJSEN) {
//        readObjsen(mpsInput);
//        DEBUG_PARSER("In ObjSen Section Data!!!");
//        //        mpsSectionData = mpsSectionData + "ObjSense: " + (mpsInput.objSense() ? "Min" : "Max") + "\r\n";
//    }
//    if (mpsInput.section() == MPSInput::OBJNAME) {
//        readObjname(mpsInput);
//        DEBUG_PARSER("In ObjName Section Data!!!");
//        //        mpsSectionData = mpsSectionData + "ObjName: " + mpsInput.objName() + "\r\n";
//    }
//    // TODO: Temporarily setting up in std vector
//    // To switch to STXXL library for storage... :)
//    std::map<std::string, int> mapRowNumber;
//    std::vector<LPRow> arrRow;
//    if (mpsInput.section() == MPSInput::ROWS) {
//        readRows(mpsInput, arrRow, mapRowNumber);
//        DEBUG_PARSER("In Rows Section Data!!!");
//        //        ///////////////////////////////////////////////////////////////
//        //        /// Debugging the logic and results
//        //        ////////////////////////////////////////////////////////////////
//        //        DEBUG_PARSER("Rows are As Follows")
//        //
//        //                int i = 0;
//        //        //        for(;i<arrRow.size(); i++){
//        //        //            LPRow &row = arrRow[i];
//        //        //            std::cout<<"Row "<<i<<": "<<"r\n";
//        //        //        }
//        //        std::map<std::string, int>::iterator itrMapRow = mapRowNumber.begin();
//        //        while (itrMapRow != mapRowNumber.end()) {
//        //            DEBUG_PARSER(itrMapRow->first << " : " << itrMapRow->second)
//        //            //mpsSectionData = mpsSectionData + itrMapRow->first.c_str() + " : " + itrMapRow->second + "r\n";
//        //            itrMapRow++;
//        //        }
//        //        DEBUG_SIMPLE("Number of rows imported: " << arrRow.size());
//        //        DEBUG_SIMPLE("Identified to optimize the Objective: " << mpsInput.objName());
//        //        //////////////////////////////////////////////////////////////////
//    }
//
//    //   addedRows(rset.num());
//    std::map<std::string, int> mapColNumber;
//    std::vector<LPCol> arrCol;
//    if (mpsInput.section() == MPSInput::COLUMNS) {
//        //        readCols(mpsInput, arrRow, mapRowNumber, arrCol, mapColNumber, p_intvars);
//        readCols(mpsInput, arrRow, mapRowNumber, arrCol, mapColNumber);
//        DEBUG_PARSER("In Cols Section Data!!!");
//    }
//
//    if (mpsInput.section() == MPSInput::RHS) {
//        readRhs(mpsInput, arrRow, mapRowNumber);
//        DEBUG_PARSER("In Rhs Section Data!!!");
//    }
//    ///////////////////////////////////////////////////////////////
//    /// Debugging the logic and results
//    ////////////////////////////////////////////////////////////////
//    //    DEBUG_PARSER("Objective Values read are: ");
//    //    for (int i = 0; i < arrCol.size(); i++) {
//    //        DEBUG_PARSER(i << ": " << arrCol[i].obj());
//    //    }
//    //    std::map<std::string, int>::iterator itrMapCol = mapColNumber.begin();
//    //    DEBUG_PARSER("Columns are Read as Follows: ");
//    //    while (itrMapCol != mapColNumber.end()) {
//    //        DEBUG_PARSER(itrMapCol->first << " : " << itrMapCol->second);
//    //        //mpsSectionData = mpsSectionData + itrMapRow->first.c_str() + " : " + itrMapRow->second + "r\n";
//    //        itrMapCol++;
//    //    }
//    DEBUG_SIMPLE("Number of Columns imported: " << arrCol.size());
//
//    DEBUG_PARSER("Problem Name: " << mpsInput.probName());
//    DEBUG_PARSER("Partial Simplex Tableau is shown as follows: ");
//    unsigned int i = 0, j = 0;
//
//    std::stringstream titleStream;
//    titleStream << setw(10) << "Row/Col" << ": ";
//    for (j = 0; j < arrCol.size(); j++)
//        titleStream << setw(10) << arrCol[j].getName() << ", ";
//    titleStream << setw(10) << mpsInput.rhsName() << ", ";
//    DEBUG_PARSER(titleStream.str());
//
//    std::stringstream objStream;
//    std::string objTitle("OBJ-");
//    objTitle = objTitle + mpsInput.objName();
//    objStream << setw(10) << objTitle << ": ";
//    for (i = 0; i < arrCol.size(); i++) {
//        objStream << setw(10) << arrCol[j].obj() << ", ";
//    }
//    /// Rhs Column in objective constraint
//    objStream << setw(10) << 0 << ", ";
//    DEBUG_PARSER(objStream.str());
//    for (i = 0; i < arrRow.size(); i++) {
//        //            string rowName(arrRow[i].)
//        std::stringstream rowStream;
//        rowStream << setw(10) << arrRow[i].getName() << ": ";
//        for (j = 0; j < arrCol.size(); j++) {
//            //                mapColNumber[]
//            rowStream << setw(10) << arrCol[j].getRowElement(i) << ", ";
//        }
//        rowStream << setw(10) << arrRow[i].rhs() << ", ";
//        DEBUG_PARSER(rowStream.str());
//    }
//    /////////////////////////////////////////////////////////////////////////
//
//
//    //   if (mps.section() == MPSInput::RANGES)
//    //      readRanges(mps, rset, *rnames);
//    //
//    //   if (mps.section() == MPSInput::BOUNDS)
//    //      readBounds(mps, cset, *cnames, p_intvars);
//    //
//    //   if (mps.section() != MPSInput::ENDATA)
//    //      mps.syntaxError();
//    //
//    //   if (mps.hasError())
//    //      clear();
//    //   else
//    //   {
//    //      changeSense(mps.objSense());
//
//    //      MSG_INFO2(
//    //         spxout << "IMPSRD06 Objective sense: " 
//    //                << ((mps.objSense() == MINIMIZE) ? "Minimize" : "Maximize") 
//    //                << std::endl;         
//    //      )
//    //
//    //      added2Set(
//    //         *(reinterpret_cast<SVSet*>(static_cast<LPRowSet*>(this))), 
//    //         *(reinterpret_cast<SVSet*>(static_cast<LPColSet*>(this))), 
//    //         cset.num());
//    //      addedCols(cset.num());
//    //      assert(isConsistent());
//    //   }
//
//    return false;
//}
//
//bool readLPF(std::istream &is, std::vector<std::string> &rowNames, std::vector<std::string> &colNames) {
//    std::cout << "Reading lpf file\n";
//    return false;
//}
//
//bool readFile(const char *filename) {
//    std::ifstream file(filename);
//    std::cout << "Reading File\n";
//    if (!file) {
//        std::cout << "File Not present";
//        return false;
//    }
//    char c;
//
//    file.get(c);
//    file.putback(c);
//    std::vector<std::string> rowNames, colNames;
//    /* MPS starts either with a comment mark '*' or with the keyword
//     * 'NAME' at the first column.
//     * LPF starts either with blanks, a comment mark '\' or with
//     * the keyword "MAX" or "MIN" in upper or lower case.
//     * There is no possible valid LPF file starting with a '*' or 'N'.
//     */
//    bool ok = ((c == '*') || (c == 'N'))
//            ? readMPS(file, rowNames, colNames)
//            : readLPF(file, rowNames, colNames);
//    return ok;
//
//}

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
        filename = argv[optidx];
        DEBUG_SIMPLE( "Accepted Input File: " << filename );
        ReadFileUtil inputReader;
        inputReader.readFile(filename);
        ExtLPCol col;
        col.displayElements();
        ExtLPMatrix matrix;
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