/**************************************************************
 *
 *                     compress40.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 3, 2019
 *
 *     Compresses or decompresses an image 
 *     
 *
 **************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "compress40.h"
#include "rgbtocmp.h"
#include "cmptorgb.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "uarray2b.h"


extern void compress40 (FILE *input)
{
        A2Methods_T methods = uarray2_methods_blocked; 
        assert(methods);
        A2Methods_mapfun *map = methods->map_block_major;

        Pnm_ppm image = Pnm_ppmread(input, methods);

        // map(image, , &closure90);

        (void) image;
        (void) map;

}

extern void decompress40 (FILE *input)
{
        (void) input;
} 

// void compression()
// {

// }