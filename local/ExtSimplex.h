/* 
 * File:   ExtSimplex.h
 * Author: Sriram Mahavadi
 *
 * Created on 13 May, 2014, 8:06 PM
 */

#ifndef EXTSIMPLEX_H
#define	EXTSIMPLEX_H

#include "ExtLPDSSet.h"
#include "ExtVectorUtil.h"
///////////////////////////////////////////////////////////
//! Simplex Algorithm.                                   //
//! Performs simplex algorithm over the external memory  //
///////////////////////////////////////////////////////////
class ExtSimplex {
private:
    //! Storage spaced of Matrix Data structures A and BInverse
    ExtLPDSSet& extDataSet;
    //! RHS vector to be stored in memory for better functionality.
    //! This vector is updated at every iteration of simplex.
    SimpleVector<REAL> m_vct_rhs;
    //! Objective Vector - Stores the Objective coefficients of base matrix
    SimpleVector<REAL> m_vct_obj;
    SimpleVector<unsigned int> m_base_col_indices;
    //! Count of number of iterations sofar
    unsigned int m_iteration_count;
public:
    //! Simple Initialization. Note that A matrix in the dataset is already 
    //! filled from MPS file
    ExtSimplex(ExtLPDSSet& p_extDataSet) :
    extDataSet(p_extDataSet),
    m_vct_rhs(p_extDataSet.A.get_rows_count()),
    m_vct_obj(p_extDataSet.A.get_rows_count()),
    m_base_col_indices(p_extDataSet.A.get_rows_count()),
    m_iteration_count(0) {
    }

    //! Initializing/Loading simplex. involves standardization of matrix A
    //! and initialization of B Inverse. 
    //! TODO - Need to find a proper base using pre-solve rather than Identity matrix every time.
    void init_simplex() {
        m_base_col_indices = extDataSet.A.standardize_matrix();
        extDataSet.BInverse.build_matrix_b_inverse(extDataSet.A, m_base_col_indices);
        extDataSet.A.store_row_rhs_values(m_vct_rhs);
        extDataSet.A.store_col_objective_values(m_vct_obj, m_base_col_indices);
    }
    //! solving the linear program
    void solve() {
        init_simplex(); // Loading the problem from simplex
        debug_base_col_indices();
        debug_rhs_values();
        debug_objective_values();
        update_simplex_iteration();
    }

    //! Returns the dimension m from the mxm base matrix
    unsigned int get_basis_dimension() {
        return m_base_col_indices.get_size();
    }

    //! Simplex iteration process
    void update_simplex_iteration() {
        PackedVector w(get_basis_dimension());
        ExtVectorUtil::store_dot_product(m_vct_obj, extDataSet.BInverse, w);
        debug_w(w);
        ExtMatrixA& A = extDataSet.A; // Simple reference to make coding easy
        ExtMatrixBInverse &b_inverse = extDataSet.BInverse;
        // Finding the Entering Variable
        // Getting j for which cj-zj gives max positive value
        unsigned int k = -1;
        REAL max_reduced_cost = 0.0F;
        for (unsigned int j = 0; j < A.get_columns_count(); j++) {
            ExtMatrixA::ColAttr col_attr = A.get_col_attr(j);
            if (!col_attr.get_is_basic_col()) {
                // If the column is a non basic column
                PackedVector a_col_vector(get_basis_dimension());
                A.store_column(j, a_col_vector, false);
                REAL cj = col_attr.get_objective_value(); // cj value
                REAL zj = 0.0F;
                ExtVectorUtil::store_dot_product(w, a_col_vector, zj);
                REAL reduced_cost = cj - zj;
                if (reduced_cost > max_reduced_cost) {
                    max_reduced_cost = reduced_cost;
                    k = j;
                }
                DEBUG("Col: " << j << ", Reduced Cost: " << reduced_cost);
            }
        }
        //! Col Vector for computation purpose
        PackedVector col_vector(get_basis_dimension());
        // Finding the Leaving Variable based on - min br/ykr
        //! Finding yk
        A.store_column(k, col_vector, false);
        debug_ak(col_vector);
        SimpleVector<REAL> y(get_basis_dimension());
        ExtVectorUtil::store_dot_product(b_inverse, col_vector, y);

        unsigned int r; // leaving variable
        REAL min_ratio = 0.0F;
        //! Finding r - leaving variable 
        for (unsigned int row_index = 0; row_index < y.get_size(); row_index++) {
            REAL ratio = m_vct_rhs[row_index] / y[row_index];
            if ((min_ratio == 0.0F && ratio > 0.0) ||
                    (ratio > 0.0 && ratio < min_ratio)) {
                min_ratio = ratio;
                r = row_index;
            }
        }
        //! Updating Rhs 
        //! Todo - Upadating with Eta Vector
        //        col_vector.store_from_vector(m_vct_rhs);
        //        m_vct_rhs.nullify(); // Clearing the existing values
        //        ExtVectorUtil::store_dot_product(b_inverse, col_vector, m_vct_rhs);
        // Getting the Eta Vector

        EtaVector eta_vector(r, y);
        debug_rhs();
        debug_y(y);
        debug_eta(eta_vector);
        debug_b_inverse();
    }
    //! validates if further iterations need to take place
    bool vaidate_next_iteration() {
        return true;
    }
    //////////////////////////////////////////////////////////////////////
    ///////////////////////// Debug Functions ////////////////////////////
    //////////////////////////////////////////////////////////////////////

