/**************************************************************
 *
 *                     cmptorgb.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     Conducts component video to RGB conversion
 *     
 **************************************************************/

#include "cmptorgb.h"
static float get_r_val(float y, float pb, float pr, unsigned denominator);
static float get_g_val(float y, float pb, float pr, unsigned denominator);
static float get_b_val(float y, float pb, float pr, unsigned denominator);

Pnm_rgb cmptorgb(comp_vid cvpixel, unsigned denominator)
{
        Pnm_rgb rgbpixel;
        NEW(rgbpixel);

        float y, pb, pr;
        y = cvpixel->y;
        pb = cvpixel->pb;
        pr = cvpixel->pr;
                        
        rgbpixel->red = (unsigned) roundf( get_r_val(y,pb,pr,denominator) );
        rgbpixel->green = (unsigned) roundf( get_g_val(y,pb,pr,denominator) );
        rgbpixel->blue = (unsigned) roundf( get_b_val(y,pb,pr,denominator) );

        return rgbpixel;
}

static float get_r_val(float y, float pb, float pr, unsigned denominator)
{
        float r = 1.0 * y + 0.0 * pb + 1.402 * pr;
        r *= denominator;

        if (r > 255)
        {
                r = 255;
        } else if (r < 0) {
                r = 0;
        }

        return r;
}

static float get_g_val(float y, float pb, float pr, unsigned denominator)
{
        float g = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
        g *= denominator;

        if (g > 255)
        {
                g = 255;
        } else if (g < 0) {
                g = 0;
        }        
        return g;
}

static float get_b_val(float y, float pb, float pr, unsigned denominator)
{
        float b = 1.0 * y + 1.772 * pb + 0.0 * pr;
        b *= denominator;

        if (b > 255)
        {
                b = 255;
        } else if (b < 0) {
                b = 0;
        }
        return b;
}