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

#include "arithstructs.h"

unsigned get_r_val(float y, float pb, float pr);
unsigned get_g_val(float y, float pb, float pr);
unsigned get_b_val(float y, float pb, float pr);


int main()  /*testing main*/
{
        comp_vid temp;
        NEW(temp);

        temp->y = 1;
        temp->pb = 2;
        temp->pr = 3;
        
        unsigned denominator = 1;
        
        float y, pb, pr;
        y = temp->y/denominator;  //may or may not need denom here
        pb = temp->pb/denominator;
        pr = temp->pr/denominator;

        float result = get_r_val(y,pb,pr);
        printf ("r = %f\n", result);
        result = get_g_val(y,pb,pr);
        printf ("g = %f\n", result);
        result = get_b_val(y,pb,pr);
        printf ("b = %f\n", result);
        FREE(temp);
}

// comp_vid rgbtocmp(Pnm_rgb rgbpixel)
// {
//         comp_vid cvpixel;
//         NEW(cvpixel);
        
//         cvpixel->y = 

// }

unsigned get_r_val(float y, float pb, float pr)
{
        float r = 1.0 * y + 0.0 * pb + 1.402 * pr;
        return (unsigned)r;
}

unsigned get_g_val(float y, float pb, float pr)
{
        float g = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
        return (unsigned)g;
}

unsigned get_b_val(float y, float pb, float pr)
{
        float b = 1.0 * y + 1.772 * pb + 0.0 * pr;
        return (unsigned)b;
}


// r = 1.0 * y + 0.0 * pb + 1.402 * pr
// g = 1.0 * y - 0.344136 * pb - 0.714136 * pr
// b = 1.0 * y + 1.772 * pb + 0.0 * pr

