/**************************************************************
 *
 *                     compress40.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 3, 2019
 *
 *     Compresses or decompresses an image from command line
 *     or stdin
 *
 **************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "compress40.h"
#include "rgbtocmp.h"
#include "cmptorgb.h"
#include "dct.h"
#include "inverse_dct.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray.h"
#include "uarray2.h"
#include <bitpack.h>

#define A2 A2Methods_UArray2
#define IMGBLOCK 2
#define DEFAULT_DENOM 255


void compression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl);
void decompression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl);
comp_cl trim_image_func(Pnm_ppm image);
uint64_t pack_to_word(post_dct dct_values);
void set_pixel_at_index(Pnm_rgb source, Pnm_rgb destination);
post_dct unpack_word(FILE *inputstream, void *cl);
void print_word(uint32_t word);


/*
*       compress40
*       Performs image compression, sets methods, sets closure
*       trims image dimensions to be passed into compression
*       apply function
*/
extern void compress40 (FILE *input)
{
        Seq_T temp_seq = Seq_new(DCT_REQ);
        
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);
        
        A2Methods_mapfun *map = methods->map_row_major;

        Pnm_ppm image = Pnm_ppmread(input, methods);

        comp_cl trimmed_image = trim_image_func(image);
        trimmed_image.cvpixel_seq = temp_seq;
        trimmed_image.methods = methods;
        trimmed_image.headerprinted = false;

        map(image->pixels, compression, &trimmed_image);
        Seq_free(&temp_seq);
        Pnm_ppmfree(&image);
}

/*
*       compression
*       Apply function for compression, calls 
*       rgbtocmp--> dct --> bitpacking --> print a word to stdout
*/
void compression(int i, int j, A2 array, A2Methods_Object *ptr, void * cl)
{
        comp_vid cv_pixel;
        post_dct dct_values;
        A2Methods_T methods = ((comp_cl *) cl)->methods;

        int h = ((comp_cl *) cl)->height;
        int w = ((comp_cl *) cl)->width;
        int denom = ((comp_cl *)cl)->denominator;
        Seq_T cvpixel_seq = ((comp_cl *)cl)->cvpixel_seq;

        if (i >= w || j >= h) { /*check to stay within trimmed bounds*/
                return;
        }

        if (i%2 == 0 && j%2 == 0) { /*call dct and bitpacking at end of block*/

        /*===COMP STAGE 1: RGB TO CMP===*/
                /*---------Y1------------*/
                cv_pixel = rgbtocmp( (Pnm_rgb)(methods->at(array, i, j)) 
                                                                    , denom);
                Seq_addlo(cvpixel_seq, cv_pixel);
                /*store current pixel, Y1*/

                /*---------Y2------------*/
                cv_pixel = rgbtocmp( (Pnm_rgb)(methods->at(array, i, j + 1)) 
                                                                    , denom);
                Seq_addlo(cvpixel_seq, cv_pixel);              
                /*store pixel to right, Y2*/

                /*---------Y3------------*/
                cv_pixel = rgbtocmp( (Pnm_rgb)(methods->at(array, i + 1, j))
                                                                    , denom);
                Seq_addlo(cvpixel_seq, cv_pixel); 
                /*store pixel below, Y3*/

                /*---------Y4------------*/
                cv_pixel = rgbtocmp((Pnm_rgb)(methods->at(array, i + 1, j + 1))
                                                                    , denom);
                Seq_addlo(cvpixel_seq, cv_pixel); 
                /*store pixel on bot right, Y4*/

        /*===COMP STAGE 2: DCT===*/
                dct_values = dc_transform(cvpixel_seq);

        /*===COMP STAGE 3: BITPACK===*/
                uint64_t packedword = pack_to_word(dct_values);

        /*===OUTPUT STAGE===*/
                if ( ((comp_cl *) cl)->headerprinted == false ) {
                        printf("COMP40 Compressed image format 2\n%u %u",
                         (unsigned) w, (unsigned) h);
                        printf("\n");
                        ((comp_cl *) cl)->headerprinted = true;
                }
                print_word(packedword);

        } else {
                return;
        }

        (void) ptr;
}

