/* 
 * File:   ExtMatrixA.h
 * Author: Sriram Mahavadi
 *
 * Created on 8 May, 2014, 6:29 PM
 */

#ifndef EXTMATRIXA_H
#define	EXTMATRIXA_H

#include "FixedString.h"
#include "PackedVector.h"
#include "PackedColVector.h"
#include "PackedRowVector.h"
#include "SimpleVector.h"

//! Stores limits for the start and end of the column in ExtMatrixA container
//! The limits to the column is [start, end) start includes and end excludes
//! Also includes the other necessary properties such as name and objective value
class MatrixAColAttr {
private:
    unsigned int start, end;
    REAL objective_value;
    FixedString col_name;
    bool is_slack_col;
    bool is_basic_col;
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
        this->col_name = FixedString::getFixedString(col_name);
    }
    void set_is_slack_col(bool is_slack_col) {
        this->is_slack_col = is_slack_col;
    }
    void set_is_basic_col(bool is_basic_col) {
        this->is_basic_col = is_basic_col;
    }
    unsigned int get_start() const {
        return this->start;
    }
    unsigned int get_end() const {
        return this->end;
    }
    REAL get_objective_value() const {
        return this->objective_value;
    }
    std::string get_col_name() const {
        return FixedString::getNormalString(this->col_name);
    }
    bool get_is_slack_col() const {
        return this->is_slack_col;
    }
    bool get_is_basic_col() const {
        return this->is_basic_col;
    }
};

//! Stores row related properties of ExtMatrixA container
//! Also includes the other necessary properties such as name and rhs value
class MatrixARowAttr {
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
        this->row_name = FixedString::getFixedString(col_name);
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
    std::string get_row_name() const {
        return FixedString::getNormalString(row_name);
    }
    int get_row_type() const {
        return row_type;
    }
    std::string get_row_type_str() const {
        switch (get_row_type()) {
            case 0: return "=";
            case 1: return ">=";
            case 2: return "<=";
            case 5: return "Range";
        }
        return "Unknown";
    }
    REAL get_lhs_value() const {
        return this->lhs_value;
    }
    REAL get_rhs_value() const {
        return this->rhs_value;
    }
};

//! Storage Container for Fixed Matrix
//! The contents of the matrix do not change after assignment/initialization
//! Storage is done in column-wise fashion
class ExtMatrixA {
private:
    //! Vector to store the Items onto external memory
    //! template parameters<ValueType, PageSize, CachePages, BlockSize, AllocStratg>
    typedef typename stxxl::VECTOR_GENERATOR<PackedElement, MATRIX_A_BLOCKS_PER_PAGE, MATRIX_A_PAGE_CACHE, MATRIX_A_BLOCK_SIZE>::result item_vector;
    item_vector m_vct_disk;
    const item_vector& m_vct_read_only_disk;
    //! Vector to keep track of columns in the matrix
    typedef typename stxxl::VECTOR_GENERATOR<MatrixAColAttr, 1, 1, MATRIX_A_COL_DATA_BLOCK_SIZE>::result col_data_vector;
    col_data_vector m_vct_col_attr;
    const col_data_vector& m_vct_read_only_col_attr;
    //! Vector to keep track of rows in the matrix
    typedef typename stxxl::VECTOR_GENERATOR<MatrixARowAttr, 1, 1, MATRIX_A_ROW_DATA_BLOCK_SIZE>::result row_data_vector;
    row_data_vector m_vct_row_attr;
    const row_data_vector& m_vct_read_only_row_attr;

    //! Adds a new slack column into the matrix
    //! Done programmatically while standardization of the matrix
    //! TODO - Incorporate processing singleton columns directly from ColAttr itself
    void _add_slack_column(unsigned int p_slack_row_index) {
        unsigned int no_cols = m_vct_col_attr.size(); // Last column number
        unsigned int last_col_end = (no_cols == 0) ? 0 : m_vct_col_attr[no_cols - 1].get_end();
        PackedElement packed_element(p_slack_row_index, 1);
        m_vct_disk.push_back(packed_element);
        MatrixAColAttr col_attr(last_col_end, last_col_end + 1); // Singleton Column
        col_attr.set_objective_value(0);
        std::stringstream row_name_stream;
        row_name_stream << "slack_" << get_row_attr(p_slack_row_index).get_row_name();
        col_attr.set_col_name(row_name_stream.str());
        col_attr.set_is_slack_col(true);
        col_attr.set_is_basic_col(true);
        //        DEBUG("Adding Col: " << col_attr.get_col_name() << " [" << col_attr.get_start() << ", " << col_attr.get_end() << ")");
        m_vct_col_attr.push_back(col_attr);
    }

public:
    //! Matrix Row and Column Attributes
    //    friend class MatrixAColAttr;
    typedef MatrixAColAttr ColAttr;
    //    friend class MatrixARowAttr;
    typedef MatrixARowAttr RowAttr;

