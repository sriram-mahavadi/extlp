/* 
 * File:   ExtMatrixBInverse.h
 * Author: Sriram Mahavadi
 *
 * Created on 11 May, 2014, 11:17 PM
 */

#ifndef EXTMATRIXBINVERSE_H
#define	EXTMATRIXBINVERSE_H
#include "GlobalDefines.h"
#include "ExtMatrixA.h"

//! Stores limits for the start and end of the column in ExtMatrixBInverse container
//! The limits to the column is [start, end) start includes and end excludes
//! Also includes the other necessary properties such as col_index for getting column details
class MatrixBInverseColAttr {
private:
    unsigned int start, end;
    unsigned int col_index;
public:
    MatrixBInverseColAttr() {
    }
    MatrixBInverseColAttr(unsigned int start, unsigned int end) {
        this->start = start;
        this->end = end;
    }
    void set_start(unsigned int start) {
        this->start = start;
    }
    void set_end(unsigned int end) {
        this->end = end;
    }
    void set_col_index(unsigned int col_index) {
        this->col_index = col_index;
    }
    unsigned int get_start() {
        return this->start;
    }
    unsigned int get_end() {
        return this->end;
    }
    //! Column index with respect to MatrixA container for fetching
    //! necessary details of this column such as name.
    unsigned int get_col_index() {
        return this->col_index;
    }
};

//! Stores row related properties of ExtMatrixBInverse container
//! The limits to the row is [start, end) start includes and end excludes
//! Also includes the other necessary properties such as row_index for fetching 
//! necessary details from ExtMatrixA
class MatrixBInverseRowAttr {
private:
    unsigned int start, end;
    unsigned int row_index;
public:
    //! Simple Initialization
    MatrixARowAttr() {
    }
    MatrixBInverseRowAttr(unsigned int start, unsigned int end) {
        this->start = start;
        this->end = end;
    }
    void set_start(unsigned int start) {
        this->start = start;
    }
    void set_end(unsigned int end) {
        this->end = end;
    }
    void set_row_index(unsigned int col_index) {
        this->row_index = col_index;
    }
    unsigned int get_start() {
        return this->start;
    }
    unsigned int get_end() {
        return this->end;
    }
    //! Column index with respect to MatrixA container for fetching
    //! necessary details of this column such as name.
    unsigned int get_row_index() {
        return this->row_index;
    }
};

//! Storage Container for Fixed Matrix
//! The contents of the matrix do not change after assignment/initialization
//! Storage is done in both column-wise and row-wise fashion
class ExtMatrixBInverse {
private:
    //! Vector to store the Items onto external memory
    //! template parameters<ValueType, PageSize, CachePages, BlockSize, AllocStratg>
    typedef typename stxxl::VECTOR_GENERATOR<PackedElement, MATRIX_A_BLOCKS_PER_PAGE, MATRIX_A_PAGE_CACHE, MATRIX_A_BLOCK_SIZE>::result item_vector;
    //! Storage for column-wise representation
    item_vector m_vct_col_disk;
    const item_vector& m_vct_col_read_only_disk;
    //! Storage for row-wise representation
    item_vector m_vct_row_disk;
    const item_vector& m_vct_row_read_only_disk;
    //! Vector to keep track of columns in the matrix
    typedef typename stxxl::VECTOR_GENERATOR<MatrixAColAttr, 1, 1, MATRIX_A_COL_DATA_BLOCK_SIZE>::result col_data_vector;
    col_data_vector m_vct_col_attr;
    const col_data_vector& m_vct_read_only_col_attr;
    //! Vector to keep track of rows in the matrix
    typedef typename stxxl::VECTOR_GENERATOR<MatrixARowAttr, 1, 1, MATRIX_A_ROW_DATA_BLOCK_SIZE>::result row_data_vector;
    row_data_vector m_vct_row_attr;
    const row_data_vector& m_vct_read_only_row_attr;

public:
    //! Matrix Row and Column Attributes
    //    friend class MatrixAColAttr;
    typedef MatrixBInverseColAttr ColAttr;
    //    friend class MatrixARowAttr;
    typedef MatrixBInverseRowAttr RowAttr;

    //! Simple Initialization
    ExtMatrixBInverse() :
    m_vct_col_read_only_disk(m_vct_col_disk),
    m_vct_row_read_only_disk(m_vct_row_disk),
    m_vct_read_only_col_attr(m_vct_col_attr),
    m_vct_read_only_row_attr(m_vct_row_attr) {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////// - Matrix Column Operations - //////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    //! Adds a new column into the matrix
    //! Input column parameters include the Matrix A column index and the column values
    //! TODO - Set the start limit of col appropriately
    void add_column(PackedVector &p_packed_vector, unsigned int p_column_index) {
        unsigned int no_cols = get_columns_count(); // Last column number
        unsigned int last_col_end = (no_cols == 0) ? 0 : m_vct_col_attr[no_cols - 1].get_end();
        PackedVector::iterator itr = p_packed_vector.begin();
        while (itr != p_packed_vector.end()) {
            PackedElement packed_element = *itr;
            //            DEBUG("Adding Element: " << packed_element.get_index() << ", " << packed_element.get_value());
            m_vct_col_disk.push_back(packed_element);
            itr++;
        }
        ColAttr col_attr(last_col_end, last_col_end + p_packed_vector.get_nnz());
        //        DEBUG("Adding Col: " << col_attr.get_col_name() << " [" << col_attr.get_start() << ", " << col_attr.get_end() << ")");
        m_vct_col_attr.push_back(col_attr);
    }

    //! Store an Entire column excluding the objective value
    //! Send by Reference... Hence parameter PackedVector will be affected
    //! TODO - Check if the objective value is added properly to the end
    void store_column(unsigned int p_column_idx, PackedVector& p_packed_col) {
        // Initializing p_packed_col for proper storage
        p_packed_col.clear();
        p_packed_col.resize(get_rows_count());
        // Limits of the requested column
        ColAttr col_attr = m_vct_read_only_col_attr[p_column_idx];
        // Adding all the column vector values
        for (unsigned int i = col_attr.get_start(); i < col_attr.get_end(); i++) {
            const PackedElement packed_element = m_vct_col_read_only_disk[i];
            p_packed_col.add(packed_element.get_index(), packed_element.get_value(), false);
        }
    }

    //! Returns the column attributes associated with the p_col_index
    ColAttr get_col_attr(unsigned int p_col_index) {
        const ColAttr col_attr = m_vct_read_only_col_attr[p_col_index];
        return col_attr;
    }
    //! Returns the columns count - Total number of columns in the matrix
    unsigned int get_columns_count() {
        return m_vct_read_only_col_attr.size();
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

    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////// - Matrix Row Operations - /////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////


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

    //! gets the row attributes specific to a column
    RowAttr get_row_attr(unsigned int p_row_index) {
        const MatrixARowAttr row_attr = m_vct_read_only_row_attr[p_row_index];
        return row_attr;
    }

    //! sets the row attributes of a column
    void set_row_attr(unsigned int p_row_index, MatrixARowAttr& p_row_attr) {
        m_vct_row_attr[p_row_index] = p_row_attr;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////// - Common Matrix Operations - //////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////


};


#endif	/* EXTMATRIXBINVERSE_H */

