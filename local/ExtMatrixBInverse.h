/* 
 * File:   ExtMatrixBInverse.h
 * Author: sriram
 *
 * Created on 10 May, 2014, 3:05 PM
 */

#ifndef EXTMATRIXBINVERSE_H
#define	EXTMATRIXBINVERSE_H
#include "FixedString.h"
#include "PackedVector.h"
#include "PackedColVector.h"
#include "FixedStringUtil.h"
#include "PackedRowVector.h"

//! Stores limits for the start and end of the column in ExtMatrixBInverse container
//! The limits to the column is [start, end) start includes and end excludes
//! Also includes the other necessary properties such as name and objective value
class MatrixBInverseColAttr {
private:
    unsigned int start, end;
    REAL objective_value;
    FixedString col_name;
    bool is_slack_col;
public:
    MatrixAColAttr() {
    }
    MatrixAColAttr(unsigned int start, unsigned int end) {
        this->start = start;
        this->end = end;
    }
    void set_start(unsigned int start) {
        this->start = start;
    }
    void set_end(unsigned int end) {
        this->end = end;
    }
    void set_objective_value(REAL objective_value) {
        this->objective_value = objective_value;
    }
    void set_col_name(std::string col_name) {
        this->col_name = FixedStringUtil::getFixedString(col_name);
    }
    void set_is_slack_col(bool is_slack_col) {
        this->is_slack_col = is_slack_col;
    }
    unsigned int get_start() {
        return this->start;
    }
    unsigned int get_end() {
        return this->end;
    }
    REAL get_objective_value() {
        return this->objective_value;
    }
    std::string get_col_name() {
        return FixedStringUtil::getNormalString(this->col_name);
    }
    bool get_is_slack_col() {
        return this->is_slack_col;
    }
};

//! Stores row related properties of ExtMatrixA container
//! Also includes the other necessary properties such as name and rhs value
class MatrixBInverseRowAttr {
private:
    FixedString row_name;
    //! Integer identifier for the row type 
    //! 0 Equals; 1 greater than Equals; 2 Less than Equals
    //! 3 Less than; 4 Greater ; 5 Range
    int row_type;
    //! left and right hand side values for the constraint
    //! Range Constraint could contain both lhs and rhs values
    REAL lhs_value, rhs_value;
public:
    void set_row_name(std::string col_name) {
        this->row_name = FixedStringUtil::getFixedString(col_name);
    }
    void set_row_type(int row_type) {
        this->row_type = row_type;
    }
    void set_lhs_value(REAL lhs_value) {
        this->lhs_value = lhs_value;
    }
    void set_rhs_value(REAL rhs_value) {
        this->rhs_value = rhs_value;
    }
    std::string get_row_name() {
        return FixedStringUtil::getNormalString(row_name);
    }
    int get_row_type() {
        return row_type;
    }
    std::string get_row_type_str(){
        switch(get_row_type()){
            case 0: return "=";
            case 1: return ">=";
            case 2: return "<=";
            case 5: return "Range";
        }
        return "Unknown";
    }
    REAL get_lhs_value() {
        return this->lhs_value;
    }
    REAL get_rhs_value() {
        return this->rhs_value;
    }
};

//! Storage Container for Fixed Matrix
//! The contents of the matrix do not change after assignment/initialization
//! Storage is done in column-wise fashion
class ExtMatrixBInverse {
private:
    //! Vector to store the Items onto external memory
    typedef typename stxxl::VECTOR_GENERATOR<PackedElement, 1, 1, MATRIX_A_BLOCK_SIZE>::result item_vector;
    item_vector m_vct_disk;
    //! Vector to keep track of columns in the matrix
    typedef typename stxxl::VECTOR_GENERATOR<MatrixAColAttr, 1, 1, MATRIX_A_COL_DATA_BLOCK_SIZE>::result col_data_vector;
    col_data_vector m_vct_col_attr;
    //! Vector to keep track of rows in the matrix
    typedef typename stxxl::VECTOR_GENERATOR<MatrixARowAttr, 1, 1, MATRIX_A_ROW_DATA_BLOCK_SIZE>::result row_data_vector;
    row_data_vector m_vct_row_attr;
public:
    //! Matrix Row and Column Attributes
    //    friend class MatrixAColAttr;
    typedef MatrixBInverseColAttr ColAttr;
    //    friend class MatrixARowAttr;
    typedef MatrixBInverseRowAttr RowAttr;