    //! Simple Initialization
    ExtMatrixA() : m_vct_read_only_disk(m_vct_disk),
    m_vct_read_only_col_attr(m_vct_col_attr),
    m_vct_read_only_row_attr(m_vct_row_attr) {
    }
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
        //! Initial basis columns would be the set of slack columns. Hence basic property set to false
        col_attr.set_is_basic_col(false);
        //        DEBUG("Adding Col: " << col_attr.get_col_name() << " [" << col_attr.get_start() << ", " << col_attr.get_end() << ")");
        m_vct_col_attr.push_back(col_attr);
    }

    //! Store an Entire column excluding the objective value
    //! Send by Reference... Hence parameter PackedVector will be affected
    //! TODO - Check if the objective value is added properly to the end
    void store_column(unsigned int p_column_idx, PackedVector& p_packed_col, bool p_add_objective = true) {
        // Initializing p_packed_col for proper storage
        p_packed_col.clear();
        if (p_add_objective)
            p_packed_col.resize(get_rows_count() + 1); // 1 more index for objective value
        else
            p_packed_col.resize(get_rows_count());
        // Limits of the requested column
        MatrixAColAttr col_attr = m_vct_read_only_col_attr[p_column_idx];
        // Adding all the column vector values
        for (unsigned int i = col_attr.get_start(); i < col_attr.get_end(); i++) {
            const PackedElement packed_element = m_vct_read_only_disk[i];
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
        const MatrixAColAttr col_attr = m_vct_read_only_col_attr[p_col_index];
        return col_attr;
    }
    //! Returns the columns count - Total number of columns in the matrix
    unsigned int get_columns_count() {
        return m_vct_read_only_col_attr.size();
    }

    //! Returns the basic columns count - Total number of basic columns in the matrix
    //! There is a possibility that a constraint can lead to more than one columns.
    //! Simply the number of basic columns should be equal the total number of constraints
    unsigned int get_basic_columns_count() {
        return get_rows_count();
    }


    //! Stores the col objective values into the packed vector in the order of base col indices
    void store_col_objective_values(SimpleVector<REAL>& p_vct_obj, SimpleVector<unsigned int> base_col_indices) {
        unsigned int base_col_size = base_col_indices.get_size();
        // Making vct obj good for storage
        p_vct_obj.clear();
        p_vct_obj.resize(base_col_size);
        for (unsigned col_index = 0; col_index < base_col_size; col_index++) {
            REAL a_col_index = base_col_indices[col_index];
            REAL obj_value = m_vct_read_only_col_attr[a_col_index].get_objective_value();
            p_vct_obj[col_index] = obj_value;
        }
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
        return m_vct_read_only_row_attr.size();
    }

    //! Returns the number of non-zeros for a given p_col_index column
    //! Use this to get nnz instead of fetching entire column.
    unsigned int get_col_nnz(unsigned int p_col_index) {
        MatrixAColAttr col_attr = m_vct_read_only_col_attr[p_col_index];
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
        const MatrixARowAttr row_attr = m_vct_read_only_row_attr[p_row_index];
        return row_attr;
    }

    //! sets the row attributes of a column
    void set_row_attr(unsigned int p_row_index, MatrixARowAttr& p_row_attr) {
        m_vct_row_attr[p_row_index] = p_row_attr;
    }

    //! Stores the row rhs values into the packedvector
    void store_row_rhs_values(SimpleVector<REAL>& p_vct_rhs) {
        // Making rhs vector appropriate for storage
        p_vct_rhs.clear();
        p_vct_rhs.resize(get_rows_count());
        for (unsigned row_index = 0; row_index < get_rows_count(); row_index++) {
            REAL rhs_value = m_vct_read_only_row_attr[row_index].get_rhs_value();
            //            p_vct_rhs.add(row_index, rhs_value, false);
            p_vct_rhs[row_index] = rhs_value;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////// - Common Matrix Operations - //////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    //! Standardizing the LP Tableau (MatrixA) for performing simplex
    //! Returns the base column indices in order 
    SimpleVector<unsigned int> standardize_matrix() {
        unsigned int i = 0;
        SimpleVector<unsigned int> vct_base_col_indices(get_rows_count());
        for (i = 0; i < get_rows_count(); i++) {
            RowAttr row_attr = get_row_attr(i);
            int row_type = row_attr.get_row_type();
            switch (row_type) {
                case 0:
                    // TODO - Need to standardize by adding new row
                    // Pre addition of the row needs to be done much before while
                    // Initialization itself
                    // Case Equals
                    break;
                case 1:
                    // Case >=
                    break;
                case 2:
                    // Case <=
                    _add_slack_column(i);
                    row_attr.set_row_type(0);
                    set_row_attr(i, row_attr);
                    // Just added Col is the base column for row i
                    vct_base_col_indices.add(i, get_columns_count() - 1);
                    break;
                case 5:
                    // Case Range
                    break;
            }
        }
        return vct_base_col_indices;
    }
};

#endif	/* EXTMATRIXA_H */

