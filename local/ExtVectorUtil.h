/* 
 * File:   ExtVectorUtil.h
 * Author: Sriram Mahavadi
 *
 * Created on 6 May, 2014, 1:47 AM
 */

#ifndef EXTVECTORUTIL_H
#define	EXTVECTORUTIL_H
#include "GlobalDefines.h"
#include "ExtStxxlVector.h"
#include "PackedVector.h"

//! Util functions for Vector Arithmetic.
//! Vector arithmetic is performed on Packed/Unpacked Vectors 
//! both from physical and external memory
class ExtVectorUtil {
private:
    //! Scratch Pad for scattering 
    static vector<REAL> m_scratch_pad;

    //! Validate if the scratch pad can access the index
    void _validate_scratch_pad(unsigned int index) {
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
public:
    //! Dot product of vectors < vct1.vct2 >
    static void store_dot_product(PackedVector& p_vct1, PackedVector& p_vct2, REAL& p_dot_product) {
        bool is_vct2_best_scatter = false;
        // Both vct1 and vct2 are packed. vct1 has greater sparsity than vct2.
        is_vct2_best_scatter = (p_vct1.get_sparsity() > p_vct2.get_sparsity());
        // vct2 is a better scatter 
        if (is_vct2_best_scatter) store_dot_product(p_vct2, p_vct1, p_dot_product);
        REAL dot_product_value = 0.0F;
        // scatter vct1 into scratch pad 
        PackedVector::iterator itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement &packed_element = *itr;
            _set_scratch_pad_element(packed_element.get_index(), packed_element.get_value());
        }
        // Calculating dot product 
        itr = p_vct2.begin();
        while (itr != p_vct2.end()) {
            PackedElement &packed_element = *itr;
            REAL vct1_value = _get_scratch_pad_element(packed_element.get_index());
            if (vct1_value != 0.0F) {
                dot_product_value += (vct1_value * packed_element.get_value());
            }
        }
        // re-initialize scratch_pad to 0
        itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement<REAL> &packed_element = *itr;
            _set_scratch_pad_element(packed_element.get_index(), 0.0F);
        }
        p_dot_product = dot_product_value;
    }

    //! Sum of vectors < vct1 + (lamda)vct2 >
    //! TODO - Check which vector can be better scattered
    static void store_sum(PackedVector<REAL>& p_vct1, PackedVector<REAL>& p_vct2, REAL lambda, PackedVector<REAL>& p_vct_result) {
        // scatter vct1 into scratch pad 
        PackedVector<REAL>::iterator itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement<REAL> &packed_element = *itr;
            _set_scratch_pad_element(packed_element.get_index(), packed_element.get_value());
        }
        // Calculating dot product 
        itr = p_vct2.begin();
        while (itr != p_vct2.end()) {
            PackedElement<REAL> &packed_element = *itr;
            unsigned int vct2_index = packed_element.get_index();
            REAL vct1_value = _get_scratch_pad_element(vct2_index);
            REAL vct2_value = packed_element.get_value();
            REAL element_value = vct1_value + lambda * vct2_value;
            p_vct_result.add(vct2_index, element_value);
            _set_scratch_pad_element(vct2_index, 0.0F);
        }
        // re-initialize scratch_pad to 0
        itr = p_vct1.begin();
        while (itr != p_vct1.end()) {
            PackedElement<REAL> &packed_element = *itr;
            unsigned int vct1_index = packed_element.get_index();
            if (_get_scratch_pad_element(vct1_index) != 0.0F) {
                REAL element_value = packed_element.get_value();
                p_vct_result.add(vct1_index, element_value);
                _set_scratch_pad_element(packed_element.get_index(), 0.0F);
            }
        }
    }
};
#endif	/* EXTVECTORUTIL_H */

