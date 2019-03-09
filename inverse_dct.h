/**************************************************************
 *
 *                     inverse_dct.h
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 3, 2019
 *
 *     Conducts the inverse of discrete cosine transformation 
 *     
 *
 **************************************************************/

#include "arithstructs.h"

/*
*       inverse_dct_transform
*       Takes in a post_dct struct of coefficient and index Pb/Pr values
*       and performs inverse DCT calculations. Returns a Hanson sequence
*       containing all corresponding component_video structs
*/
Seq_T inverse_dct_transform(post_dct unpacked_word);