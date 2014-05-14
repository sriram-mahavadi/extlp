/* 
 * File:   ExtVectorUtil.h
 * Author: Sriram Mahavadi
 *
 * Created on 6 May, 2014, 1:47 AM
 */

#ifndef EXTVECTORUTIL_H
#define	EXTVECTORUTIL_H
#include "GlobalDefines.h"
#include "PackedVector.h"
#include "ExtMatrixBInverse.h"

//! Util functions for Vector Arithmetic.
//! Vector arithmetic is performed on Packed/Unpacked Vectors 
//! both from physical and external memory
class ExtVectorUtil {
private:
    //! Scratch Pad for scattering 
    static std::vector<REAL> m_scratch_pad;

    //! Validate if the scratch pad can access the index
    static void _validate_scratch_pad(unsigned int index) {
        if (index >= m_scratch_pad.size()) {
            _resize_scratch_pad(index + 1);
        }
    }
    //! Resizes the scratch pad to the parameter size
    static void _resize_scratch_pad(unsigned int size) {
        m_scratch_pad.resize(size);
    }
    static REAL _get_scratch_pad_element(unsigned int index) {
        _validate_scratch_pad(index);
        return m_scratch_pad[index];
    }
    static void _set_scratch_pad_element(unsigned int index, REAL value) {
        _validate_scratch_pad(index);
        m_scratch_pad[index] = value;
    }
    //! checks for the condition that scratch pad needs to be zero before any operation.
    static void _validate_scratch_pad() {
        for (unsigned int i = 0; i < m_scratch_pad.size(); i++)
            assert(_get_scratch_pad_element(i) == 0);
    }
public:
    //! Scalar Product. Storing Result in packed vector
    static void store_scalar_product(REAL scalar_value, PackedVector& p_vct, PackedVector& p_result) {
        //! Setting up result for storage
        p_result.clear();
        p_result.resize(p_vct.get_real_size());
        PackedVector::iterator itr = p_vct.begin();
        while (itr != p_vct.end()) {
            PackedElement packed_element = *itr;
            REAL value = packed_element.get_value();
            packed_element.set_value(scalar_value * value);
            p_result.add(packed_element.get_index(), packed_element.get_value(), false);
            itr++;
        }
    }

    //! Scalar Product. Storing Result in Scattered Vector 
    //! appending is very necessary while doing column-wise matrix multiplication
    static void append_scalar_product(REAL scalar_value, PackedVector& p_vct, SimpleVector<REAL>& p_result) {
        //! Setting up result for storage
        //        DEBUG("Actual Size: "<<p_vct.get_real_size()<<"; Packed Size: "<<p_vct.get_nnz());
        //        DEBUG("Scalar: " << scalar_value);
        //        DEBUG("Size of SimpleVector: " << p_result.get_size());
        PackedVector::iterator itr = p_vct.begin();
        while (itr != p_vct.end()) {
            PackedElement packed_element = *itr;
            unsigned int index = packed_element.get_index();
            REAL value = packed_element.get_value() * scalar_value;

            //            p_result[index] = p_result[index] + value;
            REAL existing_value = p_result.get_element(index);
            //            DEBUG("Index: " << index << "; Value: " << value << "; Existing Value: " << existing_value);
            p_result.add(index, value + existing_value);
            itr++;
        }
    }

