/**************************************************************
 *
 *                     rgbtocmp.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     don't forget to write stuff here later
 *     
 *
 **************************************************************/


#include "arithstructs.h"
float get_y_val(float r, float g, float b);
float get_pb_val(float r, float g, float b);
float get_pr_val(float r, float g, float b);


int main()  /*testing main*/
{
        Pnm_rgb temp;
        NEW(temp);

        temp->red = 1;
        temp->green = 2;
        temp->blue = 3;
        
        unsigned denominator = 1;
        
        float red, green, blue;
        red = (float)temp->red/denominator;
        green = (float)temp->green/denominator;
        blue = (float)temp->blue/denominator;

        float result = get_y_val(red,green,blue);
        printf ("y = %f\n", result);
        result = get_pb_val(red,green,blue);
        printf ("pb = %f\n", result);
        result = get_pr_val(red,green,blue);
        printf ("pr = %f\n", result);
        FREE(temp);
}

// comp_vid rgbtocmp(Pnm_rgb rgbpixel)
// {
//         comp_vid cvpixel;
//         NEW(cvpixel);
        
//         cvpixel->y = 

// }

float get_y_val(float r, float g, float b)
{
        float y = 0.299 * r + 0.587 * g + 0.114 * b;
        return y;
}

float get_pb_val(float r, float g, float b)
{
        float pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
        return pb;
}

float get_pr_val(float r, float g, float b)
{
        float pr = 0.5 * r - 0.418688 * g - 0.081312 * b;
        return pr;
}


// y = 0.299 * r + 0.587 * g + 0.114 * b
// pb = -0.168736 * r - 0.331264 * g + 0.5 * b
// pr = 0.5 * r - 0.418688 * g - 0.081312 * b

