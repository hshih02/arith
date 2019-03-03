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

#define A2 A2Methods_UArray2

void compression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl);
trim_dim trim_image(Pnm_ppm image);

extern void compress40 (FILE *input)
{
        A2Methods_T methods = uarray2_methods_blocked; 
        assert(methods);

        A2Methods_mapfun *map = methods->map_block_major;

        Pnm_ppm image = Pnm_ppmread(input, methods);
        trim_dim trimmed_image = trim_image(image);

        map(image-> pixels, compression, &trimmed_image);

}

extern void decompress40 (FILE *input)
{
        (void) input;
} 

void compression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl)
{
        comp_vid cv_pixel;
       
        Pnm_rgb rgb_pixel = (Pnm_rgb) ptr;

        // fprintf(stderr, "red: %u, green %u, blue %u \n\n", rgb_pixel -> red, rgb_pixel -> green, rgb_pixel -> blue);

        int h = ((trim_dim *) cl) -> height;
        int w = ((trim_dim *) cl) -> width;
        int denom = ((trim_dim *)cl) -> denominator;

        if ( i >= w || j >= h) {
                return;

        }else {
                cv_pixel = rgbtocmp(rgb_pixel, denom);
                // fprintf(stderr, "y: %f, pb: %f, pr %f, \n\n", cv_pixel -> y, cv_pixel -> pb, cv_pixel -> pr);

        }
        (void) array;
}


trim_dim trim_image(Pnm_ppm image)
{
        trim_dim trimmed_image;

        trimmed_image.denominator = image->denominator;
        trimmed_image.height = image->height;
        trimmed_image.width = image->width;
        printf("denominator: %u \n\n", trimmed_image.denominator);

        if (image -> height % 2 != 0){ //(maybe assert height and width);
                

                trimmed_image.height = image->height-1;
                trimmed_image.width = image->width-1;
        } 

        return trimmed_image;
}