    //! Dot product of two vectors(one row and one col vector) < vct1.vct2 >.
    //! Both of the vectors are packed
    static void store_dot_product(PackedVector& p_vct1, PackedVector& p_vct2, REAL& p_dot_product) {
        assert(p_vct1.get_real_size() == p_vct2.get_real_size());
        //        DEBUG("Actual vct1 Size: " << p_vct1.get_real_size() << "; Packed vct1 Size: " << p_vct1.get_nnz());
        //        DEBUG("Actual vct2 Size: " << p_vct2.get_real_size() << "; Packed vct2 Size: " << p_vct2.get_nnz());
        bool is_vct2_best_scatter = false;
        // Both vct1 and vct2 are packed. vct1 has greater sparsity than vct2.
        is_vct2_best_scatter = (p_vct1.get_sparsity() > p_vct2.get_sparsity());
        // vct2 is a better scatter 
        if (is_vct2_best_scatter) {
            store_dot_product(p_vct2, p_vct1, p_dot_product);
            return;
        }
        REAL dot_product_value = 0.0F;
        // scatter vct1 into scratch pad 
        PackedVector::iterator itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement &packed_element = *itr;
            _set_scratch_pad_element(packed_element.get_index(), packed_element.get_value());
            itr++;
        }
        // Calculating dot product 
        itr = p_vct2.begin();
        while (itr != p_vct2.end()) {
            PackedElement &packed_element = *itr;
            REAL vct1_value = _get_scratch_pad_element(packed_element.get_index());
            if (vct1_value != 0.0F) {
                dot_product_value += (vct1_value * packed_element.get_value());
            }
            itr++;
        }
        // re-initialize scratch_pad to 0
        itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement &packed_element = *itr;
            _set_scratch_pad_element(packed_element.get_index(), 0.0F);
            itr++;
        }
        p_dot_product = dot_product_value;
    }


    //! Dot product of two vectors(one row and one col vector) < vct1.vct2 >.
    //! One of the vector is already scattered. 
    static void store_dot_product(SimpleVector<REAL>& p_vct1, PackedVector& p_vct2, REAL& p_dot_product) {
        REAL dot_product_value = 0.0F;
        // Calculating dot product, Already p_vct1 is scattered
        PackedVector::iterator itr = p_vct2.begin();
        while (itr != p_vct2.end()) {
            PackedElement &packed_element = *itr;
            REAL vct2_index = packed_element.get_index();
            REAL vct2_value = packed_element.get_value();
            REAL vct1_value = p_vct1[vct2_index];
            dot_product_value += (vct1_value * vct2_value);
            itr++;
        }
        p_dot_product = dot_product_value;
    }

    //! Packed Row Vector multiplied by Matrix
    static void store_dot_product(PackedVector& p_vct1, ExtMatrixBInverse& p_BInverse, PackedVector& p_dot_product) {
        for (unsigned int col_index = 0; col_index < p_BInverse.get_columns_count(); col_index++) {
            PackedVector vct2(p_BInverse.get_rows_count());
            REAL col_value = 0.0F;
            store_dot_product(p_vct1, vct2, col_value);
            p_dot_product.add(col_index, col_value, false);
        }
    }

    //! Scattered Row Vector multiplied by Matrix
    static void store_dot_product(SimpleVector<REAL>& p_vct1, ExtMatrixBInverse& p_BInverse, PackedVector& p_dot_product) {
        for (unsigned int col_index = 0; col_index < p_BInverse.get_columns_count(); col_index++) {
            PackedVector vct2(p_BInverse.get_rows_count());
            p_BInverse.store_column(col_index, vct2);
            REAL col_value = 0.0F;
            store_dot_product(p_vct1, vct2, col_value);
            p_dot_product.add(col_index, col_value, false);
        }
    }

    //! Matrix multiplied by Column Vector
    //! Used in calculating < BInverse x aj >, < BInverse x b >
    static void store_dot_product(ExtMatrixBInverse& p_BInverse, PackedVector& p_vct, SimpleVector<REAL>& p_dot_product) {
        // Making dot product vector suitable for storage
        //        p_dot_product.nullify();
        //        p_dot_product.resize(p_BInverse.get_rows_count());
        //        DEBUG("Actual Size: "<<p_vct.get_real_size()<<"; Packed Size: "<<p_vct.get_nnz());
        PackedVector::iterator itr = p_vct.begin();
        while (itr != p_vct.end()) {
            unsigned int col_index = (*itr).get_index();
            REAL col_value = (*itr).get_value();
            //            DEBUG("Index: "<<col_index<<", Value: "<<col_value);
            PackedVector bInverse_col(p_BInverse.get_rows_count());
            p_BInverse.store_column(col_index, bInverse_col);
            append_scalar_product(col_value, bInverse_col, p_dot_product);
            itr++;
        }

    }

    //! Sum of vectors < vct1 + (lamda)vct2 >
    //! TODO - Check which vector can be better scattered
    static void store_sum(PackedVector& p_vct1, PackedVector& p_vct2, REAL lambda, PackedVector& p_vct_result) {
        // scatter vct1 into scratch pad 
        PackedVector::iterator itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement &packed_element = *itr;
            _set_scratch_pad_element(packed_element.get_index(), packed_element.get_value());
            itr++;
        }
        // Calculating dot product 
        itr = p_vct2.begin();
        while (itr != p_vct2.end()) {
            PackedElement &packed_element = *itr;
            unsigned int vct2_index = packed_element.get_index();
            REAL vct1_value = _get_scratch_pad_element(vct2_index);
            REAL vct2_value = packed_element.get_value();
            REAL element_value = vct1_value + lambda * vct2_value;
            p_vct_result.add(vct2_index, element_value, false);
            _set_scratch_pad_element(vct2_index, 0.0F);
            itr++;
        }
        // re-initialize scratch_pad to 0
        itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement &packed_element = *itr;
            unsigned int vct1_index = packed_element.get_index();
            if (_get_scratch_pad_element(vct1_index) != 0.0F) {
                REAL element_value = packed_element.get_value();
                p_vct_result.add(vct1_index, element_value, false);
                _set_scratch_pad_element(packed_element.get_index(), 0.0F);
            }
            itr++;
        }
    }

        //! Sum of vectors < vct1 + (lamda)vct2 >
    //! TODO - Check which vector can be better scattered
    static void store_sum(PackedVector& p_vct1, PackedVector& p_vct2, REAL lambda, SimpleVector<REAL>& p_vct_result) {
        // scatter vct1 into scratch pad 
        PackedVector::iterator itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement &packed_element = *itr;
            _set_scratch_pad_element(packed_element.get_index(), packed_element.get_value());
            itr++;
        }
        // Calculating dot product 
        itr = p_vct2.begin();
        while (itr != p_vct2.end()) {
            PackedElement &packed_element = *itr;
            unsigned int vct2_index = packed_element.get_index();
            REAL vct1_value = _get_scratch_pad_element(vct2_index);
            REAL vct2_value = packed_element.get_value();
            REAL element_value = vct1_value + lambda * vct2_value;
            p_vct_result.add(vct2_index, element_value, false);
            _set_scratch_pad_element(vct2_index, 0.0F);
            itr++;
        }
        // re-initialize scratch_pad to 0
        itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement &packed_element = *itr;
            unsigned int vct1_index = packed_element.get_index();
            if (_get_scratch_pad_element(vct1_index) != 0.0F) {
                REAL element_value = packed_element.get_value();
                p_vct_result.add(vct1_index, element_value, false);
                _set_scratch_pad_element(packed_element.get_index(), 0.0F);
            }
            itr++;
        }
    }
    
    //! Sorts given packed vector by first scattering and then compacting
    static void sort_packed_vector_based_on_index(PackedVector& packed_vector) {
        //        _validate_scratch_pad();
        // Scattering packed vector
        PackedVector::iterator itr = packed_vector.begin();
        while (itr != packed_vector.end()) {
            _set_scratch_pad_element((*itr).get_index(), (*itr).get_value());
        }
        // Replacing Packed Vector with Sorted Packed Vector
        // Compacting
        itr = packed_vector.begin();
        for (unsigned int index = 0; index < packed_vector.get_real_size(); index++) {
            REAL value = _get_scratch_pad_element(index);
            if (value != 0.0F) {
                PackedElement packed_element(index, value);
                *itr = packed_element;
                _set_scratch_pad_element(index, 0.0F);
            }
        }
    }

    //! Updating the basis inverse. Called at every iteration step of simplex
    static void update_basis_inverse(EtaVector& eta_vector, ExtMatrixBInverse& b_inverse) {
        unsigned int r = eta_vector.get_col_index();
        PackedVector& eta_packed_vector = eta_vector.get_col_vector();
        ExtMatrixBInverse new_b_inverse;
        for (unsigned int col_index = 0; col_index < b_inverse.get_columns_count(); col_index++) {
            PackedVector b_inverse_col(b_inverse.get_rows_count());
            b_inverse.store_column(col_index, b_inverse_col);
            unsigned int a_col_index = b_inverse.get_col_attr(col_index).get_a_col_index();
            REAL r_value = b_inverse_col.get(r);
            if (r_value == 0.0F) {
                // No change in column
                // Getting the a_col_index with respect to B_inverse
                new_b_inverse.add_column(b_inverse_col, a_col_index);
            } else {
                PackedVector new_b_inverse_col(b_inverse.get_rows_count());
                PackedElement r_packed_element(r, r_value);
                b_inverse_col.remove(r_packed_element);
                store_sum(b_inverse_col, eta_packed_vector, r_value, new_b_inverse_col);
                new_b_inverse.add_column(new_b_inverse_col, a_col_index);
            }
        }
        b_inverse.swap(new_b_inverse);
        //                std::swap(b_inverse, new_b_inverse);
    }

    //! Updating the basis inverse. Called at every iteration step of simplex
    //! TODO - Improve performance by using only the rhs_vector
    static void update_rhs(EtaVector& eta_vector, SimpleVector<REAL>& rhs_vector) {
        unsigned int r = eta_vector.get_col_index();
        PackedVector& eta_packed_vector = eta_vector.get_col_vector();
        REAL r_value = rhs_vector[r]; // Getting r_value
        if(r_value==0.0F) return; // no change in rhs column
        rhs_vector[r] = 0.0F;
        PackedVector rhs_packed_vector(rhs_vector);
        store_sum(rhs_packed_vector, eta_packed_vector, r_value, rhs_vector);
    }
};
// Defining scratch pad
std::vector<REAL> ExtVectorUtil::m_scratch_pad;
#endif	/* EXTVECTORUTIL_H */

