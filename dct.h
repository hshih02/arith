/**************************************************************
 *
 *                     dct.h
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 4, 2019
 *
 *     conducts discrete cosine transformation 
 *     
 *
 **************************************************************/

#include "arithstructs.h" 
#define DCT_REQ 4      /*corresponds to blocksize^2*/

post_dct dc_transform(Seq_T component_video);
