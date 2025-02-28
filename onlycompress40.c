/**************************************************************
 *
 *                     onlycompress40.c
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
#include <bitpack.h>

#define A2 A2Methods_UArray2
#define IMGBLOCK 2
#define DEFAULT_DENOM 255


void compression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl);
// void decompression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl);
trim_dim trim_image_func(Pnm_ppm image);
uint64_t pack_to_word(post_dct dct_values);
post_dct unpack_word(uint64_t word);
void print_word(uint64_t word);


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
        trimmed_image.headerprinted = false;

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

        if (i%2 == 0 && j%2 == 0) { /*call dct and bitpacking at end of block*/
                // fprintf(stderr, "red: %u, green %u, blue %u \n\n", rgb_pixel->red, rgb_pixel->green, rgb_pixel->blue);
                // fprintf(stderr, "[%d, %d] i,j\n", i, j);
                // printf("storing values\n");
//
                cv_pixel = rgbtocmp(rgb_pixel, denom);
                Seq_addlo(cvpixel_seq, cv_pixel); /*store current pixel, Y1*/
//
                // Pnm_rgb temp = methods->at(array, i, j + 1);
                // fprintf(stderr, "red: %u, green %u, blue %u \n\n", temp->red, temp->green, temp->blue);
//
                cv_pixel = rgbtocmp(methods->at(array, i, j + 1), denom);
                // printf ("[%d, %d] i,j + 1\n", i, j + 1);
                Seq_addlo(cvpixel_seq, cv_pixel); /*store pixel to right, Y2*/
//
                // temp = methods->at(array, i + 1, j);
                // fprintf(stderr, "red: %u, green %u, blue %u \n\n", temp->red, temp->green, temp->blue);
//
                cv_pixel = rgbtocmp(methods->at(array, i + 1, j), denom);
                // printf ("[%d, %d] i + 1,j\n", i + 1, j);
                Seq_addlo(cvpixel_seq, cv_pixel); /*store pixel below, Y3*/
//
                // temp = methods->at(array, i + 1, j + 1);
                // printf("red: %u, green %u, blue %u \n\n", temp->red, temp->green, temp->blue);
//
                cv_pixel = rgbtocmp(methods->at(array, i + 1, j + 1), denom);
                Seq_addlo(cvpixel_seq, cv_pixel); /*store pixel on bot right, Y4*/
//                
                dct_values = dc_transform(cvpixel_seq);

                uint64_t packedword = pack_to_word(dct_values);

                if ( ((trim_dim *) cl)->headerprinted == false ) {
                        printf("COMP 40 Compressed image format 2\n%u %u", w, h);
                        printf("\n");
                        ((trim_dim *) cl)->headerprinted = true;
                }
                print_word(packedword);



                // unpack_word(packedword);
        } else {
                return;
        }

        (void) dct_values;
        (void) array;
}

trim_dim trim_image_func(Pnm_ppm image)
{
        trim_dim trimmed_image;

        trimmed_image.denominator = image->denominator;
        trimmed_image.height = image->height;
        trimmed_image.width = image->width;
        // printf("denominator: %u \n\n", trimmed_image.denominator);

        if (image->height % IMGBLOCK != 0) { 

                trimmed_image.height = image->height-1;
        }

        if (image->width % IMGBLOCK != 0) { 

                trimmed_image.width = image->width-1;
        } 
        return trimmed_image;
}

uint64_t pack_to_word(post_dct dct_values)
{
        uint64_t word = 0;
        word = Bitpack_newu(word, 9, 23, dct_values.a);
        word = Bitpack_news(word, 5, 18, dct_values.b);
        word = Bitpack_news(word, 5, 13, dct_values.c);
        word = Bitpack_news(word, 5, 8, dct_values.d);
        word = Bitpack_newu(word, 4, 4, dct_values.index_pb);
        word = Bitpack_newu(word, 4, 0, dct_values.index_pr);

        // printf ("packed word: %lu\n\n", word);
        return word;
}

void print_word(uint64_t word)
{
        char BE_byte4 = Bitpack_getu(word, 8, 0); /*1st byte in little endian*/
        char BE_byte3 = Bitpack_getu(word, 8, 8); /*2nd byte in little endian*/
        char BE_byte2 = Bitpack_getu(word, 8, 16);/*3rd byte in little endian*/
        char BE_byte1 = Bitpack_getu(word, 8, 24);/*4th byte in little endian*/
                               
        putchar(BE_byte1);
        putchar(BE_byte2);
        putchar(BE_byte3);
        putchar(BE_byte4);
}

extern void decompress40 (FILE *input) //currently a test function for cmptorgb only
{
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        A2Methods_mapfun *map = methods->map_row_major;

        unsigned height, width;
        int read = fscanf(stdin, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);
        assert(read == 2);
        int c = getc(stdin);
        assert(c == '\n');

        A2 array = methods->new(height, width, sizeof(struct Pnm_rgb) );

        struct Pnm_ppm pixmap = { .width = width, .height = height
                                , .denominator = DEFAULT_DENOM, .pixels = array
                                , .methods = methods
                                };


        (void) input;
        (void) pixmap;
        (void) map;
        //map(image-> pixels, compression, &image->denominator);
} 


// void decompression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl)
// {
//         Pnm_rgb rgb_pixel 
// }

// uint32_t readword(FILE *input);
// {
//         uint32_t readword = fgetc(input);
// }


post_dct unpack_word(uint64_t word)
{
        post_dct dct_values;

        dct_values.a = Bitpack_getu(word, 9, 23);
        dct_values.b = Bitpack_gets(word, 5, 18);
        dct_values.c = Bitpack_gets(word, 5, 13);
        dct_values.d = Bitpack_gets(word, 5, 8);
        dct_values.index_pb = Bitpack_getu(word, 4, 4);
        dct_values.index_pr = Bitpack_getu(word, 4, 0);

        // printf("dct_values:\n\n.a = %u\n.b = %i\n.c = %i\n.d = %i\n.index_pb = %u\nindex_pr = %u\n\n", 
        // dct_values.a, dct_values.b, dct_values.c, dct_values.d, dct_values.index_pb, dct_values.index_pr);

        return dct_values;
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

