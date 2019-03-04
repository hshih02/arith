/**************************************************************
 *
 *                     cmptorgb.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     don't forget to write stuff here later
 *     
 *
 **************************************************************/



//CURRENT ISSUES: typecasting negative values to unsigned causes issues,
//may or may not be a real problem since our test values may be 
//invalid
#include "cmptorgb.h"
static float get_r_val(float y, float pb, float pr);
static float get_g_val(float y, float pb, float pr);
static float get_b_val(float y, float pb, float pr);

// int main()  /*testing main*/
// {
//         comp_vid temp;
//         NEW(temp);

//         temp->y = 1;
//         temp->pb = 2;
//         temp->pr = 3;
        
//         unsigned denominator = 1;
        
//         float y, pb, pr;
//         y = temp->y/denominator;  //may or may not need denom here
//         pb = temp->pb/denominator;
//         pr = temp->pr/denominator;

//         float result = get_r_val(y,pb,pr);
//         printf ("r = %f\n", result);
//         result = get_g_val(y,pb,pr);
//         printf ("g = %f\n", result);
//         result = get_b_val(y,pb,pr);
//         printf ("b = %f\n", result);

//         printf("testing cmptorgb func\n");

//         Pnm_rgb postconvert = cmptorgb(temp);

//         unsigned postresult;
//         postresult = postconvert->red;
//         printf ("r = %u\n", postresult);
//         postresult = postconvert->green;
//         printf ("g = %u\n", postresult);
//         postresult = postconvert->blue;
//         printf ("b = %u\n", postresult);

//         FREE(postconvert);
//         FREE(temp);
// }

Pnm_rgb cmptorgb(comp_vid cvpixel, unsigned denominator)
{
        Pnm_rgb rgbpixel;
        NEW(rgbpixel);


        float y, pb, pr;
        y = cvpixel->y;
        pb = cvpixel->pb;
        pr = cvpixel->pr;
                        
        
        rgbpixel->red = (unsigned)(get_r_val(y,pb,pr) * denominator);
        rgbpixel->green = (unsigned)(get_g_val(y,pb,pr) * denominator);
        rgbpixel->blue = (unsigned)(get_b_val(y,pb,pr) * denominator);

        return rgbpixel;
}

static float get_r_val(float y, float pb, float pr)
{
        float r = 1.0 * y + 0.0 * pb + 1.402 * pr;
        return r;
}

static float get_g_val(float y, float pb, float pr)
{
        float g = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
        return g;
}

static float get_b_val(float y, float pb, float pr)
{
        float b = 1.0 * y + 1.772 * pb + 0.0 * pr;
        return b;
}


// the math (inverse cmpvid to rgb)
// r = 1.0 * y + 0.0 * pb + 1.402 * pr
// g = 1.0 * y - 0.344136 * pb - 0.714136 * pr
// b = 1.0 * y + 1.772 * pb + 0.0 * pr

