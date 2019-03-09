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

/*
*       dc_transform
*       Takes in a Hanson sequence of component_video structs,
*       performs discrete cosine transformation calculations and returns
*       a single struct containing all coefficients and index Pb/Pr values
*/
post_dct dc_transform(Seq_T component_video);
