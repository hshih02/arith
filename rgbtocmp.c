/**************************************************************
 *
 *                     rgbtocmp.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     Conducts RGB to component video conversion
 *     
 *
 **************************************************************/

#include "rgbtocmp.h"
static float get_y_val(float r, float g, float b);
static float get_pb_val(float r, float g, float b);
static float get_pr_val(float r, float g, float b);

comp_vid rgbtocmp(Pnm_rgb rgbpixel, unsigned denominator)
{
        comp_vid cvpixel;
        NEW(cvpixel);

        float r, g, b;
        r = (float)(rgbpixel->red)/(float)denominator;
        g = (float)(rgbpixel->green)/(float)denominator;
        b = (float)(rgbpixel->blue)/(float)denominator;

        cvpixel->y = get_y_val(r,g,b);
        cvpixel->pb = get_pb_val(r,g,b);
        cvpixel->pr = get_pr_val(r,g,b);

        return cvpixel;
}

static float get_y_val(float r, float g, float b)
{
        float y = 0.299 * r + 0.587 * g + 0.114 * b;
        return y;
}

static float get_pb_val(float r, float g, float b)
{
        float pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
        return pb;
}

static float get_pr_val(float r, float g, float b)
{
        float pr = 0.5 * r - 0.418688 * g - 0.081312 * b;
        return pr;
}