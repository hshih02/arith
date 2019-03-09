/**************************************************************
 *
 *                     rgbtocmp.h
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     Conducts RGB to component video conversion
 *     
 *
 **************************************************************/

#include "arithstructs.h"

/*
*       rgbtocmp
*       Takes in a single Pnm_rgb struct and denominator value
*       and persforms calculations to convert RGB to component 
*       video values stored in a component_video struct
*/
comp_vid rgbtocmp(Pnm_rgb rgbpixel, unsigned denominator);