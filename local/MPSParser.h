/* 
 * File:   MPSParser.h
 * Author: sriram
 *
 * Created on 13 March, 2014, 5:46 PM
 */

#ifndef MPSPARSER_H
#define	MPSPARSER_H
class MPSParser{
public stxxl::matrix<Fraction,sizeof(Fraction)*50> parseFile(string fileName){
    // Matrix dimensions
    int height = 3;
    int width = 3;

    int internal_memory = 64 * 1024 * 1024;
    const int small_block_order = 32;  // must be multiple of matrix valueType in bits

    typedef stxxl::block_scheduler<stxxl::matrix_swappable_block<int, small_block_order> > block_schedular_type;
    typedef stxxl::matrix<int, small_block_order> matrix_type;

    block_schedular_type my_bs(internal_memory);

    // Create 3 matrices with given dimensions
    matrix_type A(my_bs, height, width);
    matrix_type B(my_bs, height, width);
    matrix_type C(my_bs, height, width);
};


#endif	/* MPSPARSER_H */

