/* 
 * File:   ExtReadMPSUtil.h
 * Author: Sriram Mahavadi
 *
 * Created on 7 April, 2014, 9:54 AM
 */

#ifndef EXTREADMPSUTIL_H
#define	EXTREADMPSUTIL_H
#include "GlobalDefines.h"
#include "ExtLPDSSet.h"
#include "mpsinput.h"
class ExtReadMPSUtil {
public:
    /// Process NAME section.
    static void readName(MPSInput& mps) {
        do {
            // This has to be the Line with the NAME section.
            if (!mps.readLine()
                    || (mps.field0() == 0) || strcmp(mps.field0(), "NAME"))
                break;
            //      bool value = mps.readLine();
            //      std::cout<<value<<"\n";
            // Sometimes the name is omitted.
            mps.setProbName((mps.field1() == 0) ? "_MPS_" : mps.field1());
            //      std::cout<<"Field 0: "<<mps.field0()<<"\n";
            //      std::cout<<"Field 1: "<<mps.field1()<<"\n";
            //      
            //      std::cout<<"Problem Name: "<<mps.probName()<<"\n";
            //      MSG_INFO2( spxout << "IMPSRD01 Problem name   : " << mps.probName()
            //                           << std::endl; )

            // This hat to be a new section
            if (!mps.readLine() || (mps.field0() == 0))
                break;

            if (!strcmp(mps.field0(), "ROWS"))
                mps.setSection(MPSInput::ROWS);
            else if (!strncmp(mps.field0(), "OBJSEN", 6))
                mps.setSection(MPSInput::OBJSEN);
            else if (!strcmp(mps.field0(), "OBJNAME"))
                mps.setSection(MPSInput::OBJNAME);
            else
                break;

            return;
        } while (false);

        mps.syntaxError();
    }

    /// Process OBJSEN section. This Section is an ILOG extension.
    static void readObjsen(MPSInput& mps) {
        do {
            // This has to be the Line with MIN or MAX.
            if (!mps.readLine() || (mps.field1() == 0))
                break;

            if (!strcmp(mps.field1(), "MIN"))
                mps.setObjSense(false);
            else if (!strcmp(mps.field1(), "MAX"))
                mps.setObjSense(true);
            else
                break;

            // Look for ROWS or OBJNAME Section
            if (!mps.readLine() || (mps.field0() == 0))
                break;

            if (!strcmp(mps.field0(), "ROWS"))
                mps.setSection(MPSInput::ROWS);
            else if (!strcmp(mps.field0(), "OBJNAME"))
                mps.setSection(MPSInput::OBJNAME);
            else
                break;

            return;
        } while (false);

        mps.syntaxError();
    }

    /// Process OBJNAME section. This Section is an ILOG extension.
    static void readObjname(MPSInput& mps) {
        do {
            // This has to be the Line with the name.
            if (!mps.readLine() || (mps.field1() == 0))
                break;

            mps.setObjName(mps.field1());

            // Look for ROWS Section
            if (!mps.readLine() || (mps.field0() == 0))
                break;

            if (strcmp(mps.field0(), "ROWS"))
                break;

            mps.setSection(MPSInput::ROWS);
            return;
        } while (false);

        mps.syntaxError();
    }

    /// Process ROWS section. 
    static void readRows(
            MPSInput& mps,
            ExtLPDSSet &extDataSet) {
        //    LPRow row;
        //        DEBUG_PARSER("Started Importing/Reading Rows")
        //        LPRow row;
        PackedRowVector row;
        unsigned int i = 0;
        while (mps.readLine()) {
            if (mps.field0() != 0) {
                //            MSG_INFO2(spxout << "IMPSRD02 Objective name : " << mps.objName()
                //                    << std::endl;)

                if (strcmp(mps.field0(), "COLUMNS"))
                    break;
                DEBUG_PARSER("Next Section found to be COLUMNS");
                mps.setSection(MPSInput::COLUMNS);
                goto endReadRows;
            }
            if (*mps.field1() == 'N') {
                if (*mps.objName() == '\0') {
                    mps.setObjName(mps.field2());
                    extDataSet.setObjName(mps.objName());
                }
            } else {
                //                if (mapRowNumber.find(mps.field2()) != mapRowNumber.end())
                //                    break;
                if (extDataSet.mapRowName.contains(mps.field2()))
                    break;
                extDataSet.mapRowName.set(mps.field2(), i++);
                row.set_name(mps.field2());
                //                mapRowNumber[mps.field2()] = i++;
                //                row.setName(mps.field2());
                switch (*mps.field1()) {
                    case 'G':
                        row.set_lhs(0.0);
                        row.set_rhs(INFINITY_VALUE);
                        break;
                    case 'E':
                        row.set_lhs(0.0);
                        row.set_rhs(0.0);
                        break;
                    case 'L':
                        row.set_lhs(-INFINITY_VALUE);
                        row.set_rhs(0.0);
                        break;
                    default:
                        goto endReadRowsWithError;
                }
                //            rset.add(row);
                //                arrRow.push_back(row);
                ExtRowVector extRow(row);
                extDataSet.vctRows.push_back(extRow);
            }
            //        assert((*mps.field1() == 'N')
            //                || (rnames.number(mps.field2()) == rset.num() - 1));
        }
endReadRowsWithError:
        mps.syntaxError();
endReadRows:
        //        DEBUG_PARSER("End of Importing/Reading Rows")
        return;
    }