/*
*       trim_image_func
*       Takes in a Pnm_ppm image and trims the width and height to the 
*       nearest even number
*/
comp_cl trim_image_func(Pnm_ppm image)
{
        comp_cl trimmed_image;

        trimmed_image.denominator = image->denominator;
        trimmed_image.height = image->height;
        trimmed_image.width = image->width;

        if (image->height % IMGBLOCK != 0) { 

                trimmed_image.height = image->height-1;
        }

        if (image->width % IMGBLOCK != 0) { 

                trimmed_image.width = image->width-1;
        } 
        return trimmed_image;
}

/*
*       pack_to_word
*       Takes in a post_dct struct of coefficients and index Pb/Pr
*       Creates a new word and packs each value to specified field,
*       returns the packed uint64_t word
*/
uint64_t pack_to_word(post_dct dct_values)
{
        uint32_t word = 0;
        word = Bitpack_newu(word, 6, 26, dct_values.a);
        word = Bitpack_news(word, 6, 20, dct_values.b);
        word = Bitpack_news(word, 6, 14, dct_values.c);
        word = Bitpack_news(word, 6, 8, dct_values.d);
        word = Bitpack_newu(word, 4, 4, dct_values.index_pb);
        word = Bitpack_newu(word, 4, 0, dct_values.index_pr);
        return word;
}

/*
*       print_word
*       Takes in a uint32_t word (truncates 64 bit words) and partitions 
*       the word into individual bytes, then prints bytes out in
*       big endian order
*/
void print_word(uint32_t word)
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

/*
*       decompress40
*       Performs image decompression, sets methods, sets closure
*       to be passed into decompression apply function
*       Makes a new Pnm_ppm with width and height read in from compressed file
*/
extern void decompress40 (FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);
        A2Methods_mapfun *map = methods->map_row_major;

        decomp_cl decomp_closure;

        decomp_closure.inputstream = input;
        decomp_closure.methods = methods;
        decomp_closure.counter = 0;
        
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');
        A2 array = methods->new( width, height, sizeof(struct Pnm_rgb) );

        decomp_closure.width = width;
        decomp_closure.height = height;

        struct Pnm_ppm pixmap = { .width = width, .height = height
                                , .denominator = DEFAULT_DENOM, .pixels = array
                                , .methods = methods
                                };


        map(pixmap.pixels, decompression, &decomp_closure);

        Pnm_ppmwrite(stdout, &pixmap);
        methods->free( &(pixmap.pixels) );
}

/*
*       decompression
*       Apply function for decompression, calls 
*       unpack--> inverse dct --> cmptorgb --> writes pixels to pixmap
*/
void decompression(int i, int j, A2 array, A2Methods_Object *ptr, void *cl)
{
        Pnm_rgb source;
        Pnm_rgb destination;
        A2Methods_T methods = ((decomp_cl *) cl)->methods;

        comp_vid component_video;
        post_dct unpacked_word;
        
        Seq_T cvpixel_seq;

        if (i%2 == 0 && j%2 == 0) {
        /*===DECOMP STAGE 1: UNPACKING BITS===*/
                unpacked_word = unpack_word( ((decomp_cl *) cl)->inputstream 
                                                                       , cl);

        /*===DECOMP STAGE 2: INVERSE DCT===*/
                cvpixel_seq = inverse_dct_transform(unpacked_word);

        /*===DECOMP STAGE 3: CMP TO RGB/WRITING TO PIXMAP===*/

                /*---------Y1------------*/
                component_video = Seq_get(cvpixel_seq, 3); /*get Y1 pixel*/
                source = cmptorgb(component_video, DEFAULT_DENOM);

                /*get pointer to current pixel*/
                destination = (Pnm_rgb) methods->at(array, i, j);
                set_pixel_at_index(source, destination);
                FREE(source);

                /*---------Y2------------*/
                component_video = Seq_get(cvpixel_seq, 2); /*get Y2 pixel*/
                source = cmptorgb(component_video, DEFAULT_DENOM);

                /*get pointer to pixel to the right*/
                destination = (Pnm_rgb) methods->at(array, i , j + 1);
                set_pixel_at_index(source, destination);
                FREE(source);

                /*---------Y3------------*/
                component_video = Seq_get(cvpixel_seq, 1); /*get Y3 pixel*/
                source = cmptorgb(component_video, DEFAULT_DENOM);       

                /*get pointer to pixel to the bottom*/
                destination = (Pnm_rgb) methods->at(array, i + 1 , j);
                set_pixel_at_index(source, destination);
                FREE(source);

                /*---------Y4------------*/
                component_video = Seq_get(cvpixel_seq, 0); /*get Y4 pixel*/
                source = cmptorgb(component_video, DEFAULT_DENOM);

                /*get pointer to pixel to the bottom right*/
                destination = (Pnm_rgb) methods->at(array, i + 1 , j + 1);
                set_pixel_at_index(source, destination);
                FREE(source);

                /*free comp_vid structs within sequence, then free seq*/
                while (Seq_length(cvpixel_seq) != 0)
                {
                        component_video = (comp_vid)( Seq_remlo(cvpixel_seq) );
                        FREE(component_video);
                } 
                Seq_free(&cvpixel_seq);                
        }

        (void) ptr;
}

