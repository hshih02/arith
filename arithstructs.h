/********************************************************************
 *
 *                     arithstructs.h
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     All relevant data structures and includes to be used 
 *     among the separate components of our compressor/decompressor
 *
 ********************************************************************/

#ifndef ARITHSTRUCTS_INCLUDED
#define ARITHSTRUCTS_INCLUDED
#define DCT_REQ 4      /* corresponds to blocksize^2 */
#include <stdio.h>
#include <stdlib.h>
#include <seq.h>
#include <pnm.h>
#include <assert.h>
#include <mem.h>
#include <math.h>
#include <arith40.h>
#include <stdbool.h>

/*
*       component_video
*       struct storing all component video values 
*/
typedef struct component_video {
        float y; 
        float pb; 
        float pr;
} *comp_vid;

/*
*       compression_closure
*       closure struct holding all data needed for compression 
*/
typedef struct compression_closure {
        unsigned height; 
        unsigned width;
        unsigned denominator;
        A2Methods_T methods;
        Seq_T cvpixel_seq;
        bool headerprinted;
} comp_cl;

/*
*       post_dct
*       stores values returned from/needed by dct and inverse dct 
*/
typedef struct post_dct {
        unsigned a;
        signed b;
        signed c;
        signed d;
        unsigned index_pb;
        unsigned index_pr;
} post_dct;

/*
*       decompression_closure
*       closure struct holding all data needed for decompression 
*/
typedef struct decompression_closure {
        FILE *inputstream;  
        A2Methods_T methods;
        int counter;
        unsigned width;
        unsigned height;
} decomp_cl;

#endif