    /// Process COLUMNS section. 
    static void readCols(
            MPSInput& mps,
            ExtLPDSSet& extDataSet
            ) {
        //        DEBUG_PARSER("Started Reading Columns")
        REAL val;
        //    int idx;
        char colname[MPSInput::MAX_LINE_LEN] = {'\0'};
        //        LPCol col(arrRow.size());
        PackedColVector col(extDataSet.vctRows.size());
        //    DSVector vec;
        //        std::vector<REAL> vec(arrRow.size());
        std::vector<REAL> vec(extDataSet.vctRows.size());
        static int count_cols_read = 0;
        //        DEBUG_PARSER("Size of vector while Initialization: " << vec.size());
        col.set_objective_value(0.0F);
        //    vec.clear();

        int i = 0;
        //        int lines = 0;
        while (mps.readLine()) {
            //            DEBUG_PARSER("Lines Read: " << lines++ << "; Columns Read: " << i)
            if (mps.field0() != 0) {
                if (strcmp(mps.field0(), "RHS")) {
                    DEBUG_PARSER("Next Section is RHS")
                    break;
                }

                if (colname[0] != '\0') {
                    col.set_col_vector(vec);
                    ExtColVector extCol(col);
                    extDataSet.vctCols.push_back(extCol);
                    count_cols_read++;
                    DEBUG_SIMPLE("Read Col " << count_cols_read << ", with name: " << extCol.getName() << ", Sparsity: " << extCol.get_sparsity());
                    //                    arrCol.push_back(col);
                }
                mps.setSection(MPSInput::RHS);
                return;
            }
            if ((mps.field1() == 0) || (mps.field2() == 0) || (mps.field3() == 0))
                break;

            // new column?
            if (strcmp(colname, mps.field1())) {
                // first column?
                //            DEBUG_PARSER("Reading New Column");
                if (colname[0] != '\0') {
                    col.set_col_vector(vec);
                    //                    DEBUG_PARSER("Reading ColName: "<<colname<<"; "<<col.getName());
                    ExtColVector extCol(col);
                    //                    DEBUG_PARSER("Reading ColName: "<<colname<<"; "<<extCol.getName());
                    extDataSet.vctCols.push_back(extCol);
                    count_cols_read++;
                    DEBUG_SIMPLE("Read Col " << count_cols_read << ", with name: " << extCol.getName() << ", Sparsity: " << extCol.get_sparsity());
                    //                    arrCol.push_back(col);
                }
                // save copy of string (make sure string ends with \0)
                strncpy(colname, mps.field1(), MPSInput::MAX_LINE_LEN - 1);
                colname[MPSInput::MAX_LINE_LEN - 1] = '\0';
                extDataSet.mapColName.set(colname, i++);
                //                mapColNumber[colname] = i++;
                col.set_name(colname);

                //            cnames.add(colname);
                // Initializing back to default values
                vec.clear();
                //                vec.resize(arrRow.size());
                vec.resize(extDataSet.vctRows.size());
                col.set_objective_value(0.0);
                col.set_lower_bound(0.0);
                col.set_upper_bound(INFINITY_VALUE);

                //            if (mps.isInteger()) {
                //                assert(cnames.number(colname) == cset.num());
                //
                //                if (intvars != 0)
                //                    intvars->addIdx(cnames.number(colname));
                //
                //                // For Integer variable the default bounds are 0/1 
                //                col.setUpper(1.0);
                //            }
            }
            val = atof(mps.field3());

            if (!strcmp(mps.field2(), mps.objName()))
                col.set_objective_value(val);
            else {
                // Getting row number for given row
                //            if ((idx = rnames.number(mps.field2())) < 0)
                //                if (mapRowNumber.find(mps.field2()) == mapRowNumber.end())
                if (!extDataSet.mapRowName.contains(mps.field2()))
                    mps.entryIgnored("Column", mps.field1(), "row", mps.field2());
                else {
                    //                    int rowNumber = mapRowNumber[mps.field2()];
                    int rowNumber = extDataSet.mapRowName.get(mps.field2());
                    if (val != 0.0)
                        vec[rowNumber] = val; //.add(idx, val);
                }
            }
            if (mps.field5() != 0) {
                assert(mps.field4() != 0);

                val = atof(mps.field5());

                if (!strcmp(mps.field4(), mps.objName()))
                    col.set_objective_value(val);
                else {
                    //                    if (mapRowNumber.find(mps.field4()) == mapRowNumber.end())
                    if (!extDataSet.mapRowName.contains(mps.field4()))
                        mps.entryIgnored("Column", mps.field1(), "row", mps.field4());
                    else {
                        //                        int rowNumber = mapRowNumber[mps.field4()];
                        int rowNumber = extDataSet.mapRowName.get(mps.field4());
                        if (val != 0.0)
                            vec[rowNumber] = val; //.add(idx, val);
                    }
                }
            }
        }
        //        DEBUG_PARSER("End of Reading Columns")
        mps.syntaxError();
    }
    /// Process RHS section. 
    static void readRhs(
            MPSInput& mps,
            ExtLPDSSet& extDataSet
            ) {
        char rhsname[MPSInput::MAX_LINE_LEN] = {'\0'};
        char addname[MPSInput::MAX_LINE_LEN] = {'\0'};
        int idx;
        REAL val;

        while (mps.readLine()) {
            if (mps.field0() != 0) {
                //            MSG_INFO2(spxout << "IMPSRD03 RHS name       : " << rhsname
                //                    << std::endl;);

                if (!strcmp(mps.field0(), "RANGES"))
                    mps.setSection(MPSInput::RANGES);
                else if (!strcmp(mps.field0(), "BOUNDS"))
                    mps.setSection(MPSInput::BOUNDS);
                else if (!strcmp(mps.field0(), "ENDATA"))
                    mps.setSection(MPSInput::ENDATA);
                else
                    break;
                goto endReadRhs;
                //            return;
            }
            if (((mps.field2() != 0) && (mps.field3() == 0))
                    || ((mps.field4() != 0) && (mps.field5() == 0)))
                mps.insertName("_RHS_");

            if ((mps.field1() == 0) || (mps.field2() == 0) || (mps.field3() == 0))
                break;

            if (*rhsname == '\0')
                strcpy(rhsname, mps.field1());

            if (strcmp(rhsname, mps.field1())) {
                if (strcmp(addname, mps.field1())) {
                    assert(strlen(mps.field1()) < MPSInput::MAX_LINE_LEN);
                    strcpy(addname, mps.field1());
                    DEBUG_PARSER("Duplicate RHS ignored: " << addname);
                    //                        << std::endl;);
                    //                MSG_INFO3(spxout << "IMPSRD07 RHS ignored    : " << addname
                    //                        << std::endl;);
                }
            } else {
                //            if ((idx = rnames.number(mps.field2())) < 0)
                //                if (mapRowNumber.find(mps.field2()) == mapRowNumber.end())
                if (!extDataSet.mapRowName.contains(mps.field2()))
                    mps.entryIgnored("RHS", mps.field1(), "row", mps.field2());
                else {
                    idx = extDataSet.mapRowName.get(mps.field2());
                    val = atof(mps.field3());
                    // LE or EQ
                    //                if (rset.rhs(idx) < infinity)
                    //                    rset.rhs_w(idx) = val;
                    //                    if (arrRow[idx].rhs() < INFINITY_VALUE)
                    if (extDataSet.vctRows[idx].getRhs() < INFINITY_VALUE)
                        extDataSet.vctRows[idx].setRhs(val);
                    // GE or EQ
                    //                    if (arrRow[idx].lhs() > -INFINITY_VALUE)
                    if (extDataSet.vctRows[idx].getLhs() > -INFINITY_VALUE)
                        extDataSet.vctRows[idx].setLhs(val);
                    //                if (rset.lhs(idx) > -infinity)
                    //                    rset.lhs_w(idx) = val;
                }
                if (mps.field5() != 0) {
                    //                if ((idx = rnames.number(mps.field4())) < 0)
                    //                    if (mapRowNumber.find(mps.field4()) == mapRowNumber.end())
                    if (!extDataSet.mapRowName.contains(mps.field4()))
                        mps.entryIgnored("RHS", mps.field1(), "row", mps.field4());
                    else {
                        //                        idx = mapRowNumber[mps.field4()];
                        idx = extDataSet.mapRowName.get(mps.field4());
                        val = atof(mps.field5());
                        // LE or EQ
                        //                    if (rset.rhs(idx) < infinity)
                        //                        rset.rhs_w(idx) = val;
                        //                        if (arrRow[idx].rhs() < INFINITY_VALUE)
                        if (extDataSet.vctRows[idx].getRhs() < INFINITY_VALUE)
                            extDataSet.vctRows[idx].setRhs(val);
                        // GE or EQ
                        //                    if (rset.lhs(idx) > -infinity)
                        //                        rset.lhs_w(idx) = val;
                        if (extDataSet.vctRows[idx].getLhs() > -INFINITY_VALUE)
                            extDataSet.vctRows[idx].setLhs(val);
                    }
                }
            }
        }
        //endReadRhsWithError:
        mps.syntaxError();
endReadRhs:
        mps.setRhsName(rhsname);
        extDataSet.setRhsName(rhsname);
    }
    bool readMPS(std::istream &is, ExtLPDSSet &extDataSet) {
        //MPS
        DEBUG_FILE_WITH_TIMESTAMP(" ------------------------- LP Problem -----------------------------");
        DEBUG_FILE_WITH_TIMESTAMP(" *** Reading MPS File and Writing on External Memory *** ");
        DEBUG_PARSER("Reading MPS File!!!");
        MPSInput mpsInput(is);
        DEBUG_PARSER("Reading Problem Name!!!");
        readName(mpsInput);
        extDataSet.set_problem_name(mpsInput.probName());
        //    mpsSectionData = mpsSectionData + "Name: " + mpsInput.probName() + "\r\n";
        if (mpsInput.section() == MPSInput::OBJSEN) {
            DEBUG_PARSER("Reading Objective Sense!!!");
            readObjsen(mpsInput);
            extDataSet.setObjSense(mpsInput.objSense());
            //        mpsSectionData = mpsSectionData + "ObjSense: " + (mpsInput.objSense() ? "Min" : "Max") + "\r\n";
        }
        if (mpsInput.section() == MPSInput::OBJNAME) {
            DEBUG_PARSER("Reading Objective Name!!!");
            readObjname(mpsInput);
            extDataSet.setObjName(mpsInput.objName());
            //        mpsSectionData = mpsSectionData + "ObjName: " + mpsInput.objName() + "\r\n";
        }
        if (mpsInput.section() == MPSInput::ROWS) {
            DEBUG_PARSER("Reading the Rows Section Data!!!");
            readRows(mpsInput, extDataSet);
            //        ///////////////////////////////////////////////////////////////
            //        /// Debugging the logic and results
            //        ////////////////////////////////////////////////////////////////
            //        DEBUG_PARSER("Rows are As Follows")
            //
            //                int i = 0;
            //        //        for(;i<arrRow.size(); i++){
            //        //            LPRow &row = arrRow[i];
            //        //            std::cout<<"Row "<<i<<": "<<"r\n";
            //        //        }
            //        std::map<std::string, int>::iterator itrMapRow = mapRowNumber.begin();
            //        while (itrMapRow != mapRowNumber.end()) {
            //            DEBUG_PARSER(itrMapRow->first << " : " << itrMapRow->second)
            //            //mpsSectionData = mpsSectionData + itrMapRow->first.c_str() + " : " + itrMapRow->second + "r\n";
            //            itrMapRow++;
            //        }
            //        DEBUG_SIMPLE("Number of rows imported: " << arrRow.size());
            //        DEBUG_SIMPLE("Identified to optimize the Objective: " << mpsInput.objName());
            //        //////////////////////////////////////////////////////////////////
        }
        DEBUG("Read " << extDataSet.vctRows.size() << " rows.");
        //        DEBUG_MEMORY("Memory after Reading Rows");
        //   addedRows(rset.num());
        if (mpsInput.section() == MPSInput::COLUMNS) {
            DEBUG_PARSER("Reading Columns Section Data!!!");
            //            readCols(mpsInput, arrRow, mapRowNumber, arrCol, mapColNumber);
            readCols(mpsInput, extDataSet);
        }

        if (mpsInput.section() == MPSInput::RHS) {
            DEBUG_PARSER("Reading Rhs Section Data!!!");
            readRhs(mpsInput, extDataSet);
        }
        ///////////////////////////////////////////////////////////////
        /// Debugging the logic and results
        ////////////////////////////////////////////////////////////////
        //    DEBUG_PARSER("Objective Values read are: ");
        //    for (int i = 0; i < arrCol.size(); i++) {
        //        DEBUG_PARSER(i << ": " << arrCol[i].obj());
        //    }
        //    std::map<std::string, int>::iterator itrMapCol = mapColNumber.begin();
        //    DEBUG_PARSER("Columns are Read as Follows: ");
        //    while (itrMapCol != mapColNumber.end()) {
        //        DEBUG_PARSER(itrMapCol->first << " : " << itrMapCol->second);
        //        //mpsSectionData = mpsSectionData + itrMapRow->first.c_str() + " : " + itrMapRow->second + "r\n";
        //        itrMapCol++;
        //    }
        DEBUG_SIMPLE("Number of Rows imported: " << extDataSet.vctRows.size());
        DEBUG_SIMPLE("Number of Columns imported: " << extDataSet.vctCols.size());
        DEBUG_SIMPLE("Writing Simplex Tableau to the Log!!!");


        DEBUG_FILE_WITH_TIMESTAMP(" *** Exporting LP Tableau *** ");
        DEBUG_FILE("Number of Rows imported: " << extDataSet.vctRows.size());
        DEBUG_FILE("Number of Columns imported: " << extDataSet.vctCols.size());
        DEBUG_FILE("Problem Name: " << extDataSet.get_problem_name());
        //        DEBUG_FILE("Partial Simplex Tableau is shown as follows: ");
        unsigned int i = 0, j = 0;
        //! Row-wise display of the LP Tableau
        //        std::stringstream titleStream;
        //        titleStream << std::setw(10) << "Row/Col" << ": ";
        //        for (j = 0; j < extDataSet.vctCols.size(); j++)
        //            titleStream << std::setw(10) << extDataSet.vctCols[j].getName() << ", ";
        //        titleStream << std::setw(10) << extDataSet.getRhsName() << ", ";
        //        DEBUG_FILE(titleStream.str());
        //
        //        std::stringstream objStream;
        //        std::string objTitle("OBJ-");
        //        objTitle = objTitle + mpsInput.objName();
        //        objStream << std::setw(10) << objTitle << ": ";
        //        for (i = 0; i < extDataSet.vctCols.size(); i++) {
        //            objStream << std::setw(10) << extDataSet.vctCols[i].obj() << ", ";
        //        }
        //        /// Rhs Column in objective constraint
        //        objStream << std::setw(10) << 0 << ", ";
        //        DEBUG_FILE(objStream.str());
        //        for (i = 0; i < extDataSet.vctRows.size(); i++) {
        //            //            string rowName(arrRow[i].)
        //            std::stringstream rowStream;
        //            rowStream << std::setw(10) << extDataSet.vctRows[i].getName() << ": ";
        //            for (j = 0; j < extDataSet.vctCols.size(); j++) {
        //                //                mapColNumber[]
        //                rowStream << std::setw(10) << extDataSet.vctCols[j].getRowElement(i) << ", ";
        //            }
        //            rowStream << std::setw(10) << extDataSet.vctRows[i].getRhs() << ", ";
        //            DEBUG_FILE(rowStream.str());
        //        }
        //        DEBUG_FILE(" ------------------------- *** -----------------------------");



        //! Sparsity Analysis --- 
        //! Column-wise display of LP Tableau
        //        unsigned int width = 35;
        //
        //        std::stringstream titleStream;
        //        titleStream << std::setw(width) << "Col/Row" << ": ";
        //        for (j = 0; j < extDataSet.vctRows.size(); j++)
        //            titleStream << std::setw(width) << extDataSet.vctRows[j].getName() << ", ";
        //        //        titleStream << std::setw(width) << extDataSet.getRhsName() << ", ";
        //        titleStream << std::setw(width) << extDataSet.getObjName() << ", ";
        //        DEBUG_FILE(titleStream.str());
        //
        //        for (i = 0; i < extDataSet.vctCols.size(); i++) {
        //            // Adding cache
        //            ExtColVector &extCurrentVector = extDataSet.vctCols[i];
        //            extCurrentVector.allocate_cache();
        //            std::stringstream colStream;
        //            colStream << std::setw(width) << extCurrentVector.getName() << ": ";
        //            //            for (j = 0; j < extDataSet.vctRows.size(); j++) {
        //            //                colStream << std::setw(width) << extDataSet.vctCols[j].getRowElement(i) << ", ";
        //            //            }
        //            // Iterator for extCurrentVector
        //            ExtColVector::iterator itr = extCurrentVector.begin();
        //            unsigned int prevRowIndex = 0;
        //            while (itr != extCurrentVector.end()) {
        //                PackedElement<REAL> packedElement = *itr;
        ////                DEBUG("Adding Compressed Offset of " << (packedElement.getIndex() - prevRowIndex));
        //                assert(((int)packedElement.getIndex() - (int)prevRowIndex) >= 0);
        //                while (prevRowIndex < packedElement.getIndex()) {
        //                    colStream << std::setw(width) << 0.0F << ", ";
        //                    prevRowIndex++;
        //                }
        //                itr++;
        //            }
        //            // Padding NULL values until the end of col vector
        //            //            DEBUG("Previous Index: "<<prevRowIndex<<", Size of Vector " << i<<" is: "<<extCurrentVector.real_size()<<", Packed="<<extCurrentVector.isPacked());
        //
        //            while (prevRowIndex < extCurrentVector.real_size()) {
        //                colStream << std::setw(width) << 0.0F << ", ";
        //                prevRowIndex++;
        //            }
        //            colStream << std::setw(width) << extCurrentVector.obj() << ", ";
        //            // Removing cache
        //            extCurrentVector.deallocate_cache();
        //            DEBUG_FILE(colStream.str());
        //        }
        //
        //        std::stringstream rhsStream;
        //        std::string rhsTitle("");
        //        rhsTitle = rhsTitle + extDataSet.getRhsName();
        //        rhsStream << std::setw(width) << rhsTitle << ": ";
        //        for (i = 0; i < extDataSet.vctRows.size(); i++) {
        //            rhsStream << std::setw(width) << extDataSet.vctRows[i].getRhs() << ", ";
        //        }
        //        /// Obj Row in Rhs Column
        //        rhsStream << std::setw(width) << 0 << ", ";
        //        DEBUG_FILE(rhsStream.str());


        //! ---------------- Sparsity Analysis
        float overall_vector_sparsity = 0;
        unsigned int overall_nnz = 0;
        unsigned int overall_size = (extDataSet.vctCols.size())*(extDataSet.vctCols[i].real_size());
        DEBUG_FILE_WITH_TIMESTAMP(" *** Sparsity Analysis *** ");
        for (i = 0; i < extDataSet.vctCols.size(); i++) {
            ExtColVector &extCurrentVector = extDataSet.vctCols[i];
            DEBUG_FILE("Vector[" << i << "]: " << extCurrentVector.get_sparsity());
            overall_vector_sparsity += extCurrentVector.get_sparsity();
            overall_nnz += extCurrentVector.get_nnz();
        }
        overall_vector_sparsity /= extDataSet.vctCols.size();
        float overall_tableau_sparsity = (float) (overall_size - overall_nnz)*100.0 / overall_size;

        DEBUG_FILE("Avg sparsity of a vector in LP Tableau: " << overall_vector_sparsity);
        DEBUG_FILE("Overall sparsity of the LP Tableau: " << overall_tableau_sparsity);


        DEBUG_FILE_WITH_TIMESTAMP(" ------------------------- *** -----------------------------");
        /////////////////////////////////////////////////////////////////////////


        //   if (mps.section() == MPSInput::RANGES)
        //      readRanges(mps, rset, *rnames);
        //
        //   if (mps.section() == MPSInput::BOUNDS)
        //      readBounds(mps, cset, *cnames, p_intvars);
        //
        //   if (mps.section() != MPSInput::ENDATA)
        //      mps.syntaxError();
        //
        //   if (mps.hasError())
        //      clear();
        //   else
        //   {
        //      changeSense(mps.objSense());

        //      MSG_INFO2(
        //         spxout << "IMPSRD06 Objective sense: " 
        //                << ((mps.objSense() == MINIMIZE) ? "Minimize" : "Maximize") 
        //                << std::endl;         
        //      )
        //
        //      added2Set(
        //         *(reinterpret_cast<SVSet*>(static_cast<LPRowSet*>(this))), 
        //         *(reinterpret_cast<SVSet*>(static_cast<LPColSet*>(this))), 
        //         cset.num());
        //      addedCols(cset.num());
        //      assert(isConsistent());
        //   }

        return false;
    }
};




#endif	/* EXTREADMPSUTIL_H */