    //! Debugging base column storage
    void debug_base_col_indices() {
        DEBUG("Base Col Indices Debug. Base cols are as follows.");
        std::stringstream base_col_stream;
        for (unsigned int i = 0; i < m_base_col_indices.get_size(); i++) {
            unsigned int a_col_index = m_base_col_indices[i];
            base_col_stream << i << ":A[" << a_col_index << "]: " << extDataSet.A.get_col_attr(a_col_index).get_col_name() << ", ";
        }
        DEBUG_SIMPLE(base_col_stream.str());
    }

    //! Debugging the Rhs values retained
    void debug_rhs_values() {
        DEBUG("Rhs Values Debug. Rhs values are as follows.");
        std::stringstream rhs_stream;
        for (unsigned int i = 0; i < m_vct_rhs.get_size(); i++) {
            rhs_stream << i << ": " << m_vct_rhs[i] << ", ";
        }
        DEBUG_SIMPLE(rhs_stream.str());
    }

    //! Debugging the Objective values retained
    void debug_objective_values() {
        DEBUG("Objective Values(Coefficients) Debug. Values are as follows.");
        std::stringstream obj_stream;
        for (unsigned int i = 0; i < m_vct_obj.get_size(); i++) {
            obj_stream << "cb[" << i << "]: " << m_vct_obj[i] << ", ";
        }
        DEBUG_SIMPLE(obj_stream.str());
    }
    void debug_w(PackedVector& w) {
        DEBUG("W = C*BInverse Values Debug. Values are as follows.");
        std::stringstream w_stream;
        PackedVector::iterator itr = w.begin();
        unsigned int prev_index = 0;
        while (itr != w.end()) {
            PackedElement packed_element = *itr;
            unsigned int current_index = packed_element.get_index();
            while (prev_index < current_index) {
                w_stream << "w[" << prev_index << "]: " << 0.0F << ", ";
                prev_index++;
            }
            w_stream << "w[" << prev_index << "]: " << packed_element.get_value() << ", ";
            prev_index++;
            itr++;
        }
        // Padding trailing zeros
        while (prev_index < w.get_real_size()) {
            w_stream << "w[" << prev_index << "]: " << 0.0F << ", ";
            prev_index++;
        }
        DEBUG_SIMPLE(w_stream.str());
    }

