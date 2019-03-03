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

#include <stdio.h>
#include <stdlib.h>
#include <seq.h>
#include "pnm.h"
#include "assert.h"
#include "mem.h"

typedef struct component_video {
        float y; 
        float pb; 
        float pr;
} *comp_vid;

typedef struct trimmed_dimensions {
        unsigned height; 
        unsigned width;
        unsigned denominator;
} trim_dim;

typedef struct post_dct {
        unsigned a;
        signed b;
        signed c;
        signed d;
        unsigned index_pb;
        unsigned index_pr;
} post_dct;

#endif