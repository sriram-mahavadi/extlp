/* 
 * File:   ExtMatrixBInverse.h
 * Author: Sriram Mahavadi
 *
 * Created on 11 May, 2014, 11:17 PM
 */

#ifndef EXTMATRIXBINVERSE_H
#define	EXTMATRIXBINVERSE_H
#include "GlobalDefines.h"
#include "ExtLPDSSet.h"
#include "EtaVector.h"
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
    //    unsigned int row_index; No need of row index. By default is present in order
    //! Used for pre calculation of row-size during column-wise storage
    unsigned int capacity_end;
public:
    //! Simple Initialization
    MatrixBInverseRowAttr() {
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
    //    void set_row_index(unsigned int row_index) {
    //        this->row_index = row_index;
    //    }
    void set_capacity_end(unsigned int capacity_end) {
        this->capacity_end = capacity_end;
    }
    unsigned int get_start() {
        return this->start;
    }
    unsigned int get_end() {
        return this->end;
    }
    //! Column index with respect to MatrixA container for fetching
    //! necessary details of this column such as name.
    //    unsigned int get_row_index() {
    //        return this->row_index;
    //    }
    unsigned int get_capacity_end() {
        return this->capacity_end;
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
    const item_vector& m_vct_read_only_col_disk;
    //! Storage for row-wise representation
    item_vector m_vct_row_disk;
    const item_vector& m_vct_read_only_row_disk;
    //! Vector to keep track of columns in the matrix
    typedef typename stxxl::VECTOR_GENERATOR<MatrixBInverseColAttr, 1, 1, MATRIX_A_COL_DATA_BLOCK_SIZE>::result col_data_vector;
    col_data_vector m_vct_col_attr;
    const col_data_vector& m_vct_read_only_col_attr;
    //! Vector to keep track of rows in the matrix
    typedef typename stxxl::VECTOR_GENERATOR<MatrixBInverseRowAttr, 1, 1, MATRIX_A_ROW_DATA_BLOCK_SIZE>::result row_data_vector;
    row_data_vector m_vct_row_attr;
    const row_data_vector& m_vct_read_only_row_attr;
    //! Indicator whether row-wise storage is necessary
    bool m_is_row_build_necessary;
public:
    //! Matrix Row and Column Attributes
    friend class MatrixBInverseColAttr;
    typedef MatrixBInverseColAttr ColAttr;
    //    friend class MatrixARowAttr;
    typedef MatrixBInverseRowAttr RowAttr;

    //! Simple Initialization
    ExtMatrixBInverse(bool p_is_row_build_necessary = true) :
    m_vct_read_only_col_disk(m_vct_col_disk),
    m_vct_read_only_row_disk(m_vct_row_disk),
    m_vct_read_only_col_attr(m_vct_col_attr),
    m_vct_read_only_row_attr(m_vct_row_attr),
    m_is_row_build_necessary(p_is_row_build_necessary) {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////// - Matrix Column Operations - //////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    //! Adds a new column into the matrix
    //! Input column parameters include the Matrix A column index and the column values
    void add_column(PackedVector &p_packed_vector, unsigned int p_column_index) {
        unsigned int no_cols = get_columns_count(); // Last column number
        unsigned int last_col_end = (no_cols == 0) ? 0 : m_vct_col_attr[no_cols - 1].get_end();
        PackedVector::iterator itr = p_packed_vector.begin();
        while (itr != p_packed_vector.end()) {
            PackedElement packed_element = *itr;
            //            DEBUG("Adding Element: " << packed_element.get_index() << ", " << packed_element.get_value());
            // If row wise representation is necessary
            if (m_is_row_build_necessary) {
                // Keeping track of rows capacity pertaining to the packed element
                unsigned int row_index = packed_element.get_index();
                if (get_rows_count() <= row_index) {
                    m_vct_row_attr.resize(row_index + 1);
                    m_vct_row_attr[row_index].set_capacity_end(0);
                }
                m_vct_row_attr[row_index].set_capacity_end(m_vct_row_attr[row_index].get_capacity_end() + 1);
            }
            m_vct_col_disk.push_back(packed_element);
            itr++;
        }
        ColAttr col_attr(last_col_end, last_col_end + p_packed_vector.get_nnz());
        col_attr.set_col_index(p_column_index);
        //        DEBUG("Adding Col: " << col_attr.get_col_name() << " [" << col_attr.get_start() << ", " << col_attr.get_end() << ")");
        m_vct_col_attr.push_back(col_attr);
    }

    //! Store an Entire column of basis inverse matrix
    //! Send by Reference... Hence parameter PackedVector will be affected
    void store_column(unsigned int p_column_idx, PackedVector& p_packed_col) {
        // Initializing p_packed_col for proper storage
        p_packed_col.clear();
        p_packed_col.resize(get_rows_count());
        // Limits of the requested column
        ColAttr col_attr = m_vct_read_only_col_attr[p_column_idx];
        // Adding all the column vector values
        for (unsigned int i = col_attr.get_start(); i < col_attr.get_end(); i++) {
            const PackedElement packed_element = m_vct_read_only_col_disk[i];
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
        ColAttr col_attr = m_vct_read_only_col_attr[p_col_index];
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

    //! PreCalculation of row-wise matrix storage structure
    //! The capacity_end refers to the end index till which the column can be reached
    //! start and end by default refer to the start index of column. (empty column)
    void pre_calculate_row_structure() {
        unsigned int capacity_end = 0;
        RowAttr row_attr;
        for (unsigned int i = 0; i < m_vct_row_attr.size(); i++) {
            //            DEBUG("Row: "<<i<<"; Start: "<<m_vct_col_attr)
            row_attr.set_start(capacity_end);
            row_attr.set_end(capacity_end);
            capacity_end += m_vct_row_attr[i].get_capacity_end();
            row_attr.set_capacity_end(capacity_end);
            set_row_attr(i, row_attr);
            //            DEBUG("Row: "<<i<<"; end: "<<row_attr.get_end()<<"; capacity: "<<row_attr.get_capacity_end());
        }
    }

    //! Adds a new row element into the matrix
    //! Clumsy operation - Might involve random block fetching without any locality of reference.
    void add_row_element(unsigned int row_index, PackedElement packed_element) {
        RowAttr row_attr = m_vct_row_attr[row_index];
        unsigned int end_index = row_attr.get_end();
        unsigned int capacity_end = row_attr.get_capacity_end();
        assert(end_index < capacity_end); // Check if there is space in the column to add
        if (end_index >= m_vct_row_disk.size()) {
            m_vct_row_disk.resize(end_index + 1);
        }
        m_vct_row_disk[end_index] = packed_element;
        row_attr.set_end(end_index + 1);
        m_vct_row_attr[row_index] = row_attr;
    }

    //! Store an Entire row of the Basis inverse matrix
    //! Send by Reference... Hence parameter PackedVector will be affected
    void store_row(unsigned int p_row_idx, PackedVector& p_packed_row) {
        // Initializing p_packed_row for proper storage
        p_packed_row.clear();
        p_packed_row.resize(get_columns_count());
        // Limits of the requested column
        RowAttr row_attr = m_vct_read_only_row_attr[p_row_idx];
        // Adding all the column vector values
        for (unsigned int i = row_attr.get_start(); i < row_attr.get_end(); i++) {
            const PackedElement packed_element = m_vct_read_only_row_disk[i];
            p_packed_row.add(packed_element.get_index(), packed_element.get_value(), false);
        }
    }

    //! Returns the rows count - Total number of rows in the matrix
    unsigned int get_rows_count() {
        return m_vct_read_only_row_attr.size();
    }

    //! gets the row attributes specific to a column
    RowAttr get_row_attr(unsigned int p_row_index) {
        const RowAttr row_attr = m_vct_read_only_row_attr[p_row_index];
        return row_attr;
    }

    //! sets the row attributes of a column
    void set_row_attr(unsigned int p_row_index, RowAttr p_row_attr) {
        m_vct_row_attr[p_row_index] = p_row_attr;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////// - Common Matrix Operations - //////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    //! Clearing the whole BInverse container 
    void clear_matrix_b_inverse() {
        m_vct_col_attr.clear();
        m_vct_row_attr.clear();
        m_vct_col_disk.clear();
        m_vct_row_disk.clear();
    }

    //! Building the matrix from Matrix A and the basis col matrix
    //! Initial Assumption AX + S = b making S (Identity matrix)to be the B and B inverse
    //! TODO - Always it is not fair to choose identity matrix as base. Choose wisely.
    void build_matrix_b_inverse(ExtMatrixA &A, SimpleVector<unsigned int> base_col_indices) {
        // Building column matrix
        for (unsigned int i = 0; i < base_col_indices.get_size(); i++) {
            unsigned int col_index = base_col_indices[i];
            PackedVector packed_col_vector_i(A.get_rows_count());
            A.store_column(col_index, packed_col_vector_i, false);
            add_column(packed_col_vector_i, col_index);
        }
        // If row wise storage is not necessary
        if(!m_is_row_build_necessary)return;
        // Building row matrix
        pre_calculate_row_structure();
        for (unsigned int col_index = 0; col_index < get_columns_count(); col_index++) {
            PackedVector packed_col(get_rows_count());
            store_column(col_index, packed_col);
            PackedVector::iterator itr = packed_col.begin();
            while (itr != packed_col.end()) {
                // Row packed Vector with <column index, value> pair
                unsigned int row_index = (*itr).get_index();
                PackedElement packed_element;
                packed_element.set_index(col_index);
                packed_element.set_value((*itr).get_value());
                add_row_element(row_index, packed_element);
                itr++;
            }
        }

    }

    //! ReBuilding the matrix from itself and Eta Vector
    void rebuild_matrix_b_inverse(EtaVector& eta_vector) {

    }
};


#endif	/* EXTMATRIXBINVERSE_H */

