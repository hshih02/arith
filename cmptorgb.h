/**************************************************************
 *
 *                     cmptorgb.h
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     Conducts component video to RGB conversion
 *     
 *
 **************************************************************/

#include "arithstructs.h"

/*
*       cmptorgb
*       Takes in a component_video struct and denominator and performs 
*       the inverse calculations to convert from component video to RGB 
*       values, returns a Pnm_rgb struct
*/
Pnm_rgb cmptorgb(comp_vid cvpixel, unsigned denominator);