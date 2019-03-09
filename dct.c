/**************************************************************
 *
 *                     dct.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 3, 2019
 *
 *     Conducts discrete cosine transformation 
 *     
 *
 **************************************************************/

// #include "arithstructs.h" 
#include "dct.h"

static unsigned get_a_coefficient(float Y4, float Y3, float Y2, float Y1);
static int get_b_coefficient(float Y4, float Y3, float Y2, float Y1);
static int get_c_coefficient(float Y4, float Y3, float Y2, float Y1);
static int get_d_coefficient(float Y4, float Y3, float Y2, float Y1);
static int quantize_coefficient(float coeff);
static float get_y_val(Seq_T component_video, int index);
static void free_seq_elements(Seq_T component_video);

post_dct dc_transform(Seq_T component_video)
{
        post_dct dct_values;
        int i;
        float avg_pb = 0, avg_pr = 0; 
        float Y1, Y2, Y3, Y4;
        unsigned indexpb, indexpr;
        comp_vid temp;

        Y4 = get_y_val(component_video, 0);
        Y3 = get_y_val(component_video, 1);
        Y2 = get_y_val(component_video, 2);
        Y1 = get_y_val(component_video, 3);

        for (i = 0; i < DCT_REQ; i++) { /*get average pb values*/
                temp = Seq_get(component_video, i);
                avg_pb += temp->pb;
                avg_pr += temp->pr;
        }

        avg_pb /= DCT_REQ;  /*average out the stored pb/pr values*/
        avg_pr /= DCT_REQ;

        indexpb = Arith40_index_of_chroma(avg_pb);
        indexpr = Arith40_index_of_chroma(avg_pr);

        dct_values.a = get_a_coefficient(Y4, Y3, Y2, Y1);
        dct_values.b = get_b_coefficient(Y4, Y3, Y2, Y1);
        dct_values.c = get_c_coefficient(Y4, Y3, Y2, Y1);
        dct_values.d = get_d_coefficient(Y4, Y3, Y2, Y1);
        dct_values.index_pb = indexpb;
        dct_values.index_pr = indexpr;

        free_seq_elements(component_video);

        return dct_values;
}


static unsigned get_a_coefficient(float Y4, float Y3, float Y2, float Y1)
{
        float a_coeff = (Y4 + Y3 + Y2 + Y1)/DCT_REQ; /*assuming dct works the way we expect*/
        
        a_coeff = roundf(a_coeff * 511);
        return (unsigned)a_coeff;
}

static int get_b_coefficient(float Y4, float Y3, float Y2, float Y1)
{
        // printf("b Y vals\n Y4 = %f, Y3 = %f, Y2 = %f, Y1 = %f\n", Y4, Y3, Y2, Y1);
        float b_coeff = (Y4 + Y3 - Y2 - Y1)/DCT_REQ; 
        return quantize_coefficient(b_coeff);
}
static int get_c_coefficient(float Y4, float Y3, float Y2, float Y1)
{
        
        float c_coeff = (Y4 - Y3 + Y2 - Y1)/DCT_REQ; 
        return quantize_coefficient(c_coeff);
}

static int get_d_coefficient(float Y4, float Y3, float Y2, float Y1)
{
        float d_coeff = (Y4 - Y3 - Y2 + Y1)/DCT_REQ; 
        return quantize_coefficient(d_coeff);
}

static float get_y_val(Seq_T component_video, int index)
{
        comp_vid temp = Seq_get(component_video, index);
        return temp->y;
}

/*
*       free_seq_elements
*       Loops through a Hanson sequence of component_video
*       structs and frees the structs (but not the Seq_T)
*/
static void free_seq_elements(Seq_T component_video)
{
        comp_vid temp;
        while (Seq_length(component_video) != 0)
        {
                temp = (comp_vid)( Seq_remlo(component_video) );
                FREE(temp);
        } 
}

/*
*       quantize_coefficient
*       Takes in a float and quantizes the value to 
*       an int between -15 and 15 
*/
static int quantize_coefficient(float coeff)
{
        /*quantize to -0.3 or 0.3*/
        if (fabsf(coeff) > 0.3 && coeff >= 0) {
                coeff = 0.3;
        } else if (fabsf(coeff) > 0.3  && coeff < 0) {
                coeff = -0.3;
        }

        /*round value and *50 to make value fit as an int between -15 ~ 15*/
        coeff = roundf(coeff*50);

        return (int)coeff;
}