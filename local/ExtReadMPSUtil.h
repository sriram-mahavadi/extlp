/* 
 * File:   ExtReadMPSUtil.h
 * Author: Sriram Mahavadi
 *
 * Created on 7 April, 2014, 9:54 AM
 */

#ifndef EXTREADMPSUTIL_H
#define	EXTREADMPSUTIL_H
#include "GlobalDefines.h"
#include "PackedRowVector.h"
#include "PackedColVector.h"
#include "ExtLPDSSet.h"
#include "mpsinput.h"
class ExtReadMPSUtil {
public:
    //! Process NAME section.
    //! TODO - Also store name into the extDataSet
    static void readName(MPSInput& mps) {
        do {
            // This has to be the Line with the NAME section.
            if (!mps.readLine()
                    || (mps.field0() == 0) || strcmp(mps.field0(), "NAME"))
                break;
            // Sometimes the name is omitted.
            mps.setProbName((mps.field1() == 0) ? "_MPS_" : mps.field1());
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

    //! Process OBJSEN section. This Section is an ILOG extension.
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

    //! Process OBJNAME section. This Section is an ILOG extension.
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

    //! Process ROWS section. 
    static void readRows(
            MPSInput& mps,
            ExtLPDSSet &extDataSet) {
        PackedRowVector row;
        unsigned int i = 0;
        while (mps.readLine()) {
            if (mps.field0() != 0) {
                if (strcmp(mps.field0(), "COLUMNS"))
                    break;
                //                DEBUG_PARSER("Next Section found to be COLUMNS");
                mps.setSection(MPSInput::COLUMNS);
                goto endReadRows;
            }
            if (*mps.field1() == 'N') {
                if (*mps.objName() == '\0') {
                    mps.setObjName(mps.field2());
                    extDataSet.setObjName(mps.objName());
                }
            } else {
                if (extDataSet.mapRowName.contains(mps.field2()))
                    break;
                extDataSet.mapRowName.set(mps.field2(), i++);
                row.set_name(mps.field2());
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
                extDataSet.A.add_row(row);
            }
        }
endReadRowsWithError:
        mps.syntaxError();
endReadRows:
        return;
    }

    //! Process COLUMNS section. 
    static void readCols(
            MPSInput& mps,
            ExtLPDSSet& extDataSet
            ) {
        REAL val;
        char colname[MPSInput::MAX_LINE_LEN] = {'\0'};
        PackedColVector col(extDataSet.A.get_rows_count());
        std::vector<REAL> vec(extDataSet.A.get_rows_count());
        static int count_cols_read = 0;
        col.set_objective_value(0.0F);
        int i = 0;
        while (mps.readLine()) {
            if (mps.field0() != 0) {
                if (strcmp(mps.field0(), "RHS")) {
                    DEBUG_PARSER("Next Section is RHS")
                    break;
                }
                if (colname[0] != '\0') {
                    col.set_col_vector(vec);
                    extDataSet.A.add_column(col);
                    //                    ExtColVector extCol(col);
                    //                    extDataSet.vctCols.push_back(extCol);
                    count_cols_read++;
                    //                    DEBUG_SIMPLE("Read Col " << count_cols_read << ", with name: " << extCol.getName() << ", Sparsity: " << extCol.);
                }
                mps.setSection(MPSInput::RHS);
                return;
            }
            if ((mps.field1() == 0) || (mps.field2() == 0) || (mps.field3() == 0))
                break;

            // new column?
            if (strcmp(colname, mps.field1())) {
                // first column?
                if (colname[0] != '\0') {
                    col.set_col_vector(vec);
                    //                    ExtColVector extCol(col);
                    //                    extDataSet.vctCols.push_back(extCol);
                    extDataSet.A.add_column(col);
                    count_cols_read++;
                    //                    DEBUG_SIMPLE("Read Col " << count_cols_read << ", with name: " << extCol.getName() << ", Sparsity: " << extCol.get_sparsity());
                }
                // save copy of string (make sure string ends with \0)
                strncpy(colname, mps.field1(), MPSInput::MAX_LINE_LEN - 1);
                colname[MPSInput::MAX_LINE_LEN - 1] = '\0';
                extDataSet.mapColName.set(colname, i++);
                //                mapColNumber[colname] = i++;
                col.set_name(colname);
                // Initializing back to default values
                vec.clear();
                vec.resize(extDataSet.A.get_rows_count());
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
                if (!extDataSet.mapRowName.contains(mps.field2()))
                    mps.entryIgnored("RHS", mps.field1(), "row", mps.field2());
                else {
                    idx = extDataSet.mapRowName.get(mps.field2());
                    val = atof(mps.field3());
                    // LE or EQ
                    ExtMatrixA::RowAttr row_attr = extDataSet.A.get_row_attr(idx);
                    if (row_attr.get_rhs_value() < INFINITY_VALUE) {
                        row_attr.set_rhs_value(val);
                        extDataSet.A.set_row_attr(idx, row_attr);
                    }
                    //                    if (extDataSet.vctRows[idx].getRhs() < INFINITY_VALUE)
                    //                        extDataSet.vctRows[idx].setRhs(val);
                    // GE or EQ
                    if (row_attr.get_lhs_value() > -INFINITY_VALUE) {
                        row_attr.set_lhs_value(val);
                        extDataSet.A.set_row_attr(idx, row_attr);
                    }
                    //                    if (extDataSet.vctRows[idx].getLhs() > -INFINITY_VALUE)
                    //                        extDataSet.vctRows[idx].setLhs(val);
                }
                if (mps.field5() != 0) {
                    if (!extDataSet.mapRowName.contains(mps.field4()))
                        mps.entryIgnored("RHS", mps.field1(), "row", mps.field4());
                    else {
                        //                        idx = mapRowNumber[mps.field4()];
                        idx = extDataSet.mapRowName.get(mps.field4());
                        val = atof(mps.field5());
                        // LE or EQ
                        ExtMatrixA::RowAttr row_attr = extDataSet.A.get_row_attr(idx);
                        if (row_attr.get_rhs_value() < INFINITY_VALUE) {
                            row_attr.set_rhs_value(val);
                            extDataSet.A.set_row_attr(idx, row_attr);
                        }
                        // GE or EQ
                        if (row_attr.get_lhs_value() > -INFINITY_VALUE) {
                            row_attr.set_lhs_value(val);
                            extDataSet.A.set_row_attr(idx, row_attr);
                        }
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
            DEBUG_FILE_WITH_TIMESTAMP(" --- Reading Rows Completed --- ");
        }
        DEBUG("Read " << extDataSet.A.get_rows_count() << " rows.");
        //        DEBUG_MEMORY("Memory after Reading Rows");
        //   addedRows(rset.num());
        if (mpsInput.section() == MPSInput::COLUMNS) {
            DEBUG_PARSER("Reading Columns Section Data!!!");
            //            readCols(mpsInput, arrRow, mapRowNumber, arrCol, mapColNumber);
            readCols(mpsInput, extDataSet);
            DEBUG_FILE_WITH_TIMESTAMP(" --- Reading Columns Completed --- ");
        }

        if (mpsInput.section() == MPSInput::RHS) {
            DEBUG_PARSER("Reading Rhs Section Data!!!");
            readRhs(mpsInput, extDataSet);
        }

        DEBUG_SIMPLE("Number of Rows imported: " << extDataSet.A.get_rows_count());
        DEBUG_SIMPLE("Number of Columns imported: " << extDataSet.A.get_columns_count());
        DEBUG_FILE_WITH_TIMESTAMP(" --- Completed Reading LP Tableau --- ");
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

