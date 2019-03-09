/**************************************************************
 *
 *                     inverse_dct.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 3, 2019
 *
 *     Conducts the inverse of discrete cosine transformation 
 *     
 *
 **************************************************************/


#include "inverse_dct.h"

static float get_y1_value(post_dct unpacked_word);
static float get_y2_value(post_dct unpacked_word);
static float get_y3_value(post_dct unpacked_word);
static float get_y4_value(post_dct unpacked_word);
static Seq_T set_component_video(post_dct word, Seq_T component_video);
Seq_T inverse_dct_transform(post_dct unpacked_word);

#define DENOMINATOR 255

Seq_T inverse_dct_transform(post_dct unpacked_word)
{
        Seq_T component_video = Seq_new(DCT_REQ);

        component_video = set_component_video(unpacked_word, component_video);

        return component_video;
}

static float get_y1_value(post_dct unpacked_word)
{
        float y = (unpacked_word.a - unpacked_word.b - unpacked_word.c + 
                unpacked_word.d);
        y /= DENOMINATOR;

        if (y > 1) {  /*round to 1 to stay within range*/
                y = 1;
        } else if (y < 0) {
                y = 0;
        }

        return y;
}

static float get_y2_value(post_dct unpacked_word)
{
        float y = (unpacked_word.a - unpacked_word.b + unpacked_word.c - 
                unpacked_word.d);
        y /= DENOMINATOR;

        if (y > 1) {  /*round to 1 to stay within range*/
                y = 1;
        } else if (y < 0) {
                y = 0;
        }        

        return y;
}

static float get_y3_value(post_dct unpacked_word)
{
        float y = (unpacked_word.a + unpacked_word.b - unpacked_word.c - 
                unpacked_word.d);
        y /= DENOMINATOR;

        if (y > 1) {  /*round to 1 to stay within range*/
                y = 1;
        } else if (y < 0) {
                y = 0;
        }

        return y;
}

static float get_y4_value(post_dct unpacked_word)
{
        float y = (unpacked_word.a + unpacked_word.b + unpacked_word.c + 
                unpacked_word.d);
        y /= DENOMINATOR;

        if (y > 1) {  /*round to 1 to stay within range*/
                y = 1;
        } else if (y < 0) {
                y = 0;
        }

        return y;
}

/*
*       set_component_video
*       Main conversion function, creates 4 component_video structs
*       and adds each to a Hanson sequence in order of Y4-Y1
*/
static Seq_T set_component_video(post_dct word, Seq_T component_video)
{
        comp_vid cv_pixel1, cv_pixel2, cv_pixel3, cv_pixel4;

        float avgpb, avgpr;

        NEW(cv_pixel1);
        NEW(cv_pixel2); 
        NEW(cv_pixel3);
        NEW(cv_pixel4);

        avgpb = Arith40_chroma_of_index(word.index_pb);
        avgpr = Arith40_chroma_of_index(word.index_pr);

        cv_pixel1->y  = get_y1_value(word);
        cv_pixel1->pb = avgpb;
        cv_pixel1->pr = avgpr;

        cv_pixel2->y  = get_y2_value(word);
        cv_pixel2->pb = avgpb;
        cv_pixel2->pr = avgpr;

        cv_pixel3->y  = get_y3_value(word);
        cv_pixel3->pb = avgpb;
        cv_pixel3->pr = avgpr;

        cv_pixel4->y  = get_y4_value(word);
        cv_pixel4->pb = avgpb;
        cv_pixel4->pr = avgpr;

        Seq_addlo(component_video, cv_pixel1);
        Seq_addlo(component_video, cv_pixel2);
        Seq_addlo(component_video, cv_pixel3);
        Seq_addlo(component_video, cv_pixel4);

        return component_video;
}
