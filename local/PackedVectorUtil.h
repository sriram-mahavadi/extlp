/* 
 * File:   PackedVectorUtil.h
 * Author: sriram
 *
 * Created on 5 April, 2014, 12:43 PM
 */

#ifndef PACKEDVECTORUTIL_H
#define	PACKEDVECTORUTIL_H

#include "PackedVector.h"
class PackedVectorUtil {
    
    PackedVector multiply(PackedVector &vct1, PackedVector &vct2) {
        PackedVector result;
        // Make sure that the first vector is the one which is unpacked
        if (!vct1.isPacked() && vct2.isPacked()) multiply(vct1, vct2);
        if (!vct1.isPacked()) { // Both are unpacked vectors

        } else if (!vct2.isPacked()) { // vct1 is packed and vct2 is unpacked

        } else { // Both vct1 and vct2 are packed vectors

        }
        return result;
    }
    
    PackedVector add(PackedVector vct1, PackedVector vct2) {
        PackedVector result;
        // Make sure that the first vector is the one which is unpacked
        if (!vct1.isPacked() && vct2.isPacked()) multiply(vct1, vct2);
        if (!vct1.isPacked()) { // Both are unpacked vectors

        } else if (!vct2.isPacked()) { // vct1 is packed and vct2 is unpacked

        } else { // Both vct1 and vct2 are packed vectors

        }
        return result;
    }

};

#endif	/* PACKEDVECTORUTIL_H */