    //! Debugging the y values retained
    void debug_y(SimpleVector<REAL> y) {
        DEBUG("y(Simplex Multipliers) Debug. Values are as follows.");
        std::stringstream y_stream;
        for (unsigned int i = 0; i < y.get_size(); i++) {
            y_stream << "y[" << i << "]: " << y[i] << ", ";
        }
        DEBUG_SIMPLE(y_stream.str());
    }
    void debug_ak(PackedVector& ak) {
        DEBUG("ak = [ Entering col ]. Values are as follows.");
        std::stringstream ak_stream;
        PackedVector::iterator itr = ak.begin();
        unsigned int prev_index = 0;
        while (itr != ak.end()) {
            PackedElement packed_element = *itr;
            unsigned int current_index = packed_element.get_index();
            while (prev_index < current_index) {
                ak_stream << "ak[" << prev_index << "]: " << 0.0F << ", ";
                prev_index++;
            }
            ak_stream << "ak[" << prev_index << "]: " << packed_element.get_value() << ", ";
            prev_index++;
            itr++;
        }
        // Padding trailing zeros
        while (prev_index < ak.get_real_size()) {
            ak_stream << "w[" << prev_index << "]: " << 0.0F << ", ";
            prev_index++;
        }
        DEBUG_SIMPLE(ak_stream.str());
    }
    void debug_rhs() {
        DEBUG("rhs Debug. Values are as follows.");
        std::stringstream rhs_stream;
        for (unsigned int i = 0; i < m_vct_rhs.get_size(); i++) {
            rhs_stream << "rhs[" << i << "]: " << m_vct_rhs[i] << ", ";
        }
        DEBUG_SIMPLE(rhs_stream.str());
    }
    void debug_eta(EtaVector& eta) {
        PackedVector& eta_col = eta.get_col_vector();
        DEBUG("eta_col = [ Non-Identity col ]. Values are as follows.");
        std::stringstream eta_stream;
        eta_stream << "eta_col[r]: " << eta.get_col_index() << ", ";
        PackedVector::iterator itr = eta_col.begin();
        unsigned int prev_index = 0;
        while (itr != eta_col.end()) {
            PackedElement packed_element = *itr;
            unsigned int current_index = packed_element.get_index();
            while (prev_index < current_index) {
                eta_stream << "eta_col[" << prev_index << "]: " << 0.0F << ", ";
                prev_index++;
            }
            eta_stream << "eta_col[" << prev_index << "]: " << packed_element.get_value() << ", ";
            prev_index++;
            itr++;
        }
        // Padding trailing zeros
        while (prev_index < eta_col.get_real_size()) {
            eta_stream << "eta_col[" << prev_index << "]: " << 0.0F << ", ";
            prev_index++;
        }
        DEBUG_SIMPLE(eta_stream.str());
    }
    void debug_b_inverse() {
        ExtMatrixA& A = extDataSet.A;
        ExtMatrixBInverse& BInverse = extDataSet.BInverse;
        unsigned int width = MAX_KEY_LEN;
        unsigned int rows_count = BInverse.get_rows_count();
        unsigned int cols_count = BInverse.get_columns_count();
        /////////////////////////////////////////////////////////////////////
        ////////////////////////// - Column wise display - //////////////////
        /////////////////////////////////////////////////////////////////////
        DEBUG("Column-wise Display of B-Inverse: ");
        DEBUG_SIMPLE("** Rows: " << BInverse.get_rows_count() << "; Columns: " << BInverse.get_columns_count() << " ***");
        std::stringstream rows_title_stream;
        rows_title_stream << std::setw(width) << "Cols/Rows";
        for (unsigned int i = 0; i < rows_count; i++) {
            rows_title_stream << std::setw(width) << A.get_row_attr(i).get_row_name() << ", ";
        }
        DEBUG_SIMPLE(rows_title_stream.str());

        for (unsigned int col_index = 0; col_index < cols_count; col_index++) {
            PackedVector packed_column(rows_count);
            BInverse.store_column(col_index, packed_column);
            unsigned int a_col_index = BInverse.get_col_attr(col_index).get_col_index();
            std::stringstream col_stream;
            col_stream << std::setw(width) << A.get_col_attr(a_col_index).get_col_name();
            unsigned int prev_row_index = 0;
            PackedVector::iterator itr = packed_column.begin();
            while (itr != packed_column.end()) {
                PackedElement packed_element = *itr;
                unsigned int current_row_index = packed_element.get_index();
                assert(current_row_index >= prev_row_index);
                while (prev_row_index < current_row_index) {
                    col_stream << std::setw(width) << 0.0F << ", ";
                    prev_row_index++;
                }
                col_stream << std::setw(width) << packed_element.get_value() << ", ";
                prev_row_index++;
                itr++;
            }
            while (prev_row_index < rows_count) {
                col_stream << std::setw(width) << 0.0F << ", ";
                prev_row_index++;
            }
            DEBUG_SIMPLE(col_stream.str());
        }
    }
};

#endif	/* EXTSIMPLEX_H */

