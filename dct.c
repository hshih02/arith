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

#include "artihstructs.h" 

#define DCT_REQ 4;
float get_a_coefficient(Seq_T component_video);
float get_b_coefficient(Seq_T component_video);
float get_c_coefficient(Seq_T component_video)
float get_d_coefficient(Seq_T component_video);
void free_Seq(Seq_T component_video);

float get_a_coefficient(Seq_T component_video)
{
        int i;
        float a;
        comp_vid temp;
        for (i = 0; i < DCT_REQ; i++){
               temp = Seq_get(component_video, i);
        a += temp -> y;  
        }
        a = a/DCT_REQ;
        return a;     
}

float get_b_coefficient(Seq_T component_video)
{
        int i;
        float b;
        comp_vid temp;
        for (i = 0; i < DCT_REQ; ++i){
                temp = Seq_get(component_video, i);
                b += temp -> y;
                if (i > 1){
                        b = b - temp -> y;
                }   
        }
        b = b/DCT_REQ;
        return b;

}
float get_c_coefficient(Seq_T component_video)
{
        int i;
        float c;
        comp_vid temp = Seq_get(component_video, 0);
        c = temp -> y;
        for (i = 1; i < DCT_REQ; ++i)
        {
                temp = Seq_get(component_video, i);
                if (i == 2){
                        c += temp->y;
                }else{
                    c = c - temp->y;    
                }           
        }
        c = c/DCT_REQ;
        return c;
}

float get_d_coefficient(Seq_T component_video)
{
        int i;
        float d;
        comp_vid temp = Seq_get(component_video, 0)
        d = temp->y;
        for (i = 1; i < DCT_REQ; ++i)
        {
                temp = Seq_get(component_video, i);
                if(i == 3){
                        d += temp->y;
                }else{
                        d = d - temp->y;
                }
        }
        d = d/DCT_REQ;
        return d;
}
void free_Seq(Seq_T component_video)
{
        (void) component_video;
        return;
}