/**************************************************************
 *
 *                     dct.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 3, 2019
 *
 *     conducts discrete cosine transformation 
 *     
 *
 **************************************************************/

#include "arithstructs.h" 
#define DCT_REQ 4

unsigned get_a_coefficient(float Y1, float Y2, float Y3, float Y4);
int get_b_coefficient(float Y1, float Y2, float Y3, float Y4);
int get_c_coefficient(float Y1, float Y2, float Y3, float Y4);
int get_d_coefficient(float Y1, float Y2, float Y3, float Y4);
int quantize_coefficient(float coeff);
float get_y_val(Seq_T component_video, int index);
void free_Seq(Seq_T component_video);
post_dct dc_transform(Seq_T component_video);


int main()
{
        Seq_T temp_seq = Seq_new(DCT_REQ);

        comp_vid test1, test2, test3, test4;
        NEW(test1);
        NEW(test2);
        NEW(test3);
        NEW(test4);

        // test1->y = 5;
        test1->y =   0.394627;
        test1->pb = -0.039017;
        test1->pr =  0.110123;

        test2->y =   0.556086;
        test2->pb = -0.059315;
        test2->pr = -0.027417;

        test3->y =          1;
        test3->pb =         0;
        test3->pr =         0;

        test4->y =   0.998827;
        test4->pb =  0.000662;
        test4->pr = -0.001961;

        Seq_addhi(temp_seq, test1);
        Seq_addhi(temp_seq, test2);
        Seq_addhi(temp_seq, test3);
        Seq_addhi(temp_seq, test4);

        dc_transform(temp_seq);

        return 1;

}

post_dct dc_transform(Seq_T component_video)
{
        post_dct dct_values;
        int i;
        float avg_pb = 0, avg_pr = 0; 
        float Y1, Y2, Y3, Y4;
        unsigned indexpb, indexpr;
        comp_vid temp;

        Y1 = get_y_val(component_video, 0);
        Y2 = get_y_val(component_video, 1);
        Y3 = get_y_val(component_video, 2);
        Y4 = get_y_val(component_video, 3);

        for (i = 0; i < DCT_REQ; i++) { /*get average pb values*/
                temp = Seq_get(component_video, i);
                avg_pb += temp->pb;
                avg_pr += temp->pr;
        }

        avg_pb /= DCT_REQ;  /*average out the stored pb/pr values*/
        avg_pr /= DCT_REQ;

        printf("average pb = %f\n", avg_pb);
        printf("average pr = %f\n", avg_pr);

        indexpb = Arith40_index_of_chroma(avg_pb);
        indexpr = Arith40_index_of_chroma(avg_pr);

        printf("indexpb pre saved = %u\n", indexpb);

        printf ("a coeff: %u\n", get_a_coefficient(Y1, Y2, Y3, Y4));
        printf ("b coeff: %d\n", get_b_coefficient(Y1, Y2, Y3, Y4));
        printf ("c coeff: %d\n", get_c_coefficient(Y1, Y2, Y3, Y4));
        printf ("d coeff: %d\n", get_d_coefficient(Y1, Y2, Y3, Y4));

        dct_values.a = get_a_coefficient(Y1, Y2, Y3, Y4);
        dct_values.b = get_b_coefficient(Y1, Y2, Y3, Y4);
        dct_values.c = get_c_coefficient(Y1, Y2, Y3, Y4);
        dct_values.d = get_d_coefficient(Y1, Y2, Y3, Y4);
        dct_values.index_pb = indexpb;
        dct_values.index_pr = indexpr;



        printf("index_pb = %u\n", dct_values.index_pb);
        printf("index_pr = %u\n", dct_values.index_pr);

        free_Seq(component_video);

        return dct_values;
}


unsigned get_a_coefficient(float Y1, float Y2, float Y3, float Y4)
{
        float a_coeff = (Y4 + Y3 + Y2 + Y1)/DCT_REQ; /*assuming dct works the way we expect*/
        
        a_coeff = roundf(a_coeff * 511);
        return (unsigned)a_coeff;
}

int get_b_coefficient(float Y1, float Y2, float Y3, float Y4)
{
        float b_coeff = (Y4 + Y3 - Y2 - Y1)/DCT_REQ; /*assuming dct works the way we expect*/
        return quantize_coefficient(b_coeff);
}
int get_c_coefficient(float Y1, float Y2, float Y3, float Y4)
{
        float c_coeff = (Y4 - Y3 + Y2 - Y1)/DCT_REQ; /*assuming dct works the way we expect*/
        return quantize_coefficient(c_coeff);
}

int get_d_coefficient(float Y1, float Y2, float Y3, float Y4)
{
        float d_coeff = (Y4 - Y3 - Y2 + Y1)/DCT_REQ; /*assuming dct works the way we expect*/
        return quantize_coefficient(d_coeff);
}
float get_y_val(Seq_T component_video, int index)
{
        comp_vid temp = Seq_get(component_video, index);
        return temp->y;
}


void free_Seq(Seq_T component_video)
{
        comp_vid temp;
        while (Seq_length(component_video) != 0)
        {
                temp = (comp_vid)( Seq_remlo(component_video) );
                FREE(temp);
        } 
        Seq_free(&component_video);
}

int quantize_coefficient(float coeff)
{
        /*quantize to -0.3 or 0.3*/
        if (fabsf(coeff) > 0.3 && coeff >= 0) {
                coeff = 0.3;
                printf("quantize to 0.3\n");
        } else if (fabsf(coeff) > 0.3  && coeff < 0) {
                coeff = -0.3;
                printf("quantize to -0.3\n");
        }

        coeff = roundf(coeff*50);

        return (int)coeff;
}