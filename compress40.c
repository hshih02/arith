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
#include "dct.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray.h"
#include "uarray2.h"

#define A2 A2Methods_UArray2
#define IMGBLOCK 2


void compression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl);
//void decompression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl);
trim_dim trim_image_func(Pnm_ppm image);

extern void compress40 (FILE *input)
{
        Seq_T temp = Seq_new(DCT_REQ);
        
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);
        
        A2Methods_mapfun *map = methods->map_row_major;

        Pnm_ppm image = Pnm_ppmread(input, methods);


        trim_dim trimmed_image = trim_image_func(image);
        trimmed_image.cvpixel_seq = temp;
        trimmed_image.methods = methods;

        map(image->pixels, compression, &trimmed_image);
        Seq_free(&temp);
        Pnm_ppmfree(&image);
}

void compression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl)
{
        comp_vid cv_pixel;
        Pnm_rgb rgb_pixel = (Pnm_rgb) ptr;
        post_dct dct_values;
        A2Methods_T methods = ((trim_dim *) cl)->methods;





        int h = ((trim_dim *) cl)->height;
        int w = ((trim_dim *) cl)->width;
        int denom = ((trim_dim *)cl)->denominator;
        Seq_T cvpixel_seq = ((trim_dim *)cl)->cvpixel_seq;

        if (i >= w || j >= h) { /*check to stay within trimmed bounds*/
                return;
        }

        if (i%2 == 0 && j%2 == 0) {
                // fprintf(stderr, "red: %u, green %u, blue %u \n\n", rgb_pixel->red, rgb_pixel->green, rgb_pixel->blue);
                // fprintf(stderr, "[%d, %d] i,j\n", i, j);
                // printf("storing values\n");
//
                cv_pixel = rgbtocmp(rgb_pixel, denom);
                Seq_addlo(cvpixel_seq, cv_pixel); /*store current pixel, Y1*/
//
                
                Pnm_rgb temp = methods->at(array, i, j + 1);
                // fprintf(stderr, "red: %u, green %u, blue %u \n\n", temp->red, temp->green, temp->blue);
//
                cv_pixel = rgbtocmp(methods->at(array, i, j + 1), denom);
                // printf ("[%d, %d] i,j + 1\n", i, j + 1);
                Seq_addlo(cvpixel_seq, cv_pixel); /*store pixel to right, Y2*/
//
                temp = methods->at(array, i + 1, j);
                // fprintf(stderr, "red: %u, green %u, blue %u \n\n", temp->red, temp->green, temp->blue);
//
                cv_pixel = rgbtocmp(methods->at(array, i + 1, j), denom);
                // printf ("[%d, %d] i + 1,j\n", i + 1, j);
                Seq_addlo(cvpixel_seq, cv_pixel); /*store pixel below, Y3*/
//
                temp = methods->at(array, i + 1, j + 1);
                // printf("red: %u, green %u, blue %u \n\n", temp->red, temp->green, temp->blue);
//
                cv_pixel = rgbtocmp(methods->at(array, i + 1, j + 1), denom);
                Seq_addlo(cvpixel_seq, cv_pixel); /*store pixel on bot right, Y4*/
//                
                dct_values = dc_transform(cvpixel_seq);

                (void) temp;
                
        }

        else {
                return;
        }


        printf("current seq length: %d\n\n", Seq_length(cvpixel_seq));



        (void) dct_values;
        (void) array;
        
}

extern void decompress40 (FILE *input) //currently a test function for cmptorgb only
{
        comp_vid cv_pixel;
        NEW(cv_pixel);
        Pnm_rgb rgb_pixel;
        NEW(rgb_pixel);
        cv_pixel-> y = 0.556086;
        cv_pixel -> pb = -0.059315;
        cv_pixel-> pr = -0.027417;

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        A2Methods_mapfun *map = methods->map_default;

        Pnm_ppm image = Pnm_ppmread(input, methods);
        rgb_pixel = cmptorgb(cv_pixel, image->denominator);
        fprintf(stderr, "red %u, green %u, blue %u\n", rgb_pixel->red, rgb_pixel->green, rgb_pixel->blue);
        
        (void) map;
        (void) image;
        //map(image-> pixels, compression, &image->denominator);
} 


// void decompression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl)
// {
//         Pnm_rgb rgb_pixel 
// }
trim_dim trim_image_func(Pnm_ppm image)
{
        trim_dim trimmed_image;

        trimmed_image.denominator = image->denominator;
        trimmed_image.height = image->height;
        trimmed_image.width = image->width;
        printf("denominator: %u \n\n", trimmed_image.denominator);

        if (image -> height % IMGBLOCK != 0) { //(maybe assert height and width);
                
                trimmed_image.height = image->height-1;
                trimmed_image.width = image->width-1;
        } 

        return trimmed_image;
}

//version that does stage one only of compression
// void compression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl)
// {
//         comp_vid cv_pixel;
       
//         Pnm_rgb rgb_pixel = (Pnm_rgb) ptr;

//         // fprintf(stderr, "red: %u, green %u, blue %u \n\n", rgb_pixel -> red, rgb_pixel -> green, rgb_pixel -> blue);

//         int h = ((trim_dim *) cl) -> height;
//         int w = ((trim_dim *) cl) -> width;
//         int denom = ((trim_dim *)cl) -> denominator;

//         if ( i >= w || j >= h) {
//                 return;

//         }else {
//                 cv_pixel = rgbtocmp(rgb_pixel, denom);
//                 // fprintf(stderr, "y: %f, pb: %f, pr %f, \n\n", cv_pixel -> y, cv_pixel -> pb, cv_pixel -> pr);

//         }
//         (void) array;
// }

//tests last stage of decompression only
//maybe call a rounding function on the conversions
// extern void decompress40 (FILE *input)
// {
//         comp_vid cv_pixel;
//         NEW(cv_pixel);
//         Pnm_rgb rgb_pixel;
//         NEW(rgb_pixel);
//         cv_pixel-> y = 0.556086;
//         cv_pixel -> pb = -0.059315;
//         cv_pixel-> pr = -0.027417;

//         A2Methods_T methods = uarray2_methods_blocked; 
//         assert(methods);

//         A2Methods_mapfun *map = methods->map_block_major;

//         Pnm_ppm image = Pnm_ppmread(input, methods);
//          rgb_pixel = cmptorgb(cv_pixel, image-> denominator);
//          fprintf(stderr, "red %u, green %u, blue %u\n", rgb_pixel-> red, rgb_pixel->green, rgb_pixel-> blue);
        
//         (void)map;
//         (void) image;
//         //map(image-> pixels, compression, &image->denominator);
// } 