    //! Adds a new column into the matrix
    //! TODO - Set the start limit of col appropriately
    void add_column(PackedColVector &p_packed_col) {
        unsigned int no_cols = m_vct_col_attr.size(); // Last column number
        unsigned int last_col_end = (no_cols == 0) ? 0 : m_vct_col_attr[no_cols - 1].get_end();
        PackedVector &packed_col = p_packed_col.get_col_vector();
        PackedVector::iterator itr = packed_col.begin();
        while (itr != packed_col.end()) {
            PackedElement packed_element = *itr;
//            DEBUG("Adding Element: " << packed_element.get_index() << ", " << packed_element.get_value());
            m_vct_disk.push_back(packed_element);
            itr++;
        }
        MatrixAColAttr col_attr(last_col_end, last_col_end + packed_col.get_nnz());
        col_attr.set_objective_value(p_packed_col.get_objective_value());
        col_attr.set_col_name(p_packed_col.get_name());
        col_attr.set_is_slack_col(false);
//        DEBUG("Adding Col: " << col_attr.get_col_name() << " [" << col_attr.get_start() << ", " << col_attr.get_end() << ")");
        m_vct_col_attr.push_back(col_attr);
    }

    //! Store an Entire column excluding the objective value
    //! Send by Reference... Hence parameter PackedVector will be affected
    //! TODO - Check if the objective value is added properly to the end
    void store_column(unsigned int p_column_idx, PackedVector& p_packed_col, bool p_add_objective = true) {
        // Initializing p_packed_col for proper storage
        p_packed_col.clear();
        p_packed_col.resize(get_rows_count());
        // Limits of the requested column
        MatrixAColAttr col_attr = m_vct_col_attr[p_column_idx];
        // Adding all the column vector values
        for (unsigned int i = col_attr.get_start(); i < col_attr.get_end(); i++) {
            const PackedElement packed_element = m_vct_disk[i];
            p_packed_col.add(packed_element.get_index(), packed_element.get_value(), false);
        }
//        DEBUG("Storing Col: " << col_attr.get_col_name() << " [" << col_attr.get_start() << ", " << col_attr.get_end() << ")");
        // Adding the objective value of column to the end
        if (p_add_objective) {
            unsigned int last_index = get_rows_count();
            REAL objective_value = col_attr.get_objective_value();
            p_packed_col.add(last_index, objective_value, false);
        }
    }

    //! Returns the column attributes associated with the p_col_index
    ColAttr get_col_attr(unsigned int p_col_index) {
        const MatrixAColAttr col_attr = m_vct_col_attr[p_col_index];
        return col_attr;
    }
    //! Returns the columns count - Total number of columns in the matrix
    unsigned int get_columns_count() {
        return m_vct_col_attr.size();
    }

    //! Adds a new row into the matrix
    //! TODO - Check setting of RHS is correct or not.
    void add_row(PackedRowVector &p_packed_row) {
        MatrixARowAttr row_attr;
        row_attr.set_row_name(p_packed_row.get_name());
        row_attr.set_lhs_value(p_packed_row.get_lhs());
        row_attr.set_rhs_value(p_packed_row.get_rhs());
        row_attr.set_row_type(p_packed_row.get_row_type());
        m_vct_row_attr.push_back(row_attr);
    }

    //! Returns the rows count - Total number of rows in the matrix
    unsigned int get_rows_count() {
        return m_vct_row_attr.size();
    }

    //! Returns the number of non-zeros for a given p_col_index column
    //! Use this to get nnz instead of fetching entire column.
    unsigned int get_col_nnz(unsigned int p_col_index) {
        MatrixAColAttr col_attr = m_vct_col_attr[p_col_index];
        return (col_attr.get_end() - col_attr.get_start());
    }

    //! Returns the sparsity of given p_col_index column. 
    //! Use this to get sparsity instead of fetching entire column
    float get_col_sparsity(unsigned int p_col_index) {
        unsigned int nnz = get_col_nnz(p_col_index);
        unsigned int real_size = get_rows_count();
        return ((float) (real_size - nnz))*100.0 / real_size;
    }

    //! gets the row attributes specific to a column
    RowAttr get_row_attr(unsigned int p_row_index) {
        const MatrixARowAttr row_attr = m_vct_row_attr[p_row_index];
        return row_attr;
    }

    //! sets the row attributes of a column
    void set_row_attr(unsigned int p_col_index, MatrixARowAttr& p_row_attr) {
        m_vct_row_attr[p_col_index] = p_row_attr;
    }
};



#endif	/* EXTMATRIXBINVERSE_H */