/*
*       set_pixel_at_index
*       Performs a deep copy between two Pnm_rgbs 
*/
void set_pixel_at_index(Pnm_rgb source, Pnm_rgb destination)
{
        destination->red = source->red;
        destination->green = source->green;
        destination->blue = source->blue;
}

/*
*       unpack_word
*       Takes in a compressed file in binary format, reads byte by byte
*       and reassembles 4 bytes into a 32 byte word in big endian order
*       then unpacks each into coefficients and index Pb/Pr, stores each
*       to a post_dct struct
*/
post_dct unpack_word(FILE *inputstream, void *cl)
{
        post_dct dct_values;
        uint32_t unpacked = 0;
        int total_pixels = ( ((decomp_cl *) cl)->width) * 
                           ( ((decomp_cl *) cl)->height);

        /*get and store bits 0-7 (in big endian)-----------*/
        uint32_t readbyte = fgetc(inputstream);

        /*check that we read in the expected amount of bytes*/
        ((decomp_cl *) cl)->counter++;
        assert(((decomp_cl *) cl)->counter <= total_pixels);
        
        /*reassemble bits to word*/
        unpacked = Bitpack_newu(unpacked, 8, 24, readbyte);
        /*-------------------------------------------------*/

        /*get and store bits 8-15 (in big endian)----------*/
        readbyte = fgetc(inputstream);

        /*check read bytes*/
        ((decomp_cl *) cl)->counter++;
        assert(((decomp_cl *) cl)->counter <= total_pixels);

        /*reassemble bits to word*/
        unpacked = Bitpack_newu(unpacked, 8, 16, readbyte);
        /*-------------------------------------------------*/

        /*get and store bits 16-23 (in big endian)---------*/
        readbyte = fgetc(inputstream);
        
        /*check read bytes*/
        ((decomp_cl *) cl)->counter++;
        assert(((decomp_cl *) cl)->counter <= total_pixels);        
        
        /*reassemble bits to word*/
        unpacked = Bitpack_newu(unpacked, 8, 8, readbyte); 
        /*--------------------------------------------------*/

        /*get and store bits 24-31 (in big endian)----------*/
        readbyte = fgetc(inputstream);
        
        /*check read bytes*/
        ((decomp_cl *) cl)->counter++;
        assert(((decomp_cl *) cl)->counter <= total_pixels);   
        
        /*reassemble bits to word*/
        unpacked = Bitpack_newu(unpacked, 8, 0, readbyte);
        /*--------------------------------------------------*/

        /*Unpack reassembled word to post_dct struct--------*/
        dct_values.a = Bitpack_getu(unpacked, 6, 26);
        dct_values.b = Bitpack_gets(unpacked, 6, 20);
        dct_values.c = Bitpack_gets(unpacked, 6, 14);
        dct_values.d = Bitpack_gets(unpacked, 6, 8);
        dct_values.index_pb = Bitpack_getu(unpacked, 4, 4);
        dct_values.index_pr = Bitpack_getu(unpacked, 4, 0);

        return dct_values;
}